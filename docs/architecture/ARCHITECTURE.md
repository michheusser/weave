# Architecture
In this document a brief description of Weave's different core modules are explained. In the future a more comprehensive
description, together with technical documentation, class and architectural diagrams will complement this for a more
comprehensive view.

The general architectural philosophy of this framework is to offload all the resource-intensive processes in terms
of time, CPU, and memory to compilation, and then to construction and initialization of the pipelines, modules, and
objects. This allows for a seamless and truly performant running of pipelines without any extra runtime overhead,
mostly thanks to pre-initialized modules, pre-allocated buffers, and zero-copy data flow across all elements of the
pipeline.

## Graph Layer
Graphs are constructed using the graph builder. This allows for compile time definition of the graph's nodes, which
represent the user-defined processing modules and the edges, which represent the ring-buffers that connect
processing modules.

The below example depicts the API for building an edge and building a node, using user-define structs as unique
identifiers or tags for them.

To create a builder we call it:
```cpp
auto builder = weave::graph::Builder();
```

To add edges and nodes we use:
```cpp
weave::graph::Builder::addNode<...>(...);
```
and 
```cpp
weave::graph::Builder::addEdge<>(...);
```
Leaving the node and edge options and focusing only on the graph topology by defining the nodes each edge connects to,
we define our whole graph in one call like so:
```cpp
auto builder = weave::graph::Builder()
                .addNode<MyImageCapturerTag, ...>(capturerConfiguration)
                .addNode<MyImageResizerTag, ...>(resizerConfiguration)
                .addNode<MyImageDisplayerTag, ...>(displayerConfiguration)
                .addEdge<MyRawImageBufferTag, MyImageCapturerTag, MyImageResizerTag, ...>(rawImageBufferConfiguration)
                .addEdge<MyResizedImageBufferTag, MyImageResizerTag, MyImageDisplayerTag, ...>(resizedImageBufferConfiguration);
```
Having done this we build our pipeline and start it
```cpp
auto pipeline = builder.build();
pipeline.start();
```

## Worker And Modules
The worker and processing modules part of the framework deals with the intricacies of the nodes within the graphs.
Each node contains a worker wrapper that takes care of initializing each user-defined processing module (for example an inference model)
and starting it in a new thread where it cycles. 

The worker is in charge of managing the synchronization and acquisition of reading and writing constructs to be able to safely and concurrently access the buffers, according
to their defined policies and apply the zero-copy, user-defined processing operations on the input and output buffers. This
is done using a Reader/Writer objects that act as RAII accessors of the buffers, that release/publish once the actions
are finished for other threads to act on the buffers.

```cpp
error::Result result = _processor.process(inputReadersDataTuple, outputWritersDataTuple);

// ...

_release(inputReaders, std::make_index_sequence<std::tuple_size_v<ReadersTupleType> >());
_publish(outputWriters, std::make_index_sequence<std::tuple_size_v<WritersTupleType> >());
```
This is done in a generic way that works for all types of topologies, including any multi-input and multi-output
processing modules, or Multiple Producer Multiple Consumper (MPMC).

It is worth mentioning that modules with zero inputs (sources) or zero outputs (sinks) are naturally supported, and
these could be frame generation modules (e.g. a video/audio or I/O capturing module), a displayer or dumping modules,
as well as Network sender/receiver modules where pipelines start or end.

## Buffer System
The buffer part of the framework contains the Channel class, which is the analogous to the Worker in nodes. It is the
highest abstraction layer within the edge and offers the API to accessing the data within the ring-buffer. It
contains the synchronization primitives for the worker to deal with. This is achieved through the calling of the functions to
acquire the Reader and Writer RAII accessor objects.

```cpp
weave::buffer::Channel::reader();
weave::buffer::Channel::writer();
```
The Channel contains the policy for contention handling, which is chosen at compile time. In the future, the framework
will offer an adaptive functionality using a multiplexer with an array of pre-allocated ring-buffers for dynamic switching
without overhead or virtual dispatching. The policies are:
- **Lossless**: It has an infinite timeout strategy (no timeout) and a backpressure contengion strategy.
- **Realtime**: It has a timed attempt (e.g. to handle network connection jitters) and then drops the oldest slot.
- **Throttled**: It has a timed attempt and drops the newest slot.

Further flexibility and policies will be added in the future.

The ring-buffer within the channel is a fixed-size array defined at compile-time, that implements a queue with user-defined
data slots. This allows for complex maximum flexibility as well as custom and complex initializations, for example
the ones needed by third party libraries like ONNX Inference tensors (```Ort::Value```), or pre-allocation of
frame matrices in OpenCV (```cv::Mat```).

## Profiling and Performance
Weave offers embedded Profiling capabilities, particularly metrics and tracing, for profiling and performance tuning.
This is offered by the profiling macros which are initialized automatically during the running of each pipeline.
```cpp
TRACE_INIT(session, description);
METRICS_INIT(session, description);
```
Using a custom reflection utility (which will be solved natively with the C++26 reflection capabilities), all cycles
offer metrics and tracing per cycle which can be activated individually via preprocessor directives, for example,
for release/debug builds.

Apart from the initial memory overhead, metrics aggregate over-time in constant memory O(1) complexity, since they
only add to the final statistical aggregates and are defined per module to aggregate each cycle. The delivered
metrics at shutdown and a dump is called are the following:
- **Observed Module Throughput**: The number of counts per cycle across all time elapsed
- **Theoretical Module Throughput**: The number of counts per cycle across each individual cycle time
- **Min. Latency**: Minimum latency across all module's cycles
- **P50 Latency**: Median latency across all module's cycles
- **P95 Latency**: 95-percentile latency across all module's cycles
- **P99 Latency**: 99-percentile latency across all module's scycles
- **Max. Latency**: Maximum latency across all module's cycles

Users can integrate the metrics framework for their own user-defined modules using the following macros:
```cpp
METRICS_COUNT(metricsName);
```
For one count per cycle, for example in the case of frames in video pipelines. And:
```cpp
METRICS_BYTES(metricsName, data);
```
For bytes per cycle, for example in the case of encoding or networking modules.

Tracing, on the other hand, aggregates each single cycle, and is therefore
extremely memory intensive, especially in real-time pipelines (e.g. video pipelines). For
that reason it should be used with care and mostly for debugging and performance tuning.

The trace tree is created via scoped trace-spans as RAII objects that generate the 
trace tree automatically through the macro calls in the function/nested-function trees

Users can integrate into the embedded tracing using the folowing macros:
```cpp
TRACE_FUNCTION(className);
```
which will automatically add the scope it is called at (ideally one per function) to the trace tree.

To have the results display or dumped for either the metrics or the trace, the following macros can be used after the
pipeline has been completed:
```cpp
METRICS_DISPLAY();
METRICS_DUMP(directory); // CSV file
```
and
```cpp
TRACE_DISPLAY();
TRACE_DUMP(directory); // JSON Chrome Tracing (chrome://tracing)
```
The metrics will be outputted as CSV file, and the Traces will be a JSON file compatible with Chrome Trace (chrome://tracing).

## Logging
Logging in weave uses spdlog in the background and offers a simple API to be used
in user defined modules to have a unified and streamlined logging syntax and output. The
different logging levels are the following:

- **TRACE**: Called using ```LOG_TRACE(...)``` and used for detailed (trace) debug information (unrelated to the trace framework from above).
- **DEBUG**: Called using ```LOG_DEBUG(...)``` and used for debug information that should be removed in release builds 
- **INFO**: Called using ```LOG_INFO(...)``` and used for important information that must be shown to users. For this reason it only used in non-hot paths
- **WARN**: Called using ```LOG_WARN(...)``` and used for warnings, also outside hot-paths
- **ERROR**; Called using ```LOG_ERROR(...)``` and used for errors, mostly used within the exception handling mechanisms outside hot-paths
- **FATAL**; Called using ```LOG_FATAL(...)``` and used for critical errors that grant a shutdown

## Error and Exception Handling
The error handling framework in Weave is split in two parts. The first one is for non-hot paths, for example initialization phases
for graphs, configuration, workers and channels, and is what should be used for user-defined processing modules. It is 
derived from ```std::exception```. Despite the additional overhead for the compiler when dealing with exception-throwing
functions, this is reserved for the parts of the framework that need extensive logging and transparency to ensure correct
preparation for when the running happens.

User defined modules and buffers should use their own try-catch logic in initialization parts to make sure errors are not
propagated up the owning components, but rather re-throw the frameworks own exception type which is the supported one:

```cpp
try
{
    // ...
}
catch (...)
{
    throw weave::error::Exception("Failed");
}
```

This is then caught by the exception hierarchy of the framework for correct stack-logging, which is embedded in the following ways:
```cpp
try
{
    // ...
}
catch (weave::error::Exception& exception)
{
    LOG_ERROR(exception.what());
    throw weave::error::Exception("Failed");
}
```
For hot paths, all functions have been defined as no-except to ensure proper compiler optimizations and no-exception throwing
logic is included. Instead, we use error-codes implemented as enums for a more modern C++ approach (as opposed to classic
C-style error throwing). These are expected from the framework on user-defined modules in their implementation:

```cpp
weave::error::Result Preprocessor::process(const cv::Mat& sourceFrame, cv::Mat& destinationFrame) const noexcept
{
    cv::resize(sourceFrame, destinationFrame, _destinationSize); // If throws error, terminates due to noexcept (fatal error in hot path)
    return weave::error::Result::success();
}
```
Error propagation throughout hot-paths is done through simple "re-throw" via returns.



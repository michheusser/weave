# Weave ![License](https://img.shields.io/badge/license-MIT-blue) ![C++20](https://img.shields.io/badge/C%2B%2B-20-blue) ![CMake 3.31](https://img.shields.io/badge/CMake-3.31-blue) ![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen) 
Weave is a generic, graph-based data-processing framework meant for low-latency, high-throughput pipelines in the domains of
real-time video, audio, inference, and high-volume observability. It delivers compile-time graph construction with arbitrary topology with 
heavy use of C++20 Template Metaprogramming capabilities, lock-free synchronization primitives for critical paths, 
policy-based buffer management (selecting between backpressure and dropping strategies), and integrated distributed logging, tracing, and metrics
across each pipeline stage. This is all while maintaining the flexibility to inject and profile custom data slots and processing modules
without modifying core infrastructure.

## Why Weave Exists

While developing real-time video inference pipelines across networked GPUs, I encountered fundamental limitations in existing 
frameworks: Google's GStreamer has a type system that imposes virtual dispatch overhead on every buffer pass, 
FFmpeg's filter graphs required runtime string parsing with no compile-time validation, and NVIDIA's DeepStream is a closed-source,
proprietary ecosystem. I designed Weave not only to have full control and transparency over my needs, but also to eliminate
these constraints aiming for a flexible framework that adapts to any type of generic data processing while offering the
highest-level of performance when low-latency and high-throughput are a priority.

## Key Features
- Generic, compile-time graph topology of nodes (processing units) and edges (queues based on ring buffers)
- Injection, initialization, and performance profiling of custom processing modules (e.g. Inference, Network Servers/Clients, Video/Audio, Observability Data, etc.)
- Custom data slots with complex initialization (e.g. ONNX Inference Tensors, OpenCV Mat Frames, STL Containers, etc.)
- Zero-copy data movement between pipeline stages
- Compile-time type safety: no runtime type errors or virtual dispatch
- Policy-based congestion management
- Built-in distributed logging, tracing, and metrics and integration in custom modules
- Lock-free synchronization

## Examples
### Example 1: Capture, Resize, Display

A simple example using OpenCV to capturer, resize and display frames. The complete example is found [here](docs/examples/DISPLAY.md):
```cpp
// Configuration / Context
CapturerConfig capturerConfig = {1920, 1080}; // User defined type
ResizerConfig resizerConfig = {640, 480};
DisplayerConfig displayerConfig = {DEFAULT_DISPLAYER_MODE};

CapturerConfig capturerConfig = {1920, 1080}; // User defined type
ImageSlotContext rawImageBufferContext{1920, 1080};
ResizerConfig resizerConfig = {640, 480};
ImageSlotContext resizedImageBufferContext = {640, 480};
DisplayerConfig displayerConfig = {DEFAULT_DISPLAYER_MODE};

auto pipeline = weave::graph::Builder()
    .addNode<MyCapturerNode, Capturer>(capturerConfig)
    .addEdge<MyRawImageEdge, MyCapturerNode, MyResizerNode, 16, PolicyType::Lossless>(rawImageBufferContext)
    .addNode<MyResizerNode, Resizer>(resizerConfig)
    .addEdge<MyResizedImageEdge, MyResizerNode, MyDisplayerNode, 16, PolicyType::LossLess>(resizedImageBufferContext)
    .addNode<MyDisplayerNode, Displayer>(displayerConfig)
    .build();
    
pipeline.start();
pipeline.waitForShutdown();
```



### Example 2: GPU Edge Inference Network Server
This example sets up a server for GPU-based inference on an edge device (e.g. NVIDIA Jetson). It first defines the configuration objects (contexts), 
which contain the (runtime) initialization constructs for the user-defined processing units and buffer-slots. It then defines
and builds the pipeline, starts the network modules (which are passed in the corresponding context), and finally runs the pipeline.

For the complete implementation on this inference example refer to [testGraph.cpp](test/testGraph.cpp).

```cpp
/**** CONFIGURATION ****/

ImageReceiverContext imageReceiverContext;
// ... configuration and pointer to network module
ReceiverBufferContext imageReceiveBufferContext;
// ...
InferencePreprocessorContext inferenceInputProcessorContext;
// ...
InferenceInputBufferContext inferenceInputTensorContext;
// ...
InferenceModelContext inferenceModelContext;
// ...
ImageSenderContext imageSendBufferContext;
// ...
SenderBufferContext imageSenderContext;
// ...configuration and pointer to network module


/**** PIPELINE BUILD ****/
using weave::buffer::constants::PolicyType;

auto serverPipeline = weave::graph::Builder()
    .addNode<ServerImageReceiver, ServerReceiver>(imageReceiverContext)
    .addEdge<ServerImageReceiveEdge, ServerImageReceiver, ServerInferenceInputProcessor, 32, PolicyType::Lossless>(imageReceiveBufferContext)
    .addNode<ServerInferenceInputProcessor, InferenceInputProcessor>(inferenceInputProcessorContext)
    .addEdge<ServerInferenceInputTensorEdge, InferenceInputProcessor, ServerInferenceModel, 8, PolicyType::Realtime>(inferenceInputTensorContext)
    .addNode<ServerInferenceModel, InferenceModel>(inferenceModelContext)
    .addEdge<ServerImageSendEdge, ServerInferenceModel, ServerImageSender, 16, PolicyType::Throttled>(imageSendBufferContext)
    .addNode<ServerImageSender, ServerSender>(imageSenderContext)
    .build();

weave::utilities::SignalManager::installHandlers();


/**** RUNNING ****/

networkServer->initialize();
networkServer->listen();
networkServer->accept();
serverPipeline.start();
serverPipeline.waitForShutdown();
```

## Architecture
The technical details of the architecture and the individual module of the library can be found in the [Architecture Document](docs/architecture/ARCHITECTURE.md)
- [Graph Layer](docs/architecture/ARCHITECTURE.md#graph-layer) Main API, graph definition, compile-time building, buffer-slot definition, and module injection.
- [Buffer System](docs/architecture/ARCHITECTURE.md#buffer-system) Ring-buffer queues construction, initialization, and congestion-management policy.
- [Worker And Modules](docs/architecture/ARCHITECTURE.md#worker-and-modules) Module construction and initialization, thread and cycle management, synchronization read-write mechanics.
- [Profiling and Performance](docs/architecture/ARCHITECTURE.md#profiling-and-performance) Trace and metrics for observability, macros, display, and dumping.
- [Logging](docs/architecture/ARCHITECTURE.md#logging) Command line and file-based logging and logging levels.
- [Error and Exception Handling](docs/architecture/ARCHITECTURE.md#error-and-exception-handling) Exception handling for construction and initialization and error codes (enums) for hot-paths.
- [Utilities](docs/architecture/ARCHITECTURE.md#utilities) Signal-Managers for shutdown and reflection constructs. 

# Roadmap
- Trace streamlining
- CMake Install, Config exports, Conan, CI/CD
- Lock-free MPMC ring-buffer queues
- Unit Tests (GTest)
- Doxygen Technical Documentation 
- Use of extensive C++20 Concepts for improved user friendliness and debugging
- Adaptive buffer policies using buffer multiplexing

# Building and Linking
To build the project first clone the repository:
``` 
git clone https://github.com/michheusser/weave
```
You will need CMake version at least 3.31 to be able to build the project: https://cmake.org/download/

In the project's local directory create a build directory and run the CMake configuration
```
mkdir build
cmake -S . -B build
```
Once the configuration is successful, run the build and then install:
```
cmake --build build
cmake --install build
```
This will install the needed headers and libraries for you to be able to link statically to the project, as well 
as the cmake configuration files to be able to find them.

Once this is successfully done, you can use Weave in your CMake project as such:
```
find_package(weave REQUIRED)
target_link_libraries(MyTarget PRIVATE weave::weave)
```
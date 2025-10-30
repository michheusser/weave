# Weave ![License](https://img.shields.io/badge/license-MIT-blue) ![C++20](https://img.shields.io/badge/C%2B%2B-20-blue) ![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen) 
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
- Generic, compile-time graph topology of nodes (processing units) and edges (ring buffers)
- Injection, initialization, and performance profiling of custom processing modules (e.g. Inference, Network Servers/Clients, Video/Audio, Observability Data, etc.)
- Custom-data slots for ring buffers with complex initialization (e.g. ONNX Inference Tensors, OpenCV Mat Frames, STL Containers, etc.)
- Zero-copy data movement between pipeline stages
- Compile-time type safety: no runtime type errors or virtual dispatch
- Policy-based congestion management
- Built-in distributed logging, tracing, and metrics and integration in custom modules
- Lock-free synchronization (in development)

## Quick Example
...

TODO PUT ARCHITECTURE IN OWN FILE

## Architecture
...

### Graph Layer
...

### Buffer System
...

### Worker Architecture
...

### Observability
...

## Performance
...

## Roadmap
- Comprehensive Unit Tests (GTest)
- Use of extensive C++20 Concepts for improved user friendliness and debugging
- Lock-free MPMC ring-buffer queues
- Adaptive buffer policies using buffer multiplexing 

## Building and Installing
...
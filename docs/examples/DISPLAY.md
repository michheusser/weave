### Example 1: Capture, Resize, Display
A simple example using OpenCV to capturer, resize and display frames.

First we define our user-defined processing modules:
```cpp
// ProcessorModules.h

struct CapturerContext{int width; int height;};
class Capturer
{
public:
    explicit Capturer(const CapturerContext& context);
    void initialize();
    weave::error::Result process(cv::Mat& frameBuffer) noexcept;
    ...
};

struct ResizerContext{...};
class Resizer{...};

struct ResizerContext{...};
class Resizer{
    //...
    weave::error::Result process(const cv::Mat& sourceFrame, cv::Mat& destinationFrame) const noexcept;
    //...
};

struct DisplayerContext{...};
class Displayer{
    //...
    weave::error::Result process(const cv::Mat& sourceFrame) const noexcept;
    //...
};
```
We create our slots:
```cpp
// Slots.h

struct ImageSlotContext{...}
class ImageSlot
{
public:
    using StorageType = cv::Mat;
    using ContextType = ImageSlotContext;
    explicit Image();
    void initialize(const ContextType& context);
    StorageType& data();
private:
    StorageType _data;
};
```

We then create our tags and mappings:
```cpp
// TagsAndTraits.h

#include "ProcessorModules.h"

struct MyCapturerNode{};
struct MyResizerNode{};
struct MyDisplayerNode{};
template<> struct weave::user::ProcessorTraits<MyCapturerNode>{using ModuleType = Capturer;};
template<> struct weave::user::ProcessorTraits<MyResizerNode>{using ModuleType = Resizer;};
template<> struct weave::user::ProcessorTraits<MyDisplayerNode>{using ModuleType = Displayer;};

struct MyRawImageEdge{};
struct MyResizedImageEdge{};
template<> struct weave::user::SlotTraits<MyRawImageEdge>{using SlotDataType = ImageSlot;};
template<> struct weave::user::SlotTraits<MyResizedImageEdge>{using SlotDataType = ImageSlot;};
```
And finally we create the pipeline once all our types have been defined:
```cpp
// main.cpp
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
    .addNode<MyCapturerNode>(capturerConfig)
    .addEdge<MyRawImageEdge, MyCapturerNode, MyResizerNode, 16, PolicyType::Lossless>(rawImageBufferContext)
    .addNode<MyResizerNode>(resizerConfig)
    .addEdge<MyResizedImageEdge, MyResizerNode, MyDisplayerNode, 16, PolicyType::LossLess>(resizedImageBufferContext)
    .addNode<MyDisplayerNode>(displayerConfig)
    .build();
    
pipeline.start();
pipeline.waitForShutdown();
```
### Example 1: Capture, Resize, Display
A simple example using OpenCV to capturer, resize and display frames.

First we define our user-defined processing modules:
```cpp
// ProcessorModules.h

struct CapturerContext{int width; int height;};
class Capturer
{
public:
    using ContextType = CapturerContext;
    explicit Capturer(const ContextType& context);
    void initialize();
    weave::error::Result process(cv::Mat& frameBuffer) noexcept;
    ...
};

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

We then create our tags:
```cpp
// Tags.h
struct MyCapturerNode{};
struct MyResizerNode{};
struct MyDisplayerNode{};

struct MyRawImageEdge{};
struct MyResizedImageEdge{};
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
    .addNode<MyCapturerNode, Capturer>(capturerConfig)
    .addEdge<MyRawImageEdge, MyCapturerNode, MyResizerNode, 16, PolicyType::Lossless>(rawImageBufferContext)
    .addNode<MyResizerNode, Preprocessor>(resizerConfig)
    .addEdge<MyResizedImageEdge, MyResizerNode, MyDisplayerNode, 16, PolicyType::LossLess>(resizedImageBufferContext)
    .addNode<MyDisplayerNode, Displayer>(displayerConfig)
    .build();
    
pipeline.start();
pipeline.waitForShutdown();
```
add_library(worker STATIC
        include/weave/worker/Constants.h
        include/weave/worker/Context.h
        source/Processor.cpp
        include/weave/worker/Processor.h
        include/weave/worker/Synchronizer.h
        include/weave/worker/Traits.h
        include/weave/worker/Worker.h
)

target_include_directories(worker PUBLIC include)
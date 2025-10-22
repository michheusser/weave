add_library(worker STATIC
        include/weave/worker/Constants.h
        source/Dummy.cpp
        include/weave/worker/Synchronizer.h
        include/weave/worker/Worker.h
        include/weave/worker/Processor.h
)

target_include_directories(worker PUBLIC include)
target_link_libraries(worker PUBLIC error logging profiling utilities)
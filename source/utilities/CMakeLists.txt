add_library(utilities STATIC
        include/weave/utilities/Executable.h
        include/weave/utilities/SignalManager.h
        Executable.cpp
        SignalManager.cpp
)

target_include_directories(utilities PUBLIC include)
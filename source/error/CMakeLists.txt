add_library(error STATIC
        include/weave/error/Constants.h
        source/Exception.cpp
        include/weave/error/Exception.h
        include/weave/error/Result.h
)

target_include_directories(error PUBLIC include)
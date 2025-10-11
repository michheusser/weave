add_library(user STATIC
        include/weave/user/Module.h
        include/weave/user/BufferData.h
        source/dummy.cpp
        source/dummy.h
)

target_include_directories(user PUBLIC include)

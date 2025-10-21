add_library(user STATIC
        include/weave/user/Slot.h
        include/weave/user/ProcessorTraits.h
        source/Dummy.cpp
)

target_include_directories(user PUBLIC include)

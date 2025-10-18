add_library(user STATIC
        include/weave/user/EdgeTraits.h
        include/weave/user/ChannelTraits.h
        include/weave/user/RingBufferTraits.h
        include/weave/user/Module.h
        include/weave/user/Slot.h
        include/weave/user/ProcessorTraits.h
        include/weave/user/NodeTraits.h
        include/weave/user/WorkerTraits.h
        include/weave/user/SynchronizerTraits.h
        source/Dummy.cpp
)

target_include_directories(user PUBLIC include)

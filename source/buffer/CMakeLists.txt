add_library(buffer STATIC
        include/weave/buffer/Channel.h
        include/weave/buffer/constants.h
        include/weave/buffer/Policy.h
        include/weave/buffer/RingBuffer.h
        include/weave/buffer/Reader.h
        include/weave/buffer/ReaderAcquirer.h
        include/weave/buffer/Writer.h
        include/weave/buffer/WriterAcquirer.h
        include/weave/buffer/Slot.h
        source/Dummy.cpp
        source/Dummy.h
)

target_include_directories(buffer PUBLIC include)
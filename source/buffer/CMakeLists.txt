add_library(buffer STATIC
        include/weave/buffer/Buffer.h
        include/weave/buffer/Constants.h
        source/InternalData.cpp
        include/weave/buffer/InternalData.h
        include/weave/buffer/Policy.h
        include/weave/buffer/QueueBuffer.h
        include/weave/buffer/Reader.h
        include/weave/buffer/ReaderAcquirer.h
        include/weave/buffer/Traits.h
        include/weave/buffer/Writer.h
        include/weave/buffer/WriterAcquirer.h
)

target_include_directories(buffer PUBLIC include)
add_library(core STATIC
        include/weave/core/Builder.h
        include/weave/core/Constants.h
        include/weave/core/Edge.h
        include/weave/core/EdgeDescriptor.h
        include/weave/core/Graph.h
        include/weave/core/GraphDescriptor.h
        include/weave/core/Node.h
        include/weave/core/NodeDescriptor.h
        include/weave/core/Utilities.h
        source/Dummy.cpp
        source/Dummy.h
)

target_include_directories(core PUBLIC include)
target_link_libraries(core PUBLIC error user)
add_library(graph STATIC
        include/weave/graph/Builder.h
        include/weave/graph/Constants.h
        include/weave/graph/Edge.h
        include/weave/graph/EdgeDescriptor.h
        include/weave/graph/Graph.h
        include/weave/graph/GraphDescriptor.h
        include/weave/graph/Node.h
        include/weave/graph/NodeDescriptor.h
        include/weave/graph/Utilities.h
        source/Dummy.cpp
        source/Dummy.h
)

target_include_directories(graph PUBLIC include)
target_link_libraries(graph PUBLIC error user buffer)
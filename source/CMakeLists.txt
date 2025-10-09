add_library(weave STATIC
        core/BufferData.cpp
        core/BufferData.h
        core/Builder.cpp
        core/Builder.h
        core/Constants.h
        core/Edge.cpp
        core/Edge.h
        core/EdgeDescriptor.h
        core/Graph.cpp
        core/Graph.h
        core/GraphDescriptor.h
        core/Module.cpp
        core/Module.h
        core/Node.cpp
        core/Node.h
        core/NodeDescriptor.h
        core/Utilities.h
)

target_include_directories(weave PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(weave PUBLIC ${OpenCV_LIBS})

add_subdirectory(test)
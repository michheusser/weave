add_library(test STATIC
        modules/Capturer.cpp
        modules/Capturer.h
        modules/Configuration.h
        modules/Constants.h
        modules/Context.h
        utilities/DisplayBridge.cpp
        utilities/DisplayBridge.h
        modules/Displayer.cpp
        modules/Displayer.h
        modules/Preprocessor.cpp
        modules/Preprocessor.h

        buffer/Context.h
        buffer/Configuration.h
        buffer/Constants.h

        worker/Context.h

        Specializations/SlotSpecialization.h
        Specializations/SlotSpecialization.cpp
        Specializations/EdgeTraitsSpecialization.h
        Specializations/ChannelTraitsSpecialization.h
        Specializations/RingBufferTraitsSpecialization.h
        Specializations/ProcessorTraitsSpecialization.h
        Specializations/WorkerTraitsSpecialization.h
        Specializations/NodeTraitsSpecialization.h
        Specializations/SynchronizerTraitsSpecialization.h
)

target_include_directories(test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(test PUBLIC ${OpenCV_LIBS} graph error logging utilities user)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Detected macOS")

    add_executable(testGraph MACOSX_BUNDLE testGraph.cpp)
    set(MACOSX_BUNDLE_BUNDLE_NAME "testGraph")
    set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/test/Info.plist)
    set_target_properties(testGraph PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${MACOSX_BUNDLE_INFO_PLIST}
    )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detected Linux")
    add_executable(server Streaming/Test/serverTest.cpp)
else ()
    message(FATAL_ERROR "Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif ()

target_link_libraries(testGraph PRIVATE test)
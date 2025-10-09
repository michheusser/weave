add_library(user STATIC
        modules/Capturer.cpp
        modules/Capturer.h
        modules/Configuration.h
        modules/Constants.h
        modules/DisplayBridge.cpp
        modules/DisplayBridge.h
        modules/Displayer.cpp
        modules/Displayer.h
        modules/Preprocessor.cpp
        modules/Preprocessor.h
)

target_include_directories(weave PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(weave PUBLIC ${OpenCV_LIBS})

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Detected macOS")

    # Client
    add_executable(test MACOSX_BUNDLE testGraph.cpp)
    set(MACOSX_BUNDLE_BUNDLE_NAME "test")
    set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/source/Info.plist)
    set_target_properties(test PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${MACOSX_BUNDLE_INFO_PLIST}
    )

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Detected Linux")
    add_executable(server Streaming/Test/serverTest.cpp)
else ()
    message(FATAL_ERROR "Unknown platform: ${CMAKE_SYSTEM_NAME}")
endif ()

target_link_libraries(test PRIVATE weave)
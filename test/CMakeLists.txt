# OpenCV
find_package(OpenCV REQUIRED)

# ONNX
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(ENV{ONNXRUNTIME_ROOT} "/Users/michelsmacbookpro/workspace/onnxruntime")
find_package(ONNXRuntime REQUIRED)

add_library(test STATIC
        modules/Capturer.cpp
        modules/Capturer.h
        modules/Configuration.h
        modules/Constants.h
        modules/Context.h
        modules/Decoder.cpp
        modules/Decoder.h
        modules/Displayer.cpp
        modules/Displayer.h
        modules/Encoder.cpp
        modules/Encoder.h
        modules/InferenceInputProcessor.cpp
        modules/InferenceInputProcessor.h
        modules/InferenceModel.cpp
        modules/InferenceModel.h
        modules/Preprocessor.cpp
        modules/Preprocessor.h
        modules/MessageHeader.h
        modules/ClientReceiver.cpp
        modules/ClientReceiver.h
        modules/ClientSender.cpp
        modules/ClientSender.h
        modules/ServerReceiver.cpp
        modules/ServerReceiver.h
        modules/ServerSender.cpp
        modules/ServerSender.h

        slots/Image.cpp
        slots/Image.h
        slots/InferenceTensor.cpp
        slots/InferenceTensor.h

        utilities/DisplayBridge.cpp
        utilities/DisplayBridge.h

        buffer/Context.h
        buffer/Configuration.h
        buffer/Constants.h

        worker/Context.h

        streaming/Configuration.h
        streaming/Constants.h
        streaming/NetworkClient.cpp
        streaming/NetworkClient.h
        streaming/NetworkServer.cpp
        streaming/NetworkServer.h
        streaming/NetworkUtilities.cpp
        streaming/NetworkUtilities.h

        specializations/ProcessorTraitsSpecialization.h
        specializations/SlotTraitsSpecialization.h
)

target_include_directories(test PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(test PUBLIC ${OpenCV_LIBS} ONNXRuntime::ONNXRuntime graph error logging utilities user)
target_compile_definitions(test PUBLIC WEAVE_MODELS_LOCATION=\"${CMAKE_HOME_DIRECTORY}/test/models/fast-neural-stayle-transfer\")


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
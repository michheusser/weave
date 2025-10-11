add_library(logging STATIC
        include/weave/logging/Definitions.h
        include/weave/logging/Logger.h
        include/weave/logging/Macros.h
        include/weave/logging/Constants.h
        source/Logger.cpp
)

target_include_directories(logging PUBLIC include)
target_link_libraries(logging PRIVATE utilities)

if (ENABLE_LOGGING)
    target_link_libraries(logging PUBLIC spdlog::spdlog)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_LOGGING=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_TRACE=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_DEBUG=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_INFO=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_WARN=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_ERROR=1)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_FATAL=1)
else ()
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_LOGGING=0)
endif ()

if (ENABLE_MONITORING)
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_MONITORING=1)
else ()
    target_compile_definitions(logging PUBLIC WEAVE_ENABLE_MONITORING=0)
endif ()
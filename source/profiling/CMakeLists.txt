
add_library(profiling STATIC
        include/weave/profiling/MetricsContext.h
        include/weave/profiling/MetricsCollector.h
        include/weave/profiling/ConfigurationSerialization.h
        include/weave/profiling/ConfigurationDataBuilder.h
        include/weave/profiling/TraceSpanDataNode.h
        include/weave/profiling/TraceCollector.h
        include/weave/profiling/TraceSpan.h
        include/weave/profiling/MetricsSpanData.h
        include/weave/profiling/MetricsSpan.h
        include/weave/profiling/Constants.h
        include/weave/profiling/Macros.h
        include/weave/profiling/ConfigurationCollector.h
        include/weave/profiling/ConfigurationContext.h
        include/weave/profiling/TraceContext.h
        include/weave/profiling/ConfigurationDataNode.h
        source/MetricsSpan.cpp
        source/ConfigurationContext.cpp
        source/ConfigurationCollector.cpp
        source/MetricsCollector.cpp
        source/TraceSpan.cpp
        source/MetricsPrinter.h
        source/Configuration.h
        source/ConfigurationPrinter.cpp
        source/ConfigurationDataBuilder.cpp
        source/MetricsContext.cpp
        source/TracePrinter.h
        source/TracePrinter.cpp
        source/MetricsPrinter.cpp
        source/TraceContext.cpp
        source/TraceCollector.cpp
        source/ConfigurationPrinter.h
        # pending/clientSessionMain.cpp
        # pending/SessionExecutor.h
        # pending/ConfigurationGenerator.cpp
        # pending/SessionExecutor.cpp
        # pending/ConfigurationGenerator.h
        # pending/serverSessionMain.cpp
)

target_include_directories(profiling PUBLIC include)
target_link_libraries(profiling PUBLIC error logging)
target_compile_definitions(profiling PUBLIC WEAVE_MONITORING_REPORTS_LOCATION=\"${CMAKE_HOME_DIRECTORY}/Sessions\") # TODO This should be customizable by user
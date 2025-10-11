add_library(profiling STATIC
        source/Configuration.h
        source/ConfigurationCollector.cpp
        include/weave/profiling/ConfigurationCollector.h
        source/ConfigurationContext.cpp
        include/weave/profiling/ConfigurationContext.h
        source/ConfigurationDataBuilder.cpp
        include/weave/profiling/ConfigurationDataBuilder.h
        include/weave/profiling/ConfigurationDataNode.h
        source/ConfigurationPrinter.cpp
        source/ConfigurationPrinter.h
        include/weave/profiling/ConfigurationSerialization.h
        include/weave/profiling/Constants.h
        include/weave/profiling/Macros.h
        source/MetricsCollector.cpp
        include/weave/profiling/MetricsCollector.h
        source/MetricsContext.cpp
        include/weave/profiling/MetricsContext.h
        source/MetricsPrinter.cpp
        source/MetricsPrinter.h
        source/MetricsSpan.cpp
        include/weave/profiling/MetricsSpan.h
        include/weave/profiling/MetricsSpanData.h
        source/TraceCollector.cpp
        include/weave/profiling/TraceCollector.h
        source/TraceContext.cpp
        include/weave/profiling/TraceContext.h
        source/TracePrinter.cpp
        source/TracePrinter.h
        source/TraceSpan.cpp
        include/weave/profiling/TraceSpan.h
        include/weave/profiling/TraceSpanDataNode.h
)

target_include_directories(profiling PUBLIC include)
target_link_libraries(profiling PUBLIC logging error)
target_compile_definitions(profiling PUBLIC WEAVE_MONITORING_REPORTS_LOCATION=\"${CMAKE_HOME_DIRECTORY}/Sessions\")
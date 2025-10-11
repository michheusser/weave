add_library(profiling STATIC
        Configuration.h
        ConfigurationCollector.cpp
        include/weave/profiling/ConfigurationCollector.h
        ConfigurationContext.cpp
        include/weave/profiling/ConfigurationContext.h
        ConfigurationDataBuilder.cpp
        include/weave/profiling/ConfigurationDataBuilder.h
        include/weave/profiling/ConfigurationDataNode.h
        ConfigurationPrinter.cpp
        ConfigurationPrinter.h
        include/weave/profiling/ConfigurationSerialization.h
        include/weave/profiling/Constants.h
        include/weave/profiling/Macros.h
        MetricsCollector.cpp
        include/weave/profiling/MetricsCollector.h
        MetricsContext.cpp
        include/weave/profiling/MetricsContext.h
        MetricsPrinter.cpp
        MetricsPrinter.h
        MetricsSpan.cpp
        include/weave/profiling/MetricsSpan.h
        include/weave/profiling/MetricsSpanData.h
        TraceCollector.cpp
        include/weave/profiling/TraceCollector.h
        TraceContext.cpp
        include/weave/profiling/TraceContext.h
        TracePrinter.cpp
        TracePrinter.h
        TraceSpan.cpp
        include/weave/profiling/TraceSpan.h
        include/weave/profiling/TraceSpanDataNode.h
)

target_include_directories(profiling PUBLIC include)
target_link_libraries(profiling PUBLIC logging error)
target_compile_definitions(profiling PUBLIC WEAVE_MONITORING_REPORTS_LOCATION=\"${CMAKE_HOME_DIRECTORY}/Sessions\")
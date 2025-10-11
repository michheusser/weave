// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MACROS_H_2025_09_03_15_42_54
#define MACROS_H_2025_09_03_15_42_54

#include <weave/profiling/TraceContext.h>
#include <weave/profiling/MetricsContext.h>
#include <weave/profiling/ConfigurationContext.h>
#include <weave/profiling/Constants.h>
#include <weave/profiling/ConfigurationDataBuilder.h>
#include <weave/profiling/ConfigurationDataNode.h>

#ifdef WEAVE_ENABLE_MONITORING
		#define TRACE_INIT(sessionName, sessionDescription) weave::profiling::TraceContext::init(sessionName, sessionDescription)
		#define TRACE_SET_FRAME(frameID) weave::profiling::TraceContext::setCurrentFrame(frameID)
		#define TRACE_FUNCTION(className) weave::profiling::TraceSpan traceSpanFrame##__LINE__ = weave::profiling::trace(std::string(className) + "::" + __FUNCTION__)
		#define TRACE_DUMP(directory) weave::profiling::TraceContext::dump(directory)
		#define TRACE_DISPLAY() weave::profiling::TraceContext::display()

		#define METRICS_INIT(sessionName, sessionDescription) weave::profiling::MetricsContext::init(sessionName, sessionDescription)
		#define METRICS_FRAME(className) weave::profiling::MetricsSpan metricsSpanFrame##__LINE__ = weave::profiling::metric(std::string(className) + "::" + __FUNCTION__, std::string(weave::profiling::Constants::METRIC_FRAME_TYPE_STRING), 1)
		#define METRICS_BYTES(className, data) weave::profiling::MetricsSpan metricsSpanData##__LINE__ = weave::profiling::metric(std::string(className) + "::" + __FUNCTION__,std::string(weave::profiling::Constants::METRIC_BYTE_TYPE_STRING), data)
		#define METRICS_DUMP(directory) weave::profiling::MetricsContext::dump(directory)
		#define METRICS_DISPLAY() weave::profiling::MetricsContext::display()

		#define CONFIG_INIT(sessionName, sessionDescription) weave::profiling::ConfigurationContext::init(sessionName, sessionDescription)
		#define CONFIG_ADD(name, config) weave::profiling::ConfigurationContext::addConfiguration(name, config)
		#define CONFIG_DUMP(directory) weave::profiling::ConfigurationContext::dump(directory)
		#define CONFIG_DISPLAY() weave::profiling::ConfigurationContext::display()
		// Since we have no reflections, we use this macro. In C++26 this will change (hopefully)
		#define CONFIG_PAIR(name, value) std::make_pair(name,value)
		#define CONFIG_MAKE_DATA(...) \
		std::vector<std::shared_ptr<weave::profiling::ConfigurationDataNode>> getChildrenDataTrees() const \
		{ \
			return weave::profiling::ConfigurationDataBuilder::buildDataTrees(__VA_ARGS__); \
		}
	#else
		#define TRACE_INIT(sessionName, sessionDescription) ((void)0)
		#define TRACE_SET_FRAME(frameID) ((void)0)
		#define TRACE_FUNCTION(className) ((void)0)
		#define TRACE_DUMP() ((void)0)
		#define TRACE_DISPLAY() ((void)0)

		#define METRICS_INIT(sessionName, sessionDescription) ((void)0)
		#define METRICS_FRAME(worker) ((void)0)
		#define METRICS_BYTES(worker, data) ((void)0)
		#define METRICS_DUMP() ((void)0)

		#define CONFIG_INIT(sessionName, sessionDescription) ((void)0)
		#define CONFIG_ADD(config) ((void)0)
		#define CONFIG_DUMP() ((void)0)
		#define CONFIG_DISPLAY() ((void)0)
		#define CONFIG_PAIR() ((void)0)
		#define CONFIG_MAKE_DATA(...) ((void)0)
	#endif




#endif

// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef DEFINITIONS_H_2025_09_02_12_46_19
#define DEFINITIONS_H_2025_09_02_12_46_19

// Master logging control
#ifndef WEAVE_ENABLE_LOGGING
#ifdef NDEBUG
		#define WEAVE_ENABLE_LOGGING 0
#else
		#define WEAVE_ENABLE_LOGGING 1
#endif
#endif

// Individual level controls for fine-grained optimization
#ifndef WEAVE_ENABLE_TRACE
#define WEAVE_ENABLE_TRACE WEAVE_ENABLE_LOGGING
#endif

#ifndef WEAVE_ENABLE_DEBUG
#define WEAVE_ENABLE_DEBUG WEAVE_ENABLE_LOGGING
#endif

#ifndef WEAVE_ENABLE_INFO
#define WEAVE_ENABLE_INFO WEAVE_ENABLE_LOGGING
#endif

#ifndef WEAVE_ENABLE_WARN
#define WEAVE_ENABLE_WARN WEAVE_ENABLE_LOGGING
#endif

#ifndef WEAVE_ENABLE_ERROR
#define WEAVE_ENABLE_ERROR WEAVE_ENABLE_LOGGING
#endif

#ifndef WEAVE_ENABLE_FATAL
#define WEAVE_ENABLE_FATAL WEAVE_ENABLE_LOGGING
#endif

// Validation: ensure at least ERROR and FATAL are enabled if logging is on
#if WEAVE_ENABLE_LOGGING
#if !WEAVE_ENABLE_ERROR
		#undef WEAVE_ENABLE_ERROR
		#define WEAVE_ENABLE_ERROR 1
		#pragma message("Warning: ERROR logging force-enabled when logging is active")
#endif

#if !WEAVE_ENABLE_FATAL
		#undef WEAVE_ENABLE_FATAL
		#define WEAVE_ENABLE_FATAL 1
		#pragma message("Warning: FATAL logging force-enabled when logging is active")
#endif
#endif

#endif

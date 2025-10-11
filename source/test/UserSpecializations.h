// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef USERSPECIALIZATIONS_H_2025_10_08_15_40_19
#define USERSPECIALIZATIONS_H_2025_10_08_15_40_19

#include "modules/Capturer.h"
#include "modules/Context.h"
#include "modules/Preprocessor.h"
#include "modules/Displayer.h"

#include <weave/user/Module.h>
#include <weave/user/BufferData.h>

// USER DEFINED SPECIALIZATIONS!

// Module Tags
struct ImageCapturer
{};

struct ImageProcessor
{};

struct ImageDisplayer
{};

template <>
class weave::user::Module<ImageCapturer>
{
public:
	using ModuleType = test::module::Capturer;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageCapturer>;
};

template <>
class weave::user::Module<ImageProcessor>
{
public:
	using ModuleType = test::module::Preprocessor;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
};

template <>
class weave::user::Module<ImageDisplayer>
{
public:
	using ModuleType = test::module::Displayer;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
};


// Buffer Tags
struct ImageData
{};


template <>
class weave::user::BufferData<ImageData>
{
	// TODO Buffer stuff
public:
	using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
};

#endif
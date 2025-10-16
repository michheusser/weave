// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULESPECIALIZATIONS_H_2025_10_08_15_40_19
#define MODULESPECIALIZATIONS_H_2025_10_08_15_40_19

/*
#include "../modules/Capturer.h"
#include "../modules/Context.h"
#include "../modules/Preprocessor.h"
#include "../modules/Displayer.h"

#include <weave/user/Module.h>

// USER DEFINED SPECIALIZATIONS!

// Node Tags (Module)
struct FirstImageCapturer
{};

struct FirstImageProcessor
{};

struct FirstImageDisplayer
{};

template <>
class weave::user::Module<FirstImageCapturer>
{
public:
	using ModuleType = test::module::Capturer;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageCapturer>;
};

template <>
class weave::user::Module<FirstImageProcessor>
{
public:
	using ModuleType = test::module::Preprocessor;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
};

template <>
class weave::user::Module<FirstImageDisplayer>
{
public:
	using ModuleType = test::module::Displayer;
	using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
};

*/
#endif
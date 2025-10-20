// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00
#define PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00

#include <weave/user/ProcessorTraits.h>
#include "../modules/Context.h"
#include "../modules/Capturer.h"
#include "../modules/ClientReceiver.h"
#include "../modules/ClientSender.h"
#include "../modules/Preprocessor.h"
#include "../modules/Displayer.h"
#include "../modules/InferenceInputProcessor.h"
#include "../modules/InferenceModel.h"
#include "../modules/ServerReceiver.h"
#include "../modules/ServerSender.h"

struct DefaultCapturerProcessor
{};

struct DefaultNormalizerProcessor
{};

struct DefaultClientSenderProcessor
{};

struct DefaultServerReceiverProcessor
{};

struct DefaultInferenceInputProcessorProcessor
{};

struct DefaultInferenceModelProcessor
{};

struct DefaultServerSenderProcessor
{};

struct DefaultClientReceiverProcessor
{};

struct DefaultDisplayerProcessor
{};

namespace weave
{
	namespace user
	{
		template<>
		struct ProcessorTraits<DefaultCapturerProcessor>
		{
			using ProcessorTag = DefaultCapturerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageCapturer>;
			using ModuleType = test::module::Capturer;
		};

		template<>
		struct ProcessorTraits<DefaultNormalizerProcessor>
		{
			using ProcessorTag = DefaultNormalizerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
			using ModuleType = test::module::Preprocessor;
		};

		template<>
		struct ProcessorTraits<DefaultClientSenderProcessor>
		{
			using ProcessorTag = DefaultClientSenderProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ClientSender>;
			using ModuleType = test::module::ClientSender;
		};

		template<>
		struct ProcessorTraits<DefaultServerReceiverProcessor>
		{
			using ProcessorTag = DefaultServerReceiverProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ServerReceiver>;
			using ModuleType = test::module::ServerReceiver;
		};

		template<>
		struct ProcessorTraits<DefaultInferenceInputProcessorProcessor>
		{
			using ProcessorTag = DefaultInferenceInputProcessorProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::InferenceInputProcessor>;
			using ModuleType = test::module::InferenceInputProcessor;
		};

		template<>
		struct ProcessorTraits<DefaultInferenceModelProcessor>
		{
			using ProcessorTag = DefaultInferenceModelProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::InferenceModel>;
			using ModuleType = test::module::InferenceModel;
		};

		template<>
		struct ProcessorTraits<DefaultServerSenderProcessor>
		{
			using ProcessorTag = DefaultServerSenderProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ServerSender>;
			using ModuleType = test::module::ServerSender;
		};

		template<>
		struct ProcessorTraits<DefaultClientReceiverProcessor>
		{
			using ProcessorTag = DefaultClientReceiverProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ClientReceiver>;
			using ModuleType = test::module::ClientReceiver;
		};

		template<>
		struct ProcessorTraits<DefaultDisplayerProcessor>
		{
			using ProcessorTag = DefaultDisplayerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
			using ModuleType = test::module::Displayer;
		};
	}
}

#endif

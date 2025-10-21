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

struct ClientImageCapturer
{};

struct ClientFirstImageNormalizer
{};

struct ClientImageSender
{};

struct ClientImageReceiver
{};

struct ClientSecondImageNormalizer
{};

struct ClientImageDisplayer
{};


struct ServerImageReceiver
{};

struct ServerInferenceInputProcessor
{};

struct ServerInferenceModel
{};

struct ServerImageSender
{};


namespace weave
{
	namespace user
	{
		// CLIENT

		template<>
		struct ProcessorTraits<ClientImageCapturer>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageCapturer>;
			using ModuleType = test::module::Capturer;
		};

		template<>
		struct ProcessorTraits<ClientFirstImageNormalizer>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
			using ModuleType = test::module::Preprocessor;
		};

		template<>
		struct ProcessorTraits<ClientImageSender>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ClientSender>;
			using ModuleType = test::module::ClientSender;
		};

		template<>
		struct ProcessorTraits<ClientImageReceiver>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ClientReceiver>;
			using ModuleType = test::module::ClientReceiver;
		};

		template<>
		struct ProcessorTraits<ClientSecondImageNormalizer>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
			using ModuleType = test::module::Preprocessor;
		};

		template<>
		struct ProcessorTraits<ClientImageDisplayer>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
			using ModuleType = test::module::Displayer;
		};

		// SERVER
		template<>
		struct ProcessorTraits<ServerImageReceiver>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ServerReceiver>;
			using ModuleType = test::module::ServerReceiver;
		};

		template<>
		struct ProcessorTraits<ServerInferenceInputProcessor>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::InferenceInputProcessor>;
			using ModuleType = test::module::InferenceInputProcessor;
		};

		template<>
		struct ProcessorTraits<ServerInferenceModel>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::InferenceModel>;
			using ModuleType = test::module::InferenceModel;
		};

		template<>
		struct ProcessorTraits<ServerImageSender>
		{
			using ContextType = test::module::Context<test::module::constants::ModuleType::ServerSender>;
			using ModuleType = test::module::ServerSender;
		};
	}
}

#endif

// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SYNCHRONIZERTRAITSSPECIALIZATION_H_2025_10_14_18_37_11
#define SYNCHRONIZERTRAITSSPECIALIZATION_H_2025_10_14_18_37_11

#include <weave/user/SynchronizerTraits.h>
#include "ProcessorTraitsSpecialization.h"
#include "../modules/Context.h"

struct DefaultCapturerSynchronizer
{};

struct DefaultNormalizerSynchronizer
{};

struct DefaultClientSenderSynchronizer
{};

struct DefaultServerReceiverSynchronizer
{};

struct DefaultInferenceInputProcessorSynchronizer
{};

struct DefaultInferenceModelSynchronizer
{};

struct DefaultServerSenderSynchronizer
{};

struct DefaultClientReceiverSynchronizer
{};

struct DefaultDisplayerSynchronizer
{};

namespace weave
{
	namespace user
	{
		template<>
		struct SynchronizerTraits<DefaultCapturerSynchronizer>
		{
			using ProcessorTag = DefaultCapturerProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultNormalizerSynchronizer>
		{
			using ProcessorTag = DefaultNormalizerProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultClientSenderSynchronizer>
		{
			using ProcessorTag = DefaultClientSenderProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultServerReceiverSynchronizer>
		{
			using ProcessorTag = DefaultServerReceiverProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultInferenceInputProcessorSynchronizer>
		{
			using ProcessorTag = DefaultInferenceInputProcessorProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultInferenceModelSynchronizer>
		{
			using ProcessorTag = DefaultInferenceModelProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultServerSenderSynchronizer>
		{
			using ProcessorTag = DefaultServerSenderProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultClientReceiverSynchronizer>
		{
			using ProcessorTag = DefaultClientReceiverProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};

		template<>
		struct SynchronizerTraits<DefaultDisplayerSynchronizer>
		{
			using ProcessorTag = DefaultDisplayerProcessor;
			using ContextType = ProcessorTraits<ProcessorTag>::ContextType;
		};
	}
}
#endif

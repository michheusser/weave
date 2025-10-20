// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef WORKERTRAITSSPECIALIZATION_H_2025_10_14_18_17_53
#define WORKERTRAITSSPECIALIZATION_H_2025_10_14_18_17_53

#include <weave/user/WorkerTraits.h>
#include "SynchronizerTraitsSpecialization.h"
#include "../modules/Context.h"

struct DefaultCapturerWorker
{};

struct DefaultNormalizerWorker
{};

struct DefaultClientSenderWorker
{};

struct DefaultServerReceiverWorker
{};

struct DefaultInferenceInputProcessorWorker
{};

struct DefaultInferenceModelWorker
{};

struct DefaultServerSenderWorker
{};

struct DefaultClientReceiverWorker
{};

struct DefaultDisplayerWorker
{};

namespace weave
{
	namespace user
	{
		template<>
		struct WorkerTraits<DefaultCapturerWorker>
		{
			using SynchronizerTag = DefaultCapturerSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultNormalizerWorker>
		{
			using SynchronizerTag = DefaultNormalizerSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultClientSenderWorker>
		{
			using SynchronizerTag = DefaultClientSenderSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultServerReceiverWorker>
		{
			using SynchronizerTag = DefaultServerReceiverSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultInferenceInputProcessorWorker>
		{
			using SynchronizerTag = DefaultInferenceInputProcessorSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultInferenceModelWorker>
		{
			using SynchronizerTag = DefaultInferenceModelSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultServerSenderWorker>
		{
			using SynchronizerTag = DefaultServerSenderSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultClientReceiverWorker>
		{
			using SynchronizerTag = DefaultClientReceiverSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};

		template<>
		struct WorkerTraits<DefaultDisplayerWorker>
		{
			using SynchronizerTag = DefaultDisplayerSynchronizer;
			using ContextType = SynchronizerTraits<SynchronizerTag>::ContextType;
		};
	}
}

#endif

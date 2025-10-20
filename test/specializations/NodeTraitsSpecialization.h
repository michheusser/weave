// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODETRAITSSPECIALIZATION_H_2025_10_14_18_19_14
#define NODETRAITSSPECIALIZATION_H_2025_10_14_18_19_14

#include <weave/user/NodeTraits.h>
#include "WorkerTraitsSpecialization.h"
#include "../modules/Context.h"

struct ClientImageCapturerNode
{};

struct ClientFirstImageNormalizerNode
{};

struct ClientImageSenderNode
{};

struct ClientImageReceiverNode
{};

struct ClientSecondImageNormalizerNode
{};

struct ClientImageDisplayerNode
{};


struct ServerImageReceiverNode
{};

struct ServerInferenceInputProcessorNode
{};

struct ServerInferenceModelNode
{};

struct ServerImageSenderNode
{};


namespace weave
{
	namespace user
	{
		template<>
		struct NodeTraits<ClientImageCapturerNode>
		{
			using WorkerTag = DefaultCapturerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ClientFirstImageNormalizerNode>
		{
			using WorkerTag = DefaultNormalizerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ClientImageSenderNode>
		{
			using WorkerTag = DefaultClientSenderWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ClientImageReceiverNode>
		{
			using WorkerTag = DefaultClientReceiverWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ClientSecondImageNormalizerNode>
		{
			using WorkerTag = DefaultNormalizerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ClientImageDisplayerNode>
		{
			using WorkerTag = DefaultDisplayerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ServerImageReceiverNode>
		{
			using WorkerTag = DefaultServerReceiverWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ServerInferenceInputProcessorNode>
		{
			using WorkerTag = DefaultInferenceInputProcessorWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ServerInferenceModelNode>
		{
			using WorkerTag = DefaultInferenceModelWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<ServerImageSenderNode>
		{
			using WorkerTag = DefaultServerSenderWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};
	}
}

#endif

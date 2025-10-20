// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODETRAITSSPECIALIZATION_H_2025_10_14_18_19_14
#define NODETRAITSSPECIALIZATION_H_2025_10_14_18_19_14

#include <weave/user/NodeTraits.h>
#include "WorkerTraitsSpecialization.h"
#include "../modules/Context.h"

struct FirstCapturerNode
{};

struct FirstNormalizerNode
{};

struct FirstClientSenderNode
{};

struct FirstServerReceiverNode
{};

struct FirstInferenceInputProcessorNode
{};

struct FirstInferenceModelNode
{};

struct FirstServerSenderNode
{};

struct FirstClientReceiverNode
{};

struct SecondNormalizerNode
{};

struct FirstDisplayerNode
{};

namespace weave
{
	namespace user
	{
		template<>
		struct NodeTraits<FirstCapturerNode>
		{
			using WorkerTag = DefaultCapturerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstNormalizerNode>
		{
			using WorkerTag = DefaultNormalizerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstClientSenderNode>
		{
			using WorkerTag = DefaultClientSenderWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstServerReceiverNode>
		{
			using WorkerTag = DefaultServerReceiverWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstInferenceInputProcessorNode>
		{
			using WorkerTag = DefaultInferenceInputProcessorWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstInferenceModelNode>
		{
			using WorkerTag = DefaultInferenceModelWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstServerSenderNode>
		{
			using WorkerTag = DefaultServerSenderWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstClientReceiverNode>
		{
			using WorkerTag = DefaultClientReceiverWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<SecondNormalizerNode>
		{
			using WorkerTag = DefaultNormalizerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstDisplayerNode>
		{
			using WorkerTag = DefaultDisplayerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};
	}
}

#endif

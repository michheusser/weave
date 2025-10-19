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

struct FirstDisplayerNode
{};

struct FirstNormalizerNode
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
		struct NodeTraits<FirstDisplayerNode>
		{
			using WorkerTag = DefaultDisplayerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};

		template<>
		struct NodeTraits<FirstNormalizerNode>
		{
			using WorkerTag = DefaultNormalizerWorker;
			using ContextType = WorkerTraits<WorkerTag>::ContextType;
		};
	}
}

#endif

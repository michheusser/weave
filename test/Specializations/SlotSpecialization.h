// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SLOTSPECIALIZATION_H_2025_10_13_18_14_08
#define SLOTSPECIALIZATION_H_2025_10_13_18_14_08

#include <vector>
#include <opencv2/core/mat.hpp>
#include <weave/user/Slot.h>
#include "../buffer/Context.h"

struct ImageSlot
{};

// Defines a type of slot to be reused by multiple channels
namespace weave
{
	namespace user
	{
		template<>
		class Slot<ImageSlot>
		{
		public: // TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ImageSlot;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};
	}
}
#endif

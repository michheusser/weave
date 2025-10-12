// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef INTERNALDATA_H_2025_09_30_16_59_17
#define INTERNALDATA_H_2025_09_30_16_59_17
#include <vector>

#include <weave/buffer/Constants.h>
#include <weave/buffer/Traits.h>

namespace weave
{
	namespace buffer
	{
		template <Constants::BufferType type>
		class InternalData; // Cannot be instantiated as generic

		template<>
		class InternalData<Constants::BufferType::Image>
		{
		public:
			static constexpr Constants::BufferType type = Constants::BufferType::Image;
			using StorageType = typename Traits<type>::StorageType;
			InternalData();
			void initialize(const typename Traits<type>::ConfigurationType& configuration);
			StorageType& data();
		private:
			StorageType _data;
		};

		template<>
		class InternalData<Constants::BufferType::Inference>
		{
		public:
			static constexpr Constants::BufferType type = Constants::BufferType::Inference;
			using StorageType = typename Traits<type>::StorageType;
			InternalData();
			void initialize(const typename Traits<type>::ConfigurationType& configuration);
			StorageType& data();
		private:
			StorageType _data;
			std::vector<float> _internalData;
		};
	}
}

#endif

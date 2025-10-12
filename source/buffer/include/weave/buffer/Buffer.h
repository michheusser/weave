// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUFFER_H_2025_09_17_11_14_09
#define BUFFER_H_2025_09_17_11_14_09

#include <weave/buffer/Traits.h>
#include <weave/buffer/Reader.h>
#include <weave/buffer/Writer.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/QueueBuffer.h>

namespace weave
{
	namespace buffer
	{
		template <Constants::BufferType type, Constants::PolicyType policy>
		class Buffer
		{
		public:
			using StorageType = typename Traits<type>::StorageType;
			static constexpr Constants::PolicyType policyType = policy;

			explicit Buffer()
			{}

			void initialize(const typename Traits<type>::ConfigurationType& configuration)
			{
				_queueBuffer.initialize(configuration);
			}

			Reader<type, policy> reader() noexcept
			{
				return Reader<type, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _queueBuffer);
			}

			Writer<type, policy> writer() noexcept
			{
				return Writer<type, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _queueBuffer);
			}

		private:
			mutable std::shared_mutex _mutex;
			mutable std::condition_variable_any _conditionVariableRead;
			mutable std::condition_variable_any _conditionVariableWrite;
			QueueBuffer<type> _queueBuffer;
		};
	}
}

#endif

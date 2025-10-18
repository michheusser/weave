// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSOR_H_2025_09_20_15_34_46
#define PROCESSOR_H_2025_09_20_15_34_46

#include <weave/user/ProcessorTraits.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace worker
	{
		template<typename ProcessorTag>
		class Processor
		{
		public:
			using ModuleType = typename user::ProcessorTraits<ProcessorTag>::ModuleType;
			explicit Processor(const typename user::ProcessorTraits<ProcessorTag>::ContextType& context) : _module(context.configuration)
			{}
			void initialize()
			{
				_module.initialize();
			}
			template<typename InputDataTupleType, typename OutputDataTupleType>
			error::Result process(InputDataTupleType& inputDataTuple, OutputDataTupleType& outputDataTuple) noexcept
			{
				error::Result result = _process(inputDataTuple, std::make_index_sequence<std::tuple_size_v<InputDataTupleType> >{}, outputDataTuple, std::make_index_sequence<std::tuple_size_v<OutputDataTupleType> >{});
				if (!result.ok())
				{
					LOG_ERROR("Displayer: Error in processing.");
					return {error::Type::Processing, 0};
				}
				return error::Result::success();
			}

		private:
			template<typename InputDataTupleType, size_t... InputIndices, typename OutputDataTupleType, size_t... OutputIndices>
			error::Result _process(InputDataTupleType& inputDataTuple, std::index_sequence<InputIndices...> inputSequence, OutputDataTupleType& outputDataTuple, std::index_sequence<OutputIndices...> outputSequence)
			{
				error::Result result = _module.process(std::get<InputIndices>(inputDataTuple)..., std::get<OutputIndices>(outputDataTuple)...);
				return result;
			}
			ModuleType _module;
		};
	};
}

#endif

// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSOR_H_2025_09_20_15_34_46
#define PROCESSOR_H_2025_09_20_15_34_46

#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace worker
	{
		template<typename T>
		struct ShowType;

		template<typename ProcessorTag, typename ModuleType> // TODO Probably only needs moduletype and not processortag
		class Processor
		{
		public:
			static constexpr std::string_view name = utilities::typeName<ProcessorTag>();
			using ContextType = typename ModuleType::ContextType;
			explicit Processor(const ContextType& context) : _module(context)
			{}
			void initialize()
			{
				try
				{
					_module.initialize();
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed.");
				}
			}
			template<typename InputDataTupleType, typename OutputDataTupleType>
			error::Result process(InputDataTupleType& inputDataTuple, OutputDataTupleType& outputDataTuple) noexcept
			{
				TRACE_FUNCTION("Process");
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
			error::Result _process(InputDataTupleType& inputDataTuple, std::index_sequence<InputIndices...> inputSequence, OutputDataTupleType& outputDataTuple, std::index_sequence<OutputIndices...> outputSequence) noexcept
			{
				error::Result result = _module.process(std::get<InputIndices>(inputDataTuple)..., std::get<OutputIndices>(outputDataTuple)...);
				return result;
			}
			ModuleType _module;
		};
	};
}

#endif

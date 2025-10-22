// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SYNCHRONIZER_H_2025_09_20_15_53_09
#define SYNCHRONIZER_H_2025_09_20_15_53_09

#include <iostream>
#include <weave/worker/Processor.h>
#include <weave/error/Result.h>

namespace weave
{
	namespace worker
	{
		template<typename ChannelsTupleType>
		struct ChannelsTupleToDataAccessTuple;
		template<typename... Channels>
		struct ChannelsTupleToDataAccessTuple<std::tuple<Channels&...> >
		{
			using ReaderTuple = std::tuple<buffer::Reader<typename Channels::Tag, Channels::policyType, Channels::slots>...>;
			using WriterTuple = std::tuple<buffer::Writer<typename Channels::Tag, Channels::policyType, Channels::slots>...>;
			using ReaderDataTuple = std::tuple<const typename buffer::Reader<typename Channels::Tag, Channels::policyType, Channels::slots>::StorageType&...>;
			using WriterDataTuple = std::tuple<typename buffer::Writer<typename Channels::Tag, Channels::policyType, Channels::slots>::StorageType&...>;
		};

		template<typename SynchronizerTag>
		class Synchronizer
		{
		public:
			using ProcessorTag = SynchronizerTag;
			using ContextType = typename Processor<ProcessorTag>::ContextType;
			explicit Synchronizer(const ContextType& context) : _processor(context)
			{}

			void initialize()
			{
				try
				{
					_processor.initialize();
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed.");
				}
			}

			template<typename InChannelTupleType, typename OutChannelTupleType>
			error::Result cycle(InChannelTupleType& inChannelTuple, OutChannelTupleType& outChannelTuple) noexcept
			{
				// Readers / Writers don't need to be references (they contain references)
				using ReadersTupleType = typename ChannelsTupleToDataAccessTuple<InChannelTupleType>::ReaderTuple;
				ReadersTupleType inputReaders = _getChannelsTupleReaders<ReadersTupleType>(inChannelTuple, std::make_index_sequence<std::tuple_size_v<InChannelTupleType> >());

				using WritersTupleType = typename ChannelsTupleToDataAccessTuple<OutChannelTupleType>::WriterTuple;
				WritersTupleType outputWriters = _getChannelsTupleWriters<WritersTupleType>(outChannelTuple, std::make_index_sequence<std::tuple_size_v<OutChannelTupleType> >());

				using ReadersDataTupleType = typename ChannelsTupleToDataAccessTuple<InChannelTupleType>::ReaderDataTuple;
				ReadersDataTupleType inputReadersDataTuple = _getReadersTupleData<ReadersDataTupleType>(inputReaders, std::make_index_sequence<std::tuple_size_v<ReadersTupleType> >());

				using WritersDataTupleType = typename ChannelsTupleToDataAccessTuple<OutChannelTupleType>::WriterDataTuple;
				WritersDataTupleType outputWritersDataTuple = _getWritersTupleData<WritersDataTupleType>(outputWriters, std::make_index_sequence<std::tuple_size_v<WritersTupleType> >());

				bool inputReadersActive = _active(inputReaders, std::make_index_sequence<std::tuple_size_v<ReadersTupleType> >());
				bool outputWritersActive = _active(outputWriters, std::make_index_sequence<std::tuple_size_v<WritersTupleType> >());
				if (inputReadersActive && outputWritersActive) // TODO Really necessary?
				{
					error::Result result = _processor.process(inputReadersDataTuple, outputWritersDataTuple);
					if (!result.ok())
					{
						return {error::Type::Processing, 0};
					}
					_release(inputReaders, std::make_index_sequence<std::tuple_size_v<ReadersTupleType> >());
					_publish(outputWriters, std::make_index_sequence<std::tuple_size_v<WritersTupleType> >());
				}
				return error::Result::success();
			}

		private:
			// Functions are inlined since we're on function definition. Also, by constructing in return statements we have copy-elision.
			template<typename ReadersTupleType, typename ChannelTupleType, size_t... Indices>
			ReadersTupleType _getChannelsTupleReaders(ChannelTupleType& channelTuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				return ReadersTupleType(std::get<Indices>(channelTuple).reader()...);
			}

			template<typename WritersTupleType, typename ChannelTupleType, size_t... Indices>
			WritersTupleType _getChannelsTupleWriters(ChannelTupleType& channelTuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				return WritersTupleType(std::get<Indices>(channelTuple).writer()...);
			}

			template<typename ReadersDataTupleType, typename ReadersTupleType, size_t... Indices>
			ReadersDataTupleType _getReadersTupleData(ReadersTupleType& readersTuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				return ReadersDataTupleType(std::get<Indices>(readersTuple).data()...);
			}

			template<typename WritersDataTupleType, typename WritersTupleType, size_t... Indices>
			WritersDataTupleType _getWritersTupleData(WritersTupleType& writersTuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				return WritersDataTupleType(std::get<Indices>(writersTuple).data()...);
			}

			template<typename TupleType, size_t... Indices>
			bool _active(TupleType& tuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				return (std::get<Indices>(tuple).active() && ...);
			}

			template<typename TupleType, size_t... Indices>
			void _release(TupleType& tuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				(std::get<Indices>(tuple).release(), ...);
			}

			template<typename TupleType, size_t... Indices>
			void _publish(TupleType& tuple, std::index_sequence<Indices...> sequence) const noexcept
			{
				int frameID = 0; // TODO Deal with frame
				(std::get<Indices>(tuple).publish(frameID), ...);
			}

			Processor<ProcessorTag> _processor;
		};
	}
}

#endif

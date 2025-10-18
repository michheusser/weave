// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SYNCHRONIZER_H_2025_09_20_15_53_09
#define SYNCHRONIZER_H_2025_09_20_15_53_09

#include <iostream>
#include <weave/worker/Processor.h>
#include <weave/user/SynchronizerTraits.h>
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
			using ReaderTuple = std::tuple<buffer::Reader<typename Channels::Tag, Channels::policyType>...>;
			using WriterTuple = std::tuple<buffer::Writer<typename Channels::Tag, Channels::policyType>...>;
			using ReaderDataTuple = std::tuple<const typename buffer::Reader<typename Channels::Tag, Channels::policyType>::StorageType&...>;
			using WriterDataTuple = std::tuple<typename buffer::Writer<typename Channels::Tag, Channels::policyType>::StorageType&...>;
		};

		template<typename SynchronizerTag>
		class Synchronizer
		{
		public:
			// TODO Strategy: Make generic, list of inbuffers, list of outbuffers and then assert for maximum one of each and at least one of either (static_assert)
			using ProcessorTag = typename user::SynchronizerTraits<SynchronizerTag>::ProcessorTag;
			explicit Synchronizer(const typename user::SynchronizerTraits<SynchronizerTag>::ContextType& context) : _processor(context)
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

			template<typename InChannelTupleType, typename OutChannelTupleType> // TODO Make sure tuple contains references
			error::Result cycle(InChannelTupleType& inChannelTuple, OutChannelTupleType& outChannelTuple) noexcept
			{
				// Readers / Writers don't need to be references (they contain references)
				using ReadersTupleType = typename ChannelsTupleToDataAccessTuple<InChannelTupleType>::ReaderTuple;
				using WritersTupleType = typename ChannelsTupleToDataAccessTuple<OutChannelTupleType>::WriterTuple;

				using ReadersDataTupleType = typename ChannelsTupleToDataAccessTuple<InChannelTupleType>::ReaderDataTuple;
				using WritersDataTupleType = typename ChannelsTupleToDataAccessTuple<OutChannelTupleType>::WriterDataTuple;

				ReadersTupleType inputReaders = _getChannelsTupleReaders<ReadersTupleType>(inChannelTuple, std::make_index_sequence<std::tuple_size_v<InChannelTupleType> >());
				WritersTupleType outputWriters = _getChannelsTupleWriters<WritersTupleType>(outChannelTuple, std::make_index_sequence<std::tuple_size_v<OutChannelTupleType> >());

				ReadersDataTupleType inputReadersDataTuple = _getReadersTupleData<ReadersDataTupleType>(inputReaders, std::make_index_sequence<std::tuple_size_v<ReadersTupleType> >());
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
			// TODO Optimize and see if possible not to pass object, but just do with template
			template<typename ReadersTupleType, typename ChannelTupleType, size_t... Indices>
			ReadersTupleType _getChannelsTupleReaders(ChannelTupleType& channelTuple, std::index_sequence<Indices...> sequence)
			{
				// return ReadersTupleType(std::get<Indices>(channelTuple).reader()...); // TODO Change to this to avoid copy
				ReadersTupleType readersTuple = {std::get<Indices>(channelTuple).reader()...}; // Uses move semantics for correct RAII behavior.
				return readersTuple;
			}

			template<typename WritersTupleType, typename ChannelTupleType, size_t... Indices>
			WritersTupleType _getChannelsTupleWriters(ChannelTupleType& channelTuple, std::index_sequence<Indices...> sequence)
			{
				// return WritersTupleType(std::get<Indices>(channelTuple).writer()...);  // TODO Change to this to avoid copy
				WritersTupleType writersTuple = {std::get<Indices>(channelTuple).writer()...}; // Uses move semantics for correct RAII behavior.
				return writersTuple;
			}

			template<typename ReadersDataTupleType, typename ReadersTupleType, size_t... Indices>
			ReadersDataTupleType _getReadersTupleData(ReadersTupleType& readersTuple, std::index_sequence<Indices...> sequence)
			{
				ReadersDataTupleType dataTuple = {std::get<Indices>(readersTuple).data()...};
				return dataTuple;
			}

			template<typename WritersDataTupleType, typename WritersTupleType, size_t... Indices>
			WritersDataTupleType _getWritersTupleData(WritersTupleType& writersTuple, std::index_sequence<Indices...> sequence)
			{
				WritersDataTupleType dataTuple = {std::get<Indices>(writersTuple).data()...};
				return dataTuple;
			}

			template<typename TupleType, size_t... Indices>
			bool _active(TupleType& tuple, std::index_sequence<Indices...> sequence)
			{
				bool allActive = (std::get<Indices>(tuple).active() && ...);
				return allActive;
			}

			template<typename TupleType, size_t... Indices>
			void _release(TupleType& tuple, std::index_sequence<Indices...> sequence)
			{
				(std::get<Indices>(tuple).release(), ...);
			}

			template<typename TupleType, size_t... Indices>
			void _publish(TupleType& tuple, std::index_sequence<Indices...> sequence)
			{
				int frameID = 0; // TODO Deal with frame
				(std::get<Indices>(tuple).publish(frameID), ...);
			}

			Processor<ProcessorTag> _processor;
		};
	}
}

#endif

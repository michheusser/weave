// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SYNCHRONIZER_H_2025_09_20_15_53_09
#define SYNCHRONIZER_H_2025_09_20_15_53_09

#include <weave/buffer/Constants.h>
#include <weave/buffer/Traits.h>
#include <weave/buffer/Processor.h>
#include <weave/buffer/Context.h>
#include <weave/error/Result.h>

namespace weave
{
	worker
	{
		namespace Worker
		{
			template <Constants::Module module, Constants::Type type>
			class SynchronizerImpl; // Never instanciated as generic

			template <Constants::Module module>
			class SynchronizerImpl<module, Constants::Type::Source>
			{
			public:
				explicit SynchronizerImpl(const typename Module::Context<module>& context): _processor(context)
				{}

				void initialize()
				{
					try
					{
						LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", initializing...");
						_processor.initialize();
					}
					catch (Error::Exception& exception)
					{
						LOG_ERROR(exception.what());
						throw Error::Exception("Failed.");
					}
				}

				template <typename... BufferRefs>
				Error::Result cycle(BufferRefs&... buffers) noexcept
				{
					TRACE_FUNCTION("Module: " + std::string(Module::Traits<module>::name));
					METRICS_FRAME("Module: " + std::string(Module::Traits<module>::name));
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", starting cycle...");
					auto bufferTuple = std::tie(buffers...); // TODO Use static_assert to ensure types and numBuffers match
					buffer::Buffer<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]>& outputBuffer = std::get<0>(bufferTuple);
					buffer::Writer<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]> outputWriter = outputBuffer.writer();
					if (outputWriter.active())
					{
						uint32_t frameID;
						Error::Result result = _processor.process(outputWriter.data(), &frameID);
						if (!result.ok())
						{
							LOG_ERROR("Synchronizer: Error in processing.");
							return {Error::Type::Processing, 0};
						}
						TRACE_SET_FRAME(frameID);
						outputWriter.publish(frameID);
					}
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", cycle finished.");
					return Error::Result::success();
				}

				Processor<module> _processor;
			};

			template <Constants::Module module>
			class SynchronizerImpl<module, Constants::Type::Process>
			{
			public:
				explicit SynchronizerImpl(const typename Module::Context<module>& context): _processor(context)
				{}

				void initialize()
				{
					try
					{
						LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", initializing...");
						_processor.initialize();
					}
					catch (Error::Exception& exception)
					{
						LOG_ERROR(exception.what());
						throw Error::Exception("Failed.");
					}
				}

				template <typename... BufferRefs>
				Error::Result cycle(BufferRefs&... buffers) noexcept
				{
					TRACE_FUNCTION("Module: " + std::string(Module::Traits<module>::name));
					METRICS_FRAME("Module: " + std::string(Module::Traits<module>::name));
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", starting cycle...");
					auto bufferTuple = std::tie(buffers...);
					buffer::Buffer<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]>& inputBuffer = std::get<0>(bufferTuple);
					buffer::Buffer<Module::Traits<module>::buffers[1], Module::Traits<module>::bufferPolicies[1]>& outputBuffer = std::get<1>(bufferTuple);
					buffer::Reader<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]> inputReader = inputBuffer.reader();
					buffer::Writer<Module::Traits<module>::buffers[1], Module::Traits<module>::bufferPolicies[1]> outputWriter = outputBuffer.writer();

					if (inputReader.active() && outputWriter.active())
					{
						Error::Result result = _processor.process(inputReader.data(), outputWriter.data());
						if (!result.ok())
						{
							LOG_ERROR("Synchronizer: Error in processing.");
							return {Error::Type::Processing, 0};
						}
						uint32_t frameID = inputReader.getFrame();
						TRACE_SET_FRAME(frameID);
						inputReader.release();
						outputWriter.publish(frameID);
					}
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", cycle finished.");
					return Error::Result::success();
				}
				Processor<module> _processor;
			};

			template <Constants::Module module>
			class SynchronizerImpl<module, Constants::Type::Sink>
			{
			public:
				explicit SynchronizerImpl(const typename Module::Context<module>& context) : _processor(context)
				{}

				void initialize()
				{
					try
					{
						LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", initializing...");
						_processor.initialize();
					}
					catch (Error::Exception& exception)
					{
						LOG_ERROR(exception.what());
						throw Error::Exception("Failed.");
					}
				}

				template <typename... BufferRefs>
				Error::Result cycle(BufferRefs&... buffers) noexcept
				{
					TRACE_FUNCTION("Module: " + std::string(Module::Traits<module>::name));
					METRICS_FRAME("Module: " + std::string(Module::Traits<module>::name));
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", starting cycle...");
					auto bufferTuple = std::tie(buffers...);
					buffer::Buffer<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]>& inputBuffer = std::get<0>(bufferTuple);
					buffer::Reader<Module::Traits<module>::buffers[0], Module::Traits<module>::bufferPolicies[0]> inputReader = inputBuffer.reader();
					if (inputReader.active())
					{
						// A pattern that appears in pipeline/sequential kind of frameworks: One of the operations happens in one step (client send/receive, or here set frameID/publish)
						// and the counterpart is split since it happens at the beginning and end (server send/receive, or here getFrame, and release).
						uint32_t frameID = inputReader.getFrame();
						TRACE_SET_FRAME(frameID);
						Error::Result result = _processor.process(inputReader.data(), frameID);
						if (!result.ok())
						{
							LOG_ERROR("Synchronizer: Error in processing.");
							return {Error::Type::Processing, 0};
						}
						inputReader.release(); // Frame "dies" here since it's a sink. Return value not used
					}
					LOG_TRACE("Module: " + std::string(Module::Traits<module>::name) + ", cycle finished.");

					return Error::Result::success();
				}

				Processor<module> _processor;
			};

			template <Constants::Module module>
			using Synchronizer = SynchronizerImpl<module, Module::Traits<module>::type>;
		}
	}
}

#endif

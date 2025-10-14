#ifndef CONSTANTS_H_2025_09_17_11_15_36
#define CONSTANTS_H_2025_09_17_11_15_36

namespace weave
{
	namespace buffer
	{
		namespace constants
		{
			/*enum class BufferType // TODO Remove
			{
				Image,
				Network,
				Inference
			};*/

			enum class BufferState
			{
				Empty,
				Full
			};

			enum class WriterState
			{
				Active,
				Published,
				Discarded
			};

			enum class ReaderState
			{
				Active,
				Released
			};

			// General philosophy: DropOldest where latency is critical, Backpressure where correctness is critical, Timed strategies where absorption of transient behavior is critical
			enum class PolicyType
			{
				Lossless,
				Realtime,
				LiveStream,
				Throttled,
				Attempts
			};

			enum class CongestionStrategy
			{
				Backpressure,
				DropNewest,
				DropOldest
			};

			enum class TimeoutStrategy
			{
				Infinite,
				Timed,
				Attempts
			};
		}
	}
}

#endif

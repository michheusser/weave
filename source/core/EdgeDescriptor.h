// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGEDESCRIPTOR_H_2025_10_06_15_53_24
#define EDGEDESCRIPTOR_H_2025_10_06_15_53_24

namespace weave
{
		template<typename BufferDataTag, typename FromModuleTag, typename ToModuleTag>
		class EdgeDescriptor
		{
		public:
			using Tag = BufferDataTag;
			using FromModule = FromModuleTag;
			using ToModule = ToModuleTag;
		};
}

#endif

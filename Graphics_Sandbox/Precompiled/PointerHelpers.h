#pragma once

#include <algorithm>

#include "PrecompilationMacros.h"

BEGIN_NAMESPACE1(pointer)

// Allocates memory and copies array to new address
template <typename PtrType>
static PtrType* CopyArray(const PtrType* source, int arraySize) 
{
	PtrType* p = new PtrType[arraySize];
	return (PtrType*)std::memcpy(p, source, sizeof(PtrType) * arraySize);
}

template <typename PtrType>
static void FreeArray(const PtrType* source) 
{
	delete source;
	source = nullptr;
}

END_NAMESPACE1;
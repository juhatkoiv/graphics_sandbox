#pragma once

#include <concepts>
#include <cinttypes>

#include "Rendering/RenderingPrecompilationMacros.h"

enum class BufferUsage : unsigned
{
	UBO = 0,
	SSBO = 1,
	None = ~0u,
};

struct GfxBufferImpl;

template <typename T>
concept TBufferData = requires(T t)
{
	{ t.toBufferData() } -> std::convertible_to<GfxBufferImpl>;
};

class ConstBuffer
{
	static int getBufferEnum( unsigned usage );
public:
	unsigned position = ~0u;
	unsigned usage = 0;
	BufferHandle buffer = ~0u;
	uint8_t* bufferData = nullptr;
	size_t size = 0;
	size_t capacity = 0;
	size_t stride = 0;
	int bufferEnum = 0;
	int bindingPoint = 0;

	ConstBuffer() = default;
	ConstBuffer( const GfxBufferImpl& b, int bufferUsage = 0, int slot = 0, size_t vcapacity = 0 );
	template<TBufferData T>
	ConstBuffer( T b, int bufferUsage = 0, int slot = 0, size_t vcapacity = 0 );
	~ConstBuffer();

	void bind();
	void update( const GfxBufferImpl& b );
};

template<TBufferData T>
inline ConstBuffer::ConstBuffer( T b, int bufferUsage, int slot, size_t vcapacity )
	: ConstBuffer( b.toBufferData(), bufferUsage, slot, vcapacity )
{}

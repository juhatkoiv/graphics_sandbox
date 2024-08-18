#pragma once

struct GfxBufferImpl
{
	size_t size = ~0u;
	size_t stride = ~0u;
	uint8_t* data = nullptr;
};

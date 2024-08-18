#pragma once

#define CBUFFER alignas(8)

#define OPEN_GL

#ifdef OPEN_GL
using GfxHandle = GLuint;
using BufferHandle = GfxHandle;
#elif DX11
using GfxHandle = void*;
using BufferHandle = ID3D11Buffer*;
#endif // OPEN_GL
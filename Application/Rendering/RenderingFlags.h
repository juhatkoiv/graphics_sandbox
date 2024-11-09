#pragma once

#include <glm/glm.hpp>
#include "Precompiled/EnumClassFlagsDefines.h"

#include <nlohmann/json.hpp>
#include "Rendering/Serialization/RendererSerializationDefines.h"

enum class ClearFlags
{
	None = 0,
	Color = 1 << 0,
	Depth = 1 << 1,
	Stencil = 1 << 2,
	Blend = 1 << 3,
};
FLAG_ENUM(ClearFlags)

enum class Culling
{
	None = 0,
	FrontFace = 1 << 1,
	BackFace = 1 << 2,
};
FLAG_ENUM(Culling);

enum class StencilFunc
{
	Never = 0x0200,
	Less = 0x0201,
	Equal = 0x0202,
	LEqual = 0x0203,
	Greater = 0x0204,
	NotEqual = 0x0205,
	GEqual = 0x0206,
	Always = 0x0207,
};

enum class StencilOp
{
	Keep = 0x1E00,
	Zero = 0,
	Replace = 0x1E01,
	Incr = 0x1E02,
	IncrWrap = 0x8507,
	Decr = 0x1E03,
	DecrWrap = 0x8508,
	Invert = 0x150A,
};

enum class DepthFunc
{
	Never = 0x0200,
	Less = 0x0201,
	Equal = 0x0202,
	LEqual = 0x0203,
	Greater = 0x0204,
	NotEqual = 0x0205,
	GEqual = 0x0206,
	Always = 0x0207,
};

enum class BlendEquation
{
	Add = 0x8006,
	Subtract = 0x800A,
	ReverseSubtract = 0x800B,
	Min = 0x8007,
	Max = 0x8008,
};

enum class BlendOp 
{
	Zero = 0,
	One = 1,
	SrcColor = 0x0300,
	OneMinusSrcColor = 0x0301,
	DstColor = 0x0306,
	OneMinusDstColor = 0x0307,
	SrcAlpha = 0x0302,
	OneMinusSrcAlpha = 0x0303,
	DstAlpha = 0x0304,
	OneMinusDstAlpha = 0x0305,
	ConstantColor = 0x8001,
	OneMinusConstantColor = 0x8002,
	ConstantAlpha = 0x8003,
	OneMinusConstantAlpha = 0x8004,
	SrcAlphaSaturate = 0x0308,
	Src1Color = 0x88F9,
	OneMinusSrc1Color = 0x88FA,
	Src1Alpha = 0x8589,
	OneMinusSrc1Alpha = 0x88FB,
};

enum class CullingEquation
{
	Ccw = 0x0901,
	Cw = 0x0900,
};

struct BlendDecs 
{	
	BlendOp srcBlend = BlendOp::One;
	BlendOp dstBlend = BlendOp::Zero;
	BlendEquation blendEquation = BlendEquation::Add;
	BlendOp srcBlendAlpha = BlendOp::One;
	BlendOp dstBlendAlpha = BlendOp::Zero;
	BlendEquation blendEquationAlpha = BlendEquation::Add;
	glm::vec4 blendColor{};
};
DECLARE_SERIALIZABLE(BlendDecs, srcBlend, dstBlend, blendEquation, srcBlendAlpha, dstBlendAlpha, blendEquationAlpha)

struct StencilDesc 
{
	StencilFunc stencilFunc = StencilFunc::Never;
	StencilOp stencilFailOp = StencilOp::Keep;
	StencilOp stencilDepthFailOp = StencilOp::Keep;
	StencilOp stencilBothOkOp = StencilOp::Keep;
	unsigned sencilMask = 0x0;
	int stencilRef = 0;
};
DECLARE_SERIALIZABLE(StencilDesc, stencilFunc, stencilFailOp, stencilDepthFailOp, stencilBothOkOp, sencilMask, stencilRef)

struct CullingDesc 
{
	Culling culling = Culling::BackFace;
	CullingEquation cullingEquation = CullingEquation::Ccw;
};
DECLARE_SERIALIZABLE(CullingDesc, culling, cullingEquation)

struct DepthDesc 
{
	bool depthMask = true;
	DepthFunc depthFunc = DepthFunc::Less;
};
DECLARE_SERIALIZABLE( DepthDesc, depthMask, depthFunc )

// TODO - add depth funcs
struct GfxFlags
{
	ClearFlags clearFlags = ClearFlags::Color | ClearFlags::Depth;
	ClearFlags enableFlags = ClearFlags::Depth;
	ClearFlags disableFlags = ClearFlags::Blend;
	CullingDesc cullingDesc{};
	StencilDesc stencilDesc{};
	BlendDecs blendDesc{};
	DepthDesc depthDesc{};
	glm::vec4 clearColor{};

	static GfxFlags none() { return GfxFlags{}; }
}; 
DECLARE_SERIALIZABLE(GfxFlags, clearFlags, enableFlags, disableFlags, cullingDesc, stencilDesc, blendDesc, depthDesc)


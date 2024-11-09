#include "Precompiled/Precompiled.h"
#include "RendererSerializationDefines.h"
#include "ECS/Serialization/GlmSerializationDefines.h"
#include "Rendering/RenderDefinitions.h"
#include "RendererJsons.h"

NLOHMANN_JSON_NAMESPACE_BEGIN

void adl_serializer<RenderResourcesJson>::to_json( json& j, const RenderResourcesJson& q )
{
	j = json
	{
		{"flags", q.flags},
		{"sourceFramebuffers", q.sourceFramebuffers},
		{"targetFrameBuffer", q.targetFrameBuffer},
		{"shaderName", q.shaderName},
		{"passIndex", q.passIndex}
	};
}

void adl_serializer<RenderResourcesJson>::from_json( const json& j, RenderResourcesJson& q )
{
	j.at( "flags" ).get_to( q.flags );
	j.at( "sourceFramebuffers" ).get_to( q.sourceFramebuffers );
	j.at( "targetFrameBuffer" ).get_to( q.targetFrameBuffer );
	j.at( "shaderName" ).get_to( q.shaderName );
	j.at( "passIndex" ).get_to( q.passIndex );
}

void adl_serializer<GfxFlags>::to_json( json& j, const GfxFlags& q )
{
	j = json
	{
		{"clearFlags", q.clearFlags},
		{"enableFlags", q.enableFlags},
		{"disableFlags", q.disableFlags},
		{"cullingDesc", q.cullingDesc},
		{"stencilDesc", q.stencilDesc},
		{"blendDesc", q.blendDesc},
		{"depthDesc", q.depthDesc},
		{"clearColor", q.clearColor}
	};
}

void adl_serializer<GfxFlags>::from_json( const json& j, GfxFlags& q )
{
	j.at( "clearFlags" ).get_to( q.clearFlags );
	j.at( "enableFlags" ).get_to( q.enableFlags );
	j.at( "disableFlags" ).get_to( q.disableFlags );
	j.at( "cullingDesc" ).get_to( q.cullingDesc );
	j.at( "stencilDesc" ).get_to( q.stencilDesc );
	j.at( "blendDesc" ).get_to( q.blendDesc );
	j.at( "depthDesc" ).get_to( q.depthDesc );
	j.at( "clearColor" ).get_to( q.clearColor );
}

void adl_serializer<CullingDesc>::to_json( json& j, const CullingDesc& q )
{
	j = json
	{
		{"culling", q.culling},
		{"cullingEquation", q.cullingEquation}
	};
}

void adl_serializer<CullingDesc>::from_json( const json& j, CullingDesc& q )
{
	j.at( "culling" ).get_to( q.culling );
	j.at( "cullingEquation" ).get_to( q.cullingEquation );
}

void adl_serializer<StencilDesc>::to_json( json& j, const StencilDesc& q )
{
	j = json
	{
		{"stencilFunc", q.stencilFunc},
		{"stencilFailOp", q.stencilFailOp},
		{"stencilDepthFailOp", q.stencilDepthFailOp},
		{"stencilBothOkOp", q.stencilBothOkOp},
		{"sencilMask", q.sencilMask},
		{"stencilRef", q.stencilRef}
	};
}

void adl_serializer<StencilDesc>::from_json( const json& j, StencilDesc& q )
{
	j.at( "stencilFunc" ).get_to( q.stencilFunc );
	j.at( "stencilFailOp" ).get_to( q.stencilFailOp );
	j.at( "stencilDepthFailOp" ).get_to( q.stencilDepthFailOp );
	j.at( "stencilBothOkOp" ).get_to( q.stencilBothOkOp );
	j.at( "sencilMask" ).get_to( q.sencilMask );
	j.at( "stencilRef" ).get_to( q.stencilRef );
}

void adl_serializer<BlendDecs>::to_json( json& j, const BlendDecs& q )
{
	j = json
	{
		{"srcBlend", q.srcBlend},
		{"dstBlend", q.dstBlend},
		{"blendEquation", q.blendEquation},
		{"srcBlendAlpha", q.srcBlendAlpha},
		{"dstBlendAlpha", q.dstBlendAlpha},
		{"blendEquationAlpha", q.blendEquationAlpha},
		{"blendColor", q.blendColor}
	};
}

void adl_serializer<BlendDecs>::from_json( const json& j, BlendDecs& q )
{
	j.at( "srcBlend" ).get_to( q.srcBlend );
	j.at( "dstBlend" ).get_to( q.dstBlend );
	j.at( "blendEquation" ).get_to( q.blendEquation );
	j.at( "srcBlendAlpha" ).get_to( q.srcBlendAlpha );
	j.at( "dstBlendAlpha" ).get_to( q.dstBlendAlpha );
	j.at( "blendEquationAlpha" ).get_to( q.blendEquationAlpha );
	j.at( "blendColor" ).get_to( q.blendColor );
}

void adl_serializer<DepthDesc>::to_json( json& j, const DepthDesc& q )
{
	j = json
	{
		{"depthMask", q.depthMask},
		{"depthFunc", q.depthFunc},
	};
}

void adl_serializer<DepthDesc>::from_json( const json& j, DepthDesc& q )
{
	j.at( "depthMask" ).get_to( q.depthMask );
	j.at( "depthFunc" ).get_to( q.depthFunc );
}

NLOHMANN_JSON_NAMESPACE_END
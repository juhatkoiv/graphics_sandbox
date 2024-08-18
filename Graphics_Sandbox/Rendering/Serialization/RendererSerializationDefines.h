#pragma once

#include "Serialization/SerializationDefinitions.h"

DECLARE1( struct, rendering, PassResources );
DECLARE( struct, GfxFlags );
DECLARE( struct, CullingDesc );
DECLARE( struct, StencilDesc );
DECLARE( struct, BlendDecs );
DECLARE( struct, DepthDesc );
DECLARE( struct, RenderResourcesJson );

BEGIN_SERIALIZER_NAMESPACE

template<> struct adl_serializer<RenderResourcesJson>
{
	static void to_json( nlohmann::json& j, const RenderResourcesJson& q );
	static void from_json( const nlohmann::json& j, RenderResourcesJson& q );
};

template<> struct adl_serializer<GfxFlags>
{
	static void to_json( nlohmann::json& j, const GfxFlags& q );
	static void from_json( const nlohmann::json& j, GfxFlags& q );
};

template<> struct adl_serializer<CullingDesc>
{
	static void to_json( nlohmann::json& j, const CullingDesc& q );
	static void from_json( const nlohmann::json& j, CullingDesc& q );
};

template<> struct adl_serializer<StencilDesc>
{
	static void to_json( nlohmann::json& j, const StencilDesc& q );
	static void from_json( const nlohmann::json& j, StencilDesc& q );
};

template<> struct adl_serializer<BlendDecs>
{
	static void to_json( nlohmann::json& j, const BlendDecs& q );
	static void from_json( const nlohmann::json& j, BlendDecs& q );
};

template<> struct adl_serializer<DepthDesc>
{
	static void to_json( nlohmann::json& j, const DepthDesc& q );
	static void from_json( const nlohmann::json& j, DepthDesc& q );
};
END_SERIALIZER_NAMESPACE

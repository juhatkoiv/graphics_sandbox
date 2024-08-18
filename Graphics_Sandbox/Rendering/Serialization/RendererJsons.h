#pragma once

#include "Rendering/RenderDefinitions.h"

#include "Rendering/Serialization/RendererSerializationDefines.h"
#include "Serialization/SerializationDefinitions.h"

namespace rendering 
{
	using PassIndex = size_t;
};

using namespace nlohmann;

struct RenderResourcesJson
{
	GfxFlags flags{};
	std::vector<rendering::RenderTargetType> sourceFramebuffers{};
	rendering::RenderTargetType targetFrameBuffer{};
	std::string shaderName{};
	rendering::PassIndex passIndex = ~0u;
};
DECLARE_SERIALIZABLE(RenderResourcesJson, flags, sourceFramebuffers, shaderName, passIndex);

struct RenderPassJson
{
	std::string passType{};
	std::string passName{};
	rendering::PassIndex passIndex = ~0u;
};
DECLARE_SERIALIZABLE(RenderPassJson, passType, passName, passIndex);

struct RenderFeatureJson
{
	std::string featureName{};
	std::vector<RenderPassJson> passes{};
};
DECLARE_SERIALIZABLE(RenderFeatureJson, featureName, passes);

struct RenderQueueJson
{
	int order = -1;
	std::string queueName{};
	std::vector<RenderFeatureJson> features{};
};
DECLARE_SERIALIZABLE(RenderQueueJson, order, queueName, features);

struct RenderGraphJson
{
	std::vector<RenderQueueJson> queues{};
};
DECLARE_SERIALIZABLE(RenderGraphJson, queues);
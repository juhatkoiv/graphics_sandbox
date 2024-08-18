#pragma once

#include "FrameResources.h"
#include "Serialization/RendererJsons.h"

#include <span>
#include <vector>

BEGIN_NAMESPACE1(rendering)

struct Defaults
{
	static RenderGraphJson buildGraphJson();
	static std::vector<RenderResourcesJson> buildResourcesJson();
	static FrameResources buildFrameResources();
	static std::span<std::string> buildPassNameList();
	static std::span<std::string> buildFeatureNameList();
	static void validateBuild();
};

END_NAMESPACE1
#pragma once

#include <functional>
#include <unordered_map>
#include <string>

DECLARE1( struct, rendering, GfxQueue );
BEGIN_NAMESPACE1( rendering )

using FilterImpl = std::function<bool( const GfxQueue& )>;

struct FeatureFilters
{
	static std::unordered_map<std::string, FilterImpl>& filters();
	static void initalizeFilters();
	static bool enabled( const std::string& name, const GfxQueue& queue );
};

END_NAMESPACE1
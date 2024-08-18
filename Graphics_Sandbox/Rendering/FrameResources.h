#pragma once

#include <unordered_map>

DECLARE1( struct, rendering, PassResources )

BEGIN_NAMESPACE1( rendering )

using PassIndex = size_t;

struct FrameResources
{
	FrameResources() = default;
	~FrameResources() = default;

	void setPassResources( PassIndex passIndex, const PassResources& resources );

	const PassResources& passResources( PassIndex passIndex ) const;

	std::unordered_map<PassIndex, PassResources> passData{};
};

END_NAMESPACE1
#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "RenderFeature.h"

#include <string>

DECLARE1( class, rendering, GfxDevice );
DECLARE1( struct, rendering, GfxQueue );
DECLARE1( struct, rendering, FrameResources );

BEGIN_NAMESPACE1( rendering )

using PassIndex = size_t;

class RenderQueue
{
private:
	std::vector<RenderFeature> _renderFeatures;
	std::string _queueName;

public:
	RenderQueue( const std::string& queueName );
	~RenderQueue();
	
	void execute( GfxQueue& queue, GfxDevice* device, const FrameResources& frameData );

	RenderFeature& addRenderFeature( const std::string& name );
	const std::string& queueName() const { return _queueName; }
};

END_NAMESPACE1
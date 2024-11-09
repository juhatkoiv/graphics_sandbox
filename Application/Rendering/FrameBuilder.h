#pragma once

#include "RenderDefinitions.h"

DECLARE1( struct, rendering, FrameResources )
DECLARE1( class, rendering, RenderGraph )

BEGIN_NAMESPACE1( rendering )

class FrameBuilder
{
public:
	static const FrameResources& buildFrameResources();
	static RenderGraph& buildGraph();
};

END_NAMESPACE1
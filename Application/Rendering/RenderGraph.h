#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "RenderDefinitions.h"

#include <unordered_map>
#include <span>

DECLARE1( struct, rendering, Gfx )
DECLARE1( class, rendering, GfxDevice )
DECLARE1( struct, rendering, RenderPass )
DECLARE1( struct, rendering, FrameResources )
DECLARE1( class, rendering, RenderQueue )
DECLARE1( struct, rendering, Features )

BEGIN_NAMESPACE1( rendering )

class RenderGraph
{
public:
	static RenderGraph& get()
	{
		static RenderGraph graph;
		return graph;
	}

	RenderGraph() = default;
	~RenderGraph();

	RenderQueue& addQueue( const std::string& queueName, int order );
	void execute( GfxFrame& gfx, GfxDevice* device, const FrameResources& frameResources ) const;
	bool empty() const;
	bool changed() const;
	
};

END_NAMESPACE1
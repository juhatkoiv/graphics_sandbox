#pragma once

#include "Precompiled/PrecompilationMacros.h"

#include "RenderPass.h"

#include <vector>
#include <string>

DECLARE1( struct, rendering, FrameResources )
DECLARE1( struct, rendering, GfxQueue )
DECLARE1( class, rendering, GfxDevice )
DECLARE1( struct, rendering, GraphFilters );
DECLARE1( struct, rendering, Passes );

BEGIN_NAMESPACE1( rendering )

using PassIndex = size_t;

class RenderFeature
{
public:
	RenderFeature( const std::string & name );
	~RenderFeature();

	void execute( GfxQueue& ctx, GfxDevice* device, const FrameResources& input ) const;

	void addPasses( const Passes& passes );
	const std::string& name() const { return featureName; }

private:
	void addPass( PassIndex index, const RenderPass& pass );

	std::vector<PassIndex> passOrder;
	std::unordered_map<PassIndex, RenderPass> passes;
	std::string featureName;
};


END_NAMESPACE1
#pragma once

#include "RenderDefinitions.h"

DECLARE1( struct, rendering, PassResources )
DECLARE1( class, rendering, GfxDevice )

BEGIN_NAMESPACE1( rendering )

using PassImpl = std::function<void( GfxQueue&, GfxDevice*, const PassResources& )>;
using PassIndex = size_t;

template<typename T>
concept TPass = requires(T t, GfxQueue& gfx, GfxDevice* device, const PassResources& resources)
{
	{ t.execute( gfx, device, resources ) };
};

class PassBase
{
public:
	virtual void execute( GfxQueue& gfx, GfxDevice* device, const PassResources& resources ) = 0;
};

template<TPass T>
class RenderPassImpl : public PassBase
{
public:
	virtual void execute( GfxQueue& gfx, GfxDevice* device, const PassResources& resources ) override
	{
		(T*)(this)->execute( gfx, device, resources );
	}
};

/*class ClearPass : public RenderPassImpl<ClearPass>
{
public:
	void execute( Gfx& gfx, GfxDevice* device, const PassResources& resources ) override
	{
		const GfxFlags& flags = resources.getFlags();
		fn::clear( flags );
	}
};
*/
struct RenderPass
{
	std::string name{};
	PassImpl impl;
};

END_NAMESPACE1
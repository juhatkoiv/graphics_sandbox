#pragma once

#include "Precompiled/PrecompilationMacros.h"

#include <memory>
#include <span>

DECLARE1( class, rendering, GfxDevice );
DECLARE1( class, rendering, GfxContext );
DECLARE1( class, resources, ResourceSystem );
DECLARE1( struct, rendering, WindowSize );
DECLARE1( struct, rendering, GfxDeviceArgs );

BEGIN_NAMESPACE1( api )
static constexpr int OPENGL = 0;
static constexpr int DX11 = 1; // ;)
END_NAMESPACE1

BEGIN_NAMESPACE1( rendering )

class GfxDeviceFactory
{
public:
	GfxDeviceFactory( const GfxDeviceArgs& args );
	std::unique_ptr<GfxDevice> createDevice( int renderer );

private:
	const GfxDeviceArgs& _args;
};
END_NAMESPACE1
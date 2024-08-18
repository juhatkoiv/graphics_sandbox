#include "Precompiled/Precompiled.h"
#include "GfxDeviceFactory.h"

#include "GfxDevice.h"
#include "Resources/ResourceSystem.h"
#include "Window/Window.h"

#include "OpenGL/GfxDeviceOpenGL.h"

#include <glm/glm.hpp>

BEGIN_NAMESPACE1( rendering )

GfxDeviceFactory::GfxDeviceFactory( const GfxDeviceArgs& args )
	: _args( args )
{}

std::unique_ptr<GfxDevice> GfxDeviceFactory::createDevice( int renderer )
{
	if (renderer == api::OPENGL) 
	{
		return std::make_unique<GfxDeviceOpenGL>( _args );
	}
	throw;
}


END_NAMESPACE1

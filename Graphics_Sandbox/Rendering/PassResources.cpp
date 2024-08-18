#include "Precompiled/Precompiled.h"
#include "PassResources.h"

BEGIN_NAMESPACE1( rendering )

PassResources::PassResources( GfxFlags flags, std::vector<RenderTargetType> sourceFramebuffers, RenderTargetType targetFrameBuffer, id::ShaderId shaderId )
	: flags( flags ), sourceFramebuffers( sourceFramebuffers ), targetFrameBuffer( targetFrameBuffer ), shaderId( shaderId )
{
}

GfxFlags PassResources::getFlags() const
{
	return flags;
}

void PassResources::setFlags( const GfxFlags& flags )
{
	this->flags = flags;
}

void PassResources::addSourceFrameBuffer( RenderTargetType frameBuffer )
{
	sourceFramebuffers.push_back( frameBuffer );
}

void PassResources::setTargetFrameBuffer( RenderTargetType frameBuffer )
{
	targetFrameBuffer = frameBuffer;
}

id::ShaderId PassResources::getShaderId() const
{
	return shaderId;
}

void PassResources::setShaderId( const char* shaderName )
{
	shaderId = shader::getShaderId( shaderName );
	if (shaderId == id::InvalidShaderId)
		throw std::runtime_error( "Invalid shader id" );
}

const char* PassResources::getShaderName() const
{
	return shader::getShaderName( shaderId );
}

PassResources PassResources::none()
{
	return PassResources{};
}

END_NAMESPACE1
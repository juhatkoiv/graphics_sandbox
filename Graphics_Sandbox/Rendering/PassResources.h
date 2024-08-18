#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "RenderDefinitions.h"

BEGIN_NAMESPACE1(rendering)

struct PassResources
{
	GfxFlags flags{};
	std::vector<RenderTargetType> sourceFramebuffers{};
	RenderTargetType targetFrameBuffer{};
	id::ShaderId shaderId = id::InvalidShaderId;

	PassResources() = default;
	PassResources( GfxFlags flags, std::vector<RenderTargetType> sourceFramebuffers, RenderTargetType targetFrameBuffer, id::ShaderId shaderId );

	static PassResources none();

	void setFlags( const GfxFlags& flags );
	void setTargetFrameBuffer( RenderTargetType frameBuffer );
	void setShaderId( const char* shaderName );
	void addSourceFrameBuffer( RenderTargetType frameBuffer );

	GfxFlags getFlags() const;
	id::ShaderId getShaderId() const;
	const char* getShaderName() const;
};

END_NAMESPACE1;

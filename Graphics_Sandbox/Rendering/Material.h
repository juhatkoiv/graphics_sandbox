#pragma once

#include "Rendering/Texture.h"

#include <string>

BEGIN_NAMESPACE1(rendering)

struct GfxSprite
{
	id::TextureId textureId;
	unsigned bindPosition;
};

struct GfxDiffuseMaterial 
{
	id::TextureId textureId;
	unsigned bindPosition;
	float diffuseCoeff = 0.75f;
};

struct GfxSpecularMaterial 
{
	id::TextureId textureId;
	unsigned bindPosition;
	float specularCoeff = 1.0f;
};

struct GfxShader
{
	id::ShaderId shaderId{ id::InvalidShaderId };
};

END_NAMESPACE1
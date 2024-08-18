#pragma once

#include "Precompiled/EnumClassFlagsDefines.h"

BEGIN_NAMESPACE1(rendering)

enum class RenderEffect : size_t
{
	None = 0,
	HighlightOutline3D = (1 << 0),
	COUNT
};
FLAG_ENUM(RenderEffect);

static inline RenderEffect getRenderEffect( const char* name )
{
	static std::unordered_map<std::string, RenderEffect> map
	{
		{"HighlightOutline3D", RenderEffect::HighlightOutline3D}
	};
	return map.at(name);
}

static inline const char* getRenderEffectName( RenderEffect effect )
{
	static std::unordered_map<RenderEffect, const char*> map
	{
		{RenderEffect::HighlightOutline3D, "HighlightOutline3D"}
	};
	return map.at(effect);
}

enum class RenderToTextureEffect
{
	Blur
};

enum class PostProcessingEffects
{
	Nothing = 0,
	Inverted = 1,
	Blur = 2,
	ColorCorrection = 3,
	ChromeAberration = 4,
	Pixelation = 5,
	COUNT = 6
};

FLAG_ENUM(PostProcessingEffects);

END_NAMESPACE1
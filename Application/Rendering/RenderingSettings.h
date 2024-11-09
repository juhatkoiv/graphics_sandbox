#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "RenderEffect.h"

BEGIN_NAMESPACE1(rendering)

struct FullScreenBlurSettings 
{
	int kernelSize = 1;
	float sigma = 1.0f;
};

struct PixelatedSettings
{
	float fragmentScale = 1.0;
};

struct ColorCorrectionSettings
{
	glm::vec3 colorCorrection = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct ChromeAberrationSettings 
{
	float aberration = 0.0f;

};

struct LightingSettings 
{
	float exposure = 2.0f;
	float gamma = 0.5f;
};

enum class ClearMode 
{
	SolidColor,
	Skybox,
	COUNT
};

struct ClearSettings
{
	ClearMode mode = ClearMode::SolidColor;
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
};

struct RenderingSettings 
{
	rendering::LightingSettings lightingSettings{};
	rendering::PostProcessingEffects fullScreenEffect{};
	rendering::ClearSettings clearSettings{};
	FullScreenBlurSettings fullScreenBlurSettings{};
	PixelatedSettings pixelatedSettings{};
	ColorCorrectionSettings colorCorrectionSettings{};
	ChromeAberrationSettings chromeAberrationSettings{};
};

END_NAMESPACE1
#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include <vector>
#include <string>

BEGIN_NAMESPACE1(rendering)

static constexpr int MAX_LIGHT_COUNT = 20;

static constexpr const char* LIGHT_START = "lights[";
static constexpr const char* LIGHT_POS_END = "].lightPos";
static constexpr const char* LIGHT_COLOR_END = "].lightColor";
static constexpr const char* LIGHT_INTENSITY_END = "].intensity";
static constexpr const char* LIGHT_ATTENUATION_END = "].attenuation";
static constexpr const char* LIGHT_COUNT = "lightCount";
static constexpr const char* MODEL_MATRIX = "model";
static constexpr const char* VIEW_MATRIX = "view";
static constexpr const char* PROJECTION_MATRIX = "projection";
static constexpr const char* FRAG_COLOR = "fragColor";
static constexpr const char* VIEW_POSITION = "viewPos";
static constexpr const char* DIFFUSE_TEXTURE = "diffuseTexture";
static constexpr const char* SPECULAR_TEXTURE = "specularTexture";
static constexpr const char* CUBEMAP_TEXTURE = "cubemapTexture";
static constexpr const char* DIFFUSE_COEFF = "diffuseCoeff";
static constexpr const char* SPECULAR_COEFF = "specularCoeff";
static constexpr const char* EXPOSURE = "exposure";
static constexpr const char* GAMMA = "gamma";
static constexpr const char* KERNEL_SIZE = "kernelSize";
static constexpr const char* SIGMA = "sigma";
static constexpr const char* WINDOW_SIZE_INVERTED = "windowSizeInverted";
static constexpr const char* FRAGMENT_SCALE = "fragmentScale";
static constexpr const char* COLOR_CORRECTION = "colorCorrection";
static constexpr const char* ABERRATION = "aberration";

//////// UNIFORMS ////////

inline const std::string& getLightPositionUniformKey(int index)
{
	static std::vector<std::string> lightPosUniforms;
	if (lightPosUniforms.empty())
	{
		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			std::string s = std::string(LIGHT_START);
			s += std::to_string(i);
			s += std::string(LIGHT_POS_END);
			lightPosUniforms.push_back(s);
		}
	}
	return lightPosUniforms[index];
}

inline const std::string& getLightColorUniformKey(int index)
{
	static std::vector<std::string> lightColorUniforms;
	if (lightColorUniforms.empty())
	{
		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			std::string s = std::string(LIGHT_START);
			s += std::to_string(i);
			s += std::string(LIGHT_COLOR_END);
			lightColorUniforms.push_back(s);
		}
	}
	return lightColorUniforms[index];
}

inline const std::string& getLightIntensityUniformKey(int index)
{
	static std::vector<std::string> lightIntensityUniforms;
	if (lightIntensityUniforms.empty())
	{
		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			std::string s = std::string(LIGHT_START);
			s += std::to_string(i);
			s += std::string(LIGHT_INTENSITY_END);
			lightIntensityUniforms.push_back(s);
		}
	}
	return lightIntensityUniforms[index];
}

inline const std::string& getLightAttenuationUniformKey(int index)
{
	static std::vector<std::string> lightAttenuationUniforms;
	if (lightAttenuationUniforms.empty())
	{
		for (int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			std::string s = std::string(LIGHT_START);
			s += std::to_string(i);
			s += std::string(LIGHT_ATTENUATION_END);
			lightAttenuationUniforms.push_back(s);
		}
	}
	return lightAttenuationUniforms[index];
}

END_NAMESPACE1;
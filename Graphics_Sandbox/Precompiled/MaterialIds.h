#pragma once

#include <vector>

#include "ECS/UniqueId.h"

namespace shader
{
	static constexpr const char* LAMBERTIAN = "LAMBERTIAN";
	static constexpr const char* LAMBERTIAN_TRANSPARENT = "LAMBERTIAN_TRANSPARENT";
	static constexpr const char* LAMBERTIAN_DIFFUSE_TEXTURE = "LAMBERTIAN_DIFFUSE_TEXTURE";
	static constexpr const char* LIGHTS = "LIGHTS";
	static constexpr const char* SINGLE_COLOR_BORDER = "SINGLE_COLOR_BORDER";
	static constexpr const char* SPRITE_DEFAULT = "SPRITE_DEFAULT";
	static constexpr const char* LAMBERTIAN_INSTANCED = "LAMBERTIAN_INSTANCED";

	static constexpr const char* FULL_SCREEN = "FULL_SCREEN_QUAD";
	static constexpr const char* FULL_SCREEN_COLOR_INVERTED = "FULL_SCREEN_COLOR_INVERTED";
	static constexpr const char* FULL_SCREEN_BLUR_VERTICAL = "FULL_SCREEN_BLUR_VERTICAL";
	static constexpr const char* FULL_SCREEN_BLUR_HORIZONTAL = "FULL_SCREEN_BLUR_HORIZONTAL";
	static constexpr const char* FULL_SCREEN_PIXELATED = "FULL_SCREEN_PIXELATED";
	static constexpr const char* FULL_SCREEN_CHROME_ABERRATION = "FULL_SCREEN_CHROME_ABERRATION";
	static constexpr const char* FULL_SCREEN_COLOR_CORRECTION = "FULL_SCREEN_COLOR_CORRECTION";

	static constexpr const char* SKYBOX = "SKYBOX";

	static constexpr const char* NULL_SHADER = "NULL_SHADER";

	inline const std::vector<std::tuple<id::ShaderId, const char*>>& getShaderNames()
	{
		static std::vector<std::tuple<id::ShaderId, const char*>> names = {};
		if (names.empty())
		{
			names.push_back({ id::NewShaderId(), LAMBERTIAN });
			names.push_back({ id::NewShaderId(), LIGHTS });
			names.push_back({ id::NewShaderId(), LAMBERTIAN_DIFFUSE_TEXTURE });
			names.push_back({ id::NewShaderId(), SINGLE_COLOR_BORDER });
			names.push_back({ id::NewShaderId(), SPRITE_DEFAULT });
			names.push_back({ id::NewShaderId(), LAMBERTIAN_TRANSPARENT });
			names.push_back({ id::NewShaderId(), LAMBERTIAN_INSTANCED });
			names.push_back({ id::NewShaderId(), FULL_SCREEN });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_COLOR_INVERTED });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_BLUR_VERTICAL });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_BLUR_HORIZONTAL });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_PIXELATED });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_CHROME_ABERRATION });
			names.push_back({ id::NewShaderId(), FULL_SCREEN_COLOR_CORRECTION });
			names.push_back({ id::NewShaderId(), SKYBOX });
			names.push_back({ id::InvalidShaderId, NULL_SHADER });
		}
		return names;
	}

	inline id::ShaderId getShaderId(const char* vshaderName)
	{ 
		auto& idNamePairs = getShaderNames();
		for (int i = 0; i < idNamePairs.size(); i++)
		{
			auto& [shaderId, shaderName] = idNamePairs[i];
			if (strcmp(shaderName, vshaderName) == 0)
				return shaderId;
		}
		return id::InvalidShaderId;
	}


	inline const char* getShaderName(id::ShaderId id)
	{
		auto& idNamePairs = getShaderNames();
		for (int i = 0; i < idNamePairs.size(); i++)
		{
			auto& [shaderId, shaderName] = idNamePairs[i];
			if (shaderId == id)
				return shaderName;
		}
		return NULL_SHADER;
	}
}
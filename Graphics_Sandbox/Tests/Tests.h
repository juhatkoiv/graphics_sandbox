#pragma once

#include <glm/glm.hpp>

#include <nlohmann/json.hpp>

#include "ECS/Serialization/GlmSerializationDefines.h"

using namespace nlohmann;

struct LightParam2
{
	glm::vec3 lightColor = {};
	float intensity = 1.0f;
	float attenuation = 1.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LightParam2, lightColor, intensity, attenuation);

/*
static void to_json(json& j, const LightParam2& value)
{
	j = json{ {"ligthColor", value.lightColor }, {"attenuation", value.attenuation}, {"intensity", value.intensity} };
}

static void from_json(const json& j, LightParam2& value)
{
	j.at("ligthColor").get_to(value.lightColor);
	j.at("attenuation").get_to(value.attenuation);
	j.at("intensity").get_to(value.intensity);
}
*/
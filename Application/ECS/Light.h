#pragma once

#include <glm/glm.hpp>

#include "ECS/Serialization/ComponentSerializationDefinitions.h"
#include "Component.h"

BEGIN_NAMESPACE1(ecs)

const glm::vec3 DEFAULT_LIGHT_COLOR = { 1,1,1 };

struct Light : public Component<cid::LIGHT>
{
	glm::vec3 lightColor = DEFAULT_LIGHT_COLOR;
	float intensity = 1.0f;
	float attenuation = 1.0f;

};
DECLARE_SERIALIZABLE(Light, lightColor, intensity, attenuation);

END_NAMESPACE1
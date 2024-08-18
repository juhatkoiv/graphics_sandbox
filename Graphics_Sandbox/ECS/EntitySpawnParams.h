#pragma once

#include "UniqueId.h"

#include <glm/glm.hpp>
#include <optional>

#include "Material.h"
#include "Mesh.h"

BEGIN_NAMESPACE1(ecs)

struct EntitySpawnParam
{
	glm::vec3 position = {};
	glm::vec3 scale = {};
	glm::vec3 rotationAxis = {};
	glm::vec3 lightColor = {};
	glm::vec4 meshColor = {};
	float deltaAngle = 0.0f;
};

END_NAMESPACE1
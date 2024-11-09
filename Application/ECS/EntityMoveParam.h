#pragma once

#include "UniqueId.h"
#include <glm/glm.hpp>

#include "Precompiled/PrecompilationMacros.h"

BEGIN_NAMESPACE1(ecs)

struct EntityMoveParam
{
	id::EntityId entityId = ~0u;
	glm::vec3 deltaPosition = {};
	glm::vec3 deltaScale = {};
	glm::vec3 rotationAxis = {};
	float deltaAngle = 0.0f;
};

END_NAMESPACE1
#pragma once

#include "ECS/UniqueId.h"
#include "ECS/ComponentIncludes.h"

#include <vector>

BEGIN_NAMESPACE1(scene)

struct SceneData
{
	unsigned int sceneId;
	std::vector<id::EntityId> sceneEntities;
};


END_NAMESPACE1
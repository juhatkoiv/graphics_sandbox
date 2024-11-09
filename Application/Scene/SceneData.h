#pragma once

#include "ECS/UniqueId.h"
#include "ECS/ComponentIncludes.h"

#include <vector>

BEGIN_NAMESPACE1(scene)

struct SceneData
{
	std::string name;
	std::vector<id::EntityId> sceneEntities;
};


END_NAMESPACE1
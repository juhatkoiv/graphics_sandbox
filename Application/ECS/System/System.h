#pragma once

#include "Precompiled/PrecompilationMacros.h"

#include "ECS/World.h"

DECLARE1(class, ecs, World);

BEGIN_NAMESPACE1(ecs)

class System
{
public:
	virtual ~System() {}
	virtual void update(ecs::World& world, float deltaTime) = 0;
};

END_NAMESPACE1
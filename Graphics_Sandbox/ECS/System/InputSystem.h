#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "ECS/System/System.h"

DECLARE1(class, window, Window);

BEGIN_NAMESPACE1(ecs)

class InputSystem : public System 
{
public:
	InputSystem(window::Window& window);
	~InputSystem() {};
	void update(ecs::World& world, float deltaTime) override;
};

END_NAMESPACE1
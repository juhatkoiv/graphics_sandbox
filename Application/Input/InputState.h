#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "InputStateDefinition.h"

BEGIN_NAMESPACE1(input)

const InputState& getInputState();

glm::vec2 getLastMousePosition();

glm::vec2 getLastMouseOffset();

bool consumeKey(InputEvent inputEvent);

bool popRay(world::Ray& ray);

END_NAMESPACE1


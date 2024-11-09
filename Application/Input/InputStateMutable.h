#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "InputStateDefinition.h"

BEGIN_NAMESPACE1(input)

InputState& getInputStateMutable();

void keyInputFound(InputEvent inputEvent);

void setFoundRay(const world::Ray& ray);

void clearAllKeys();

void clearMouseOffset();

END_NAMESPACE1


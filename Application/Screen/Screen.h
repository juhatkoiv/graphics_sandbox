#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "glm/glm.hpp"

DECLARE1(class, window, Window);

BEGIN_NAMESPACE1(screen)

glm::ivec2 getWindowPosition();
glm::uvec2 getScreenSize();

END_NAMESPACE1
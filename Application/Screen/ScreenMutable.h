#pragma once
#include "Precompiled/PrecompilationMacros.h"
#include "glm/glm.hpp"

DECLARE1(class, window, Window);

BEGIN_NAMESPACE1(screen)

glm::uvec2& getScreenSizeMutable();
glm::ivec2& getWindowPositionMutable();

void setScreenSize(int widht, int height);
void setScreenPosition( int x, int y );

END_NAMESPACE1
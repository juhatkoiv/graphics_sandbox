#include "Precompiled/Precompiled.h"
#include "Screen.h"

#include "ScreenMutable.h"

BEGIN_NAMESPACE1(screen)

glm::ivec2 getWindowPosition()
{
    return getWindowPositionMutable();
}

glm::uvec2 getScreenSize()
{
    return getScreenSizeMutable();
}

END_NAMESPACE1
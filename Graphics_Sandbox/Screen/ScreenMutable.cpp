#include "Precompiled/Precompiled.h"
#include "ScreenMutable.h"

BEGIN_NAMESPACE1(screen)

glm::uvec2& getScreenSizeMutable()
{
	static glm::uvec2 screenSize{};
	return screenSize;
}

glm::ivec2& getWindowPositionMutable()
{
	static glm::ivec2 windowPosition{};
	return windowPosition;
}

void setScreenPosition( int x, int y )
{
	getWindowPositionMutable() = { x, y };
}

void setScreenSize(int width, int height)
{
	getScreenSizeMutable() = { static_cast<unsigned>(width), static_cast<unsigned>(height) };
}

END_NAMESPACE1
#include "Precompiled/Precompiled.h"
#include "AppContext.h"
#include "Window/Window.h"

void AppContext::initialize()
{
	if (!glfwInit())
	{
		LOG_ERROR( "Glfw init failed! Aborting..." );
		exit( EXIT_FAILURE );
	}
}

void AppContext::cleanup()
{
	glfwTerminate();
}

void AppContext::closeApplication( window::Window& window )
{
	glfwSetWindowShouldClose( window.getWindowImpl(), true );
}

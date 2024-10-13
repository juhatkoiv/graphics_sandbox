#include "Precompiled/Precompiled.h"
#include "Window.h"

#include "Screen/ScreenMutable.h"

BEGIN_NAMESPACE1( window )

namespace
{
	void windowSizeCallback( GLFWwindow* window, int width, int height )
	{
		screen::setScreenSize( width, height );
	}

	void windowPosCallback( GLFWwindow* window, int x, int y )
	{
		screen::setScreenPosition( x, y );
	}

	void error_callback( int error, const char* description ) 
	{
		fprintf( stderr, "Error: %s\n", description );
	}

}

Window::Window( const char* title, WindowMode windowMode )
{
	if (title == nullptr)
		throw;

	glfwSetErrorCallback( error_callback );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_FLOATING, GLFW_FALSE );

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode( monitor );

	glfwWindowHint( GLFW_RED_BITS, mode->redBits );
	glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
	glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
	glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );
	glfwWindowHint( GLFW_SAMPLES, 4 );

	if (windowMode == WindowMode::Windowed)
	{
		_window = glfwCreateWindow( mode->width, mode->height, title, NULL, NULL );
		glfwMaximizeWindow( _window );
	}
	else if (windowMode == WindowMode::Fullscreen)
	{
		_window = glfwCreateWindow( mode->width, mode->height, title, monitor, NULL );
	}
	else
	{
		throw;
	}
	
	if (!isValid())
	{
		LOG_ERROR( "Glfw window initalization failed! Aborting..." );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwMakeContextCurrent( _window );
	glfwSwapInterval( 1 );

	glfwSetWindowSizeCallback( _window, windowSizeCallback );
	glfwSetWindowPosCallback( _window, windowPosCallback );

	screen::setScreenSize( mode->width, mode->height );
	screen::setScreenPosition( 0, 0 );
}

Window::Window( int width, int height, const char* title, bool setContextCurrent )
{
	_window = glfwCreateWindow( width, height, title, NULL, NULL );

	if (!isValid())
	{
		LOG_ERROR( "Glfw window initalization failed! Aborting..." );
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	if (setContextCurrent)
	{
		glfwMakeContextCurrent( _window );
	}

	glfwSwapInterval( 1 );
	glfwSetInputMode( _window, GLFW_STICKY_KEYS, GLFW_TRUE );

	screen::setScreenSize( width, height );

	int screenPosX = 0;
	int screenPosY = 0;

	glfwGetWindowPos( _window, &screenPosX, &screenPosY );
	screen::setScreenPosition( screenPosX, screenPosY );

	glfwSetWindowSizeCallback( _window, windowSizeCallback );
	glfwSetWindowPosCallback( _window, windowPosCallback );

}

GLFWwindow* Window::getWindowImpl()
{
	return _window;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose( _window );
}

void Window::close()
{
	glfwDestroyWindow( _window );
}

bool Window::isValid() const
{
	return _window != nullptr;
}

void Window::present()
{
	glfwSwapBuffers( _window );
}

void Window::use()
{
	glfwMakeContextCurrent( _window );
}

glm::vec2 Window::getSize() const
{
	int width = 0;
	int height = 0;

	glfwGetWindowSize( _window, &width, &height );

	return glm::vec2( width, height );
}

END_NAMESPACE1
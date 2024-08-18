#pragma once

DECLARE( struct, GLFWwindow );

BEGIN_NAMESPACE1( window )

class Window
{
public:
	Window( const char* title, bool windowed );
	Window( int width, int height, const char* title, bool setContextCurrent = true );

	bool shouldClose() const;
	bool isValid() const;

	void close();
	void present();
	void use();

	glm::vec2 getSize() const;
	GLFWwindow* getWindowImpl();

private:
	GLFWwindow* _window = nullptr;
};

END_NAMESPACE1
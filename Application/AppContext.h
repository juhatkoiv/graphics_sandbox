#pragma once

class Window;

class AppContext
{
public:
	static void initialize();
	static void cleanup();
	static void closeApplication(window::Window& window);
};
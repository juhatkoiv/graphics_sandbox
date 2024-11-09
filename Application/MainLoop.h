#pragma once

#include "ApplicationMode.h"

class MainLoop
{
public:
	struct Args
	{
		const char* title = nullptr;
		int windowWidth = 0;
		int windowHeight = 0;
		ApplicationMode appMode = ApplicationMode::NONE;
	};

	MainLoop( Args args );
	~MainLoop();

	void run();

private:
	void generateSpirv(); 
	void handleProfilerConnectedChanged();
	void update( float deltaTime );
	void render( float deltaTime );
	void cleanup();
	

	bool running();
};
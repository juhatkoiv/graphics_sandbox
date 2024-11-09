#pragma once

#include <memory>

#include "ApplicationMode.h"
#include "Precompiled/PrecompilationMacros.h"

DECLARE1( class, window, Window );
DECLARE1( class, rendering, GfxDevice );
DECLARE1( class, rendering, GfxWorker );
DECLARE1( class, resources, ResourceSystem );
DECLARE1( class, appdata, AppData );
DECLARE1( class, ecs, World );
DECLARE( class, SystemContainer );
DECLARE( class, Application );

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

	std::unique_ptr<window::Window> _window;
	std::unique_ptr<rendering::GfxDevice> _gfxDevice;
	std::unique_ptr<rendering::GfxWorker> _gfxWorker;
	std::unique_ptr<resources::ResourceSystem> _resouceSystem;
	std::unique_ptr<Application> _editor;
	std::unique_ptr<appdata::AppData> _appData;
	std::unique_ptr<ecs::World> _world;
	std::unique_ptr<SystemContainer> _systemContainer;
	bool _profilerConnected = false;

};
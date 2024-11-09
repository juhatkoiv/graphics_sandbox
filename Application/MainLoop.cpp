#include "Precompiled/Precompiled.h"
#include "MainLoop.h"

#include <chrono>

#include "AppContext.h"
#include "AppData/AppData.h"
#include "AppData/RepositoryItemTypes.h"
#include "ECS/World.h"
#include "ECS/Renderer.h"
#include "ECS/System/System.h"
#include "ECS/System/InputSystem.h"
#include "ECS/System/CameraSystem.h"
#include "ECS/ModelSerializer.h"
#include "Editor/Editor.h"
#include "Rendering/GfxWorker.h"
#include "Rendering/GfxDevice.h"
#include "Rendering/GfxDeviceFactory.h"
#include "Resources/ResourceSystem.h"
#include "Rendering/ShaderCompilation/ShaderCompilation.h"
#include "Window/Window.h"
#include "SystemContainer.h"

namespace
{
	using namespace std::chrono;
	using Clock = high_resolution_clock;
	using Time = high_resolution_clock::time_point;
	using Duration = duration<float, std::chrono::seconds::period>;

	std::unique_ptr<window::Window> _window;
	std::unique_ptr<rendering::GfxDevice> _gfxDevice;
	std::unique_ptr<rendering::GfxWorker> _gfxWorker;
	std::unique_ptr<resources::ResourceSystem> _resouceSystem;
	appdata::AppData _appData;
	SystemContainer _systemContainer;

	std::vector<std::unique_ptr<ecs::System>> _systems;
	std::unique_ptr<Application> _editor;
	ecs::World _world;
	bool _profilerConnected;

}

MainLoop::MainLoop( Args args ) {
	ApplicationMode appMode = args.appMode;
	
	if (has( appMode, ApplicationMode::GENERATE_SPIRV )) {
		generateSpirv();
	}
	
	AppContext::initialize();

	_appData.initialize();
	_window.reset( new window::Window( args.title, window::WindowMode::Windowed ) );

	_resouceSystem.reset( new resources::ResourceSystem( _appData ) );
	rendering::GfxDeviceArgs deviceArgs = _resouceSystem->getGfxDeviceArgs( _window->getSize() );
	rendering::GfxShaderArgs shaderArgs = _resouceSystem->getShaderArgs();

	rendering::GfxDeviceFactory deviceFactory{ deviceArgs };
	_gfxWorker.reset( new rendering::GfxWorker{ deviceFactory } );
	_gfxWorker->setApi( api::OPENGL );
	_gfxWorker->compileShaders( shaderArgs );

	_systemContainer
		.withSystem( std::make_unique<ecs::InputSystem>( *_window ) )
		.withSystem( std::make_unique<ecs::CameraSystem>( *_window ) )
		.withSystem( std::make_unique<ecs::Renderer>( _gfxWorker.get(), _resouceSystem.get() ) );

	_editor.reset( new editor::Editor( *_window, _world, *_resouceSystem.get(), _appData ) );
}

MainLoop::~MainLoop() {}

void MainLoop::run()
{
	_profilerConnected = PROFILER_ENABLED;

	float timeSinceLastUpdate = 0.0f;
	Time previousUpdateTime;

	do
	{
		handleProfilerConnectedChanged();

		Time currentTime = Clock::now();
		float elapsed = Duration( currentTime - previousUpdateTime ).count();

		timeSinceLastUpdate += elapsed;
		if (timeSinceLastUpdate > global::FRAME_TIME_SECONDS)
		{
			update( global::FRAME_TIME_SECONDS );

			timeSinceLastUpdate -= global::FRAME_TIME_SECONDS;
			previousUpdateTime = Clock::now();
		}

		render( global::FRAME_TIME_SECONDS );
		
		FRAME_MARK;

	} while (running());

	cleanup();
}

void MainLoop::generateSpirv()
{
	auto result = shader_compilation::generate_spirv();
	if (result.success()) {
		std::cout << "SpirV generated successfully." << std::endl;
	}
	else {
		std::cerr << "Error: Failed to generate spirv." << std::endl;
		for (const auto& error : result.errors) {
			std::cerr << error << std::endl;
		}
	}
}

void MainLoop::handleProfilerConnectedChanged()
{
	const bool changed = _profilerConnected != PROFILER_ENABLED;
	if (!changed)
		return;

	if (changed && PROFILER_ENABLED)
	{
		LOG_INFO( "Profiler Connected" );
	}

	if (changed && !PROFILER_ENABLED)
	{
		LOG_INFO( "Profiler Disconnected" );
	}
	_profilerConnected = PROFILER_ENABLED;
}

void MainLoop::update( float deltaTime )
{
	_systemContainer.update( _world, deltaTime );
	_editor->update( deltaTime );

	if (!app::hasState( app::AppState::Quits ))
		return;

	AppContext::closeApplication( *_window );
}

void MainLoop::render( float deltaTime )
{
	_editor->render( deltaTime );
	_window->present();
}

void MainLoop::cleanup()
{
	assert( _window != nullptr );
	_window->close();
	AppContext::cleanup();
}

bool MainLoop::running()
{
	assert( _window != nullptr );
	return !_window->shouldClose();
}

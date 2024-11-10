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

#include <thread>
#include <mutex>

template<typename T>
class AsyncResult
{
public:
	AsyncResult( std::function<T()> function ) : 
		_impl( function ), 
		_thread( &AsyncResult::run, this ),
		_result( Result<T>::failed() )
	{}

	void run() 
	{
		_result = Result<T>::ok( _impl() );
	}

	void wait() const
	{
		_thread.join();
	}

	Result<T> get() const
	{
		std::lock_guard<std::mutex> lock( _mutex );
		return _result;
	}
private:
	std::function<T()> _impl;
	mutable std::thread _thread;
	mutable std::mutex _mutex;
	Result<T> _result;
};

namespace
{
	using namespace std::chrono;
	using Clock = high_resolution_clock;
	using Time = high_resolution_clock::time_point;
	using Duration = duration<float, std::chrono::seconds::period>;

	
}

MainLoop::MainLoop() {}

MainLoop::~MainLoop() {}

void MainLoop::initialize( Args args )
{
	ApplicationMode appMode = args.appMode;

	if (has( appMode, ApplicationMode::GENERATE_SPIRV )) {
		generateSpirv();
	}

	AppContext::initialize();

	_appData.reset( new appdata::AppData() );
	_appData->initialize();

	_world.reset( new ecs::World() );
	_window.reset( new window::Window( args.title, window::WindowMode::Windowed ) );

	_resouceSystem.reset( new resources::ResourceSystem( *_appData ) );
	rendering::GfxDeviceArgs deviceArgs = _resouceSystem->getGfxDeviceArgs( _window->getSize() );

	rendering::GfxDeviceFactory deviceFactory{ deviceArgs };
	_gfxWorker.reset( new rendering::GfxWorker{ deviceFactory } );
	_gfxWorker->setApi( api::OPENGL );

	_systemContainer = SystemContainer::Builder::create()
		.withSystem( std::make_unique<ecs::InputSystem>( *_window ) )
		.withSystem( std::make_unique<ecs::CameraSystem>( *_window ) )
		.withSystem( std::make_unique<ecs::Renderer>( _gfxWorker.get(), _resouceSystem.get() ) )
		.build();

	_editor.reset( new editor::Editor( *_window, *_world, *_resouceSystem.get(), *_appData ) );
}

static const AsyncResult<bool>& compileShadersAsync( std::function<void()> gen, window::Window& w, resources::ResourceSystem& r, rendering::GfxWorker& gw )
{
	window::Window shaderContext{ "Shader Compilation Context", w };
	static AsyncResult<bool> compileShadersResult( [shaderContext, gen, &r, &gw]() mutable {
		
		app::toggleState( app::AppState::GeneratingSpirv );
		gen();
		app::toggleState( app::AppState::GeneratingSpirv );


		app::toggleState( app::AppState::CompilingShaders );
		rendering::GfxShaderArgs shaderArgs = r.getShaderArgs();
		app::toggleState( app::AppState::CompilingShaders );

		shaderContext.use(); 
		gw.compileShaders( shaderArgs );
		shaderContext.close();
		shaderContext.stopUsing();

		return true;
	} );

	return compileShadersResult;
}

void MainLoop::run()
{
	_profilerConnected = PROFILER_ENABLED;
	
	float timeSinceLastUpdate = 0.0f;
	Time previousUpdateTime;


	const AsyncResult<bool>& compileShadersResult = compileShadersAsync( [this]() { generateSpirv(); }, *_window, *_resouceSystem, *_gfxWorker );
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

		if (compileShadersResult.get().success)
		{
			_window->use();
			render( global::FRAME_TIME_SECONDS );
		}
		else 
		{
			glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			_editor->render( global::FRAME_TIME_SECONDS );
			_window->present();
		}

		FRAME_MARK;

	} while (running());

	compileShadersResult.wait();

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
	_systemContainer->update( *_world, deltaTime );
	_editor->update( deltaTime );

	if (!app::hasState( app::AppState::Quits ))
		return;

	AppContext::closeApplication( *_window );
}

void MainLoop::render( float deltaTime )
{
	_gfxWorker->render();
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

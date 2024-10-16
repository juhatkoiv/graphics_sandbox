#include "Precompiled/Precompiled.h"
#include "MainLoop.h"

#include <chrono>

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
#include "Window/Window.h"

template<typename T>
concept SystemFactory = requires(T t)
{
	{ t() } -> std::convertible_to<std::unique_ptr<ecs::System>>;
};

class SystemContainer
{
public:
	void update( ecs::World& world, float deltaTime )
	{
		for (const auto& system : _systems)
		{
			system->update( world, deltaTime );
		}
	}

	SystemContainer& withSystem( std::unique_ptr<ecs::System>&& system )
	{
		_systems.emplace_back( std::move( system ) );
		return *this;
	}

	// Lazy init overload
	template<SystemFactory Func>
	SystemContainer& withSystem( Func func )
	{
		_systems.push_back( func() );
		return *this;
	}

	template<typename T>
	Result<T*> getSystem()
	{
		for (auto& system : _systems)
		{
			if (typeid(system.get()) != typeid(T*))
				continue;

			return Result<T*>::ok( system.get() );
		}

		return Result<T*>::failed();
	}


private:
	std::vector<std::unique_ptr<ecs::System>> _systems;
};
class AppContext
{
public:
	static void initialize()
	{
		if (!glfwInit())
		{
			LOG_ERROR( "Glfw init failed! Aborting..." );
			exit( EXIT_FAILURE );
		}
	}

	static void cleanup()
	{
		glfwTerminate();
	}
};

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

	void update( float deltaTime );
	void render( float deltaTime );
	void postUpdate( float deltaTime );
	void cleanup();
	bool running();

	static void update( float deltaTime )
	{
		_systemContainer.update( _world, deltaTime );
		_editor->update( deltaTime );
	}

	static void render( float deltaTime )
	{
		_editor->render( deltaTime );
		_window->present();
	}

	static void postUpdate( float deltaTime )
	{
		if (!app::hasState( app::AppState::Quits ))
			return;

		glfwSetWindowShouldClose( _window->getWindowImpl(), true );
	}

	static bool running()
	{
		return !_window->shouldClose();
	}

	static rendering::GfxDeviceArgs getDeviceArgs( appdata::AppData& appData )
	{
		rendering::GfxDeviceArgs deviceArgs{};
		deviceArgs.windowSize = _window->getSize();

		auto shaderSources = appData.getShaderSources();
		if (shaderSources.empty())
		{
			LOG_ERROR( "No shader sources found!" );
			exit( EXIT_FAILURE );
		}

		for (const auto& shaderSource : shaderSources)
		{
			deviceArgs.shaderIds.push_back( shader::getShaderId( shaderSource.name.c_str() ) );
			deviceArgs.vertexShaderFiles.push_back( shaderSource.vertexSourceFile.c_str() );
			deviceArgs.fragmentShaderFiles.push_back( shaderSource.fragmentSourceFile.c_str() );
		}

		const resources::ResourceContainer& container = _resouceSystem->getResourceContainer();
		
		const auto& textureData = container.getTextureMap();
		
		for (const auto& [handle, param] : textureData)
		{
			deviceArgs.textureData.push_back( &param );
			deviceArgs.textureIds.push_back( handle.id );
		}

		const auto& vertexData = container.getVertexDataMap();
		
		for (const auto& [handle, param] : vertexData)
		{
			deviceArgs.vertexData.push_back( &param );
			deviceArgs.meshIds.push_back( handle.id );
		}

		const auto& textureDescs = container.getTextureDescriptorMap();
		
		for (const auto& [handle, param] : textureDescs)
		{
			deviceArgs.textureDescriptorIds.push_back( handle.id );
			deviceArgs.textureDescriptors.push_back( param );
		}

		return deviceArgs;
	}


	static void handleProfilerConnectedChanged()
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

	static void cleanup()
	{
		_window->close();
		AppContext::cleanup();
	}
}

MainLoop::MainLoop( Args args )
{
	AppContext::initialize();

	_appData.initialize();
	_window.reset( new window::Window( args.title, window::WindowMode::Windowed ) );

	_resouceSystem.reset( new resources::ResourceSystem( _appData ) );
	rendering::GfxDeviceArgs deviceArgs = getDeviceArgs( _appData );
	rendering::GfxDeviceFactory deviceFactory{ deviceArgs };

	_gfxWorker.reset( new rendering::GfxWorker{ deviceFactory } );
	_gfxWorker->setApi( api::OPENGL );

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
		postUpdate( global::FRAME_TIME_SECONDS );

		FRAME_MARK;

	} while (running());

	cleanup();
}


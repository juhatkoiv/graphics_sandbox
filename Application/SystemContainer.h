#pragma once

#include <concepts>
#include <memory>
#include <vector>

#include "ECS/System/System.h"
#include "Result.h"

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
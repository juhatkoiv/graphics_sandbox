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
	class Builder 
	{
	public:
		static Builder create()
		{
			return Builder();
		}

		Builder& withSystem( std::unique_ptr<ecs::System>&& system )
		{
			_factories.push_back( std::move( system ) );
			return *this;
		}

		std::unique_ptr<SystemContainer> build()
		{
			return std::make_unique<SystemContainer>( std::move( _factories ) );
		}

	private:
		std::vector<std::unique_ptr<ecs::System>> _factories;
	};

	void update( ecs::World& world, float deltaTime )
	{
		for (const auto& system : _systems)
		{
			system->update( world, deltaTime );
		}
	}

	SystemContainer( std::vector<std::unique_ptr<ecs::System>>&& systems )
		: _systems( std::move( systems ) )
	{}

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
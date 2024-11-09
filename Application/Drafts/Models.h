#pragma once
/*
#include <fstream>
#include <variant>

#include "ECS/World.h"
#include "ECS/ComponentIncludes.h"
#include "ECS/Component.h"
#include "AssetLoading/AssimpMeshLoader.h"
#include "AssetLoading/TextureLoader.h"
#include "Resources/ResourceSystem.h"
#include "Serialization/SerializationDefinitions.h"

namespace draft {

	class Serializer
	{
	private:
		json j;
		ecs::World& _world;
		resources::ResourceSystem& _resourceSystem;
	public:
		template<ecs::IsComponent T>
		T serialize()
		{
			T t = j.template get<T>();
			return t;
		}

		Serializer( const json& j, ecs::World& _world, resources::ResourceSystem& _resourceSystem )
			: j( j ), _world( _world ), _resourceSystem( _resourceSystem )
		{
		}
	};

	struct Transform : public ecs::Component<ecs::cid::TRANSFORM>
	{
		glm::vec3 position = {};
		glm::vec3 scale = {};
		glm::quat rotation = {};
		
		void deserialize( Serializer& s )
		{
			Transform other = s.serialize<Transform>();
			position = other.position;
			scale = other.scale;
			rotation = other.rotation;
		}
	};

	DECLARE_SERIALIZABLE( Transform, position, scale, rotation );

	template<ecs::IsComponent ...T>
	using ComponentVariant = std::variant<T...>;

	resources::ResourceSystem _resourceSystem;
	ecs::World _world;

	void createComponent( ecs::cid::ComponentType type, json j ) 
	{
		Serializer s( j, _world, _resourceSystem );
		switch (type)
		{
			case ecs::cid::MESH:
				s.serialize<ecs::Mesh>();
				break;
			case ecs::cid::MATERIAL:
				s.serialize<ecs::Material>();
				break;
			case ecs::cid::TRANSFORM:
				s.serialize<ecs::Transform>();
				break;
			case ecs::cid::CAMERA:
				s.serialize<ecs::Camera>();
				break;
			case ecs::cid::LIGHT:
				s.serialize<ecs::Light>();
				break;
			case ecs::cid::CAMERA_TRANSFORM:
				s.serialize<ecs::Transform>();
				break;
			case ecs::cid::CAMERA_EULERS:
				s.serialize<ecs::Transform>();
				break;
		}
	}

	id::EntityId load( const std::string& key, ecs::World& world )
	{
		auto path = key;

		std::ifstream file;
		file.open( path );

		nlohmann::json j;
		if (file.is_open())
		{
			file >> j;
		}
		file.close();

		ecs::World world;

		id::EntityId id = id::NewEntityId();
		json components = j["components"];

		const auto& types = ecs::cid::getComponentTypes();
		
		for (auto& v : components.items())
		{
			if (key == "mesh" )
			{
				ecs::Mesh me = j.template get<ecs::Mesh>();
				world.set( id, me );
			}
			else if (key == "material")
			{
				ecs::Material ma = j.template get<ecs::Material>();
				world.set( id, ma );
			}
			else if (key == "transform") 
			{
				ecs::Transform tr = j.template get<ecs::Transform>();
				world.set( id, tr );
			}
			else if (key == "camera") 
			{
				ecs::Camera ca = j.template get<ecs::Camera>();
				world.set( id, ca );
			}
			else if (key == "light") 
			{
				ecs::Light li = j.template get<ecs::Light>();
				world.set( id, li );
			}
			else if (key == "camera_transform") 
			{
				ecs::Transform tr = j.template get<ecs::Transform>();
				world.set( id, tr );
			}
			else if (key == "camera_eulers") 
			{
				ecs::Transform tr = j.template get<ecs::Transform>();
				world.set( id, tr );
			}
		}

		return id;
	}
}*/

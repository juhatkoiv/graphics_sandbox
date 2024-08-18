#pragma once

DECLARE1( class, ecs, World );
DECLARE1( class, resources, ResourceSystem );

#include "Serialization/SerializationDefinitions.h"
#include "Component.h"
#include "UniqueId.h"

BEGIN_NAMESPACE1(ecs)

class ModelSerializer
{
public:
	ModelSerializer( World& world, resources::ResourceSystem& resourceSystem );
	~ModelSerializer();

	nlohmann::json serialize( id::EntityId entity, const std::string& modelName );
	

	std::vector<ComponentBuffer> deserialize( const std::string& modelName );
	ComponentBuffer createComponentBuffer( cid::ComponentType componentType, const nlohmann::json& j );
	std::vector<ComponentBuffer> createComponentBuffers( const nlohmann::json& modelGroupJson );
	
private:
	World& _world;
	resources::ResourceSystem& _resourceSystem;
};

END_NAMESPACE1

#include "Precompiled/Precompiled.h"
#include "ModelSerializer.h"

#include "ECS/Component.h"
#include "ECS/ComponentIncludes.h"
#include "ECS/World.h"
#include "Resources/ResourceSystem.h"

#include "AssetLoading/JsonFileSystem.h"

#include "Hash/Hash.h"

#include <filesystem>
#include <unordered_map>

BEGIN_NAMESPACE1( ecs )

namespace 
{
	using json = nlohmann::json;
	using namespace rendering;

	static std::unordered_map<size_t, json>& cache()
	{
		static std::unordered_map<size_t, json> modelCache{};
		return modelCache;
	};

	static TextureType parseTextureType( const json& j ) 
	{
		std::vector<std::string> textureTypeStrings;
		if ( !j.contains( "textureTypes") )
		{
			return TextureType::None;
		}
		j["textureTypes"].get_to( textureTypeStrings );

		TextureType textureTypes = TextureType::None;
		for (const std::string& textureTypeString : textureTypeStrings) 
		{
			textureTypes |= texture::getTextureType( textureTypeString.c_str() );
		}
		return textureTypes;
	}

	static std::vector<std::string> parseTextureTypeNames( TextureType textureTypes ) 
	{
		std::vector<std::string> textureTypeStrings;
		for (size_t i = 0; i < 4; i++)
		{
			if ((textureTypes & texture::toTypeFlag( i )) != TextureType::None) 
			{
				textureTypeStrings.push_back( texture::getTextureTypeName( texture::toTypeFlag( i ) ) );
			}
		}
		return textureTypeStrings;
	}

	static std::array<id::TextureId, texture::TYPE_COUNT> parseTextureIds( const json& j, TextureType textureTypes, resources::ResourceSystem& resourceSystem )
	{
		std::vector<std::string> texturePaths;
		if ( !j.contains( "texturePaths") )
		{
			return {};
		}
		j["texturePaths"].get_to( texturePaths );

		std::array<id::TextureId, texture::TYPE_COUNT> textureIds;
		int index = 0;
		for (size_t i = 0; i < 4; i++)
		{
			if ((textureTypes & texture::toTypeFlag( i )) != TextureType::None) 
			{
				textureIds[i] = resourceSystem.findTextureId( texturePaths[index++] );
			}
		}
		return textureIds;
	}

	static std::vector<std::string> parseTexturePaths( const std::array<id::TextureId, texture::TYPE_COUNT>& arr, TextureType textureTypes, resources::ResourceSystem& resourceSystem )
	{
		std::vector<std::string> texturePaths;
		for (int i = 0; i < arr.size(); i++)
		{
			if ((textureTypes & texture::toTypeFlag( i )) == TextureType::None)
			{
				continue;
			}
			
			id::TextureId textureId = arr[i];

			Result<const std::string&> textureNameResult = resourceSystem.getTextureName( textureId );
			if (textureNameResult.success)
			{
				texturePaths.push_back( textureNameResult.get() );
			}
		}
		return texturePaths;
	}

	static rendering::RenderEffect parseRenderEffect( const json& j ) 
	{
		if ( !j.contains( "renderEffects") )
		{
			return rendering::RenderEffect::None;
		}
		std::vector<std::string> renderEffects;
		j["renderEffects"].get_to( renderEffects );

		rendering::RenderEffect renderEffect = rendering::RenderEffect::None;
		for (const std::string& renderEffectString : renderEffects)
		{
			renderEffect |= rendering::getRenderEffect( renderEffectString.c_str() );
		}
		
		return renderEffect;
	}

	static std::vector<std::string> parseRenderEffectNames( rendering::RenderEffect renderEffect ) 
	{
		std::vector<std::string> renderEffectStrings;
		for (size_t i = 0; i < static_cast<size_t>(rendering::RenderEffect::COUNT); i++)
		{
			rendering::RenderEffect effects = static_cast<rendering::RenderEffect>(1 << i);
			if (( effects & renderEffect ) != rendering::RenderEffect::None) 
			{
				renderEffectStrings.push_back( rendering::getRenderEffectName( effects ) );
			}
		}
		return renderEffectStrings;
	}

	static int parseRenderQueue( const json& j ) 
	{
		std::string queueName{};
		j["renderQueue"].get_to( queueName );
		
		static std::unordered_map<std::string, int> map
		{
			{"Opaque", 0},
			{"SkyBox", 1},
			{"Transparent", 2},
		};
		return map.at( queueName );
	}

	static std::string parseRenderQueueName( int queue )
	{
		static std::unordered_map<int, std::string> map
		{
			{0, "Opaque"},
			{1, "SkyBox"},
			{2, "Transparent"},
		};
		return map.at( queue );
	}
}

ModelSerializer::ModelSerializer( ecs::World& world, resources::ResourceSystem& resourceSystem )
	: _world( world ), _resourceSystem( resourceSystem )
{}

ModelSerializer::~ModelSerializer()
{
}

json ModelSerializer::serialize( id::EntityId entity, const std::string& modelName )
{
	json entityJson{};
	entityJson["name"] = modelName;

	json& components = entityJson["components"];
	for (unsigned i = 0; i < cid::ComponentType::COUNT; i++) 
	{
		cid::ComponentType type = static_cast<cid::ComponentType>(i);
		if (!_world.has( entity, type ))
		{
			continue;
		}

		switch (type)
		{	
		case ecs::cid::TRANSFORM:
		{
			const Transform* transform = _world.get<ecs::Transform>( entity );
			
			json& j = components["transform"];
			j = *transform;
			break;
		}
		case ecs::cid::MESH:
		{
			const Mesh* mesh = _world.get<ecs::Mesh>( entity );
			json& j = components["mesh"];

			Result<const std::string&> meshNameResult = _resourceSystem.getMeshName( mesh->meshId );
			std::string name = std::filesystem::path { meshNameResult.value }.filename().string();
			if (meshNameResult.success)
			{
				j["path"] = meshNameResult.get();
				j["name"] = name;
			}
			break;
		}
		case ecs::cid::MATERIAL:
		{
			const Material* material = _world.get<ecs::Material>( entity );
			json& j = components["material"];

			j["shaderName"] = _resourceSystem.getShaderName( material->shaderId );
			j["meshColor"] = material->meshColor;
			j["renderQueue"] = parseRenderQueueName( material->renderQueue );
			j["textureTypes"] = parseTextureTypeNames( material->textureTypes );
			j["texturePaths"] = parseTexturePaths( material->textureIds, material->textureTypes, _resourceSystem );
			break;
		}
		case ecs::cid::CAMERA:
		{
			const Camera* camera = _world.get<ecs::Camera>( entity );
			json& j = components["camera"];
			j = *camera;
			break;
		}
		case ecs::cid::CAMERA_TRANSFORM:
		{
			const CameraTransform* cameraTransform = _world.get<ecs::CameraTransform>( entity );
			json& j = components["camera_transform"];
			j = *cameraTransform;
			break;
		}
		case ecs::cid::CAMERA_EULERS:
		{
			const CameraEulers* cameraEulers = _world.get<ecs::CameraEulers>( entity );
			json& j = components["camera_eulers"];
			j = *cameraEulers;
			break;
		}
		case ecs::cid::LIGHT:
		{
			const Light* light = _world.get<ecs::Light>( entity );
			json& j = components["light"];
			j = *light;
			break;
		}
		case ecs::cid::RENDER_EFFECTS:
		{
			const RenderEffects* renderEffects = _world.get<ecs::RenderEffects>( entity );
			json& j = components["render_effects"];
			j["renderEffects"] = parseRenderEffectNames( renderEffects->effects );
			break;
		}
		case ecs::cid::COUNT:
		{
			break;
		}
		default:
			break;
		}
	}
	return entityJson;
}

std::vector<ComponentBuffer> ModelSerializer::deserialize( const std::string& modelName )
{
	size_t hash = hash::get64( modelName );
	auto& jsonCache = cache();
	if (jsonCache.contains( hash ))
	{
		return createComponentBuffers( jsonCache[hash] );
	}
	
	json j = assets::JsonFileSystem::LoadJson( modelName.c_str() );
	if (j.is_null())
	{
		throw std::runtime_error( "model name " + modelName + ".json " + "could not be loaded." );
	}
	jsonCache[hash] = j;

	return createComponentBuffers( j );
}

std::vector<ComponentBuffer> ModelSerializer::createComponentBuffers( const nlohmann::json& modelGroupJson )
{
	const json& other = modelGroupJson["components"];

	std::vector<ComponentBuffer> componentBuffers;
	for (auto& [type, name] : cid::componentNames)
	{
		if (other.contains( name ))
		{
			const json& componentJson = other[name];
			if (componentJson.is_null())
				throw;

			ComponentBuffer buffer = createComponentBuffer( type, componentJson );
			componentBuffers.push_back( buffer );
		}
	}
	return componentBuffers;

}

ComponentBuffer ModelSerializer::createComponentBuffer( cid::ComponentType componentType, const nlohmann::json& j )
{
	switch (componentType)
	{
	case cid::TRANSFORM:
	{
		Transform tranform = j;
		return ComponentBuffer::copyToBuffer( tranform );
	}
	case cid::MESH:
	{
		Mesh mesh;
		
		std::string meshPath = j["path"];
		mesh.meshId = _resourceSystem.findMeshId( meshPath );
		
		return ComponentBuffer::copyToBuffer( mesh );
	}
	case cid::MATERIAL:
	{
		std::string shaderName = j["shaderName"];
		TextureType textureTypes = parseTextureType( j );

		Material material;
		material.meshColor = j["meshColor"];
		material.shaderId = _resourceSystem.findShaderId( shaderName );
		material.renderQueue = parseRenderQueue( j );
		material.textureTypes = textureTypes;
		material.textureIds = parseTextureIds( j, textureTypes, _resourceSystem );

		return ComponentBuffer::copyToBuffer( material );
	}
	case cid::CAMERA:
	{
		Camera camera = j;
		return ComponentBuffer::copyToBuffer( camera );
	}
	case cid::CAMERA_TRANSFORM:
	{
		CameraTransform cameraTransform = j;
		return ComponentBuffer::copyToBuffer( cameraTransform );
	}
	case cid::CAMERA_EULERS:
	{
		CameraEulers cameraEulers = j;
		return ComponentBuffer::copyToBuffer( cameraEulers );
	}
	case cid::LIGHT:
	{
		Light light = j;
		return ComponentBuffer::copyToBuffer( light );
	}
	case cid::RENDER_EFFECTS:
	{
		RenderEffects renderEffects;
		renderEffects.effects = parseRenderEffect( j );
		return ComponentBuffer::copyToBuffer( renderEffects );
	}
	case cid::COUNT:
	{
		break;
	}
	default:
		break;
	}
	throw std::runtime_error( "Component type not found." );
}

END_NAMESPACE1

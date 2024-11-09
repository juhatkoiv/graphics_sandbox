#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Serialization/ComponentSerializationDefinitions.h"
#include "Rendering/Texture.h"
#include "Component.h"

BEGIN_NAMESPACE1( ecs )

static constexpr glm::vec4 DEFAULT_MESH_COLOR = { 1, 1, 1, 1 };

struct Material : public Component<cid::MATERIAL>
{
	glm::vec4 meshColor = DEFAULT_MESH_COLOR;					// 16 bytes
	std::array<id::TextureId, texture::TYPE_COUNT> textureIds = { ~0u, ~0u, ~0u, ~0u };	// 16 bytes
	int renderQueue = 0;										// 4 bytes
	id::ShaderId shaderId;										// 4 bytes
	rendering::TextureType textureTypes;									// 4 bytes

	const Material& operator=( const Material& other ) 
	{
		meshColor = other.meshColor;
		shaderId = other.shaderId;
		renderQueue = other.renderQueue;
		textureTypes = other.textureTypes;
		textureIds = other.textureIds;
		for (size_t i = 0; i < textureIds.size(); i++) 
		{
			textureIds[i] = other.textureIds[i];
		}
		return *this;
	}
};

//DECLARE_SERIALIZABLE( Material, shaderId, textureTypes, textureParams );

END_NAMESPACE1
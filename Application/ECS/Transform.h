#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ECS/Serialization/ComponentSerializationDefinitions.h"
#include "Containers/SparseMap.h"
#include "Component.h"

DECLARE1(struct, ecs, EntitySpawnParam);

BEGIN_NAMESPACE1(ecs)

using OldNewMap = std::unordered_map<id::EntityId, id::EntityId>;

struct Transform : public Component<cid::TRANSFORM>
{
	glm::vec3 position = {};	// 12 bytes
	glm::vec3 scale = {};		// 12 bytes
	glm::quat rotation = {};	// 16 bytes

	static Transform create(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& axis, float angle);
	
	static Transform create(const EntitySpawnParam& params);
	
	void updateModelMatrix(glm::mat4& modelMatrix);

	void rotateBy(const glm::vec3& axis, float angle);

	void moveBy(const glm::vec3& positionDelta);

	void scaleBy(const glm::vec3& scaleDelta);

	static void updateModelMatrices(std::vector<Transform*>& transforms, std::vector<glm::mat4>& modelMatrices);
};
DECLARE_SERIALIZABLE( Transform, position, scale, rotation );

END_NAMESPACE1

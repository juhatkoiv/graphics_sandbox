#include "Precompiled/Precompiled.h"
#include "Transform.h"

#include "EntitySpawnParams.h"

BEGIN_NAMESPACE1(ecs)

Transform Transform::create(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& axis, float angle)
{
	Transform transform;
	transform.position = position;
	transform.scale = scale;
	transform.rotation = glm::angleAxis(angle, axis);

	return transform;
}

Transform Transform::create(const EntitySpawnParam& params)
{
	Transform transform;
	transform.position = params.position;
	transform.scale = params.scale;
	transform.rotation = glm::angleAxis(params.deltaAngle, params.rotationAxis);

	return transform;
}

void Transform::updateModelMatrix(glm::mat4& modelMatrix)
{
	glm::mat4 translateMatrix(1.0f);
	translateMatrix = glm::translate(translateMatrix, position);

	glm::mat4 scaleMatrix(1.0f);
	scaleMatrix[0].x = scale.x;
	scaleMatrix[1].y = scale.y;
	scaleMatrix[2].z = scale.z;

	glm::mat4 rot = glm::mat4_cast(rotation);

	modelMatrix = translateMatrix * rot * scaleMatrix;
}

void Transform::rotateBy(const glm::vec3& axis, float angle)
{
	rotation = glm::rotate(rotation, angle, axis);
}

void Transform::moveBy(const glm::vec3& positionDelta)
{
	position += positionDelta;
}

void Transform::scaleBy(const glm::vec3& scaleDelta)
{
	scale += scaleDelta;
}

void Transform::updateModelMatrices(std::vector<Transform*>& transforms, std::vector<glm::mat4>& modelMatrices)
{
	for (size_t i = 0; i < transforms.size(); i++)
	{
		glm::mat4 modelMatrix;
		transforms[i]->updateModelMatrix(modelMatrix);
		modelMatrices.emplace_back(std::move(modelMatrix));
	}
}


END_NAMESPACE1
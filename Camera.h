#pragma once

#include <glm/glm.hpp>

class Camera 
{
public:
	Camera(const glm::vec3& worldPosition);

	const glm::vec3& GetPosition() const;
	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;

private:
	glm::vec3 _worldPosition;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
};
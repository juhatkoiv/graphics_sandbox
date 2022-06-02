#include "Precompiled.h"
#include "Camera.h"

#include <glm/ext.hpp>

Camera::Camera(const glm::vec3& worldPosition) 
	: _worldPosition(worldPosition)
{
	_viewMatrix = glm::perspective(glm::radians(45.0f), (float)4 / (float)3, 0.1f, 100.0f);
	_projectionMatrix = glm::translate(glm::mat4(1.0f), _worldPosition);
}

const glm::vec3& Camera::GetPosition() const
{
	return _worldPosition;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return _projectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return _viewMatrix;
}

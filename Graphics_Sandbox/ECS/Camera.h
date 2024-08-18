#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Precompiled/PrecompilationMacros.h"
#include "Rendering/RenderingSettings.h"
#include "Serialization/ComponentSerializationDefinitions.h"
#include "Component.h"

BEGIN_NAMESPACE1(ecs);

struct Camera : public Component<cid::CAMERA>
{
	rendering::RenderingSettings renderingSettings;
	glm::mat4 viewMatrix{};
	glm::mat4 projectionMatrix{};
	glm::vec3 position{};
	glm::uvec2 viewPortSize{};
	glm::ivec2 viewPortPosition{};
};
DECLARE_SERIALIZABLE(Camera, viewMatrix, projectionMatrix, position, viewPortSize, viewPortPosition);

struct CameraTransform : public Component<cid::CAMERA_TRANSFORM>
{
	glm::vec3 cameraForward{};
	glm::vec3 cameraRight{};
	glm::vec3 cameraUp{};
};
DECLARE_SERIALIZABLE(CameraTransform, cameraForward, cameraRight, cameraUp);

struct CameraEulers : public Component<cid::CAMERA_EULERS>
{
	float pitch = 0.0f;
	float yaw = 0.0f;
};
DECLARE_SERIALIZABLE(CameraEulers, pitch, yaw);


END_NAMESPACE1;
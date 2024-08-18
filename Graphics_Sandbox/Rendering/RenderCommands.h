#pragma once

#include <glm/glm.hpp>
#include <array>

#include "Rendering/Texture.h"
#include "Rendering/Vertex.h"
#include "Rendering/RenderEffect.h"
#include "Rendering/RenderingSettings.h"
#include "ECS/Light.h"
#include "ECS/Mesh.h"
#include "ECS/Material.h"
#include "ECS/UniqueId.h"

BEGIN_NAMESPACE1( rendering )

enum class CommandType
{
	NONE,
	PrepareTransparentQueue,
	CreateTexture,
	CreateMesh,
	CreateFullScreenQuad,
	UpdateCameraProjection,
	UpdateCameraTransform,
	UpdateWindowSize,
	UpdateCameraSettings,
	UpdateModelMatrix,
	SetTexture,
	SetLight,
	SetEffect,
	SetMesh,
	SetMeshInstanced,
	SetMaterial,
	SetSkyBox,
	Destroy,
};

struct CreateTextureCmd 
{
	const rendering::TextureData* textureData = nullptr;
	id::TextureId textureId = id::InvalidTextureId;
};

struct CreateMeshCmd 
{
	const rendering::VertexData* vertexData = nullptr;
	id::MeshId meshId = id::InvalidMeshId;
};

struct SetTextureCmd
{
	id::TextureId textureId = id::InvalidTextureId;
	TextureType type = TextureType::None;
};

struct SubmitToEffectQueueCmd
{
	rendering::RenderEffect effects = rendering::RenderEffect::None;
};

struct SetSkyBoxCmd
{
	id::TextureId textureId = id::InvalidTextureId;
	id::MeshId meshId = id::InvalidMeshId;
};

struct UpdateWindowCmd
{
	glm::uvec2 windowSize = {};
	glm::ivec2 windowPosition = {};
};

struct UpdateProjectionMatrixCmd
{
	glm::mat4 projectionMatrix = {};
};

struct UpdateCameraTransformCmd
{
	glm::vec3 viewPosition = {};
	glm::mat4 viewMatrix = {};
};

struct SetEntityMeshCmd
{
	id::MeshId meshId = id::InvalidMeshId;
};

struct UpdateModelMatrixCmd
{
	glm::mat4 modelMatrix = {};
};

struct SetLightCmd
{
	glm::vec3 lightColor = ecs::DEFAULT_LIGHT_COLOR;
	float intensity = 1.0f;
	float attenuation = 0.0f;
};

struct DestroyEntityCmd
{
	// TODO - add entity id and logic
};

struct SetEntityMaterialCmd
{
	glm::vec4 meshColor{};
	id::ShaderId shaderId;
	int queue = -1;
};

struct SetFullScreenQuadCmd
{
	id::MeshId meshId = id::InvalidMeshId;
};

struct UpdateCameraRenderSettingsCmd
{
	rendering::RenderingSettings settings;
};

struct RenderCommand
{
	RenderCommand() {}

	CommandType type = CommandType::NONE;
	unsigned id = ~0u;
	unsigned resourceSlot = ~0u;
	union
	{
		SubmitToEffectQueueCmd submitToEffectQueueCmd;
		UpdateWindowCmd updateWindowSizeCmd;
		UpdateProjectionMatrixCmd updateProjectionMatrixCmd;
		UpdateCameraTransformCmd updateCameraTrasformCmd;
		CreateMeshCmd createMeshCmd;
		CreateTextureCmd createTextureCmd;
		SetTextureCmd setTextureCmd;
		DestroyEntityCmd destroyEntityCmd;
		SetLightCmd setlightCmd;
		SetEntityMaterialCmd setEntityMaterialCmd;
		SetEntityMeshCmd setEntityMeshCmd;
		UpdateModelMatrixCmd updateModelMatrixCmd;
		SetFullScreenQuadCmd setFullScreeQuadCmd;
		UpdateCameraRenderSettingsCmd updateCameraRenderingSettingsCmd;
		SetSkyBoxCmd setSkyboxCmd;
	};
};

END_NAMESPACE1
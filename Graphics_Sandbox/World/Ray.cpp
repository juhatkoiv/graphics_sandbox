#include "Precompiled/Precompiled.h"
#include "Ray.h"

#include "glm/gtx/string_cast.hpp"

#include "Window/Window.h"
#include "ECS/UniqueId.h"
#include "ECS/Camera.h"

BEGIN_NAMESPACE1( world )

namespace
{
	static glm::vec2 GetNormalizedMouseCoords( const glm::vec2& windowSize, const glm::vec2& screenCoord )
	{
		Logger::LogInfo( glm::to_string( screenCoord ).c_str() );

		float normalizedX = (2.0f * screenCoord.x) / windowSize.x - 1.0f;
		float normalizedY = (2.0f * screenCoord.y) / windowSize.y - 1.0f;
		return glm::vec2{ normalizedX, -normalizedY };
	}

	static glm::vec4 GetMouseClipSpaceCoords( const glm::vec2& normalizedMouseCoords )
	{
		return glm::vec4{ normalizedMouseCoords.x, normalizedMouseCoords.y, -1.0f, 1.0f };
	}

	static glm::vec4 GetMouseViewSpaceCoods( const glm::mat4& projectionMatrix, const glm::vec4& mouseClipSpaceCoords )
	{
		glm::mat4 invProjectionMatrix = glm::inverse( projectionMatrix );
		glm::vec4 mousePointEye = invProjectionMatrix * mouseClipSpaceCoords;
		mousePointEye.z = -1;
		mousePointEye.w = 0;

		return mousePointEye;
	}

	static glm::vec4 GetMouseViewSpaceCoods( const ecs::Camera& camera, const glm::vec4& mouseClipSpaceCoords )
	{
		return GetMouseViewSpaceCoods( camera.projectionMatrix, mouseClipSpaceCoords );
	}

	static glm::vec3 GetWorldRayDirection( const glm::mat4& viewMatrix, const glm::vec4& mouseViewSpaceCoords )
	{
		glm::mat4 invViewMatrix = glm::inverse( viewMatrix );
		glm::vec4 mousePointWorld = invViewMatrix * mouseViewSpaceCoords;

		return glm::normalize( mousePointWorld );
	}

	static glm::vec3 GetWorldRayDirection( const ecs::Camera& camera, const glm::vec4& mouseViewSpaceCoords )
	{
		return GetWorldRayDirection( camera.viewMatrix, mouseViewSpaceCoords );
	}
}

Ray Ray::FindMouseToWorldRay( const ecs::Camera& camera, const glm::vec2& screenCoord )
{
	glm::vec2 mouseCoordNormalized = GetNormalizedMouseCoords( camera.viewPortSize, screenCoord );
	glm::vec4 mouseClipSpaceCoords = GetMouseClipSpaceCoords( mouseCoordNormalized );
	glm::vec4 mouseViewSpaceCoords = GetMouseViewSpaceCoods( camera, mouseClipSpaceCoords );

	glm::vec3 dir = GetWorldRayDirection( camera, mouseViewSpaceCoords );
	Ray ray{ camera.position, dir, id::NewRayId() };

	return ray;
}

END_NAMESPACE1
#include "Precompiled/Precompiled.h"
#include "CameraSystem.h"
#include "Screen/Screen.h"
#include "Window/Window.h"
#include "World/Ray.h"
#include "Input/InputStateMutable.h"

BEGIN_NAMESPACE1( ecs )

namespace
{
	void updateCameraTransform( const CameraEulers& cameraEulers, CameraTransform& cameraTransform )
	{
		glm::vec3 front;
		front.x = cos( glm::radians( cameraEulers.yaw ) ) * cos( glm::radians( cameraEulers.pitch ) );
		front.y = sin( glm::radians( cameraEulers.pitch ) );
		front.z = sin( glm::radians( cameraEulers.yaw ) ) * cos( glm::radians( cameraEulers.pitch ) );

		cameraTransform.cameraForward = glm::normalize( front );
		cameraTransform.cameraRight = glm::normalize( glm::cross( cameraTransform.cameraForward, global::WORLD_UP ) );
		cameraTransform.cameraUp = glm::normalize( glm::cross( cameraTransform.cameraRight, cameraTransform.cameraForward ) );
	}

	void updateCameraViewMatrix( const glm::vec3& position, const CameraTransform& cameraTransform, glm::mat4& cameraViewMatrix )
	{
		cameraViewMatrix = glm::lookAt( position, position + cameraTransform.cameraForward, cameraTransform.cameraUp );
	}

	void updateProjectionMatrix( const glm::vec2& windowSize, glm::mat4& projectionMatrix )
	{
		float aspect = (float)windowSize.x / (float)windowSize.y;
		if (std::isnan(aspect))
			return;

		projectionMatrix = glm::perspective( glm::radians( 45.0f ), aspect, 0.1f, 50.f );
	}

	void updateCameraEulers( const glm::vec2& mouseOffset, CameraEulers& cameraEulers )
	{
		if (app::hasState( app::AppState::CursorVisible ))
			return;

		const float mouseSensitivity = 0.015f;

		auto cameraEulerOffset = mouseOffset * mouseSensitivity;

		cameraEulers.yaw += cameraEulerOffset.x;
		cameraEulers.pitch += cameraEulerOffset.y;

		if (cameraEulers.pitch > 89.0f)
			cameraEulers.pitch = 89.0f;
		if (cameraEulers.pitch < -89.0f)
			cameraEulers.pitch = -89.0f;
	}

	void updateCameraMove( Camera& camera, CameraTransform& cameraTransform )
	{
		const float movementSpeed = 20.f;

		float velocity = movementSpeed * global::FRAME_TIME_SECONDS;

		if (input::consumeKey( input::InputEvent::MOVE_FORWARD ))
			camera.position += cameraTransform.cameraForward * velocity;
		if (input::consumeKey( input::InputEvent::MOVE_BACKWARDS ))
			camera.position -= cameraTransform.cameraForward * velocity;
		if (input::consumeKey( input::InputEvent::STRAFE_LEFT ))
			camera.position -= cameraTransform.cameraRight * velocity;
		if (input::consumeKey( input::InputEvent::STRAFE_RIGHT ))
			camera.position += cameraTransform.cameraRight * velocity;
		if (input::consumeKey( input::InputEvent::MOVE_UP ))
			camera.position += cameraTransform.cameraUp * velocity;
		if (input::consumeKey( input::InputEvent::MOVE_DOWN ))
			camera.position -= cameraTransform.cameraUp * velocity;
	}

	void updateWorldRay( const Camera& camera )
	{
		if (input::consumeKey( input::InputEvent::MOUSE_BUTTON_RIGHT ))
		{
			glm::vec2 mousePos = input::getLastMousePosition();
			input::setFoundRay( world::Ray::FindMouseToWorldRay( camera, mousePos ) );
		}
	}

	void updateViewPortSize( Camera& camera )
	{
		camera.viewPortSize = screen::getScreenSize();
		camera.viewPortPosition = screen::getWindowPosition();
	}

	static id::EntityId createCameraEntity( World& world, glm::vec2 windowSize )
	{
		id::EntityId cameraEntityId = world.spawn(
			ecs::CameraTransform
			{
				.cameraForward = glm::vec3( 0, 0, -1 ),
				.cameraRight = glm::vec3( 1, 0, 0 ),
				.cameraUp = glm::vec3( 0, 1, 0 )
			},
			ecs::CameraEulers
			{
				.pitch = 0.0f,
				.yaw = -90.0f
			},
			ecs::Camera
			{
				.renderingSettings = rendering::RenderingSettings
				{
					.lightingSettings = rendering::LightingSettings
					{
						.exposure = 1.0f,
						.gamma = 1.0f
					},
					.fullScreenEffect = rendering::PostProcessingEffects::Nothing,
					.fullScreenBlurSettings = rendering::FullScreenBlurSettings
					{
						.kernelSize = 1,
						.sigma = 1.0f
					}
				},
				.viewMatrix = glm::mat4{ 1.0f },
				.projectionMatrix = glm::mat4{ 0.0f },
				.position = glm::vec3{ 0.0f, 0.0f, 6.0f },
				.viewPortSize = windowSize,
			}
		);

		return cameraEntityId;
	}

}

CameraSystem::CameraSystem( const window::Window& window )
	: _window( window )
{}


void CameraSystem::update( World& world, float deltaTime )
{
	if (!world.hasAny<Camera>())
	{
		_cameraEntityId = createCameraEntity( world, _window.getSize() );
	}

	auto& cameraTransform = *world.getMutable<CameraTransform>( _cameraEntityId );
	auto& camera = *world.getMutable<Camera>( _cameraEntityId );
	auto& cameraEuler = *world.getMutable<CameraEulers>( _cameraEntityId );

	if ( camera.viewPortSize.x < 1 || camera.viewPortSize.y < 1)
	{
		return;
	}

	glm::vec2 lastMouseOffset = input::getLastMouseOffset();

	updateViewPortSize( camera );
	updateCameraEulers( lastMouseOffset, cameraEuler );
	updateCameraTransform( cameraEuler, cameraTransform );
	updateCameraViewMatrix( camera.position, cameraTransform, camera.viewMatrix );
	updateProjectionMatrix( camera.viewPortSize, camera.projectionMatrix );
	updateCameraMove( camera, cameraTransform );
	updateWorldRay( camera );
}

END_NAMESPACE1
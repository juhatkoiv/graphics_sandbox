#pragma once

#include "System.h"
#include "ECS/UniqueId.h"


DECLARE1( class, window, Window );

BEGIN_NAMESPACE1(ecs)

class CameraSystem : public System 
{
public:
	CameraSystem( const window::Window& window );
	~CameraSystem() {}
	void update(World& world, float deltaTime) override;

private:
	id::EntityId _cameraEntityId{};
	const window::Window& _window;
};

END_NAMESPACE1;
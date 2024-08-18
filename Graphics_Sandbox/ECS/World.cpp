#include "Precompiled/Precompiled.h"
#include "World.h"

#include "Light.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

BEGIN_NAMESPACE1(ecs)

void components_api_test()
{
	id::EntityId entity = 0;
	World ecs{};
	
	Transform* t = ecs.add<Transform>(entity);
	t->position = { 1,-1,1 };

	ecs.add<Mesh>(entity);
	ecs.add<Material>(entity);
	
	Light light;
	light.attenuation = 0.1f;
	ecs.set<Light>(entity, light);

	ecs.set<Light, Mesh>(entity, Light{}, Mesh{});

	if (ecs.has<Light>(entity))
	{
		Light* l = ecs.getMutable<Light>(entity);
		l->attenuation = 0.5f;
	}

	if (ecs.has<Transform>(entity))
	{
		Transform* t = ecs.getMutable<Transform>(entity);
		t->scale = {0.1f, 0.1f, 1.f};
	}

	if (ecs.has<Material>(entity)) 
	{
		ecs.destroyEntity(entity);
	}
}

END_NAMESPACE1

#pragma once

#include <string>

#include "ECS/Serialization/ComponentSerializationDefinitions.h"
#include "Component.h"

BEGIN_NAMESPACE1(ecs)

struct Mesh : public Component<cid::MESH>
{
	id::MeshId meshId = id::InvalidMeshId;
};

//DECLARE_SERIALIZABLE(Mesh, meshName);

END_NAMESPACE1

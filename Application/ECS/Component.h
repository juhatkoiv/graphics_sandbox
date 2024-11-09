#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include <concepts>

constexpr size_t COMPONENT_ALIGNMENT = 4;
#define COMPONENT_ALIGNMENT alignas(COMPONENT_ALIGNMENT)

BEGIN_NAMESPACE1( ecs )

using ComponentId = size_t;

template<typename T>
concept IsComponent = requires(T)
{
	{ T::getId() } -> std::same_as<ComponentId>;
};

namespace cid
{
	enum ComponentType
	{
		TRANSFORM = 0,
		MESH = 1,
		MATERIAL = 2,
		CAMERA = 3,
		CAMERA_TRANSFORM = 4,
		CAMERA_EULERS = 5,
		LIGHT = 6,
		RENDER_EFFECTS = 7,
		COUNT = 8
	};

	inline static const std::unordered_map<ComponentType, std::string> componentNames
	{
		{TRANSFORM, "transform"},
		{MESH, "mesh"},
		{MATERIAL, "material"},
		{CAMERA, "camera"},
		{CAMERA_TRANSFORM, "camera_transform"},
		{CAMERA_EULERS, "camera_eulers"},
		{LIGHT, "light"},
		{RENDER_EFFECTS, "render_effects"}
	};

	inline static std::string getComponentName( ComponentType type )
	{
		return componentNames.at(type);
	}

	inline static ComponentType getComponentTypeByName( const char* name )
	{
		static std::unordered_map<std::string, ComponentType> map
		{
			{"transform", TRANSFORM},
			{"mesh", MESH},
			{"material", MATERIAL},
			{"camera", CAMERA},
			{"camera_transform", CAMERA_TRANSFORM},
			{"camera_eulers", CAMERA_EULERS},
			{"light", LIGHT},
			{"render_effects", RENDER_EFFECTS}
		};
		return map.at(name);
	}

	inline static std::vector<ComponentType> getComponentTypes()
	{
		static auto result = std::vector<ComponentType>();
		if (result.empty())
		{
			result.reserve( ComponentType::COUNT );
			for (auto i = 0; i < ComponentType::COUNT; ++i)
			{
				result.push_back( static_cast<ComponentType>(i) );
			}
		}
		return result;
	}
}

// Transient object, doesn't own the memory, just passes it forward to the actual owner
struct ComponentBuffer 
{
	uint8_t* data = nullptr;
	ComponentId typeId;

	template<IsComponent T>
	static ComponentBuffer copyToBuffer( const T& t )
	{
		uint8_t* ptr = new uint8_t[sizeof(T)];
		memcpy( (void*)ptr, (void*)&t, sizeof(T));
		
		ComponentBuffer buffer;
		buffer.data = ptr;
		buffer.typeId = T::getId();
		return buffer;
	}

	template<IsComponent T>
	static T copyFromBuffer( const ComponentBuffer& buffer )
	{
		T t;
		memcpy( (void*)&t, (void*)buffer.data, sizeof(T) );
		return t;
	}
};

struct ComponentBase {};

template<cid::ComponentType TYPE>
struct alignas(8) Component : public ComponentBase
{
	inline static ComponentId getId() { return static_cast<ComponentId>(TYPE); }

};

template<cid::ComponentType TYPE>
struct ComponentTraits;

#define DECLARE_COMPONENT( type, enumType ) \
template<> \
struct ComponentTraits<enumType> \
{ \
	using type = type; \
};


END_NAMESPACE1
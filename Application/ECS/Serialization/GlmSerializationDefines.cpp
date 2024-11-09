#include "Precompiled/Precompiled.h"
#include "GlmSerializationDefines.h"

#include <glm/gtc/type_ptr.hpp>

using namespace nlohmann;

NLOHMANN_JSON_NAMESPACE_BEGIN

namespace
{
	template<typename T>
	concept Subscriptable = requires(T a, size_t index) {
		{ a[index] } -> std::convertible_to<typename T::value_type>;
	};


	template<size_t SIZE, Subscriptable T>
	void from_json_impl( const json& j, T& v)
	{
		float arr[SIZE];
		j.at( "values" ).get_to( arr );

		for ( size_t i = 0; i < SIZE; i++ )
		{
			v[i] = arr[i];
		}
	}

	template<size_t SIZE, Subscriptable T>
	void to_json_impl( json& j, const T& v )
	{
		float arr[SIZE];
		for ( size_t i = 0; i < SIZE; i++ )
		{
			arr[i] = v[i];
		}

		j = json{ {"values", arr} };
	}
}

void adl_serializer<glm::mat4>::to_json( json& j, const glm::mat4& v )
{
	for ( int x = 0; x < 4; x++ )
	{
		for ( int y = 0; y < 4; y++ )
		{
			j[std::to_string( x ) + std::to_string( y )] = v[x][y];
		}
	}
}

void adl_serializer<glm::mat4>::from_json( const json& j, glm::mat4& v )
{
	for ( int x = 0; x < 4; x++ )
	{
		for ( int y = 0; y < 4; y++ )
		{
			v[x][y] = j[std::to_string( x ) + std::to_string( y )];
		}
	}
}

void adl_serializer<glm::vec2>::to_json( json& j, const glm::vec2& v )
{
	to_json_impl<2>( j, v );
}

void adl_serializer<glm::vec2>::from_json( const json& j, glm::vec2& v )
{
	from_json_impl<2>( j, v );
}

void adl_serializer<glm::ivec2>::to_json( json& j, const glm::ivec2& v )
{
	to_json_impl<2>( j, v );
}

void adl_serializer<glm::ivec2>::from_json( const json& j, glm::ivec2& v )
{
	from_json_impl<2>( j, v );
}

void adl_serializer<glm::uvec2>::to_json( json& j, const glm::uvec2& v )
{
	to_json_impl<2>( j, v );
}

void adl_serializer<glm::uvec2>::from_json( const json& j, glm::uvec2& v )
{
	from_json_impl<2>( j, v );
}

void adl_serializer<glm::vec3>::to_json(json& j, const glm::vec3& v)
{
	to_json_impl<3>( j, v );
}

void adl_serializer<glm::vec3>::from_json(const json& j, glm::vec3& v)
{
	from_json_impl<3>( j, v );
}

void adl_serializer<glm::vec4>::to_json(json& j, const glm::vec4& v)
{
	to_json_impl<4>( j, v );
}

void adl_serializer<glm::vec4>::from_json(const json& j, glm::vec4& v)
{
	from_json_impl<4>( j, v );
}

void adl_serializer<glm::quat>::to_json(json& j, const glm::quat& q)
{
	to_json_impl<4>( j, q );
}

void adl_serializer<glm::quat>::from_json(const json& j, glm::quat& q)
{
	from_json_impl<4>( j, q );
}

NLOHMANN_JSON_NAMESPACE_END

#include "ECS/Light.h"

void TestSerialization()
{
	json jp = { {"lightColor", glm::vec3{1,1,1} }, {"intensity", 1.0f}, {"attenuation", 1.0f} };
	json jv = { {"x", 0.1f}, {"y", 0.1f}, {"z", 0.1f} };
	json jq = { {"w", 0.1f}, {"x", 0.1f}, {"y", 0.1f}, {"z", 0.1f} };

	ecs::Light p = jp;
	glm::vec3 v = jv;
	glm::quat q = jq;

	jv = v;
	jq = q;
	jp = p;
}

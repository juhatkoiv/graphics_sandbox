#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <nlohmann/json.hpp>

using namespace nlohmann;

NLOHMANN_JSON_NAMESPACE_BEGIN

// TODO NAMESPACE
template<> struct adl_serializer<glm::vec2>
{
	static void to_json( json& j, const glm::vec2& q );
	static void from_json( const json& j, glm::vec2& q );
};

template<> struct adl_serializer<glm::ivec2>
{
	static void to_json( json& j, const glm::ivec2& q );
	static void from_json( const json& j, glm::ivec2& q );
};

template<> struct adl_serializer<glm::uvec2>
{
	static void to_json( json& j, const glm::uvec2& q );
	static void from_json( const json& j, glm::uvec2& q );
};

template<> struct adl_serializer<glm::mat4>
{
	static void to_json( json& j, const glm::mat4& q );
	static void from_json( const json& j, glm::mat4& q );
};

template<> struct adl_serializer<glm::vec3>
{
	static void to_json(json& j, const glm::vec3& q);
	static void from_json(const json& j, glm::vec3& q);
};

template<> struct adl_serializer<glm::vec4>
{
	static void to_json(json& j, const glm::vec4& q);
	static void from_json(const json& j, glm::vec4& q);
};

template<> struct adl_serializer<glm::quat>
{
	static void to_json(json& j, const glm::quat& q);
	static void from_json(const json& j, glm::quat& q);
};

NLOHMANN_JSON_NAMESPACE_END

void TestSerialization();
	
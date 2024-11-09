#include "Precompiled/Precompiled.h"
#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include "Rendering/OpenGL/GLFunctions.h"
#include "ShaderBase.h"

BEGIN_NAMESPACE1( rendering )

ShaderProgram::ShaderProgram( unsigned handle )
	: _handle( handle )
{
	assert( _handle != ~0u );
}

void ShaderProgram::bind()
{
	glUseProgram( _handle );
}

void ShaderProgram::setBool( int bindPosition, bool value ) const
{
	glUniform1i( bindPosition, (int)value );
}

void ShaderProgram::setInt( int bindPosition, int value ) const
{
	glUniform1i( bindPosition, value );
}

void ShaderProgram::setFloat( int bindPosition, float value ) const
{
	glUniform1f( bindPosition, value );
}

void ShaderProgram::setMatrix( int bindPosition, const glm::mat4& value ) const
{
	glUniformMatrix4fv( bindPosition, 1, GL_FALSE, glm::value_ptr( value ) );
}

void ShaderProgram::setVec4( int bindPosition, const glm::vec4& value ) const
{
	glUniform4f( bindPosition, value.x, value.y, value.z, value.w );
}

void ShaderProgram::setVec3( int bindPosition, const glm::vec3& value ) const
{
	glUniform3f( bindPosition, value.x, value.y, value.z );
}

void ShaderProgram::setVec2( int bindPosition, const glm::vec2& value ) const
{
	glUniform2f( bindPosition, value.x, value.y );
}

END_NAMESPACE1
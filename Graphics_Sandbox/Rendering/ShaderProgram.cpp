#include "Precompiled/Precompiled.h"
#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include "Rendering/OpenGL/GLFunctions.h"
#include "ShaderBase.h"

BEGIN_NAMESPACE1( rendering )

ShaderProgram::ShaderProgram( const std::string& vertexSource, const std::string& fragmentSource )
{
	VertexShader vertexShader( vertexSource );
	FragmenShader fragmentShader( fragmentSource );

	_handle = glCreateProgram();

	glAttachShader( _handle, vertexShader.GetHandle() );
	glAttachShader( _handle, fragmentShader.GetHandle() );

	glLinkProgram( _handle );

	gl::validateLink( _handle );
}

void ShaderProgram::bind()
{
	glUseProgram( _handle );
}

void ShaderProgram::setBool( const std::string& name, bool value ) const
{
	glUniform1i( glGetUniformLocation( _handle, name.c_str() ), (int)value );
}

void ShaderProgram::setInt( const std::string& name, int value ) const
{
	auto loc = glGetUniformLocation( _handle, name.c_str() );
	glUniform1i( loc, value );
}

void ShaderProgram::setFloat( const std::string& name, float value ) const
{
	auto loc = glGetUniformLocation( _handle, name.c_str() );
	glUniform1f( loc, value );
}

void ShaderProgram::setMatrix( const std::string& name, const glm::mat4& value ) const
{
	const GLchar* pName = name.c_str();
	int bufferLoc = glGetUniformLocation( _handle, pName );
	glUniformMatrix4fv( bufferLoc, 1, GL_FALSE, glm::value_ptr( value ) );
}

void ShaderProgram::setVec4( const std::string& name, const glm::vec4& value ) const
{
	auto loc = glGetUniformLocation( _handle, name.c_str() );
	glUniform4f( loc, value.x, value.y, value.z, value.w );
}

void ShaderProgram::setVec3( const std::string& name, const glm::vec3& value ) const
{
	auto loc = glGetUniformLocation( _handle, name.c_str() );
	glUniform3f( loc, value.x, value.y, value.z );
}

void ShaderProgram::setVec2( const std::string& name, const glm::vec2& value ) const
{
	auto loc = glGetUniformLocation( _handle, name.c_str() );
	glUniform2f( loc, value.x, value.y );
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
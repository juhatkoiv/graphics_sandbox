#pragma once

#include <string>

BEGIN_NAMESPACE1( rendering )

class ShaderProgram
{
public:
	ShaderProgram() = default;
	ShaderProgram( unsigned handle );

	~ShaderProgram() = default;

	void bind();

	void setConstBuffer( unsigned bindPosition, const uint8_t* buffer, size_t size, unsigned usage ) {
		(void)bindPosition; (void)size; (void)buffer; (void)usage;
	}

	void setBool( int bindPosition, bool value ) const;
	void setInt( int bindPosition, int value ) const;
	void setFloat( int bindPosition, float value ) const;
	void setMatrix( int bindPosition, const glm::mat4& value ) const;
	void setVec4( int bindPosition, const glm::vec4& value ) const;
	void setVec3( int bindPosition, const glm::vec3& value ) const;
	void setVec2( int bindPosition, const glm::vec2& value ) const;
private:
	unsigned _handle;
};

END_NAMESPACE1

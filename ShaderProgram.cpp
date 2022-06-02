#include "Precompiled.h"
#include "ShaderProgram.h"

#include "ShaderBase.h"

ShaderProgram::ShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	VertexShader vertexShader(vertexSource);
	FragmenShader fragmentShader(fragmentSource);

	_handle = glCreateProgram();
	glAttachShader(_handle, vertexShader.GetHandle());
	glAttachShader(_handle, fragmentShader.GetHandle());

	glLinkProgram(_handle);

	gl::validateLink(_handle);
}

void ShaderProgram::Bind()
{
	glUseProgram(_handle);
}

void ShaderProgram::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_handle, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_handle, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_handle, name.c_str()), value);
}



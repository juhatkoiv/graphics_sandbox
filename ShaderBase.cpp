#include "Precompiled.h"
#include "ShaderBase.h"

#include <fstream>

ShaderBase::ShaderBase(const std::string& source, unsigned int shaderType)
{
	std::ifstream file(source);
	if (file.is_open()) 
	{
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		const GLchar* s = content.c_str();

		_handle = glCreateShader(shaderType);
		glShaderSource(_handle, 1, &s, NULL);
		glCompileShader(_handle);

		file.close();
	}
	gl::validateCompile(_handle);
}

ShaderBase::~ShaderBase()
{
	glDeleteShader(_handle);
}

unsigned ShaderBase::GetHandle() const
{
	return _handle;
}

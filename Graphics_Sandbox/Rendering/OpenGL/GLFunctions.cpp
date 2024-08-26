#include "Precompiled/Precompiled.h"
#include "GLFunctions.h"

namespace gl 
{
	bool validateLink(unsigned handle)
	{
		return validate(handle, GL_LINK_STATUS);
	}

	bool validateCompile(unsigned handle)
	{
		return validate(handle, GL_COMPILE_STATUS);
	}

	bool validate(unsigned handle, GLenum type)
	{
		int success;
		glGetProgramiv(handle, type, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(handle, 512, NULL, infoLog);
			Logger::LogError(infoLog);
		}
		return success;
	}
}
#include "Precompiled/Precompiled.h"
#include "GLFunctions.h"

namespace gl
{
	std::string getShaderErrorString( unsigned handle ) {
		int length;
		glGetShaderiv( handle, GL_INFO_LOG_LENGTH, &length );
		std::vector<char> infoLog{};
		glGetShaderInfoLog( handle, length, NULL, infoLog.data() );
		if (infoLog.empty()) {
			return "No error message";
		}
		return std::string{ infoLog.data() };
	}

	GLStatus checkStatus( unsigned handle ) {
		GLStatus result{};

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			
			std::string errorString = getShaderErrorString( handle );

			result.errorMsg = errorString;
			result.error = error;
		}
		return result;
	}

	bool validateLink( unsigned handle ) {
		int success;
		glGetProgramiv( handle, GL_LINK_STATUS, &success );
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog( handle, 512, NULL, infoLog );
			Logger::LogError( infoLog );
		}
		return success;
	}

	bool validateCompile( unsigned handle ) {
		int success;
		glGetShaderiv( handle, GL_COMPILE_STATUS, &success );
		if (!success)
		{
			std::string error = getShaderErrorString( handle );
			Logger::LogError( std::string{ error } );
		}
		return success;
	}

	bool validate( unsigned handle, GLenum type ) {
		auto status = checkStatus( handle );
		return status.ok();
	}

}
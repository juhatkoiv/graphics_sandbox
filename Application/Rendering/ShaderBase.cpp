#include "Precompiled/Precompiled.h"
#include "ShaderBase.h"

#include <fstream>

#include "Rendering/OpenGL/GLFunctions.h"
#include "Rendering/ShaderCompilation/ShaderCompilation.h"

BEGIN_NAMESPACE1( rendering )

ShaderBase::ShaderBase( const std::string& source, unsigned int shaderType ) {
	auto result = shader_compilation::load_as_spv( source );
	if (!result.success()) {
		for( auto& error : result.errors )
			std::cerr << error << std::endl;

		std::cerr << "Error: Failed to load shader binary." << std::endl;
		throw;
	}

	std::vector<uint32_t> buffer = result.spirv;

	_handle = glCreateShader( shaderType );

	glShaderBinary( 1, &_handle, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data(), buffer.size() * sizeof( uint32_t ));
	gl::GLStatus status = gl::checkStatus( _handle );
	if (!status.ok()) {
		std::cerr << "Error: Failed to load shader binary. Error code: " << status.error << std::endl;
		Logger::LogError( status.errorMsg );
		throw;
	}

	glSpecializeShaderARB( _handle, "main", 0, nullptr, nullptr );
	status = gl::checkStatus( _handle );
	if (!status.ok()) {
		std::cerr << "Error: Failure in glSpecializeShaderARB. Error code: " << status.error << std::endl;
		Logger::LogError( status.errorMsg );
		throw;
	}

	if (gl::validateCompile( _handle )) {
		Logger::LogInfo( "Shader compiled successfully." );
	} else {
		Logger::LogError( "Shader compilation failed." );
		throw;
	}
}

ShaderBase::~ShaderBase()
{
	glDeleteShader( _handle );
}

unsigned ShaderBase::GetHandle() const
{
	return _handle;
}

END_NAMESPACE1
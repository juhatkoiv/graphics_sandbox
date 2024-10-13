#include "Precompiled/Precompiled.h"
#include "ShaderBase.h"

#include <fstream>

#include "Rendering/OpenGL/GLFunctions.h"
#include "Rendering/ShaderCompilation/ShaderCompilation.h"

BEGIN_NAMESPACE1( rendering )

ShaderBase::ShaderBase( const std::string& source, unsigned int shaderType )
{
	/*std::ifstream file;
	file.open( source, std::ios::ate | std::ios::binary );
	
	if (!file.is_open()) {
		throw std::runtime_error( "failed to open file!" );
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();
	*/
	
	auto result = shader_compilation::load_as_spv( source );
	if (!result.success()) {
		std::cerr << "Error: Failed to load shader binary." << std::endl;
		throw;
	}

	std::vector<uint32_t> buffer = result.spirv;

	_handle = glCreateShader( shaderType );

	glShaderBinary( 1, &_handle, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data(), buffer.size() );
		
	GLint binaryCheck;
	glGetShaderiv( _handle, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, &binaryCheck );
	if (binaryCheck != GL_TRUE) {
		std::cerr << "Error: Shader binary was not loaded successfully." << std::endl;
		throw;
	}

	gl::GLStatus binaryStatus = gl::checkStatus( _handle );
	if (!binaryStatus.ok()) {
		std::cerr << "Error: Failed to load shader binary. Error code: " << binaryStatus.error << std::endl;
		Logger::LogError( binaryStatus.errorMsg );
		throw;
	}

	// Note specialization is not needed as all shaders are compiled to SPIR-V and have the same entry
	// point, main. Also, no need to pass any shader constants.
	glSpecializeShaderARB( _handle, "main", 0, nullptr, nullptr );
	gl::GLStatus specializeStatus = gl::checkStatus( _handle );
	if (!specializeStatus.ok()) {
		std::cerr << "Error: Failure in glSpecializeShaderARB. Error code: " << specializeStatus.error << std::endl;
		Logger::LogError( specializeStatus.errorMsg );
		throw;
	}

	GLint specializationStatus;
	glGetShaderiv( _handle, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, &specializationStatus );
	if (specializationStatus != GL_TRUE) {
		std::cerr << "Error: Shader specialization failed." << std::endl;
		throw;
	}

	glCompileShader( _handle );
	if (gl::validateCompile( _handle )) {
		Logger::LogInfo( "Shader compiled successfully." );
	}
	else {
		Logger::LogError( "Shader compilation failed." );
		throw;
	}

	gl::GLStatus compileStatus = gl::checkStatus( _handle );
	if (!compileStatus.ok()) {
		std::cerr << "Error: Failure after glCompileShader. Error code: " << compileStatus.error << std::endl;
		Logger::LogError( compileStatus.errorMsg );
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
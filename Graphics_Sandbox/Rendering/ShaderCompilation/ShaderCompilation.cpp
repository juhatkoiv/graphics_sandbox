#include "Precompiled/Precompiled.h"
#include "ShaderCompilation.h"

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "shaderc/shaderc.hpp"

namespace fs = std::filesystem;

namespace {
	static const char* SHADER_INPUT_FOLDER = "data/shaders";

	enum class ShaderType {
		Vertex = 0,
		Fragment = 1,
		COUNT
	};

	constexpr size_t ARR_SIZE = static_cast<size_t>(ShaderType::COUNT);

	ShaderType getShaderType( const std::string& pathStr ) {

		size_t max = pathStr.length() - 1;
		if (pathStr.find( ".vert" ) < max) {
			return ShaderType::Vertex;
		}

		if (pathStr.find( ".frag" ) < max) {
			return ShaderType::Fragment;
		}
		throw;
	}

	std::array<std::string, ARR_SIZE> glslSrcSuffices = { ".vert", ".frag" };
	std::array<std::string, ARR_SIZE> spvDstSuffices = { "_vert.spv", "_frag.spv" };
	std::array<shaderc_shader_kind, ARR_SIZE> shaderKinds = { shaderc_shader_kind::shaderc_glsl_vertex_shader, shaderc_shader_kind::shaderc_glsl_fragment_shader };

	std::string parseSpvFileName( const std::string& tempPathStr, ShaderType shaderType ) {

		size_t lastFolderSeparatorIndex = tempPathStr.find_last_of( "\\" );
		std::string fileName = tempPathStr.substr( lastFolderSeparatorIndex + 1 );

		size_t index = static_cast<size_t>(shaderType);
		size_t suffixIndex = fileName.find( glslSrcSuffices[index] );

		return fileName.replace( suffixIndex, fileName.length(), spvDstSuffices[index] );
	}

	shaderc_shader_kind getShadercShaderKind( ShaderType shaderType ) {
		size_t index = static_cast<size_t>(shaderType);
		return shaderKinds[index];
	}

	template<typename T>
	T loadContent( const std::string& path, std::ios_base::openmode mode ) {
		std::ifstream ifs( path, mode );
		T content( (std::istreambuf_iterator<char>( ifs )), (std::istreambuf_iterator<char>()) );
		return content;
	}

	template<typename T>
	shaderc::SpvCompilationResult compile( const T& content, const std::string& pathStr, shaderc::CompileOptions compileOptions ) {
		shaderc::Compiler compiler{};

		ShaderType shaderType = getShaderType( pathStr );
		shaderc_shader_kind shaderKind = getShadercShaderKind( shaderType );

		shaderc::SpvCompilationResult compilationResult = compiler.CompileGlslToSpv(
			content, shaderKind, pathStr.c_str(), compileOptions );

		return compilationResult;
	}
}

CompilationResult shader_compilation::load_as_spv( const std::string& pathStr ) {
	CompilationResult result{};

	std::string content = loadContent<std::string>( pathStr, std::ios::in );

	shaderc::CompileOptions compileOptions{};
	compileOptions.SetTargetEnvironment( shaderc_target_env_opengl, shaderc_env_version_opengl_4_5 );

	auto compilationResult = compile( content, pathStr, compileOptions );
	if (compilationResult.GetCompilationStatus() != shaderc_compilation_status_success) {
		result.errors.push_back( "Shader compilation failed:" + compilationResult.GetErrorMessage() );
		return result;
	}

	result.spirv = { compilationResult.cbegin(), compilationResult.cend() };
	return result;
}

CompilationResult shader_compilation::generate_spirv() {

	CompilationResult result{};

	fs::path shadersFolder = fs::current_path().append( SHADER_INPUT_FOLDER ).append( "glsl" );
	if (!fs::exists( shadersFolder )) throw;

	fs::path outputFolder = fs::current_path().append( SHADER_INPUT_FOLDER ).append( "spv" );
	if (!fs::exists( outputFolder )) throw;

	auto di = fs::directory_iterator( shadersFolder );

	shaderc::CompileOptions compileOptions{};
	compileOptions.SetTargetEnvironment( shaderc_target_env_opengl, shaderc_env_version_opengl_4_5 );

	for (const fs::directory_entry& d : di) {
		fs::path path = d.path();
		std::string pathStr = path.string();

		std::string content = loadContent<std::string>( pathStr, std::ios::in );
		shaderc::SpvCompilationResult compilationResult = compile( content, pathStr, compileOptions );

		if (compilationResult.GetCompilationStatus() != shaderc_compilation_status_success) {
			result.errors.push_back( "Shader compilation failed:" + compilationResult.GetErrorMessage() );
			continue;
		}

		ShaderType shaderType = getShaderType( pathStr );
		std::string fileName = parseSpvFileName( pathStr, shaderType );
		std::string finalOutputPath = outputFolder.string() + "//" + fileName;

		std::vector<uint32_t> spirv( compilationResult.cbegin(), compilationResult.cend() );

		std::ofstream output{ finalOutputPath, std::ios::binary };
		if (!output.is_open()) {
			std::cerr << "Failed to open output file." << std::endl;
			throw;
		}

		output.write( reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof( uint32_t ) );
		output.close();
	}

	return result;
}

CompilationResult shader_compilation::load_spirv( const std::string& pathStr ) {
	CompilationResult result{};

	std::ifstream ifs( pathStr, std::ios::binary );
	std::vector<char> content = loadContent<std::vector<char>>( pathStr, std::ios::in | std::ios::binary );

	if (content.size() % sizeof( uint32_t ) != 0) {
		result.errors.push_back( "Invalid SPIR-V file." );
		return result;
	}

	uint32_t* spirv = reinterpret_cast<uint32_t*>(content.data());
	std::vector<uint32_t> bin{ spirv, spirv + content.size() / sizeof( uint32_t ) };
	result.spirv = bin;
	return result;

}

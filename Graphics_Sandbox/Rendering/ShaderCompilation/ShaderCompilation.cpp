#include "Precompiled/Precompiled.h"
#include "ShaderCompilation.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include "shaderc/shaderc.hpp"

namespace fs = std::filesystem;

namespace {
	enum class ShaderType {
		Vertex = 0,
		Fragment = 1,
		COUNT
	};

	constexpr size_t ARR_SIZE = static_cast<size_t>( ShaderType::COUNT );

	ShaderType getShaderType( const std::string& pathStr ) {

		size_t max = pathStr.length() - 1;
		if (pathStr.find( "vert" ) < max) {
			return ShaderType::Vertex;
		}

		if (pathStr.find( "frag" ) < max) {
			return ShaderType::Fragment;
		}
		throw;
	}

	std::array<std::string, ARR_SIZE> glslSrcSuffices = { "_vert.glsl", "_frag.glsl" };
	std::array<std::string, ARR_SIZE> tempSrcSuffices = { ".vert", ".frag" };
	std::array<std::string, ARR_SIZE> spvDstSuffices = { "_vert.spv", "_frag.spv" };
	std::array<shaderc_shader_kind, ARR_SIZE> shaderKinds = { shaderc_shader_kind::shaderc_glsl_vertex_shader, shaderc_shader_kind::shaderc_glsl_fragment_shader };

	std::string parseDstFileName( const std::string& pathStr, ShaderType shaderType ) {
		
		size_t lastFolderSeparatorIndex = pathStr.find_last_of( "\\" );
		std::string fileName = pathStr.substr( lastFolderSeparatorIndex );

		size_t index = static_cast<size_t>( shaderType );
		size_t suffixIndex = fileName.find( glslSrcSuffices[index] );
		std::string dstFileName = fileName;

		return dstFileName.replace( suffixIndex, dstFileName.length(), tempSrcSuffices[index] );
	}

	std::string parseSpvFileName( const std::string& tempPathStr, ShaderType shaderType ) {
		
		size_t lastFolderSeparatorIndex = tempPathStr.find_last_of( "\\" );
		std::string fileName = tempPathStr.substr( lastFolderSeparatorIndex );

		size_t index = static_cast<size_t>( shaderType );
		size_t suffixIndex = fileName.find( tempSrcSuffices[index]);

		return fileName.replace( suffixIndex, fileName.length(), spvDstSuffices[index] );
	}

	shaderc_shader_kind getShadercShaderKind( ShaderType shaderType ) {
		size_t index = static_cast<size_t>( shaderType );
		return shaderKinds[index];
	}

}

bool shader_compilation::generate_spirv() {

	fs::path shadersFolder = fs::current_path().append( "shaders" );
	if (!fs::exists( shadersFolder )) throw;

	fs::path outputFolder = fs::current_path().append( "shaders" ).append( "spv" );
	if (!fs::exists( outputFolder )) throw;

	fs::path tempFolder = fs::current_path().append( "shaders" ).append( "temp" );
	if (!fs::exists( tempFolder )) {

		if (!fs::create_directory( tempFolder )) {
			throw;
		}
	}

	auto di = fs::directory_iterator( shadersFolder );
	
	for (const fs::directory_entry& d : di) {
		if (d.is_directory())
			continue;

		fs::path path = d.path();

		std::string pathStr = path.string();
		ShaderType shaderType = getShaderType( pathStr );

		std::string fileName = parseDstFileName( pathStr, shaderType );
		std::string finalPathString = tempFolder.string() + fileName;

		std::cout << finalPathString << std::endl;

		fs::path finalPath{ finalPathString };

		if (!fs::copy_file( path, finalPath, fs::copy_options::overwrite_existing )) {
			throw;
		}
	}

	shaderc::Compiler compiler{};
	shaderc::CompileOptions compileOptions{};

	compileOptions.SetOptimizationLevel( shaderc_optimization_level_performance );

	auto si = fs::directory_iterator( tempFolder );

	for (const fs::directory_entry& d : si) {
		fs::path path = d.path();
		std::string pathStr = path.string();

		ShaderType shaderType = getShaderType( pathStr );

		std::ifstream ifs( pathStr );
		std::string content( ( std::istreambuf_iterator<char>( ifs ) ),
			( std::istreambuf_iterator<char>() ) ); 
		
		shaderc_shader_kind shaderKind = getShadercShaderKind( shaderType );
		shaderc::SpvCompilationResult compilationResult = compiler.CompileGlslToSpv(
			content, shaderKind, pathStr.c_str(), compileOptions);
		
		if (compilationResult.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << "Shader compilation failed: " << compilationResult.GetErrorMessage();
			return false;
		}

		std::string fileName = parseSpvFileName( pathStr, shaderType );
		std::string finalOutputPath = outputFolder.string() + fileName;

		std::vector<uint32_t> spirv( compilationResult.cbegin(), compilationResult.cend() );

		std::ofstream output{ finalOutputPath, std::ios::binary };
		if (!output.is_open()) {
			std::cerr << "Failed to open output file." << std::endl;
			return false;
		}

		output.write( reinterpret_cast<const char*>( spirv.data() ), spirv.size() * sizeof( uint32_t ) );
		output.close();
	}

	uintmax_t removeResult = fs::remove_all( tempFolder );
	(void)removeResult;

	return true;
}

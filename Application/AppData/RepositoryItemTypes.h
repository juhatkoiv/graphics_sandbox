#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "Serialization/SerializationDefinitions.h"

#include <string>

BEGIN_NAMESPACE1( appdata )

struct ShaderSource 
{
	std::string name;
	std::string vertexSourceFile;
	std::string fragmentSourceFile;

	ShaderSource() = default;
	ShaderSource( const std::string& name, const std::string& vertexSourceFile, const std::string& fragmentSourceFile )
		: name( name ), vertexSourceFile( vertexSourceFile ), fragmentSourceFile( fragmentSourceFile )
	{
	}
};
DECLARE_SERIALIZABLE( ShaderSource, name, vertexSourceFile, fragmentSourceFile );

struct AppDefaults {
	std::vector<std::string> folders;
	std::vector<std::string> files;
};

struct Settings 
{	
	std::string modelPath;
	std::string texturePath;
	std::string meshPath;
	std::string shaderPath;
	std::string scenePath;
	// status
	std::string lastScenePath{};
	
	Settings() = default;
	Settings( const std::string& modelPath, const std::string& texturePath, const std::string& meshPath, const std::string& shaderPath, const std::string& scenePath, const std::string& lastScenePath )
		: modelPath( modelPath ), texturePath( texturePath ), meshPath( meshPath ), shaderPath( shaderPath ), scenePath( scenePath ), lastScenePath( lastScenePath )
	{
	}
};
DECLARE_SERIALIZABLE( Settings, modelPath, texturePath, meshPath, shaderPath, scenePath, lastScenePath );

struct TextureManifest
{
	std::string name{};
	std::vector<std::string> paths{};
};
DECLARE_SERIALIZABLE( TextureManifest, name, paths );

END_NAMESPACE1
#pragma once

#include <fstream>

#include "Serialization/SerializationDefinitions.h"

BEGIN_NAMESPACE1(assets);

class JsonFileSystem 
{
public:
	template<typename T>
	static void SaveAsJson( const T& t, const char* fileName )
	{
		nlohmann::json j = t;
		SaveJson( j, fileName );
	}

	static void SaveJson( nlohmann::json j, const char* fileName ) 
	{
		std::ofstream file;
		file.open( fileName );

		if (file.is_open())
		{
			std::string str = j.dump( 4 );
			file << str;
		}
		file.close();
	}

	template<typename T>
	static void LoadFromJson(T& t, const char* fileName)
	{
		nlohmann::json j = LoadJson( fileName );
		t = j;
	}

	template<typename T>
	static T LoadFromJson( const char* fileName )
	{
		nlohmann::json j = LoadJson( fileName );
		T t = j;

		return t;
	}

	static nlohmann::json LoadJson( const char* fileName ) 
	{
		std::ifstream file;
		file.open( fileName );

		nlohmann::json j;
		if (file.is_open())
		{
			file >> j;
		}
		file.close();

		if (j.is_null())
		{
			LOG_ERROR( "Failed to load scene" );
			throw std::runtime_error( "Failed to load scene" );
		}
		return j;
	}
};

END_NAMESPACE1
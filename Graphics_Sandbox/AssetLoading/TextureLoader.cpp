#include "Precompiled/Precompiled.h"
#include "TextureLoader.h"

#include <unordered_map>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Rendering/Texture.h"

#include "ECS/Material.h"

BEGIN_NAMESPACE1( assets )

namespace
{
	using namespace rendering;

	static TextureData& addToCache( const std::string& texturePath,
		unsigned char** data, int width, int height, int nrChannels,
		std::unordered_map<std::string, TextureData>& cache )
	{
		TextureData& t = cache[texturePath];
		t.data = *data;
		t.height = height;
		t.width = width;
		t.nrChannels = nrChannels;

		return t;
	}

	static Result<const TextureData&> getCached( const std::string& texturePath, std::unordered_map<std::string, TextureData>& cache )
	{
		auto cacheIt = cache.find( texturePath );
		if (cacheIt != cache.end())
		{
			TextureData t = cacheIt->second;
			return Result<const TextureData&>::ok( t );
		}
		return Result<const TextureData&>::failed();
	}
}

TextureLoader::TextureLoader()
{
	//stbi_set_flip_vertically_on_load( 1 );
}

TextureLoader::~TextureLoader()
{
	cleanup();
}

void TextureLoader::cleanup()
{
	if (_textureCache.empty())
		return;

	for (auto& tex : _textureCache)
	{
		stbi_image_free( tex.second.data );
	}
	_textureCache.clear();
}

Result<const TextureData&> TextureLoader::load( const std::string& texturePath, int channels )
{
	if (texturePath.empty())
	{
		LOG_ERROR( "Filename empty." );
		assert( !texturePath.empty() );
	}

	Result<const TextureData&> result = getCached( texturePath, _textureCache );
	if (result.success)
	{
		return result;
	}

	int width, height, nrChannels;

	unsigned char* data = stbi_load( texturePath.c_str(), &width, &height, &nrChannels, channels );
	if (!data)
	{
		std::cout << "Failed to load texture - path: " << texturePath << std::endl;
		return Result<const TextureData&>::failed();
	}

	TextureData& t = addToCache( texturePath, &data, width, height, nrChannels, _textureCache );
	return Result<const TextureData&>::ok( t );
}

END_NAMESPACE1
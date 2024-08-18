#pragma once

#include <cinttypes>

DECLARE1( struct, rendering, TextureData );
DECLARE1( struct, ecs, TextureParam );
DECLARE1( class, resources, ResourceSystem );
BEGIN_NAMESPACE1( assets )

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	void cleanup();
	Result<const rendering::TextureData&> load( const std::string& textureName, int channels = 4 );
	
private:
	std::unordered_map<std::string, rendering::TextureData> _textureCache;
};

END_NAMESPACE1
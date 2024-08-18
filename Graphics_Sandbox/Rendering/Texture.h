#pragma once

#include <cinttypes>
#include <vector>
#include <unordered_map>

#include "Precompiled/EnumClassFlagsDefines.h"
#include "ECS/UniqueId.h"
#include "Resources/Handle.h"

BEGIN_NAMESPACE1(rendering)

enum class Dimensions
{
	T1D = 0,
	T2D = 1,
	T3D = 2,
	TCube = 3
};

struct GfxTexture 
{
	unsigned int buffer = ~0u;
	unsigned int unit = ~0u;
	Dimensions dimensions = Dimensions::T2D;

	bool IsSet() const
	{
		return buffer != ~0u && unit != ~0u;
	}
};


// TODO - move to rendering namespace
enum class TextureType : int
{
	None = 0,
	Color = (1 << 0),
	Diffuse = (1 << 1),
	Specular = (1 << 2),
	Sprite = (1 << 3)
};
FLAG_ENUM( TextureType );

// TODO - move to rendering namespace
enum class TextureFormat : int
{
	None = 0,
	RGB = 1,
	RGBA = 2
};

// TODO - move to rendering namespace
enum class TextureWrap : int
{
	None = 0,
	Repeat = 1,
	MirroredRepeat = 2,
	ClampToEdge = 3,
	ClampToBorder = 4
};

// TODO - move to rendering namespace
enum class TextureFilter : int
{
	None = 0,
	Nearest = 1,
	Linear = 2,
	NearestMipmapNearest = 3,
	LinearMipmapNearest = 4,
	NearestMipmapLinear = 5,
	LinearMipmapLinear = 6
};
FLAG_ENUM( TextureFilter );

// TODO - move to rendering namespace
enum class TextureDimensions : int
{
	None = 0,
	T1D = 1,
	T2D = 2,
	T3D = 3,
	TCube = 4
};

struct TextureData
{
	uint8_t* data = nullptr;
	uint8_t nrChannels = 0;
	uint16_t width = 0;
	uint16_t height = 0;

	TextureData() {}

	TextureData( const TextureData& other ) noexcept
	{
		data = other.data;
		nrChannels = other.nrChannels;
		width = other.width;
		height = other.height;
	}

	TextureData( TextureData&& other ) noexcept
	{
		data = other.data;
		nrChannels = other.nrChannels;
		width = other.width;
		height = other.height;
	}

	const TextureData& operator=( const TextureData& other ) noexcept
	{
		data = other.data;
		nrChannels = other.nrChannels;
		width = other.width;
		height = other.height;
		return *this;
	}

	TextureData& operator=( TextureData&& other ) noexcept
	{
		data = other.data;
		nrChannels = other.nrChannels;
		width = other.width;
		height = other.height;
		return *this;
	}
};

struct TextureDescriptor
{
	TextureDimensions dimensions = TextureDimensions::None;
	std::vector<const TextureData*> textureData{};
};

END_NAMESPACE1


namespace texture
{
	template<rendering::TextureType Type>
	static inline constexpr size_t index()
	{
		if constexpr (Type == rendering::TextureType::None) return ~0u;
		else if constexpr (Type == rendering::TextureType::Color) return 0;
		else if constexpr (Type == rendering::TextureType::Diffuse) return 1;
		else if constexpr (Type == rendering::TextureType::Specular) return 2;
		else if constexpr (Type == rendering::TextureType::Sprite) return 3;
	}

	static inline rendering::TextureType getTextureType( const char* enumName )
	{
		static std::unordered_map<std::string, rendering::TextureType> map
		{
			{"None", rendering::TextureType::None},
			{"Color", rendering::TextureType::Color},
			{"Diffuse", rendering::TextureType::Diffuse},
			{"Specular", rendering::TextureType::Specular},
			{"Sprite", rendering::TextureType::Sprite}
		};
		return map.at( enumName );
	}

	static inline const char* getTextureTypeName( rendering::TextureType type )
	{
		static std::unordered_map<rendering::TextureType, const char*> map
		{
			{rendering::TextureType::None, "None"},
			{rendering::TextureType::Color, "Color"},
			{rendering::TextureType::Diffuse, "Diffuse"},
			{rendering::TextureType::Specular, "Specular"},
			{rendering::TextureType::Sprite, "Sprite"}
		};
		return map.at( type );
	}

	static inline constexpr size_t TYPE_COUNT = 4;
	static inline constexpr size_t FIRST_TYPE = 0;

	static constexpr rendering::TextureType toTypeFlag( size_t index )
	{
		return static_cast<rendering::TextureType>((1 << index));
	}
}

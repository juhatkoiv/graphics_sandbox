#pragma once

#if defined(__clang__)
#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wunused-function"
#endif 

#define CONCATENATE_IMPL( x, y ) x ## y
#define CONCATENATE( x, y ) CONCATENATE_IMPL( x, y )

#define UNIQUE_ID(type_name) \
namespace id \
{ \
	using type_name = unsigned int; \
	\
	inline type_name CONCATENATE(next, type_name) = 0; \
	\
	static type_name CONCATENATE(New, type_name)() \
	{ \
		return next ## type_name++; \
	}\
	\
	static void CONCATENATE(SetNext, type_name) (type_name id) \
	{ \
		next ## type_name = id; \
	} \
	\
	static constexpr type_name CONCATENATE(Invalid, type_name) = ~0u; \
}

UNIQUE_ID( EntityId );
UNIQUE_ID( RayId );
UNIQUE_ID( MeshId );
UNIQUE_ID( TextureId );
UNIQUE_ID( MaterialId );
UNIQUE_ID( CameraId );
UNIQUE_ID( ShaderId );
UNIQUE_ID( SceneId );

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
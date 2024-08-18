#include "Precompiled/Precompiled.h"
#include "GLConstBuffer.h"

#include "Rendering/GfxBufferImpl.h"
#include "Rendering/MemoryUtils.h"

int ConstBuffer::getBufferEnum( unsigned usage )
{
	static int bufferEnumMap[2]{ GL_UNIFORM_BUFFER, GL_SHADER_STORAGE_BUFFER };
	size_t bufferUsage = static_cast<size_t>(usage);

	(void)bufferUsage;
	return bufferEnumMap[usage];
}

ConstBuffer::ConstBuffer( const GfxBufferImpl &b, int bufferUsage, int slot, size_t vcapacity )
{
	usage = bufferUsage;
	size = b.size;
	stride = b.stride;
	bindingPoint = slot;
	bufferData = memory::create<uint8_t*>( b, size );
	bufferEnum = getBufferEnum( bufferUsage );

	glCreateBuffers( 1, &buffer );
	glBindBuffer( bufferEnum, buffer );
	glBufferData( bufferEnum, size, bufferData, GL_DYNAMIC_DRAW );
	glBufferSubData( bufferEnum, 0, size, bufferData );
	glBindBufferBase( bufferEnum, bindingPoint, buffer );
	glBindBuffer( bufferEnum, buffer );
}

ConstBuffer::~ConstBuffer()
{
	if (!bufferData)
		return;

	memory::deallocate(bufferData);
}

void ConstBuffer::bind()
{
	glBindBuffer( bufferEnum, buffer );
	glBindBufferRange( bufferEnum, bindingPoint, buffer, 0, size );
	glBindBuffer( bufferEnum, 0 );
}

void ConstBuffer::update( const GfxBufferImpl& b )
{
	glBindBuffer( bufferEnum, buffer );
	glBufferSubData( bufferEnum, 0, size, (void*)bufferData );
	glBindBufferBase( bufferEnum, bindingPoint, buffer );
	glBindBuffer( bufferEnum, 0 );
}

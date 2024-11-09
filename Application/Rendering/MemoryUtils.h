#pragma once

namespace memory
{
	// TODO - take into account POD vs non-POD types and alignment 
	template<typename Ptr>
	Ptr allocate( size_t size )
	{
		return (Ptr)::operator new(size);
	}

	template<typename type>
	type* allocArray( size_t size )
	{
		return (type*)::operator new[](size);
	}
	
	template<typename Ptr>
	void deallocate( Ptr ptr )
	{
		::operator delete((void*)ptr);
	}

	template<typename type>
	void deallocArray( type* ptr, size_t size )
	{
		for (int i = 0; i < size; i++)
		{
			ptr[i].~type();
		}
		::operator delete[]( ptr );
	}

	template<typename Ptr, typename U>
	Ptr create( U u, size_t size = 0 )
	{
		size_t allocSize = size == 0 ? sizeof( U ) : size;
		Ptr result = allocate<Ptr>( allocSize );
		std::memcpy( (void*)result, (void*)&u, allocSize );
		return result;
	}
}

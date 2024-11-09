#pragma once

#include <concepts>
#include <cassert>
#include <cinttypes>
#include "Precompiled/Logger.h"

template<typename T, typename U>
concept IsAllocator = requires(T allocator, U u, size_t size)
{
	{ allocator.allocate( size ) } -> std::convertible_to<U*>;
	allocator.deallocate( &u );
};

// ONLY USE LOCAL ALLOCATORS FOR A SPECIFIC USE CASE
// cache coloring
// std::uintptr_t 
// std::ptrdiff_t
// ABA problem? how to write lock free singly linked list before doing this.


// allocator is a handle to a memory resource, which gives the memory
// std::vector<T>v(&resource);
// deriving from std::pmr::memory_resource > has built in implementations
// check: std::aligned_storage_t<size_t> buffer;
// chaining for book keeping _o/

// multipool allocator
// same as pool but multiple pools and depending on the requested memory amount
// harder to set up: how many pools? what is the chunk size?

// pool allocator
// fixed sized chunks
// some fragmentation > BUT we can can deallocate and reallocate easily
// track free blocks > build linked list of pointers > advance by traversing the list
// on deallocate > to the first free head
// issue is diffusion -> the order of the elements in the list will change
// -> poor cache usage
// -> might want to release the memory to get rid of the diffusion artefact
// -> cannot serve 
// + easy to reason about -> deterministic runtime behavious
// + implicit fragmentatoin doesn't have runtime cost as explicit one

// stack allocator
// -> FILO allocations
// -> can only deallocate the last inserted
// -> upon deallocation, we can't know what the next head is.
// -> alignment == how many of the least-significant bits of the address are 0
// 1111 > 0xf > no alignment
// 1110 > 0xe > alignment to 2 bytes
// 1100 > 0xc > alignment to 4 bytes
// 1000 > 0x8 > alignment to 8 bytes
// allocators can't give arbitrary memory addresses > 
// for custom types the user may extend the aligment with alignas for custom types

// monotonic allocator 
// -> when data can be thrown away 
// -> when containers don't change
// -> vector reserve works fine
//	-> growing doesn't play well
// -> doesn't work well with unordered map, or sets.delete etc.
//	-> especially rehashing is an issue
// extension > auxiliary nodes to point to the previous point 
// this data can be stored in the internal or external data structure
// -> generally you don't want to do this -> easily becomes a gp allocator which std already already has.

class MonotonicAllocator
{
public:
	void* allocate( size_t size ) {}
	void deallocate( void* ptr ) {}
	void release() {}
};

// 
template <typename T, size_t ALIGNMENT = alignof(T)>
class BlockAllocator
{
public:
	T* allocate( size_t size )
	{

	}

	void deallocate( T* ptr )
	{

	}

private:
	uint8_t* buffer = nullptr;
	uint8_t* current = nullptr;

};

template <typename T>
struct StdHeapAllocator
{
	T* allocate( size_t size )
	{
		return (T*)::operator new(size);
	}

	void deallocate( T* ptr )
	{
		if constexpr (!std::is_trivially_copyable<T>::value)
		{
			ptr->~T();
		}
		::operator delete((void*)ptr);
	}
};

template<typename T, IsAllocator<T> Allocator = StdHeapAllocator<T>>
class LinearHeapBuffer
{
private:
	Allocator _allocator;
	T* _impl = nullptr;

public:
	LinearHeapBuffer( size_t capacity )
	{
		_impl = _allocator.allocate( capacity );
	}

	~LinearHeapBuffer()
	{
		_allocator.deallocate( _impl );
	}
};


template<typename T, IsAllocator<T> Allocator = StdHeapAllocator<T>, size_t INIT_CAPACITY = 100>
class DynamicVector
{
private:
	Allocator _allocator;
	T* _impl = nullptr;
	size_t _size = 0;
	size_t _capacity = 0;

	void copyImpl( const DynamicVector<T>& other )
	{
		_allocator.deallocate( _impl );
		_impl = other._impl;
		_size = other._size;
		_capacity = other._capacity;
	}

	void moveImpl( DynamicVector<T>&& other )
	{
		_allocator.deallocate( _impl );
		_impl = std::forward<T*>( other._impl );
		_size = std::forward<size_t>( other._size );
		_capacity = std::forward<size_t>( other._capacity );
	}

	void reserveImpl( size_t newCapacity )
	{
		T* temp = _allocator.allocate( newCapacity );
		std::memcpy( (void*)temp, (void*)_impl, _size );
		_allocator.deallocate( _impl );
		_impl = temp;
		_capacity = newCapacity;
	}

public:
	DynamicVector() : DynamicVector( INIT_CAPACITY )
	{}

	~DynamicVector()
	{
		_allocator.deallocate( _impl );
	}

	DynamicVector( size_t capacity )
	{
		assert( capacity >= 0 && "Capacity must be >= 0." );
		_impl = _allocator.allocate( capacity );
		_capacity = capacity;
	}

	DynamicVector( const DynamicVector<T>& other )
	{
		copyImpl( other );
	}

	const T& operator=( const DynamicVector<T>& other )
	{
		copyImpl( other );
		return *this;
	}

	DynamicVector( DynamicVector<T>&& other )
	{
		moveImpl( std::forward<DynamicVector<T>>( other ) );
	}

	const T& operator=( DynamicVector<T>&& other )
	{
		moveImpl( std::forward<DynamicVector<T>>( other ) );
		return *this;
	}

	void resize( size_t size )
	{
		if (size >= _capacity)
		{
			reserveImpl( size );
		}
		_size = size;
	}

	void reserve( size_t capacity )
	{
		if (capacity >= _capacity)
			return;

		reserveImpl( capacity );
	}

	void pushBack( const T& t )
	{
		if (_capacity <= _size)
		{
			reserveImpl( _capacity * 2 );
		}
		_impl[_size++] = t;
	}

	void pushBack( T&& t )
	{
		if (_capacity <= _size)
		{
			reserveImpl( _capacity * 2 );
		}
		_impl[_size++] = std::forward<T>( t );
	}

	size_t size() const
	{
		return _size;
	}

	bool empty() const
	{
		return _size == 0;
	}

	T& operator[]( size_t index )
	{
		if (index >= _size)
		{
			assert( index >= _size && "index >= _size" );
			Logger::LogError( "index >= _size" );
		}
		return _impl[index];
	}
};

void run_test()
{
	DynamicVector<int> v = {};
	for (int i = 0; i < 100; i++)
		v.pushBack( i );

}
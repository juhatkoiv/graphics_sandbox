#pragma once

#include <concepts>
#include <iterator>

#include "Rendering/MemoryUtils.h"

template <typename T>
struct Iterator
{
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	pointer _ptr = nullptr;

	Iterator( pointer ptr ) : _ptr( ptr ) {}

	reference operator*() const { return *_ptr; }
	pointer operator->() { return _ptr; }
	Iterator& operator++() { _ptr++; return *this; }
	Iterator operator++( int ) { Iterator tmp = *this; ++(*this); return tmp; }
	friend bool operator== ( const Iterator& a, const Iterator& b ) { return a._ptr == b._ptr; };
	friend bool operator!= ( const Iterator& a, const Iterator& b ) { return a._ptr != b._ptr; };
};

template <typename T>
struct ConstIterator
{
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = const T*;
	using reference = const T&;
	pointer _ptr = nullptr;

	ConstIterator( pointer ptr ) : _ptr( ptr ) {}

	reference operator*() const { return *_ptr; } const
		pointer operator->() { return _ptr; } const
		ConstIterator& operator++() { _ptr++; return *this; }
	ConstIterator operator++( int ) { ConstIterator tmp = *this; ++(*this); return tmp; }
	friend bool operator== ( const ConstIterator& a, const ConstIterator& b ) { return a._ptr == b._ptr; };
	friend bool operator!= ( const ConstIterator& a, const ConstIterator& b ) { return a._ptr != b._ptr; };
};

template<typename T, size_t CAPACITY = 50>
class SmallVector
{
private:
	size_t _capacity = CAPACITY;
	size_t _size = 0;
	T _impl[CAPACITY];

public:
	SmallVector() {}
	~SmallVector() {}

	SmallVector( const SmallVector& other )
	{
		_capacity = other._capacity;
		_size = other._size;
		for (int i = 0; i < _size; i++)
		{
			_impl[i] = other._impl[i];
		}
	}

	SmallVector( SmallVector&& other )
	{
		_capacity = other._capacity;
		_size = other._size;
		for (int i = 0; i < _size; i++)
		{
			_impl[i] = other._impl[i];
		}

		other._capacity = 0;
		other._size = 0;
		std::memset( (void*)other._impl, 0, other._capacity );
	}

	const SmallVector& operator=( const SmallVector& other )
	{
		_capacity = other._capacity;
		_size = other._size;
		for (int i = 0; i < _size; i++)
		{
			_impl[i] = other._impl[i];
		}
		return *this;
	}

	SmallVector& operator=( SmallVector&& other ) 
	{
		_capacity = other._capacity;
		_size = other._size;
		for (int i = 0; i < _size; i++)
		{
			_impl[i] = other._impl[i];
		}

		other._capacity = 0;
		other._size = 0;
		std::memset( (void*)other._impl, 0, other._capacity );
		return *this;
	}

	void pushBack( const T& t )
	{
		if (_size >= _capacity - 1)
		{
			assert( _size >= _capacity - 1 );
		}
		_impl[_size++] = t;
	}

	void pushBack( T&& t )
	{
		if (_size >= _capacity - 1)
		{
			assert( _size >= _capacity - 1 );
		}
		_impl[_size++] = std::forward<T>( t );
	}

	void resize( size_t newSize )
	{
		if (newSize >= _capacity - 1)
		{
			assert( _size >= _capacity - 1 );
		}
		_size = newSize;
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
			assert( index >= _size );
		}
		return _impl[index];
	}

	const T& operator[]( size_t index ) const 
	{
		if (index >= _size)
		{
			assert( index >= _size );
		}
		return _impl[index];
	}
	
	Iterator<T> begin()
	{
		return Iterator<T>{ &_impl[0] };
	}

	Iterator<T> end()
	{
		return Iterator<T>{ &_impl[_size - 1] };
	}

	ConstIterator<T> cbegin()
	{
		return ConstIterator<T>{ &_impl[0] };
	}

	ConstIterator<T> cend()
	{
		return ConstIterator<T>{ &_impl[_size - 1] };
	}
};
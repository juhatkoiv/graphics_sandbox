#pragma once

#include <vector>
#include <set>
#include <concepts>
#include <array>
#include <cinttypes>
#include <bitset>

template<typename T>
concept TKey = std::convertible_to<T, size_t>;

template<TKey Key, typename ValueType, size_t INIT_CAPACITY = 10000>
class SparseMap
{
	size_t _count = 0;
	std::bitset<INIT_CAPACITY> _exists;
	std::array<ValueType, INIT_CAPACITY> _impl;

public:
	SparseMap() 
	{
		// ensure that the array is initialized
		_impl.fill( ValueType{} );
	}

	bool has( Key key ) const
	{
		const size_t pos = static_cast<size_t>(key);
		return _exists.test( pos );
	}

	const ValueType& at( Key key )
	{
		return getImpl( key );
	}

	ValueType& operator[]( Key key )
	{
		if (has( key ))
		{
			return getImpl( key );
		}
		else
		{
			return getNew( key );
		}
	}

	void clear()
	{
		_exists.reset();
	}

	void erase( Key key )
	{
		const size_t pos = static_cast<size_t>(key);
		_exists.reset( pos );
		// TODO - is this necessary?
		_impl[pos] = ValueType{};
		_count--;
	}

	bool empty() const
	{
		return _count == 0;
	}

private:
	ValueType& getNew( Key key )
	{
		const size_t index = static_cast<size_t>(key);
		assert( _impl.size() > index );

		_impl[index] = ValueType{};
		_exists.set( index );
		_count++;

		return _impl[index];
	}

	ValueType& getImpl( Key key )
	{
		if (key >= _impl.size())
		{
			LOG_ERROR( "Invalid  key" );
			assert( key >= _impl.size() );
		}

		const size_t index = static_cast<size_t>( key );

		return _impl[index];
	}
};



#pragma once

#include <vector>
#include <ranges>

template<typename Key, typename Value, size_t INIT_CAPACITY = 10>
class LinearMap
{
private:
	struct Pair
	{
		Key key;
		Value value;
	};

	using ConstIterator = std::vector<Pair>::const_iterator;
	using Iterator = std::vector<Pair>::iterator;
	// TODO - might need to change this to two vectors, one for keys and one for values
	std::vector<Pair> _impl;

	Value& pushBack( Key key, const Value& data )
	{
		_impl.push_back( Pair
			{
				.key = key,
				.value = data
			} );

		return _impl.back().value;
	}

	Value& pushBack( Key key )
	{
		_impl.push_back( Pair
			{
				.key = key,
				.value = Value{}
			} );

		return _impl.back().value;
	}

public:
	LinearMap()
	{
		_impl.reserve( INIT_CAPACITY );
	}

	Value& operator[]( Key id )
	{
		if (has( id ))
		{
			return mutableAt( id );
		}
		else
		{
			return pushBack( id );
		}
	}

	const Value& at( Key key ) const
	{
		for (const auto& [k, value] : _impl)
		{
			if (key == k)
			{
				return value;
			}
		}
		throw;
	}

	Value& mutableAt( Key key )
	{
		for (auto& [k, value] : _impl)
		{
			if (key == k)
			{
				return value;
			}
		}
		throw;
	}

	void sort()
	{
		std::ranges::sort( _impl, []( const Pair& lhs, const Pair& rhs )
			{
				return lhs.key < rhs.key;
			} );
	}

	bool has( Key key ) const
	{
		for (const auto& [k, value] : _impl)
		{
			if (key == k)
			{
				return true;
			}
		}
		return false;
	}

	void erase( Key key )
	{
		std::erase_if( _impl, [&]( const Pair& pair )
			{
				return pair.key == key;
			} );
	}

	void clear() noexcept
	{
		_impl.clear();
	}

	size_t size() const noexcept
	{
		return _impl.size();
	}

	bool empty() const noexcept
	{
		return _impl.empty();
	}

	ConstIterator begin() const noexcept
	{
		return _impl.begin();
	}

	ConstIterator end() const noexcept
	{
		return _impl.end();
	}

	Iterator begin() noexcept
	{
		return _impl.begin();
	}

	Iterator end() noexcept
	{
		return _impl.end();
	}
};
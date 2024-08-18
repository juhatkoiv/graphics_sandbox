#pragma once

#include <vector>

// TODO NAMESPACE

template<typename T>
struct ResourceBuffer 
{
	std::vector<T> _buffer;
	size_t _capacity = 0;

	void Init(unsigned capacity);
	void Insert(const T& resource, unsigned slot);
	void Grow(unsigned minCapacity);
	T& Get(unsigned slot);
	T& operator[](unsigned slot);
};

template<typename T>
inline void ResourceBuffer<T>::Init(unsigned capacity)
{
	_capacity = capacity;
	_buffer.resize(_capacity);
}

template<typename T>
inline void ResourceBuffer<T>::Insert(const T& resource, unsigned slot)
{
	_buffer[slot] = resource;
}

template<typename T>
inline void ResourceBuffer<T>::Grow(unsigned minCapacity)
{
	if (_capacity < minCapacity) 
	{
		_buffer.resize(minCapacity * 2);
	}
}

template<typename T>
inline T& ResourceBuffer<T>::Get(unsigned slot)
{
	return _buffer[slot];
}

template<typename T>
inline T& ResourceBuffer<T>::operator[](unsigned slot) 
{
	return _buffer[slot];
}
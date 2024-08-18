#pragma once

#include <concepts>
#include <span>
#include <vector>
#include <set>
#include <unordered_set>



#include <bitset>

#include "ComponentIncludes.h"
#include "Precompiled/PrecompilationMacros.h"
#include "UniqueId.h"
#include <functional>
#include "EntitySpawnParams.h"

DECLARE1( class, scene, SceneManager );

BEGIN_NAMESPACE1( ecs )

template<typename T, typename OType>
class DebugAllocator : public std::allocator<T>
{
public:
	using value_type = T;
	using size_type = size_t;
	using pointer = T*;
	using const_pointer = const T*;

	DebugAllocator() throw() : std::allocator<T>() {}
	~DebugAllocator() throw() {}

	DebugAllocator( DebugAllocator<T, OType> const& d ) throw() : std::allocator<T>( d ) {}
	const DebugAllocator<T, OType>& operator=( const DebugAllocator<T, OType>& ) {}

	template <class U>
	DebugAllocator( DebugAllocator<U, OType> const& d ) throw() : std::allocator<T>( d ) {}

	template <class U>
	struct rebind
	{
		using other = DebugAllocator<U, OType>;
	};

	T* allocate( const size_t count )
	{
		allocated += count;

		printf( "allocate() - %s - %s - allocated: %d\n", typeid(OType).name(), typeid(T).name(), (int)allocated );
		return _impl.allocate( count );
	}

	void deallocate( T* ptr, size_t count )
	{
		allocated -= count;

		printf( "deallocate() - %s - %s - allocated: %d\n", typeid(OType).name(), typeid(T).name(), (int)allocated );
		_impl.deallocate( ptr, count );
	}
	std::allocator<T> _impl;
	static inline size_t allocated = 0;
};

struct ComponentMask2
{
	std::bitset<cid::ComponentType::COUNT> maskImpl;
	
	ComponentMask2( const std::span<ComponentId> ids )
	{
		for (auto id : ids)
		{
			set( id );
		}
	}
	ComponentMask2() = default;
	ComponentMask2( const ComponentMask2& ) = default;
	ComponentMask2& operator=( const ComponentMask2& ) = default;
	ComponentMask2( ComponentMask2&& ) = default;
	ComponentMask2& operator=( ComponentMask2&& ) = default;

	void set( ComponentId index )
	{
		maskImpl.set( index );
	}

	void unset( ComponentId index )
	{
		maskImpl.reset( index );
	}

	constexpr bool overlaps( const ComponentMask2& other ) const
	{
		return (maskImpl & other.maskImpl).any();
	}

	bool has( ComponentId index ) const
	{
		return maskImpl.test( index );
	}
};

struct ComponentBucket
{
	std::vector<uint8_t*> components;
	std::vector<id::EntityId> ids;

	ComponentBucket() = default;
	ComponentBucket(size_t capacity) 
	{
		components.reserve(capacity);
		ids.reserve(capacity);
	}

	ComponentBucket(const ComponentBucket&) = default;
	
	~ComponentBucket() 
	{
		for (auto* component : components)
		{
			component = nullptr;
			UNUSED_VAR( component );
		}
	}

	void add( uint8_t* component, id::EntityId id )
	{
		components.push_back(component);
		ids.push_back(id);
	}

	void remove( id::EntityId id )
	{
		for (size_t i = 0; i < ids.size(); i++)
		{
			if (ids[i] == id)
			{
				components.erase(components.begin() + i);
				ids.erase(ids.begin() + i);
				return;
			}
		}
	}
};

template<size_t INIT_CAPACITY = 3000>
struct ComponentLookups
{
	std::array<ComponentBucket, cid::ComponentType::COUNT> arrays;
	std::array<std::array<uint8_t*, INIT_CAPACITY>, cid::ComponentType::COUNT> lookup;
	std::array<size_t, cid::ComponentType::COUNT> counts;
	std::array<ComponentMask2, INIT_CAPACITY> componentMasks;

	ComponentLookups() 
	{
		for (size_t i = 0; i < cid::ComponentType::COUNT; i++)
		{
			arrays[i] = ComponentBucket{ INIT_CAPACITY };
		}
	}
	~ComponentLookups()
	{
		for (size_t i = 0; i < cid::ComponentType::COUNT; i++)
		{
			for (size_t j = 0; j < INIT_CAPACITY; j++)
			{
				uint8_t* ptr = lookup[i][j];
				if (!ptr)
					continue;

				delete ptr;
			}
			counts[i] = 0;
		}
	}

	template<IsComponent T>
	inline bool hasAny() const
	{
		ComponentId componentId = T::getId();
		return counts[componentId] > 0;

	}

	template<IsComponent T>
	inline bool has( id::EntityId id ) const
	{
		if (!hasAny<T>())
			return false;

		ComponentId componentId = T::getId();
		return componentMasks[id].has( componentId );
	}

	inline bool has( id::EntityId id, ComponentId componentId ) const
	{
		return componentMasks[id].has( componentId );
	}

	template<IsComponent T>
	inline T* add( id::EntityId id )
	{
		assert( !has<T>( id ) );

		T* ptr = new T();
		
		size_t componentId = T::getId();
		auto& components = lookup[componentId];
		uint8_t* impl = reinterpret_cast<uint8_t*>(ptr);
		components[id] = impl;

		arrays[componentId].add( impl, id );
		counts[componentId]++;
		componentMasks[id].set( componentId );
		
		return ptr;
	}

	template<IsComponent T>
	inline void set( id::EntityId id, const T& t )
	{
		if (has<T>( id ))
		{
			T* ptr = get<T>( id );
			memcpy( (void*)ptr, (void*)&t, sizeof( T ) );
		}
		else 
		{
			T* ptr = nullptr;

			ptr = new T();
			memcpy( (void*)ptr, (void*)&t, sizeof( T ) );

			size_t componentId = T::getId();
			auto& components = lookup[componentId];
			uint8_t* impl = reinterpret_cast<uint8_t*>(ptr);
			components[id] = impl;

			arrays[componentId].add( impl, id );
			counts[componentId]++;
			componentMasks[id].set( componentId );
		}

	}

	inline void set( id::EntityId id, const std::vector<ComponentBuffer>& components )
	{
		for (auto& buffer : components)
		{
			uint8_t* ptr = buffer.data;
			ComponentId componentId = buffer.typeId;

			auto& components = lookup[componentId];
			components[id] = ptr;

			arrays[componentId].add( ptr, id );
			counts[componentId]++;
			componentMasks[id].set( componentId );
		}
	}

	template<IsComponent T>
	inline std::span<T*> getAll()
	{
		size_t componentId = T::getId();
		if (!hasAny<T>())
			return {}; 
		
		uint8_t** arr = arrays[componentId].components.data();
		size_t size = counts[componentId];

		return std::span<T*>( reinterpret_cast<T**>(arr), size );
	}

	template<IsComponent T>
	inline T* get( id::EntityId id )
	{
		if (!has<T>( id ))
			return nullptr;

		size_t componentId = T::getId();
		
		uint8_t* ptr = lookup[componentId][id];
		return reinterpret_cast<T*>(ptr);
	}

	inline std::vector<ComponentBuffer> getBuffers( id::EntityId id )
	{
		std::vector<ComponentBuffer> result;
		for (size_t componentType = 0; componentType < cid::ComponentType::COUNT; componentType++)
		{
			if (componentMasks[id].has( componentType ))
			{
				uint8_t* ptr = lookup[componentType][id];
				result.push_back( ComponentBuffer{ ptr, componentType } );
			}
		}
		return result;
	}

	inline void destroy( id::EntityId id )
	{
		for (size_t componentType = 0; componentType < cid::ComponentType::COUNT; componentType++)
		{
			uint8_t* ptr = lookup[componentType][id];
			if (!ptr)
				continue;

			delete ptr;
			lookup[componentType][id] = nullptr;

			counts[componentType]--;
			componentMasks[id].unset( componentType );

			arrays[componentType].remove( id );
		}
	}

	template<IsComponent T>
	inline void remove( id::EntityId id )
	{
		if (!has<T>( id )
			return;

		size_t componentId = T::getId();
		uint8_t* ptr = lookup[componentId][id];
		if (!ptr)
			return;

		delete ptr;
		lookup[componentId][id] = nullptr;

		counts[componentId]--;
		componentMasks[id].unset( componentId );

		arrays[componentId].remove( id );
	}

	template<IsComponent T>
	inline std::span<id::EntityId> entitiesWith()
	{
		if (!hasAny<T>()) 
		{
			static std::span<id::EntityId> empty;
			return empty;
		}
		
		size_t componentId = T::getId();
		std::vector<id::EntityId>& ids = arrays[componentId].ids;
		return std::span{ ids.data(), ids.size() };
	}
};

class World
{
private:
	ComponentLookups<> _components;
	std::vector<id::EntityId> _destroyedQueue;
	
	// TODO - track changed components
public:
	template<IsComponent T>
	inline std::span<T*> getAll()
	{
		return _components.getAll<T>();
	}

	// TODO - return a span of const T* instead
	template<IsComponent T>
	inline std::span<T*> getAllMutable()
	{
		return _components.getAll<T>();
	}

	template <IsComponent T>
	inline const T* get( id::EntityId id )
	{
		return _components.get<T>( id );
	}

	template <IsComponent T>
	inline T* getMutable( id::EntityId id )
	{
		return _components.get<T>( id );
	}

	template<typename T>
	inline std::span<id::EntityId> getEntitiesWith()
	{
		return _components.entitiesWith<T>();
	}

	template<IsComponent T>
	inline std::tuple<std::span<id::EntityId>, std::span<T*>> getPairs()
	{
		auto entities = _components.entitiesWith<T>();
		auto components = _components.getAll<T>();

		return std::make_tuple( entities, components );
	}

	inline std::vector<ComponentBuffer> getBuffers( id::EntityId id )
	{
		return _components.getBuffers( id );
	}

	template <IsComponent T>
	inline T* add( id::EntityId id )
	{
		return _components.add<T>( id );
	}
	
	template <IsComponent T>
	inline void set( id::EntityId id, const T& t )
	{
		_components.set( id, t );
	}

	template <IsComponent T, IsComponent ...U>
	inline void set( id::EntityId id, const T& t, const U&... args )
	{
		set( id, t );
		set( id, args... );
	}

	template <IsComponent T>
	inline bool has( id::EntityId id )
	{
		return _components.has<T>( id );
	}

	template<IsComponent T>
	inline bool hasAny()
	{
		return _components.hasAny<T>();
	}

	inline bool has( id::EntityId id, cid::ComponentType componentType )
	{
		return _components.has( id, static_cast<ComponentId>( componentType ));
	}

	inline id::EntityId spawn()
	{
		return id::NewEntityId();
	}

	template <IsComponent T>
	inline id::EntityId spawn( const T& t )
	{
		id::EntityId id = spawn();

		set( id, t );
		return id;
	}

	template <IsComponent ...T>
	inline id::EntityId spawn( const T&... args )
	{
		id::EntityId id = spawn();

		set( id, args... );
		return id;
	}

	inline id::EntityId spawn( std::vector<ComponentBuffer>& commponents )
	{
		id::EntityId id = spawn();
		_components.set( id, commponents );
		return id;
	}


	void destroyEntity( id::EntityId id )
	{
		_components.destroy( id );
		_destroyedQueue.push_back( id );
	}

	std::vector<id::EntityId> flushDestroyed() 
	{
		std::vector<id::EntityId> result;
		result.swap( _destroyedQueue );
		
		return result;
	}

	// HMM...
	template<typename T>
	void load() {}
	// HMM...
	template<typename T>
	void save() {}
	// HMM...
	void load( const std::string& sceneName ) {}
	// HMM...
	void save( const std::string& sceneName ) {}
};



void components_api_test();

END_NAMESPACE1
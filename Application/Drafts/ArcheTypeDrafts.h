
#include <concepts>
#include <unordered_map>

using EntityId = unsigned;
using ComponentId = unsigned;
using ArchetypeId = unsigned;

struct ComponentBase {};

struct Entity 
{
	EntityId id;
	ArchetypeId arhetypeId;
};

template<typename T>
concept IsComponent = requires()
{
	{ T::ComponentId } -> std::unsigned_integral;
};

struct Archetype 
{
	void** components;
};

std::unordered_map<ArchetypeId, Archetype> achetypeMap;

struct Position 
{
	float value[3] = { 0,0,0 };
};

struct Velocity
{
	float value[3] = { 0,0,0 };
};

void run_tests() 
{
	Velocity velocity;

	Position position;
	size_t positionCount = 10;
	size_t componentTypeCount = 1;

	void** components = nullptr;
	components = (void**)::operator new(sizeof(void*) * componentTypeCount);
	components[0] = ::operator new(sizeof(Position) * positionCount);

	for (int i = 0; i < componentTypeCount; i++) 
	{
		Position* positonArr = (Position*)components[i];
		for (int j = 0; j < positionCount; j++)
		{
			positonArr[j] = Position
			{
				.value = {(float)i, float(j), 0}
			};
		}
	}

	for (int i = 0; i < componentTypeCount; i++)
	{
		Position* positonArr = (Position*)components[i];
		for (int j = 0; j < positionCount; j++)
		{
			Position pos = positonArr[i];
			printf("\n%f,%f,%f", pos.value[0], pos.value[1], pos.value[2]);
		}
	}

	for (int i = 0; i < componentTypeCount; i++)
	{
		Position* positonArr = (Position*)components[i];
		::operator delete(positonArr);
	}

	::operator delete(components);
}


#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "Containers/SmallVector.h"

#include <string>
#include <functional>

DECLARE1( struct, rendering, Gfx )

BEGIN_NAMESPACE1( rendering )

using GraphFilter = std::function<bool( Gfx& )>;

struct GraphFilters
{
	SmallVector<std::string, 10> filterNames{};
	SmallVector<GraphFilter, 10> filters{};
	
	GraphFilters() = default;
	
	~GraphFilters() {}

	GraphFilters( const GraphFilters& other ) noexcept
	{
		filterNames = other.filterNames;
		filters = other.filters;
	}
	
	GraphFilters( GraphFilters&& other ) noexcept
	{
		filterNames = other.filterNames;
		filters = other.filters;
	}

	const GraphFilters& operator=( const GraphFilters& other ) noexcept
	{
		filterNames = other.filterNames;
		filters = other.filters;
		return *this;
	}

	const GraphFilters& operator=( GraphFilters&& other ) noexcept
	{
		filterNames = other.filterNames;
		filters = other.filters;
		return *this;
	}

	void add( const std::string& filterName, GraphFilter filter );
	bool empty() { return filters.empty(); }
	bool enabled( const std::string& graphName, Gfx& gfx ) const;
};

END_NAMESPACE1
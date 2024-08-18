#include "Precompiled/Precompiled.h"
#include "GraphFilters.h"

BEGIN_NAMESPACE1( rendering )

void GraphFilters::add( const std::string& filterName, GraphFilter filter )
{
	filterNames.pushBack( filterName );
	filters.pushBack( filter );
}

bool GraphFilters::enabled( const std::string& graphName, Gfx& gfx ) const
{
	assert( filterNames.size() == filters.size() );

	for (size_t i = 0; i < filterNames.size(); i++) 
	{
		const std::string& str = filterNames[i];
		const bool hasFilter = str == graphName;
		
		if (!hasFilter)
			continue;

		const GraphFilter& filter = filters[i];
		return filter( gfx );
	}
	
	return true;
}

END_NAMESPACE1
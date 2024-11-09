#pragma once

#include "Precompiled/PrecompilationMacros.h"

BEGIN_NAMESPACE1( resources )

struct Handle
{
	unsigned int id = ~0u;

	bool operator == ( Handle other ) const
	{
		return other.id == id;
	}

	bool operator != ( Handle other ) const
	{
		return !(operator==( other ));
	}
};


END_NAMESPACE1


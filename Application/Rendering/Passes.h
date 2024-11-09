#pragma once

#include "Precompiled/PrecompilationMacros.h"
#include "RenderPass.h"

#include <vector>

BEGIN_NAMESPACE1( rendering )

using PassIndex = size_t;

struct Passes
{
	std::vector<RenderPass> passes{};
	std::vector<PassIndex> indices{};

	size_t count() const
	{
		return passes.size();
	}

	void validate()
	{
		assert( passes.size() == indices.size() != 0 );
	}
};

END_NAMESPACE1
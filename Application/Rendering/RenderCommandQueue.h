#pragma once

#include "RenderCommands.h"

BEGIN_NAMESPACE1( rendering )

class RenderCommandQueue
{
public:
	void queue( rendering::RenderCommand&& cmd )
	{
		impl.emplace_back( cmd );
	}

	const std::vector<rendering::RenderCommand>& get() const
	{
		return impl;
	}

	void flush( RenderCommandQueue& other )
	{
		other.impl.swap( impl );
		impl.clear();
	}
private:
	std::vector<rendering::RenderCommand> impl;
};
END_NAMESPACE1
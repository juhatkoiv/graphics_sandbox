#include "Precompiled/Precompiled.h"
#include "RenderGraph.h"

#include "RenderQueue.h"
#include "RenderPass.h"
#include "RenderDefinitions.h"
#include "GraphFilters.h"


BEGIN_NAMESPACE1( rendering )

namespace
{
	struct Pair
	{
		RenderQueue queue;
		int order;
	};

	std::vector<Pair> _queues;
}

RenderGraph::~RenderGraph(){}

RenderQueue& RenderGraph::addQueue( const std::string& queueName, int order )
{
    RenderQueue queue{ queueName };
	_queues.push_back( { queue, order } );

	RenderQueue& result = _queues.back().queue;

	std::ranges::sort( _queues, []( const auto& p1, const auto& p2 ) -> bool
		{
			return p1.order < p2.order;
		} );

	return result;
}

void RenderGraph::execute( GfxFrame& gfx, GfxDevice* device, const FrameResources& frameResources ) const
{
	for (auto& [queue, order] : _queues)
	{
		Queue q = static_cast<Queue>(order);
		
		GfxQueue& gfxQueue = gfx.queues[q];
		gfxQueue.frame = &gfx;

		queue.execute( gfxQueue, device, frameResources );
	}
}

bool RenderGraph::empty() const { return _queues.empty(); }

bool RenderGraph::changed() const
{
	// TODO
	return false;
}


END_NAMESPACE1


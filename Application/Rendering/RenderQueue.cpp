#include "Precompiled/Precompiled.h"
#include "RenderQueue.h"

#include "RenderFeature.h"
#include "RenderDefinitions.h"
#include <ranges>
#include <algorithm>
#include <functional>

#include "FeatureFilters.h"

BEGIN_NAMESPACE1( rendering )

RenderQueue::RenderQueue( const std::string& queueName )
{
	FeatureFilters::initalizeFilters();

	_queueName = queueName;

}

RenderQueue::~RenderQueue() {}

void RenderQueue::execute( GfxQueue& queue, GfxDevice* device, const FrameResources& frameResources )
{
	for (const auto& feature : _renderFeatures)
	{
		const auto& featureName = feature.name();
		if (!FeatureFilters::enabled( featureName, queue ))
			continue;

		feature.execute( queue, device, frameResources );
	}
}

RenderFeature& RenderQueue::addRenderFeature( const std::string& name )
{
	_renderFeatures.push_back( RenderFeature{ name } );
	return _renderFeatures.back();
}

END_NAMESPACE1
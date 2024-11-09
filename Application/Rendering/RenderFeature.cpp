#include "Precompiled/Precompiled.h"
#include "RenderQueue.h"

#include "RenderingFlags.h"
#include "RenderPass.h"
#include "FrameResources.h"
#include "Passes.h"

#include <functional>
#include <algorithm>
#include "RenderFeature.h"

BEGIN_NAMESPACE1( rendering )

RenderFeature::RenderFeature( const std::string& name )
	: featureName( name )
{}

RenderFeature::~RenderFeature() {}

void RenderFeature::execute( GfxQueue& ctx, GfxDevice* device, const FrameResources& input ) const
{
	for (auto passIndex : passOrder)
	{
		const auto& resources = input.passResources( passIndex );
		const auto& pass = passes.at( passIndex );

		pass.impl( ctx, device, resources );
	}
}

void RenderFeature::addPasses( const Passes& passes )
{
	const size_t count = passes.count();

	for (size_t i = 0; i < count; i++)
	{
		addPass( passes.indices[i], passes.passes[i] );
	}

	struct PassSorter
	{
		bool operator()( PassIndex one, PassIndex other )
		{
			return one < other;
		}
	};
	std::sort( passOrder.begin(), passOrder.end(), PassSorter{} );
}

void RenderFeature::addPass( PassIndex index, const RenderPass& pass )
{
	passes[index] = pass;
	passOrder.push_back( index );
}

END_NAMESPACE1
#include "Precompiled/Precompiled.h"
#include "RenderQueue.h"

#include "RenderFeature.h"
#include "RenderDefinitions.h"
#include <ranges>
#include <algorithm>
#include <functional>

BEGIN_NAMESPACE1( rendering )

namespace 
{
	struct FeatureFilters
	{

		using FilterImpl = std::function<bool( const GfxQueue& )>;

		static std::unordered_map<std::string, FilterImpl>& filters()
		{
			static std::unordered_map<std::string, FilterImpl> filters;
			return filters;
		}

		static void initalizeFilters()
		{
			auto& filterMap = filters();
			if (!filterMap.empty())
				return;

			filterMap["highlight_border"] = []( const GfxQueue& queue ) -> bool
				{
					for (auto& [id, e] : queue.frame->effectLookup)
					{
						if (e == RenderEffect::HighlightOutline3D)
							return true;
					}
					return false;
				};
			filterMap["skybox"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getClearSettings().mode == ClearMode::Skybox;
				};
			filterMap["full_screen_blur"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::Blur;
				};
			filterMap["full_screen_inverted"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::Inverted;
				};
			filterMap["full_screen_none"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::Nothing;
				};
			filterMap["full_screen_pixelated"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::Pixelation;
				};
			filterMap["full_screen_chrome_aberrated"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::ChromeAberration;
				};
			filterMap["full_screen_color_corrected"] = []( const GfxQueue& queue ) ->bool
				{
					return queue.getPostProcessingEffects() == PostProcessingEffects::ColorCorrection;
				};
		}

		static bool enabled( const std::string& name, const GfxQueue& queue )
		{
			auto& filterMap = filters();
			if (!filterMap.contains( name ))
				return true;

			return filterMap[name]( queue );
		}
	};
}
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
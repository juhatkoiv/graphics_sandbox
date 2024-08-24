#include "Precompiled/Precompiled.h"
#include "FeatureFilters.h"

#include "RenderQueue.h"
#include "RenderDefinitions.h"

BEGIN_NAMESPACE1( rendering )

std::unordered_map<std::string, FilterImpl>& FeatureFilters::filters()
{
	static std::unordered_map<std::string, FilterImpl> filters;
	return filters;
}

void FeatureFilters::initalizeFilters()
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

bool FeatureFilters::enabled( const std::string& name, const GfxQueue& queue )
{
	auto& filterMap = filters();
	if (!filterMap.contains( name ))
		return true;

	return filterMap[name]( queue );
}

END_NAMESPACE1;

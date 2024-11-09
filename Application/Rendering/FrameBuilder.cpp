#include "Precompiled/Precompiled.h"
#include "FrameBuilder.h"

#include "RenderPass.h"
#include "RenderGraph.h"
#include "RenderQueue.h"
#include "RenderFeature.h"
#include "Passes.h"

#include "RenderFunctions.h"
#include "RenderDefinitions.h"
#include "FrameResources.h"
#include "PassResources.h"
#include "GraphFilters.h"

#include "Serialization/RendererJsons.h"
#include "AssetLoading/JsonFileSystem.h"

#include <memory>

BEGIN_NAMESPACE1( rendering )

namespace
{
	RenderGraphJson loadRenderGraph( const std::string& fileName )
	{
		RenderGraphJson results{};
		assets::JsonFileSystem::LoadFromJson( results, fileName.c_str() );
		if (results.queues.empty())
			throw;

		return results;
	}

	std::vector<RenderResourcesJson> loadResourceJsons( const std::string& fileName )
	{
		std::vector<RenderResourcesJson> results{};
		assets::JsonFileSystem::LoadFromJson( results, fileName.c_str() );
		if (results.empty())
			throw;

		return results;
	}

	PassImpl resolveRenderCallback( const std::string& passType )
	{
		if (passType == "clear")
			return pass::executeClear;

		if (passType == "bindFrameBuffer")
			return pass::executeBindFrameBuffer;

		if (passType == "bindAndClearFrameBuffer")
			return pass::executeBindAndClearFrameBuffer;

		if (passType == "lit")
			return pass::executeLit;

		if (passType == "lights")
			return pass::executeLights;

		if (passType == "colorBuffersToFullScreen")
			return pass::executePostProcessingEffect;

		if (passType == "blur")
			return pass::executePostProcessingEffect;
		
		if (passType == "chromaticAberrationToFullScreen")
			return pass::executePostProcessingEffect;

		if (passType == "pixelatedToFullScreen")
			return pass::executePostProcessingEffect;

		if (passType == "colorCorrectedToFullScreen")
			return pass::executePostProcessingEffect;

		if (passType == "writeBorderStencil")
			return pass::executeWriteBorderStencil;

		if (passType == "drawOutlineBorder")
			return pass::executeDrawOutlineBorder;

		if (passType == "skybox")
			return pass::executeDrawSkybox;

		return pass::executeError;
	}

	Passes createPasses( const std::vector<RenderPassJson>& passInputs )
	{
		Passes result{};

		for (const auto& passInput : passInputs)
		{
			result.passes.push_back( RenderPass
			{
				.name = passInput.passName,
				.impl = resolveRenderCallback( passInput.passType )
			} );
			result.indices.push_back( passInput.passIndex );
		}
		return result;
	}

	RenderGraph createRenderGraph( const RenderGraphJson& renderGraphJson )
	{
		const auto& queueInputs = renderGraphJson.queues;
		auto& graph = RenderGraph::get();
		
		for (const auto& queueInput : queueInputs)
		{
			RenderQueue& queue = graph.addQueue( queueInput.queueName, queueInput.order );
			
			for (const auto& featureInput : queueInput.features)
			{
				RenderFeature& feature = queue.addRenderFeature( featureInput.featureName );
				Passes passes = createPasses( featureInput.passes );
				feature.addPasses( passes );
			}
		}

		return graph;
	}

	std::vector<PassResources> createRenderResources( const std::span<RenderResourcesJson>& jsonData )
	{
		std::vector<PassResources> results{};
		results.resize( jsonData.size() );

		for (int i = 0; i < jsonData.size(); i++)
		{
			const auto& jd = jsonData[i];
			results[i] = PassResources
			{
				jd.flags,
				jd.sourceFramebuffers,
				jd.targetFrameBuffer,
				shader::getShaderId( jd.shaderName.c_str() ),
			};
		}
		return results;
	}

	FrameResources loadFrameData( const std::string& fileName )
	{
		FrameResources result{};
		auto frameDataJson = loadResourceJsons(fileName);
		auto renderResources = createRenderResources( frameDataJson );

		assert( frameDataJson.size() == renderResources.size() && "Mismatch between json data and resources." );

		for (int i = 0; i < frameDataJson.size(); i++)
		{
			const auto& r = renderResources[i];
			const auto& f = frameDataJson[i];
			result.setPassResources( f.passIndex, r );
		}
		return result;
	}

	RenderGraph& graphCache()
	{
		static RenderGraph cache{};
		return cache;
	}

	FrameResources& getFrameDataCache()
	{
		static FrameResources cache{};
		return cache;
	}
}


RenderGraph& FrameBuilder::buildGraph()
{
	auto& queues = graphCache();
	if (!queues.empty())
	{
		return queues;
	}

	auto loadedQueues = loadRenderGraph( "data//RenderGraph.json" );
	auto graph = createRenderGraph( loadedQueues );
	
	graphCache() = graph;

	return graphCache();
}

const FrameResources& FrameBuilder::buildFrameResources()
{
	FrameResources& frameData = getFrameDataCache();
	if (!frameData.passData.empty())
		return frameData;

	auto loadedFrameData = loadFrameData( "data//RenderResources.json" );
	getFrameDataCache() = loadedFrameData;
	return getFrameDataCache();
}

END_NAMESPACE1

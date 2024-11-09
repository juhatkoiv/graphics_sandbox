#include "Precompiled/Precompiled.h"
#include "Defaults.h"

#include "RenderDefinitions.h"
#include "FrameResources.h"
#include "PassResources.h"

#include "Serialization/RendererJsons.h"

#include <memory>

BEGIN_NAMESPACE1( rendering )

static PassIndex passIndex = 0;
static PassIndex resourceIndex = 0;

RenderGraphJson Defaults::buildGraphJson()
{
	passIndex = 0;

	return RenderGraphJson
	{
		.queues
		{
			RenderQueueJson
			{
				.order = 0,
				.queueName = "opaque",
				.features
				{	
					RenderFeatureJson
					{
	
					},
					RenderFeatureJson
					{
						.featureName = "opaque_lit",
						.passes =
						{
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_postprocessing",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "lit",
								.passName = "render_lit",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "lights",
								.passName = "render_lights",
								.passIndex = passIndex++
							}
						}
					},
					RenderFeatureJson
					{
						.featureName = "highlight_border",
						.passes =
						{
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_enable_write_stencil",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "writeBorderStencil",
								.passName = "write_stencil_mask_entity",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_enable_read_stencil",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "drawOutlineBorder",
								.passName = "draw_outline",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_disable_stencil",
								.passIndex = passIndex++
							},
						}
					}
				}
			},
			RenderQueueJson
			{
				.order = 1,
				.queueName = "skybox",
				.features
				{
					RenderFeatureJson
					{
						.featureName = "skybox",
						.passes
						{
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_skybox",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "skybox",
								.passName = "render_skybox",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "clear",
								.passName = "reset_skybox",
								.passIndex = passIndex++
							}
						}
					},
				}
			},
			RenderQueueJson
			{
				.order = 2,
				.queueName = "transparent",
				.features
				{
					RenderFeatureJson
					{
						.featureName = "transparent_lit",
						.passes
						{
							RenderPassJson
							{
								.passType = "clear",
								.passName = "enable_blend",
								.passIndex = passIndex++,
							},
							RenderPassJson
							{
								.passType = "lit",
								.passName = "render_lit_transparent",
								.passIndex = passIndex++,
							},
							RenderPassJson
							{
								.passType = "lights",
								.passName = "render_lights_transparent",
								.passIndex = passIndex++,
							},
							RenderPassJson
							{
								.passType = "clear",
								.passName = "clear_and_disable_blend",
								.passIndex = passIndex++,
							}
						}
					}
				}
			},
			RenderQueueJson
			{
				.order = 3,
				.queueName = "post_processing",
				.features
				{
					RenderFeatureJson
					{
						.featureName = "full_screen_none",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "colorBuffersToFullScreen",
								.passName = "render_processing_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
					RenderFeatureJson
					{
						.featureName = "full_screen_inverted",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "colorBuffersToFullScreen",
								.passName = "render_inverted_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
					RenderFeatureJson
					{
						.featureName = "full_screen_blur",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_blur1",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "colorBuffersToFullScreen",
								.passName = "render_postprocessing_to_blur1",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_blur2",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "blur",
								.passName = "render_horizontal_blur_to_blur2",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "blur",
								.passName = "render_vertical_blur_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
					RenderFeatureJson
					{
						.featureName = "full_screen_pixelated",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "pixelatedToFullScreen",
								.passName = "render_pixelated_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
					RenderFeatureJson
					{
						.featureName = "full_screen_chrome_aberrated",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "chromaticAberrationToFullScreen",
								.passName = "render_chrome_aberrated_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
					RenderFeatureJson
					{
						.featureName = "full_screen_color_corrected",
						.passes
						{
							RenderPassJson
							{
								.passType = "bindAndClearFrameBuffer",
								.passName = "bind_and_clear_framebuffer_backbuffer",
								.passIndex = passIndex++
							},
							RenderPassJson
							{
								.passType = "colorCorrectedToFullScreen",
								.passName = "render_color_corrected_to_backbuffer",
								.passIndex = passIndex++
							},
						}
					},
				}
			}
		}
	};
}

std::vector<RenderResourcesJson> Defaults::buildResourcesJson()
{
	FrameResources frameData = buildFrameResources();
	std::vector<RenderResourcesJson> resources = {};
	for (auto& [i, r] : frameData.passData)
	{
		resources.push_back( RenderResourcesJson
			{
				.flags = r.flags,
				.sourceFramebuffers = r.sourceFramebuffers,
				.targetFrameBuffer = r.targetFrameBuffer,
				.shaderName = shader::getShaderName( r.shaderId ),
				.passIndex = i
			} );
	}
	return resources;
}

FrameResources Defaults::buildFrameResources()
{
	FrameResources resources{};

	// Opaque
	{
		auto clearFramebufferFlags = GfxFlags
		{
			.clearFlags = ClearFlags::Color | ClearFlags::Depth,
			.enableFlags = ClearFlags::Depth,
			.disableFlags = ClearFlags::None,
			.cullingDesc =
			{
				.culling = Culling::BackFace,
				.cullingEquation = CullingEquation::Ccw,
		},
		.clearColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.f )
		};

		PassResources clearBackbufferResources{};
		clearBackbufferResources.setFlags( clearFramebufferFlags );

		PassResources opaqueToPostProcessingPassResources{};
		opaqueToPostProcessingPassResources.setTargetFrameBuffer( RenderTargetType::PostProcessing );
		opaqueToPostProcessingPassResources.setFlags( clearFramebufferFlags );

		PassResources litPassResources{};
		litPassResources.setShaderId( shader::LAMBERTIAN );

		PassResources lightsPassResources{};
		lightsPassResources.setShaderId( shader::LIGHTS );


		auto clearStencilFlags = GfxFlags
		{
			.clearFlags = ClearFlags::Stencil,
			.enableFlags = ClearFlags::Stencil,
			.stencilDesc
			{
				.stencilFunc = StencilFunc::Always,
				.stencilFailOp = StencilOp::Replace,
				.stencilDepthFailOp = StencilOp::Replace,
				.stencilBothOkOp = StencilOp::Replace,
				.sencilMask = 0xFF,
				.stencilRef = 1,
		}
		};

		PassResources clearStencilPassResouces{};
		clearStencilPassResouces.setFlags( clearStencilFlags );

		PassResources writeStencilPassResources{};
		writeStencilPassResources.setShaderId( shader::SINGLE_COLOR_BORDER );
		auto drawOutlineFlags = GfxFlags
		{
			.clearFlags = ClearFlags::None,
			.enableFlags = ClearFlags::Depth | ClearFlags::Stencil,
			.stencilDesc =
			{
				.stencilFunc = StencilFunc::Equal,
				.stencilFailOp = StencilOp::Keep,
				.stencilDepthFailOp = StencilOp::Keep,
				.stencilBothOkOp = StencilOp::Keep,
				.sencilMask = 0xFF,
				.stencilRef = 0,
		}
		};

		PassResources clearDrawOutlinePassResources{};
		clearDrawOutlinePassResources.setFlags( drawOutlineFlags );

		PassResources drawOutlineBorderPassResources{};
		drawOutlineBorderPassResources.setShaderId( shader::SINGLE_COLOR_BORDER );

		auto disableStencilFlags = GfxFlags
		{
			.clearFlags = ClearFlags::None,
			.enableFlags = ClearFlags::None,
			.disableFlags = ClearFlags::Stencil
		};

		PassResources disableStencilPassResources{};
		disableStencilPassResources.setFlags( disableStencilFlags );

		resources.setPassResources( resourceIndex++, clearBackbufferResources );
		resources.setPassResources( resourceIndex++, opaqueToPostProcessingPassResources );
		resources.setPassResources( resourceIndex++, litPassResources );
		resources.setPassResources( resourceIndex++, lightsPassResources );
		resources.setPassResources( resourceIndex++, clearStencilPassResouces );
		resources.setPassResources( resourceIndex++, writeStencilPassResources );
		resources.setPassResources( resourceIndex++, clearDrawOutlinePassResources );
		resources.setPassResources( resourceIndex++, drawOutlineBorderPassResources );
		resources.setPassResources( resourceIndex++, disableStencilPassResources );
	}
	
	// Skybox
	{
		PassResources clearSkyboxResources;
		clearSkyboxResources.setFlags( GfxFlags
			{
				.clearFlags = ClearFlags::None,
				.enableFlags = ClearFlags::Depth,
				.cullingDesc =
				{
					.culling = Culling::None
				},
				.depthDesc =
				{
					.depthMask = false,
					.depthFunc = DepthFunc::LEqual
				}
			} );

		PassResources renderSkyboxResources = PassResources::none();
		renderSkyboxResources.setShaderId( shader::SKYBOX );

		PassResources resetSkyboxFlagsResources;
		resetSkyboxFlagsResources.setFlags( GfxFlags
			{
				.clearFlags = ClearFlags::None,
				.enableFlags = ClearFlags::Depth,
				.disableFlags = ClearFlags::None,
				.cullingDesc =
				{
					.culling = Culling::BackFace,
				},
				.depthDesc =
				{
					.depthMask = true,
					.depthFunc = DepthFunc::Less
				}
			} );

		resources.setPassResources( resourceIndex++, clearSkyboxResources );
		resources.setPassResources( resourceIndex++, renderSkyboxResources );
		resources.setPassResources( resourceIndex++, resetSkyboxFlagsResources );
	}
	
	// Transparent 
	{

		auto clearBlendFlags = GfxFlags
		{
			.clearFlags = ClearFlags::None,
			.enableFlags = ClearFlags::Blend | ClearFlags::Depth,
			.disableFlags = ClearFlags::Stencil,
			.blendDesc =
			{
				.srcBlend = BlendOp::SrcAlpha,
				.dstBlend = BlendOp::OneMinusSrcAlpha,
				.blendEquation = BlendEquation::Add,
				.srcBlendAlpha = BlendOp::One,
				.dstBlendAlpha = BlendOp::Zero,
				.blendColor = { 1.f, 1.f, 1.f, 1.f }
		}
		};

		auto disableBlendFlags = GfxFlags
		{
			.clearFlags = ClearFlags::None,
			.enableFlags = ClearFlags::None,
			.disableFlags = ClearFlags::Blend
		};

		auto clearBlendPassResources = PassResources{};
		clearBlendPassResources.setFlags( clearBlendFlags );
		clearBlendPassResources.setTargetFrameBuffer( RenderTargetType::PostProcessing );

		auto renderLitTransparentResources = PassResources{};
		renderLitTransparentResources.setShaderId( shader::LAMBERTIAN_TRANSPARENT );

		auto renderLightsTransparentResources = PassResources{};
		renderLightsTransparentResources.setShaderId( shader::LIGHTS );

		auto disableBlendPassResources = PassResources{};
		disableBlendPassResources.setFlags( disableBlendFlags );

		resources.setPassResources( resourceIndex++, clearBlendPassResources );
		resources.setPassResources( resourceIndex++, renderLitTransparentResources );
		resources.setPassResources( resourceIndex++, renderLightsTransparentResources );
		resources.setPassResources( resourceIndex++, disableBlendPassResources );

	}

	// Post Processing
	{
		auto clearFsFlags = GfxFlags
		{
			.clearFlags = ClearFlags::Color | ClearFlags::Depth,
			.enableFlags = ClearFlags::None,
			.disableFlags = ClearFlags::Depth,
			.clearColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.f )
		};

		// Effect - none
		PassResources bindBackbufferPassResources{};
		bindBackbufferPassResources.setTargetFrameBuffer( RenderTargetType::Backbuffer );
		bindBackbufferPassResources.setFlags( clearFsFlags );

		PassResources postProcessingToBackBufferPassResources{};
		postProcessingToBackBufferPassResources.setShaderId( shader::FULL_SCREEN );
		postProcessingToBackBufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );

		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, postProcessingToBackBufferPassResources );

		// Effect - inverted
		PassResources invertedToBackBufferPassResources{};
		invertedToBackBufferPassResources.setShaderId( shader::FULL_SCREEN_COLOR_INVERTED );
		invertedToBackBufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );

		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, invertedToBackBufferPassResources );

		// Effect - blur
		PassResources bindBlurBuffer1PassResources{};
		bindBlurBuffer1PassResources.setTargetFrameBuffer( RenderTargetType::BlurBuffer1 );
		bindBlurBuffer1PassResources.setFlags( clearFsFlags );

		PassResources postProcessingToBlurBufferPassResources{};
		postProcessingToBlurBufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );
		postProcessingToBlurBufferPassResources.setShaderId( shader::FULL_SCREEN );

		PassResources bindBlurBuffer2PassResources{};
		bindBlurBuffer2PassResources.setTargetFrameBuffer( RenderTargetType::BlurBuffer2 );
		bindBlurBuffer2PassResources.setFlags( clearFsFlags );

		PassResources blur1ToBlur2PassResources{};
		blur1ToBlur2PassResources.addSourceFrameBuffer( RenderTargetType::BlurBuffer1 );;
		blur1ToBlur2PassResources.setShaderId( shader::FULL_SCREEN_BLUR_HORIZONTAL );

		PassResources blur2ToBackbufferPassResouces{};
		blur2ToBackbufferPassResouces.addSourceFrameBuffer( RenderTargetType::BlurBuffer2 );
		blur2ToBackbufferPassResouces.setShaderId( shader::FULL_SCREEN_BLUR_VERTICAL );

		resources.setPassResources( resourceIndex++, bindBlurBuffer1PassResources );
		resources.setPassResources( resourceIndex++, postProcessingToBlurBufferPassResources );
		resources.setPassResources( resourceIndex++, bindBlurBuffer2PassResources );
		resources.setPassResources( resourceIndex++, blur1ToBlur2PassResources );
		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, blur2ToBackbufferPassResouces );

		// Effect - pixelated
		PassResources pixelatedToBackbufferPassResources{};
		pixelatedToBackbufferPassResources.setShaderId( shader::FULL_SCREEN_PIXELATED );
		pixelatedToBackbufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );

		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, pixelatedToBackbufferPassResources );

		// Effect - chrome aberration
		PassResources chromeAberrationToBackbufferPassResources{};
		chromeAberrationToBackbufferPassResources.setShaderId( shader::FULL_SCREEN_CHROME_ABERRATION );
		chromeAberrationToBackbufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );

		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, chromeAberrationToBackbufferPassResources );

		// Effect - color correction
		PassResources colorCorrectionToBackbufferPassResources{};
		colorCorrectionToBackbufferPassResources.setShaderId( shader::FULL_SCREEN_COLOR_CORRECTION );
		colorCorrectionToBackbufferPassResources.addSourceFrameBuffer( RenderTargetType::PostProcessing );

		resources.setPassResources( resourceIndex++, bindBackbufferPassResources );
		resources.setPassResources( resourceIndex++, colorCorrectionToBackbufferPassResources );

	}
	
	return resources;
}

std::span<std::string> Defaults::buildPassNameList()
{
	static std::vector<std::string> passNames
	{
		"clear_backbuffer",
		"bind_and_clear_framebuffer_postprocessing",
		"render_lit",
		"render_lights",
		"clear_enable_write_stencil",
		"write_stencil_mask_entity",
		"clear_enable_read_stencil",
		"draw_outline",
		"clear_disable_stencil",
		"clear_skybox",
		"render_skybox",
		"enable_blend",
		"render_lit_transparent",
	};
	return std::span<std::string>{ passNames.data(), passNames.size() };
}

std::span<std::string> Defaults::buildFeatureNameList()
{
	static std::vector<std::string> featureNames
	{
		"opaque_lit",
		"highlight_border",
		"skybox",
		"transparent_lit",
		"full_screen_none",
		"full_screen_inverted",
		"full_screen_blur",
		"full_screen_pixelated",
		"full_screen_chrome_aberrated",
		"full_screen_color_corrected",
	};
	return std::span<std::string>();
}

void Defaults::validateBuild()
{
	assert( passIndex == resourceIndex );
	passIndex = 0;
	resourceIndex = 0;
}

END_NAMESPACE1
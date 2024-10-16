#include "Precompiled/Precompiled.h"
#include "GfxWorker.h"

// Rendering 
#include "RenderPass.h"
#include "RenderGraph.h"
#include "RenderQueue.h"
#include "RenderDefinitions.h"

// Resources
#include "Resources/ResourceContainer.h"

// ECS
#include "ECS/UniqueId.h"

#include <set>
#include <vector>
#include <sstream>
#include <ranges>
#include <algorithm>

#include "RenderingFlags.h"
#include "RenderFunctions.h"
#include "FrameBuilder.h"
#include "RenderCommandQueue.h"

#include "OpenGL/UniformFunctions.h"
#include "GfxDeviceFactory.h"

BEGIN_NAMESPACE1( rendering )

namespace
{
	using namespace assets;
	using namespace ecs;

	GfxFrame frame{};

	struct GfxPostProcessing 
	{
		unsigned args1 = ~0u;
		unsigned args2 = ~0u;
		unsigned args3 = ~0u;
		unsigned args4 = ~0u;
	};


	GfxHandle cameraBuffer = ~0u;
	GfxHandle lightBuffer = ~0u;
	GfxHandle postProcessingBuffer = ~0u;
	GfxHandle globalLightingSettingsBuffer = ~0u;

	//GLuint objectBuffer = ~0u;

	GfxPostProcessing resolvePostProcessingArgs( GfxFrame& frame )
	{
		GfxPostProcessing args;
		args.args1 = 0;
		args.args2 = 0;
		args.args3 = 0;
		args.args4 = 0;

		auto& settings = frame.settings;

		if (settings.fullScreenEffect == PostProcessingEffects::Pixelation)
		{
			// bitwise cast floats
			args.args1 = *reinterpret_cast<unsigned int*>(&settings.pixelatedSettings.fragmentScale);
		}

		if (settings.fullScreenEffect == PostProcessingEffects::ColorCorrection)
		{
			// bitwise cast floats
			args.args1 = *reinterpret_cast<unsigned int*>(&settings.colorCorrectionSettings.colorCorrection.x);
			args.args2 = *reinterpret_cast<unsigned int*>(&settings.colorCorrectionSettings.colorCorrection.y);
			args.args3 = *reinterpret_cast<unsigned int*>(&settings.colorCorrectionSettings.colorCorrection.z);
		}

		if (settings.fullScreenEffect == PostProcessingEffects::ChromeAberration)
		{
			// bitwise cast floats
			args.args1 = *reinterpret_cast<unsigned int*>(&settings.chromeAberrationSettings.aberration);
		}

		if (settings.fullScreenEffect == PostProcessingEffects::Blur)
		{
			args.args1 = *reinterpret_cast<unsigned int*>(&settings.fullScreenBlurSettings.kernelSize);
			args.args2 = *reinterpret_cast<unsigned int*>(&settings.fullScreenBlurSettings.sigma);

			glm::vec2 windowSizeInverted = frame.camera.windowSize.getInverted();

			args.args3 = *reinterpret_cast<unsigned int*>(&windowSizeInverted.x);
			args.args4 = *reinterpret_cast<unsigned int*>(&windowSizeInverted.y);
		}

		if (settings.fullScreenEffect == PostProcessingEffects::Inverted)
		{
		}

		return args;
	}
}

GfxWorker::GfxWorker( rendering::GfxDeviceFactory& deviceFactory )
	: _deviceFactory( deviceFactory )
{}

void GfxWorker::setApi( int api )
{
	// prepare -> create full screen quads needed for post processing. should not be client's responsibility

	_device = _deviceFactory.createDevice( api );

	// allocate camera buffer
	{
		cameraBuffer = _device->allocateConstantBuffer( 2 * sizeof( glm::mat4 ), 0, 0 );
	}

	// allocate lights buffer
	{
		lightBuffer = _device->allocateConstantBuffer( sizeof( GfxLighing ), 0, 3 );
	}

	// allocate object buffer
	{
		postProcessingBuffer = _device->allocateConstantBuffer( sizeof( GfxPostProcessing ), 0, 4 );
	}

	// allocate global lighting settings buffer
	{
		globalLightingSettingsBuffer = _device->allocateConstantBuffer( sizeof( LightingSettings ), 0, 5 );}
}

void GfxWorker::render()
{
	const auto& frameResources = FrameBuilder::buildFrameResources();
	const auto& renderGraph = FrameBuilder::buildGraph();

	renderGraph.execute( frame, _device.get(), frameResources );

	for (auto& [key, value] : frame.queues)
	{
		value.batches.clear();
	}
	frame.lighting.setDirty();
}

void GfxWorker::clear() {}

void GfxWorker::update( RenderCommandQueue& queue )
{
	RenderCommandQueue renderCommandQueue;
	queue.flush( renderCommandQueue );

	for (auto& cmd : renderCommandQueue.get())
	{
		executeRenderCommand( cmd );
	}
	flushDestroyedQueue();
}


void GfxWorker::queueDestroy( unsigned id )
{
	frame.destroyQueue.insert( id );
}

bool GfxWorker::isQueuedDestroyed( unsigned id )
{
	return frame.destroyQueue.count( id ) != 0;
}

void GfxWorker::flushDestroyedQueue() {}

//////// UPDATE DATA ////////

void GfxWorker::updateCameraProjectionMatrix( const UpdateProjectionMatrixCmd& param )
{
	frame.camera.projectionMatrix = param.projectionMatrix;
}

void GfxWorker::updateWindow( const UpdateWindowCmd& param )
{
	if (frame.camera.windowSize.hasChanged( param.windowSize ))
	{
		frame.camera.windowSize.value = param.windowSize;

		constexpr size_t minI = static_cast<size_t>(RenderTargetType::PostProcessing);
		constexpr size_t maxI = static_cast<size_t>(RenderTargetType::COUNT);

		unsigned int width = param.windowSize.x;
		unsigned int height = param.windowSize.y;

		for (size_t i = minI; i < maxI; i++)
		{
			auto fb = static_cast<RenderTargetType>(i);
			_device->recreateRenderTarget( fb, width, height );
		}

		_device->setViewport( 0, 0, width, height );
	}
}

void GfxWorker::updateCameraTransform( const UpdateCameraTransformCmd& param )
{
	frame.camera.viewMatrix = param.viewMatrix;
	frame.camera.viewPosition = param.viewPosition;
}

void GfxWorker::setToEffectQueue( unsigned id, const SubmitToEffectQueueCmd& param )
{
	// TODO -- implement effect queue
}

void GfxWorker::createTexture( const CreateTextureCmd& cmd )
{
	id::TextureId textureId = cmd.textureId;
	const TextureData& textureData = *cmd.textureData;

	_device->createTexture( textureId, rendering::DIFFUSE_UNIT, Dimensions::T2D, textureData );
}

void GfxWorker::createMesh( const CreateMeshCmd& cmd )
{
	id::MeshId meshId = cmd.meshId;
	const rendering::VertexData& vertexData = *cmd.vertexData;

	_device->createVertexBuffer( meshId, vertexData );
}

void GfxWorker::updateModelMatrix( unsigned id, const UpdateModelMatrixCmd& param )
{
	frame.modelMatrices[id] = param.modelMatrix;

	if (frame.lightProperties.has( id ))
	{
		frame.lightProperties.mutableAt( id ).lightPosition = param.modelMatrix[3];
	}
}

void GfxWorker::setLight( unsigned id, const SetLightCmd& cmd )
{
	const auto& mat = frame.modelMatrices[id];

	LightProperties properties;
	properties.lightPosition = mat[3];
	properties.lightColor = cmd.lightColor;
	properties.intensity = cmd.intensity;
	properties.attenuation = cmd.attenuation;

	auto& lightProperties = frame.lightProperties;
	lightProperties[id] = properties;
	// todo add to or update uniform buffer
}

void GfxWorker::setMaterial( unsigned id, const SetEntityMaterialCmd& param )
{
	Queue q = static_cast<Queue>(param.queue);
	auto& queue = frame.queues[q];

	queue.shaders[id] = GfxShader{ param.shaderId };
	queue.meshColors[id] = param.meshColor;

	DrawCallBatch& batch = queue.batches[param.shaderId];
	auto& entities = batch.entities;
	if (std::find( entities.begin(), entities.end(), id ) == entities.end())
	{
		batch.entities.push_back( id );
	}
}

void GfxWorker::setMeshInstanced( unsigned id, const SetEntityMeshCmd& param )
{
	frame.instancedData.groupEntities.insert( id );
	frame.instancedData.shaderId = shader::getShaderId( shader::LAMBERTIAN_INSTANCED );

	id::MeshId meshId = param.meshId;

	frame.instancedData.meshId = meshId;
}

void GfxWorker::setMesh( unsigned id, const SetEntityMeshCmd& param )
{
	id::MeshId meshId = param.meshId;
	frame.meshIdLookup[id] = meshId;
}

void GfxWorker::setFullScreenMesh( const SetFullScreenQuadCmd& param )
{
	id::MeshId meshId = param.meshId;
	frame.fullScreenQuad = meshId;
}

void GfxWorker::setSkybox( const SetSkyBoxCmd& param )
{
	frame.skyboxMesh = param.meshId;
	frame.skyboxTexture = param.textureId;
}

void GfxWorker::prepareDraw()
{
	GfxQueue& queue = frame.queues[Queue::Transparent];
	for (auto& [shader, batch] : queue.batches)
	{
		// reverse the order of entities in transparent queue
		std::reverse( batch.entities.begin(), batch.entities.end() );
	}

	// NOTE - don't update everything every frame
	// 
	// update camera buffer
	{
		_device->updateConstantBuffer( cameraBuffer, (void*)glm::value_ptr( frame.camera.viewMatrix ), sizeof( glm::mat4 ), 0 );
		_device->updateConstantBuffer( cameraBuffer, (void*)glm::value_ptr( frame.camera.projectionMatrix ), sizeof( glm::mat4 ), sizeof( glm::mat4 ) );
	}

	// update light buffer
	{
		for (auto& [id, properties] : frame.lightProperties)
		{
			properties.lightPosition = frame.modelMatrices[id][3];
			frame.lighting.addLight( properties );
		}
		frame.lighting.viewPosition = frame.camera.viewPosition;

		_device->updateConstantBuffer( lightBuffer, (void*)&frame.lighting, sizeof( GfxLighing ), 0 );
	}

	// update post provcessing data
	{
		GfxPostProcessing args = resolvePostProcessingArgs( frame );
		_device->updateConstantBuffer( postProcessingBuffer, (void*)&args, sizeof( GfxPostProcessing ), 0 );
	}

	// update global lighting settings
	{
		_device->updateConstantBuffer( globalLightingSettingsBuffer, (void*)&frame.settings.lightingSettings, sizeof( LightingSettings ), 0 );
	}
}


void GfxWorker::setTexture( unsigned id, const SetTextureCmd& cmd )
{
	TextureType textureType = cmd.type;
	id::TextureId textureId = cmd.textureId;

	if (has( textureType, TextureType::Diffuse ))
	{
		GfxDiffuseMaterial& diffuse = frame.diffuseMaterials[id];
		diffuse.bindPosition = rendering::DIFFUSE_UNIT;
		diffuse.textureId = textureId;

		frame.textures[id][texture::index<TextureType::Diffuse>()] = textureId;
	}

	if (has( textureType, TextureType::Specular ))
	{
		GfxSpecularMaterial& specular = frame.specularMaterials[id];
		specular.bindPosition = rendering::SPECULAR_UNIT;
		specular.textureId = textureId;

		frame.textures[id][texture::index<TextureType::Specular>()] = textureId;
	}

	if (has( textureType, TextureType::Sprite ))
	{
		GfxSprite& sprite = frame.sprites[id];
		sprite.bindPosition = rendering::SPRITE_UINT;
		sprite.textureId = textureId;

		frame.textures[id][texture::index<TextureType::Sprite>()] = textureId;
	}
}

//////// EXECUTION ////////

void GfxWorker::updateRenderingSettings( const UpdateCameraRenderSettingsCmd& param )
{
	frame.settings = param.settings;
}

void GfxWorker::executeRenderCommand( const RenderCommand& cmd )
{
	switch (cmd.type)
	{
	case CommandType::NONE:
		throw;
		break;
	case CommandType::PrepareTransparentQueue:
		prepareDraw();
		break;
	case CommandType::SetTexture:
		setTexture( cmd.id, cmd.setTextureCmd );
		break;
	case CommandType::UpdateCameraProjection:
		updateCameraProjectionMatrix( cmd.updateProjectionMatrixCmd );
		break;
	case CommandType::UpdateCameraTransform:
		updateCameraTransform( cmd.updateCameraTrasformCmd );
		break;
	case CommandType::UpdateModelMatrix:
		updateModelMatrix( cmd.id, cmd.updateModelMatrixCmd );
		break;
	case CommandType::UpdateWindowSize:
		updateWindow( cmd.updateWindowSizeCmd );
		break;
	case CommandType::SetEffect:
		setToEffectQueue( cmd.id, cmd.submitToEffectQueueCmd );
		break;
	case CommandType::Destroy:
		queueDestroy( cmd.id );
		break;
	case CommandType::SetLight:
		setLight( cmd.id, cmd.setlightCmd );
		break;
	case CommandType::SetMaterial:
		setMaterial( cmd.id, cmd.setEntityMaterialCmd );
		break;
	case CommandType::SetMesh:
		setMesh( cmd.id, cmd.setEntityMeshCmd );
		break;
	case CommandType::SetMeshInstanced:
		setMeshInstanced( cmd.id, cmd.setEntityMeshCmd );
		break;
	case CommandType::UpdateCameraSettings:
		updateRenderingSettings( cmd.updateCameraRenderingSettingsCmd );
		break;
	case CommandType::CreateFullScreenQuad:
		setFullScreenMesh( cmd.setFullScreeQuadCmd );
		break;
	case CommandType::CreateTexture:
		createTexture( cmd.createTextureCmd );
		break;
	case CommandType::CreateMesh:
		createMesh( cmd.createMeshCmd );
		break;
	case CommandType::SetSkyBox:
		setSkybox( cmd.setSkyboxCmd );
		break;
	default:
		break;
	}
}

END_NAMESPACE1
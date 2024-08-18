#pragma once

#include <vector>
#include <functional>
#include <set>

#include "Precompiled/PrecompilationMacros.h"
#include "RenderingPrecompilationMacros.h"
#include "RenderingFlags.h"
#include "RenderCommands.h"
#include "ShaderProgram.h"
#include "RenderEffect.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "RenderingSettings.h"
#include "RenderCommands.h"

#include "GfxBufferImpl.h"
#include "MemoryUtils.h"

#include "OpenGL/GLConstBuffer.h"

#include "Containers/SparseMap.h"
#include "Containers/LinearMap.h"

BEGIN_NAMESPACE1( rendering )

struct DeferredFrameBuffer
{
	std::array<GfxHandle, 4> colorBuffers{ 0, 0, 0, 0 }; // output
	GfxHandle target; // render target
	GfxHandle rbo; // texture

	GLuint* data()
	{
		return colorBuffers.data();
	}

	const std::array<GfxHandle, 4>& buffer() const
	{
		return colorBuffers;
	}

	GfxHandle& positions()
	{
		return colorBuffers[0];
	}

	GfxHandle& normals()
	{
		return colorBuffers[1];
	}

	GfxHandle& colors()
	{
		return colorBuffers[2];
	}

	GfxHandle& depths()
	{
		return colorBuffers[3];
	}
};

struct DrawCall
{
	CommandType command;
};

struct InstancedData
{
	id::MeshId meshId = ~0u;
	id::ShaderId shaderId = ~0u;
	std::set<unsigned> groupEntities;
};

// 
enum RenderTargetType
{
	Backbuffer,
	PostProcessing,
	BlurBuffer1,
	BlurBuffer2,
	COUNT
};

struct RenderTarget
{
	GfxHandle target = 0;
	GfxHandle colorBuffer = 0;
	GfxHandle rbo = 0;
};

struct CBUFFER LightProperties
{
	glm::vec3 lightPosition = {};
	float intensity = {};
	glm::vec3 lightColor = {};
	float attenuation = {};
};

struct Lighting
{
	std::vector<LightProperties> _impl{};

	GfxBufferImpl toBufferData()
	{
		return GfxBufferImpl
		{
			.size = _impl.size() * sizeof( LightProperties ),
			.stride = sizeof( LightProperties ),
			.data = reinterpret_cast<uint8_t*>(_impl.data())
		};

	}
};

struct WindowSize
{
	glm::vec2 value{};

	bool hasChanged( glm::vec2 newSize )
	{
		assert( abs( newSize.x ) > FLT_EPSILON );
		assert( abs( newSize.y ) > FLT_EPSILON );

		return (abs( value.x - newSize.x ) > FLT_EPSILON ||
			abs( value.y - newSize.y ) > FLT_EPSILON);
	}

	GLuint getHeight() const { return static_cast<GLuint>(value.y); }

	GLuint getWidth() const { return static_cast<GLuint>(value.x); }

	glm::vec2 getInverted() const { return 1.0f / value; }
};

struct GfxCamera
{
	glm::mat4 viewMatrix = {};
	glm::vec3 viewPosition = {};
	glm::mat4 projectionMatrix = {};
	WindowSize windowSize = {};
};

struct PerSceneData
{
	GfxCamera camera;

	GfxBufferImpl toBufferData()
	{
		return GfxBufferImpl
		{
			.size = sizeof( GfxCamera ),
			.stride = sizeof( GfxCamera ),
			.data = memory::create<uint8_t*, GfxCamera>( camera )
		};
	}
};

struct PerBatchData
{
	struct Impl
	{
		glm::mat4 modelMatrix{};
	};

	std::vector<Impl> impl{};

	GfxBufferImpl toBufferData()
	{
		size_t allocSize = impl.size() * sizeof( Impl );
		return GfxBufferImpl
		{
			.size = allocSize,
			.stride = sizeof( Impl ),
			.data = memory::create<uint8_t*>( impl, allocSize )
		};
	}
};

struct DrawCallBatch
{
	std::vector<unsigned> entities{};
};


using VSConstBuffer = ConstBuffer;
using PSConstBuffer = ConstBuffer;

enum class Queue 
{
	Opaque,
	Skybox,
	Transparent
};

enum class BatchProperty : uint32_t 
{
	NONE = 0,
	Lit = (1 << 0),
	Diffuse = (1 << 1),
	Specular = (1 << 2),
	RenderEffects = (1 << 3),
};
FLAG_ENUM( BatchProperty );

struct GfxQueue;

struct GfxFrame
{
	id::MeshId fullScreenQuad{};
	id::MeshId skyboxMesh{};
	id::TextureId skyboxTexture{};

	GfxCamera camera{};
	RenderingSettings settings{};
	std::unordered_set<unsigned> destroyQueue{};
	LinearMap<unsigned, id::MeshId> meshIdLookup{};
	InstancedData instancedData{};

	LinearMap<unsigned, glm::mat4> modelMatrices{};
	LinearMap<unsigned, LightProperties> lightProperties{};
	LinearMap<unsigned, GfxDiffuseMaterial> diffuseMaterials{};
	LinearMap<unsigned, GfxSpecularMaterial> specularMaterials{};
	LinearMap<unsigned, GfxSprite> sprites{};
	LinearMap<unsigned, std::array<id::TextureId, 4>> textures{};
	LinearMap<unsigned, RenderEffect> effectLookup{};
	LinearMap<RenderEffect, std::vector<unsigned>> effectQueue{};

	std::unordered_map<Queue, GfxQueue> queues{};
	


	GfxDiffuseMaterial& getDiffuseMaterial( unsigned id )
	{
		return diffuseMaterials[id];
	}

	id::MeshId getMeshId( unsigned id )
	{
		return meshIdLookup[id];
	}

	bool isQueuedDestroyed( unsigned id )
	{
		return destroyQueue.contains( id );
	}

	const LightingSettings& getLightingSettings()
	{
		return settings.lightingSettings;
	}

	const FullScreenBlurSettings& getFsBlurSettings()
	{
		return settings.fullScreenBlurSettings;
	}

	const PixelatedSettings& getPixelatedSettings()
	{
		return settings.pixelatedSettings;
	}

	const ColorCorrectionSettings& getColorCorrectionSettings()
	{
		return settings.colorCorrectionSettings;
	}

	const ChromeAberrationSettings& getChromeAberrationSettings()
	{
		return settings.chromeAberrationSettings;
	}

	const PostProcessingEffects& getPostProcessingEffects()
	{
		return settings.fullScreenEffect;
	}

	const ClearSettings& getClearSettings()
	{
		return settings.clearSettings;
	}

	const glm::mat4& getViewMatrix()
	{
		return camera.viewMatrix;
	}

	glm::mat4 getViewMatrixStripTransform() 
	{
		return glm::mat4{ glm::mat3{ camera.viewMatrix } };
	}

	const glm::mat4& getProjectionMatrix()
	{
		return camera.projectionMatrix;
	}

	const WindowSize& getWindowSize()
	{
		return camera.windowSize;
	}

	const glm::vec3& getViewPosition()
	{
		return camera.viewPosition;
	}
};

struct GfxQueue
{
	LinearMap<unsigned, GfxShader> shaders{};
	LinearMap<unsigned, glm::vec4> meshColors{};
	LinearMap<id::ShaderId, DrawCallBatch, 5> batches{};
	std::vector<BatchProperty> properties{};

	GfxFrame* frame = nullptr;


	id::MeshId getMeshId( unsigned id ) const
	{
		return frame->getMeshId( id );
	}

	bool isQueuedDestroyed( unsigned id ) const
	{
		return frame->isQueuedDestroyed( id );
	}

	const LightingSettings& getLightingSettings() const
	{
		return frame->getLightingSettings();
	}

	const FullScreenBlurSettings& getFsBlurSettings() const
	{
		return frame->getFsBlurSettings();
	}

	const PixelatedSettings& getPixelatedSettings() const
	{
		return frame->getPixelatedSettings();
	}

	const ColorCorrectionSettings& getColorCorrectionSettings() const
	{
		return frame->getColorCorrectionSettings();
	}

	const ChromeAberrationSettings& getChromeAberrationSettings() const
	{
		return frame->getChromeAberrationSettings();
	}

	const PostProcessingEffects& getPostProcessingEffects() const
	{
		return frame->getPostProcessingEffects();
	}

	const ClearSettings& getClearSettings() const
	{
		return frame->getClearSettings();
	}

	const glm::mat4& getViewMatrix() const
	{
		return frame->getViewMatrix();
	}

	glm::mat4 getViewMatrixStripTransform() const 
	{
		return frame->getViewMatrixStripTransform();
	}

	const glm::mat4& getProjectionMatrix() const
	{
		return frame->getProjectionMatrix();
	}

	const WindowSize& getWindowSize() const
	{
		return frame->getWindowSize();
	}

	const glm::vec3& getViewPosition() const
	{
		return frame->getViewPosition();
	}
};

END_NAMESPACE1
#include "Precompiled/Precompiled.h"
#include "GfxDeviceOpenGL.h"

#include "Rendering/RenderFunctions.h"
#include "Rendering/RenderDefinitions.h"
#include "Rendering/Vertex.h"
#include "Rendering/Texture.h"

BEGIN_NAMESPACE1( rendering )

void APIENTRY messageCallback( GLenum source,
							   GLenum type,
							   GLuint id,
							   GLenum severity,
							   GLsizei length,
							   const GLchar* message,
							   const void* userParam )
{
	if (type != GL_DEBUG_TYPE_ERROR)
		return;

	std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = " << type << ", severity = " << severity
		<< ", message = " << message << std::endl;
}

static void setErrorCallback()
{
	glEnable( GL_DEBUG_OUTPUT );

	glDebugMessageCallback( messageCallback, 0 );

	glfwSetErrorCallback( []( int error_code, const char* description )
		{
			Logger::LogError( description );
		} );
}

void GfxDeviceOpenGL::bindShaderArgsImpl( id::ShaderId shaderId, ShaderProgram& shader, const GLShaderArgs& args )
{
	assert( shaderBound() );

	const auto& types = args.argTypes;
	const auto& values = args.argValues;
	const auto& metaData = args.metaData;
	const auto& bindingNames = _shaderStorage.getBindingNames( shaderId );

	size_t argsCount = args.count();

	for (size_t i = 0; i < argsCount; i++)
	{
		const auto& type = types[i];
		const auto& value = values[i];
		const auto& meta = metaData[i];
		const auto& bindingName = bindingNames[i];

		switch (type)
		{
		case ArgType::Int32:
			shader.setInt( bindingName, value.intValue );
			break;
		case ArgType::Float:
			shader.setFloat( bindingName, value.floatValue );
			break;
		case ArgType::Vec2:
			shader.setVec2( bindingName, value.vec2Value );
			break;
		case ArgType::Vec3:
			shader.setVec3( bindingName, value.vec3Value );
			break;
		case ArgType::Mat4:
			shader.setMatrix( bindingName, value.mat4Value );
			break;
		case ArgType::Texture2D:
			bindTexture( value.textureId, meta.bindPosition );
			shader.setInt( bindingName, value.textureId );
			break;
		case ArgType::ConstBuffer:
			shader.setConstBuffer( meta.bindPosition, value.constBuffer, meta.size, meta.bufferUsage );
			break;
		default:
			assert( false );
			break;
		}
	}
}

GfxDeviceOpenGL::GfxDeviceOpenGL( const GfxDeviceArgs& args )
{
	if (!gladLoadGL())
	{
		printf( "Failed to initialize OpenGL context" );
		exit( -1 );
	}

	if (GLAD_GL_VERSION_3_0)
	{
		printf( "OpenGL %d.%d\n", GLVersion.major, GLVersion.minor );
	}

	printf("OpenGL Version: %s\n",  glGetString( GL_VERSION ) );
	printf("GPU Vendor: %s\n", glGetString( GL_VENDOR ) );
	printf("Renderer: %s\n",  glGetString( GL_RENDERER ) );
	printf("GLSL Version: %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	setErrorCallback();
	//printf( "Supported Extensions: " << glGetString( GL_EXTENSIONS ) );

	GLint numFormats = 0;
	glGetIntegerv( GL_NUM_SHADER_BINARY_FORMATS, &numFormats );
	if (numFormats > 0) {
		std::vector<GLint> formats( numFormats );
		glGetIntegerv( GL_SHADER_BINARY_FORMATS, formats.data() );
		bool supported = std::find( formats.begin(), formats.end(), GL_SHADER_BINARY_FORMAT_SPIR_V_ARB ) != formats.end();
		if (!supported) {
			std::cerr << "Error: GL_SHADER_BINARY_FORMAT_SPIR_V_ARB is not supported." << std::endl;
		}
	}
	else {
		std::cerr << "Error: No shader binary formats are supported." << std::endl;
	}

	if (!GLAD_GL_ARB_gl_spirv) {
		std::cerr << "GL_ARB_gl_spirv not supported by your GPU/driver." << std::endl;
	}

	// create render targets
	{
		for (int i = RenderTargetType::PostProcessing; i < RenderTargetType::COUNT; i++)
		{
			WindowSize windowSize = { args.windowSize };
			RenderTargetType fb = static_cast<RenderTargetType>(i);
			unsigned int widht = windowSize.getWidth();
			unsigned int height = windowSize.getHeight();

			_renderTargetLookup[fb] = createRenderTarget( widht, height );
		}

		glEnable( GL_MULTISAMPLE );

		_renderTargetLookup[RenderTargetType::Backbuffer] =
		{
			.target = 0,
			.colorBuffer = 0,
			.rbo = 0
		};
	}
	
	// create shaders
	{
		for (int i = 0; i < args.shaderIds.size(); i++)
		{
			_shadersLookup[args.shaderIds[i]]
				= ShaderProgram( args.vertexShaderFiles[i], args.fragmentShaderFiles[i] );
		}
	}
		
	// create textures
	{
		for (int i = 0; i < args.textureIds.size(); i++)
		{
			_textureLookup[args.textureIds[i]] = createTexture( args.textureIds[i], ~0u, Dimensions::T2D, *args.textureData[i] );
		}
	}

	// create cubemaps
	{
		for (int i = 0; i < args.textureDescriptorIds.size(); i++)
		{
			_textureLookup[args.textureDescriptorIds[i]] = createTexture( args.textureDescriptorIds[i], ~0u, args.textureDescriptors[i] );
		}
	}

	// create vertex buffers
	{
		for (int i = 0; i < args.meshIds.size(); i++)
		{
			_vertexBufferLookup[args.meshIds[i]] = createVertexBuffer( args.meshIds[i], *args.vertexData[i] );
		}
	}

}

GfxTexture GfxDeviceOpenGL::createTexture( id::TextureId textureId, unsigned bindPosition, Dimensions dimensions, const TextureData& textureData )
{
	assert( textureId != id::InvalidTextureId );
	assert( textureData.width > 0 );
	assert( textureData.height > 0 );

	unsigned buffer = 0;
	
	glGenTextures( 1, &buffer );
	glBindTexture( GL_TEXTURE_2D, buffer );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// texture channels vary between 3 and 4.
	// TODO - if this becomes a performance issue come up wiht more selective implementations
	// i.e. GL_RGB for 3 channels and GL_RGBA for 4.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, textureData.width, textureData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data );
	glGenerateMipmap( GL_TEXTURE_2D );

	GfxTexture texture{};
	texture.unit = bindPosition;
	texture.dimensions = dimensions;
	texture.buffer = buffer;

	_textureLookup[textureId] = texture;
	return texture;
}

GfxTexture GfxDeviceOpenGL::createTexture( id::TextureId textureId, unsigned bindPosition, const TextureDescriptor& descriptor )
{
	assert( textureId != id::InvalidTextureId );
	
	unsigned buffer = 0;

	glGenTextures( 1, &buffer );
	glBindTexture( GL_TEXTURE_CUBE_MAP, buffer );
	for (int i = 0; i < descriptor.textureData.size(); i++)
	{
		auto& td = descriptor.textureData[i];
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, td->width, td->height, 0, GL_RGB, GL_UNSIGNED_BYTE, td->data );
	}
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	GfxTexture texture{};
	texture.unit = bindPosition;
	texture.dimensions = Dimensions::TCube;
	texture.buffer = buffer;

	_textureLookup[textureId] = texture;
	return texture;
}

VertexBuffer GfxDeviceOpenGL::createVertexBuffer( id::MeshId meshId, const rendering::VertexData& vertexData )
{
	auto vertexCount = vertexData.vertices.size();
	auto indexCount = vertexData.indices.size();
	auto* vertices = vertexData.vertices.data();
	auto* indices = vertexData.indices.data();

	VertexBuffer v{};

	glGenBuffers( 1, &v.vertexObjectBuffer );
	glGenBuffers( 1, &v.elementArrayObject );

	glGenVertexArrays( 1, &v.vertexArrayBuffer );

	glBindVertexArray( v.vertexArrayBuffer );

	glBindBuffer( GL_ARRAY_BUFFER, v.vertexObjectBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * vertexCount, (void*)vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, v.elementArrayObject );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * indexCount, (void*)indices, GL_STATIC_DRAW );
	v.indexCount = (int)indexCount;

	// position
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)0 );

	// normal
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)(3 * sizeof( float )) );

	// uv
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)(6 * sizeof( float )) );

	glBindVertexArray( 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	_vertexBufferLookup[meshId] = v;
	return v;
}

RenderTarget GfxDeviceOpenGL::createRenderTarget( unsigned int width, unsigned int height )
{
	RenderTarget buffer{};

	// constructor
	glGenFramebuffersEXT( 1, &buffer.target );

	glBindFramebuffer( GL_FRAMEBUFFER, buffer.target );

	// color buffer;
	glGenTextures( 1, &buffer.colorBuffer );

	glBindTexture( GL_TEXTURE_2D, buffer.colorBuffer );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glBindTexture( GL_TEXTURE_2D, 0 );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer.colorBuffer, 0 );

	// depth - stencil buffer
	glGenRenderbuffers( 1, &buffer.rbo );

	glBindRenderbuffer( GL_RENDERBUFFER, buffer.rbo );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.rbo );

	if (glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR( "Framebuffer not complete." );
		auto err = glGetError();
		if (err != GL_NO_ERROR)
			printf( "\nOpenGL error - %d", err );
	}
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return buffer;
}

GfxHandle GfxDeviceOpenGL::allocateConstantBuffer( int size, int usage, int bindPosition )
{
	static int bufferEnumMap[2]{ GL_UNIFORM_BUFFER, GL_SHADER_STORAGE_BUFFER };
	int usageEnum = bufferEnumMap[usage];

	GLuint handle;
	glGenBuffers( 1, &handle );
	glBindBuffer( usageEnum, handle );
	glBufferData( usageEnum, size, NULL, GL_DYNAMIC_DRAW );
	glBindBuffer( usageEnum, 0 );

	glBindBufferRange( usageEnum, bindPosition, handle, 0, size );

	_bufferUsage[handle] = usageEnum;
	return handle;
}

void GfxDeviceOpenGL::deleteRenderTarget( RenderTarget& frameBuffer )
{
	glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer.target );

	// color 
	glBindTexture( GL_TEXTURE_2D, 0 );
	glDeleteTextures( 1, &frameBuffer.colorBuffer );

	// depth - stencil buffer
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	glDeleteRenderbuffers( 1, &frameBuffer.rbo );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	glDeleteFramebuffers( 1, &frameBuffer.target );
}

void GfxDeviceOpenGL::recreateRenderTarget( RenderTargetType type, unsigned int width, unsigned int height )
{
	assert( _renderTargetLookup.has( type ) );
	RenderTarget& renderTarget = _renderTargetLookup[type];
	recreateRenderTarget( renderTarget, width, height );
}


void GfxDeviceOpenGL::recreateRenderTarget( RenderTarget& buffer, unsigned int width, unsigned int height )
{
	deleteRenderTarget( buffer );
	buffer = createRenderTarget( width, height );
}

void GfxDeviceOpenGL::deleteVextexBuffer( id::MeshId meshId )
{
	assert( _vertexBufferLookup.has( meshId ) );
	const VertexBuffer& buffer = _vertexBufferLookup.at( meshId );
	deleteVexterBuffer( buffer );
}

void GfxDeviceOpenGL::deleteVexterBuffer( const VertexBuffer& vertexBuffer )
{
	glDeleteBuffers( 1, &vertexBuffer.elementArrayObject );
	glDeleteBuffers( 1, &vertexBuffer.vertexArrayBuffer );
	glDeleteBuffers( 1, &vertexBuffer.vertexObjectBuffer );
}

ShaderProgram& GfxDeviceOpenGL::bindShader( id::ShaderId shaderId )
{
	const bool hasShader = _shadersLookup.has( shaderId );
	if (!hasShader)
	{
		Logger::LogError( "Attempting to bind shader that can't be found." );
		assert( hasShader );
	}

	ShaderProgram& shader = _shadersLookup[shaderId];
	shader.bind();

	_activeShader = shaderId;

	return shader;
}

ShaderProgram& GfxDeviceOpenGL::bindShader( id::ShaderId shaderId, const ShaderArgs& args )
{
	ShaderProgram& shader = bindShader( shaderId );
	
	const GLShaderArgs& apiArgs = args.get<GLShaderArgs>();
	apiArgs.validate();

	bindShaderArgsImpl( shaderId, shader, apiArgs );

	return shader;
}

GfxTexture& GfxDeviceOpenGL::bindTexture( id::TextureId textureId, unsigned bindPosition )
{
	assert( shaderBound() );

	GfxTexture& texture = _textureLookup[textureId];

	unsigned& bindPos = texture.unit;
	if (bindPosition != ~0u)
	{
		bindPos = bindPosition;
	}

	if (!texture.IsSet())
	{
		Logger::LogError( "Attempting to bind texture that can't be found." );
		assert( texture.IsSet() );
	}

	static std::array<int, 4> dimensionEnumTable = { GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP };
	int textureEnum = dimensionEnumTable[static_cast<size_t>(texture.dimensions)];

	glActiveTexture( GL_TEXTURE0 + bindPos );
	glBindTexture( textureEnum, texture.buffer );

	return texture;
}

RenderTarget& GfxDeviceOpenGL::bindRenderTarget( RenderTargetType type )
{
	assert( _renderTargetLookup.has( type ) );

	RenderTarget& renderTarget = _renderTargetLookup[type];
	glBindFramebuffer( GL_FRAMEBUFFER, renderTarget.target );

	return renderTarget;
}

void GfxDeviceOpenGL::bindRenderTargetResource( RenderTargetType type, unsigned bindPosition )
{
	assert( _renderTargetLookup.has( type ) );
	const RenderTarget& renderTarget = _renderTargetLookup.at( type );

	glActiveTexture( GL_TEXTURE0 + bindPosition );
	glBindTexture( GL_TEXTURE_2D, renderTarget.colorBuffer );
}

void GfxDeviceOpenGL::updateConstantBuffer( GfxHandle buffer, void* data, int size, int offset )
{
	int usageEnum = _bufferUsage[buffer];
	glBindBuffer( usageEnum, buffer );
	glBufferSubData( usageEnum, offset, size, data );
	glBindBuffer( usageEnum, 0 );
}

void GfxDeviceOpenGL::dispatchIndexedDirect( id::MeshId meshId )
{
	assert( _vertexBufferLookup.has( meshId ) );
	const VertexBuffer& buffer = _vertexBufferLookup.at( meshId );

	glBindVertexArray( buffer.vertexArrayBuffer );
	glDrawElements( GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}

void GfxDeviceOpenGL::dispatchIndexedInstancedDirect( id::MeshId meshId, unsigned instanceCount )
{
	assert( _vertexBufferLookup.has( meshId ) );
	const VertexBuffer& buffer = _vertexBufferLookup.at( meshId );

	glBindVertexArray( buffer.vertexArrayBuffer );
	glDrawElementsInstanced( GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_INT, 0, (GLsizei)instanceCount );
	glBindVertexArray( 0 );
}

void GfxDeviceOpenGL::setViewport( int posX, int posY, int width, int height )
{
	glViewport( posX, posY, width, height );
}

void GfxDeviceOpenGL::clear()
{
}

END_NAMESPACE1

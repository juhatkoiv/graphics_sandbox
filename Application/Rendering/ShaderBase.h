#pragma once

#include <string>

BEGIN_NAMESPACE1(rendering)

class ShaderBase
{
public:
	virtual ~ShaderBase();

	unsigned GetHandle() const;

protected:
	explicit ShaderBase(const std::string& source, unsigned int shaderType);

	unsigned int _handle = ~0u;
};

class VertexShader : public ShaderBase
{
public:
	explicit VertexShader(const std::string& source) : ShaderBase(source, GL_VERTEX_SHADER) {}
};

class FragmenShader : public ShaderBase
{
public:
	explicit FragmenShader(const std::string& source) : ShaderBase(source, GL_FRAGMENT_SHADER) {}
};

END_NAMESPACE1
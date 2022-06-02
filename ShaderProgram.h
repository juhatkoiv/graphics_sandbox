#pragma once

#include <gl/glew.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);
	~ShaderProgram() = default;

	void Bind();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;

private:
	unsigned _handle;
};
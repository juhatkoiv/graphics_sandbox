#pragma once

#include <unordered_map>
#include <memory>

#include "IRenderer.h"

#include "ShaderProgram.h"

class RendererOpenGL final : public IRenderer
{
public:
	RendererOpenGL();
	~RendererOpenGL() override = default;

	virtual void Render() override;

private:
	std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> _shadersImpl;
};
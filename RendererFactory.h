#pragma once

class IRenderer;

class RendererFactory 
{
public:
	static IRenderer *CreateRenderer();
};
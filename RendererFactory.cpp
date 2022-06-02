#include "Precompiled.h"
#include "RendererFactory.h"

#include "RendererOpenGL.h"

IRenderer *RendererFactory::CreateRenderer()
{
    return new RendererOpenGL();
}

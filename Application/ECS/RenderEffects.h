#pragma once

#include "Precompiled/PrecompilationMacros.h"

#include "Rendering/RenderEffect.h"

BEGIN_NAMESPACE1(ecs);

struct RenderEffects : public Component<cid::RENDER_EFFECTS>
{
	rendering::RenderEffect effects{};

};

END_NAMESPACE1;
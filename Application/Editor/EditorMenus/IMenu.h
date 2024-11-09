#pragma once

#include <memory>

#include "Precompiled/PrecompilationMacros.h"

DECLARE1(struct, scene, SceneData);

BEGIN_NAMESPACE1(editor)

class IMenu
{
public:
	virtual ~IMenu() {}
	virtual void Draw(scene::SceneData& scene) {};

};

END_NAMESPACE1

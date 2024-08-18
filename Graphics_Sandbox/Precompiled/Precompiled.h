#include <iostream>
#include <cassert>
#include <unordered_map>
#include <concepts>

#include <GL/glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ECS/UniqueId.h"
#include "Input/InputState.h"
#include "PrecompilationMacros.h"
#include "Precompiled/PointerHelpers.h"
#include "Precompiled/Logger.h"
#include "Precompiled/MaterialIds.h"
#include "Precompiled/AppState.h"
#include "Precompiled/EnumClassFlagsDefines.h"
#include "Profiling/ProfilingDefinitions.h"
#include "Rendering/RenderingPrecompilationMacros.h"
#include "Result.h"

namespace global
{
	static constexpr unsigned int ROOT_ID = 0;
	static constexpr float FRAME_TIME_SECONDS = 1.0f / 60.0f;
	const glm::vec3 WORLD_UP = { 0, 1, 0 };
	const glm::vec3 X_AXIS = { 1, 0, 0 };
	const glm::vec3 Y_AXIS = { 0, 1, 0 };
	const glm::vec3 Z_AXIS = { 0, 0, 1 };
}

namespace rendering
{
	static constexpr int DIFFUSE_UNIT = 0;
	static constexpr int SPECULAR_UNIT = 1;
	static constexpr int SPRITE_UINT = 0;
}


#define NO_DISCARD [[nodiscard]]
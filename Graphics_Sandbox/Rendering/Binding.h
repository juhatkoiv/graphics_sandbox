#pragma once

#include "Precompiled/PrecompilationMacros.h"

BEGIN_NAMESPACE2( rendering, binding )

static constexpr int CUBEMAP_TEXTURE = 0;
static constexpr int DIFFUSE_TEXTURE = 0;
static constexpr int SPECULAR_TEXTURE = 1;
static constexpr int CAMERA_BUFFER = 0;
static constexpr int LIGHT_BUFFER = 3;
static constexpr int POST_PROCESSING_ARGS = 4;
static constexpr int LIGHTNING_SETTINGS = 5;
static constexpr int MATERIAL_BUFFER = 4;
static constexpr int MODEL_MATRIX_BUFFER = 20;
static constexpr int PUSH_CONSTANT = 50;

END_NAMESPACE2

BEGIN_NAMESPACE2( rendering, buffer_usage )

static constexpr int UBO = 0;
static constexpr int SSBO = 1;

END_NAMESPACE2
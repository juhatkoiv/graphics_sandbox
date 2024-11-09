#pragma once

#include "Precompiled/EnumClassFlagsDefines.h"

enum class ApplicationMode
{
	NONE = 0,
	RUN_APPLICATION = (1 << 0),
	GENERATE_SPIRV = (1 << 1),
};
FLAG_ENUM( ApplicationMode );

#pragma once

#include <glm/glm.hpp>

BEGIN_NAMESPACE1(editor)

struct EditorTransform
{
	glm::vec3 position = {};
	glm::vec3 scale = {};
	glm::vec3 angleScalars = {};
};

END_NAMESPACE1
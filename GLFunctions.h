#pragma once

#include <GL/glew.h>

namespace gl
{
	bool validateLink(unsigned handle);
	bool validateCompile(unsigned handle);
	bool validate(unsigned handle, GLenum type);
}
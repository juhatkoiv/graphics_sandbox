#pragma once

namespace gl
{
	bool validateLink(unsigned handle);
	bool validateCompile(unsigned handle);
	bool validate(unsigned handle, GLenum type);
}
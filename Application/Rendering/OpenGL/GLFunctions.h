#pragma once

#include <string>

namespace gl {
	struct GLStatus {
		std::string errorMsg{};
		GLenum error = 0;

		bool ok() const {
			return errorMsg.empty();
		}
	};

	std::string getShaderErrorString(unsigned handle);
	bool validateLink(unsigned handle);
	bool validateCompile(unsigned handle);
	bool validate(unsigned handle, GLenum type);
	GLStatus checkStatus( unsigned handle );
}
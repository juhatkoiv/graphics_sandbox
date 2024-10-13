#pragma once

#include <string>
#include <vector>

struct CompilationResult {
	std::vector<std::string> errors;
	std::vector<uint32_t> spirv;

	bool success() const 
	{
		return errors.empty();
	}

	std::string getErrors() const
	{
		std::string result;
		for (const auto& error : errors)
		{
			result += error + "\n";
		}
		return result;
	}
};

namespace shader_compilation {
	CompilationResult load_as_spv( const std::string& path );
	CompilationResult generate_spirv();
}
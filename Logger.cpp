#include "Precompiled.h"
#include "Logger.h"

#include <string>
#include <sstream>

namespace
{
	static constexpr const char *ERROR = " !! [ERROR] - ";
	static constexpr const char *INFO = " [INFO] - ";

	void LogImpl(const char *prefix, const char *msg)
	{
		std::cout << prefix << msg << std::endl;
	}
}

void Logger::LogInfo(const char* msg)
{
	LogImpl(INFO, msg);
}

void Logger::LogError(const char* msg)
{
	LogImpl(ERROR, msg);
}

void Logger::LogInfoGL(const char* msg, const GLubyte* bytes)
{
	std::stringstream ss;
	ss << msg << reinterpret_cast<const char*>(bytes);

	LogInfo(ss.str().c_str());
}

void Logger::LogErrorGL(const char* msg, const GLubyte* bytes)
{
	std::stringstream ss;
	ss << msg << reinterpret_cast<const char*>(bytes);

	LogError(ss.str().c_str());
}

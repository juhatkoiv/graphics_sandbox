#include "Precompiled/Precompiled.h"
#include "Logger.h"

#include <string>
#include <sstream>
#include <stdarg.h>

namespace
{
	static constexpr const char *MSG_ERROR = " !! [ERROR] - ";
	static constexpr const char *MSG_INFO = " [INFO] - ";

	void LogImpl(const char *prefix, const char *msg)
	{
		std::cout << prefix << msg << std::endl;
	}	
}

void Logger::LogInfo(const char* msg)
{
	LogImpl(MSG_INFO, msg);
}

void Logger::LogError(const char* msg)
{
	LogImpl(MSG_ERROR, msg);
}

void Logger::LogError(const std::string& msg)
{
	LogImpl(MSG_ERROR, msg.c_str());
}

void Logger::LogError(std::string&& msg)
{
	LogImpl(MSG_ERROR, msg.c_str());
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

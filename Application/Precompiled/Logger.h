#pragma once

#include <string>

class Logger 
{
public:
	static void LogInfo(const char* msg);
	static void LogError(const char* msg);
	static void LogError(const std::string& msg);
	static void LogError(std::string&& msg);
	static void LogInfoGL(const char* msg, const GLubyte* bytes);
	static void LogErrorGL(const char* msg, const GLubyte* bytes);
};

#define TO_STR(x) STRINGIFY(x)

#define STRINGIFY(x) #x

#define LOG_INFO(msg) Logger::LogInfo(msg);

#define LOG_INFO_GL(msg, bytes) Logger::LogInfoGL(msg, bytes);

#define LOG_ERROR(msg) Logger::LogError(msg __FILE__ TO_STR(__LINE__));

#define LOG_ERROR_GL(bytes) Logger::LogErrorGL("", bytes);
#pragma once

#include "ApplicationMode.h"

class CommandLineArgsParser
{
private:
	int m_argc = 0;
	char** m_argv = nullptr;

public:
	CommandLineArgsParser( int argc, char** argv );
	
	ApplicationMode parse() const;
};
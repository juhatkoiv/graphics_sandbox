#include "Precompiled/Precompiled.h"
#include "CommanLineArgsParser.h"

CommandLineArgsParser::CommandLineArgsParser( int argc, char** argv ) {
	m_argc = argc;
	m_argv = argv;
}

ApplicationMode CommandLineArgsParser::parse() const {
	ApplicationMode mode = ApplicationMode::NONE;
	
	if (m_argc < 1) {
		mode |= ApplicationMode::RUN_APPLICATION;
	}
	else {
		for (int i = 1; i < m_argc; ++i) {
			if (strcmp( m_argv[i], "generate_spirv" ) == 0) {
				mode |= ApplicationMode::GENERATE_SPIRV;
			}
			else if (strcmp( m_argv[i], "run_application" ) == 0) {
				mode |= ApplicationMode::RUN_APPLICATION;
			}
		}
	}
	return mode;
}

#pragma once

class MainLoop
{
public:
	struct Args
	{
		const char* title = nullptr;
		int windowWidth = 0;
		int windowHeight = 0;
	};

	MainLoop( Args args );
	~MainLoop();

	void run();
};
#pragma once

#include "IRenderer.h"
#include "Window.h"

class Application
{
public:
    struct Args
    {
        int windowWidth = 0;
        int windowHeight = 0;
        const char* title = nullptr;
    };

    Application(Args args);

    void Run();

private:
    void Input();
    void Update(float dt);
    void Render();
    void Cleanup();

    bool IsRunning() const;

    std::unique_ptr<Window> _window;
    std::unique_ptr<IRenderer> _renderer;
};

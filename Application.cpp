#include "Precompiled.h"
#include "Application.h"

#include "RendererFactory.h"

#include <chrono>
namespace
{
    using namespace std::chrono;
    using Clock = high_resolution_clock;
    using Time = high_resolution_clock::time_point;
    using Duration = duration<float, std::chrono::seconds::period>;

    static constexpr float FRAME_TIME_SECONDS = 1.0f / 60.0f;
}

Application::Application(Args args)
{
    if (!glfwInit()) 
    {
        LOG_ERROR("Glfw init failed! Aborting...");
        exit(EXIT_FAILURE);
    }

    _window.reset(new Window(args.windowWidth, args.windowHeight, args.title));
    if (!_window->IsValid())
    {
        LOG_ERROR("Glfw window initalization failed! Aborting...");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback([](int error_code, const char* description)
        {
            Logger::LogError(description);
        });

    _renderer.reset(RendererFactory::CreateRenderer());
}

void Application::Run()
{
    float timeSinceLastUpdate = 0.0f;
    Time previousUpdateTime;
    do
    {
        Input();
        
        Time currentTime = Clock::now();
        float elapsed = Duration(currentTime - previousUpdateTime).count();

        timeSinceLastUpdate += elapsed;
        if (timeSinceLastUpdate > FRAME_TIME_SECONDS)
        {
            Update(FRAME_TIME_SECONDS);

            timeSinceLastUpdate -= FRAME_TIME_SECONDS;
            previousUpdateTime = Clock::now();
        }

        Render();

    } while (IsRunning());

    Cleanup();
}

void Application::Input()
{
    glfwPollEvents();
}

void Application::Update(float dt)
{

}

void Application::Render()
{
    _renderer->Render();
    _window->Present();
}

bool Application::IsRunning() const
{
    return !_window->ShouldClose();
}

void Application::Cleanup()
{
    _window->Close();
    glfwTerminate();
}

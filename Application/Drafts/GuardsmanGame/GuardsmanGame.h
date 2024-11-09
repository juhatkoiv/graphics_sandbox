#pragma once

#include "Application.h"
#include "GameInputSystem.h"

DECLARE1(class, window, Window);

BEGIN_NAMESPACE1(guardsman)

class GuardsmanGame : public Application
{
public:
    struct Args
    {
        SystemContainer* context = nullptr;
        const char* title = nullptr;
    };

    GuardsmanGame(Args& args, window::Window &window);
    ~GuardsmanGame();

    void Update(float deltaTime) override;
    void Render(float deltaTime) {}
    void PostUpdate(float deltaTime) override;
    void Cleanup() {}

private:
    input::GameInputSystem _inputSystem;
};

END_NAMESPACE1
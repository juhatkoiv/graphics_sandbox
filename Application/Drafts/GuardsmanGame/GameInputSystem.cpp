#include "Precompiled.h"
#include "GameInputSystem.h"

#include "Camera/Camera.h"
#include "World/Ray.h"
#include "Window/Window.h"

BEGIN_NAMESPACE1(input)

GameInputSystem::GameInputSystem(window::Window& window)
    : _window(window), _inputOperations()
{
}

void GameInputSystem::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos;
        double ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        _inputOperations.push_back(ScreenInputOperation{ glm::vec2{ xpos, ypos } });
    }
}

void GameInputSystem::FlushInputOperations(std::vector<ScreenInputOperation>& operations)
{
    operations.swap(_inputOperations);
}

world::Ray GameInputSystem::GetMouseWorldRay(const CameraPosition& viewPos, const ViewMatrix& viewMatrix, 
    const ProjectionMatrix& projMatrix, const ScreenInputOperation& operation)
{
    world::Ray::Args args;
    args.projMatrix = projMatrix;
    args.viewMatrix = viewMatrix;
    args.viewPosition = viewPos;
    args.windowSize = _window.GetSize();
    args.screenCoord = operation.inputCoord;

    return world::Ray::FindMouseToWorldRay(args);
}

END_NAMESPACE1
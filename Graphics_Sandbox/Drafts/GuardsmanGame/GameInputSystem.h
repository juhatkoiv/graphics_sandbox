#pragma once

#include "World/Ray.h"

DECLARE1(class, editor, EditorCamera);
DECLARE1(class, window, Window);

BEGIN_NAMESPACE1(input)

using CameraPosition = glm::vec3;
using ViewMatrix = glm::mat4;
using ProjectionMatrix = glm::mat4;

struct ScreenInputOperation
{
    glm::vec2 inputCoord {};
};

class GameInputSystem
{
public:
    GameInputSystem(window::Window& window);

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void FlushInputOperations(std::vector<ScreenInputOperation>& operations);

    world::Ray GetMouseWorldRay(const CameraPosition &viewPos, const ViewMatrix &viewMatrix, 
        const ProjectionMatrix& projMatrix, const ScreenInputOperation& operation);
private:

    std::vector<ScreenInputOperation> _inputOperations;
    window::Window& _window;
};

END_NAMESPACE1
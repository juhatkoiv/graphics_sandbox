#include "Precompiled/Precompiled.h"
#include "InputSystem.h"

#include <vector>

#include "Window/Window.h"
#include "World/Ray.h"
#include "ECS/Camera.h"
#include "Input/InputStateMutable.h"

BEGIN_NAMESPACE1(ecs)
/*

void Window::SetCursorVisibleIfPossible(bool visible)
{
    static int lastCursorMode = GLFW_CURSOR_NORMAL;
    auto value = visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
    if (value != lastCursorMode)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, value);
        lastCursorMode = value;
    }
}*/
namespace
{
    struct MouseMove
    {
        glm::vec2 mousePosition{};
    };

    std::vector<input::InputEvent> _inputQueue{};
    std::vector<MouseMove> _mouseMoveQueue{};
    GLFWwindow* windowImpl = nullptr;
    /*
    glm::vec2 GetWindowCenter() 
    {
        int width = 0;
        int height = 0;
        glfwGetWindowSize(windowImpl, &width, &height);

        return glm::vec2{ static_cast<float>(width), static_cast<float>(height) } * 0.5f;
    }
    */
    static std::unordered_map<int, input::InputEvent>& GetInputMap()
    {
        static std::unordered_map<int, input::InputEvent> inputMap;
        if (inputMap.empty()) 
        {
            inputMap[GLFW_KEY_SPACE] = input::InputEvent::KEY_SPACE;
            inputMap[GLFW_KEY_ESCAPE] = input::InputEvent::KEY_ESC;
            inputMap[GLFW_KEY_TAB] = input::InputEvent::KEY_TAB;
            inputMap[GLFW_KEY_F5] = input::InputEvent::KEY_F5;
            inputMap[GLFW_KEY_W] = input::InputEvent::MOVE_FORWARD;
            inputMap[GLFW_KEY_S] = input::InputEvent::MOVE_BACKWARDS;
            inputMap[GLFW_KEY_A] = input::InputEvent::STRAFE_LEFT;
            inputMap[GLFW_KEY_D] = input::InputEvent::STRAFE_RIGHT;
            inputMap[GLFW_KEY_Q] = input::InputEvent::MOVE_UP;
            inputMap[GLFW_KEY_E] = input::InputEvent::MOVE_DOWN;
            inputMap[GLFW_KEY_F9] = input::InputEvent::KEY_F9;
            inputMap[GLFW_KEY_F10] = input::InputEvent::KEY_F10;
            inputMap[GLFW_KEY_F11] = input::InputEvent::KEY_F11;
        }
        return inputMap;
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action != GLFW_PRESS && action != GLFW_REPEAT)
            return;
     
        auto& inputMap = GetInputMap();
        if (inputMap.contains(key)) 
        {
            _inputQueue.push_back(inputMap[key]);
        }
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            _inputQueue.push_back(input::InputEvent::MOUSE_BUTTON_RIGHT);
        
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            _inputQueue.push_back(input::InputEvent::MOUSE_BUTTON_LEFT);
    }

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
    {
        _mouseMoveQueue.push_back(
            MouseMove
            {
                .mousePosition = glm::vec2{ xpos, ypos }
            }
        );
    }

    void UpdateKeyInput() 
    {
        for (const auto& buttonInput : _inputQueue)
        {
            input::InputEvent event = buttonInput;

            if (event == input::InputEvent::KEY_ESC)
                app::ToggleState(app::AppState::Quits);

            if (event == input::InputEvent::KEY_F9)
                app::ToggleState(app::AppState::ToggleSceneCamera);
            
            if (event == input::InputEvent::KEY_F10)
                app::ToggleState(app::AppState::InstructionsVisible);
            
            if (event == input::InputEvent::KEY_F11)
                app::ToggleState(app::AppState::FpsVisible);
            
            if (event == input::InputEvent::KEY_TAB)
                app::ToggleState(app::AppState::ToggleEditorGui);
            
            if (event == input::InputEvent::KEY_SPACE)
            {
                input::getInputStateMutable().cursorVisibleChanged = true;
                app::ToggleState(app::AppState::CursorVisible);
            }
            input::keyInputFound(event);
        }

        _inputQueue.clear();

    }

    void UpdateMouseMove(const MouseMove& mouseMove)
    {
        input::InputState& inputState = input::getInputStateMutable();

        auto mousePosition = mouseMove.mousePosition;
        if (inputState.cursorVisibleChanged)
        {
            inputState.lastMousePos = mousePosition;
            inputState.cursorVisibleChanged = false;
            return;
        }

        inputState.lastMouseOffset.x = mousePosition.x - inputState.lastMousePos.x;
        inputState.lastMouseOffset.y = inputState.lastMousePos.y - mousePosition.y;

        inputState.lastMousePos = mousePosition;

    }

    void UpdateMouse() 
    {
        for (const auto& mouseInput : _mouseMoveQueue)
        {
            UpdateMouseMove(mouseInput);
        }
        _mouseMoveQueue.clear();
    }

    void toggleCursorVisibility() 
    {
        if (app::hasState(app::AppState::CursorVisible))
            glfwSetInputMode(windowImpl, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(windowImpl, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void UpdateCursorVisibility() 
    {
        const input::InputState& inputState = input::getInputState();
        if (!inputState.cursorVisibleChanged)
            return;

        toggleCursorVisibility();
    }

    void setInitialCursorState() 
    {
        app::ToggleState(app::AppState::CursorVisible);
        toggleCursorVisibility();
    }
}

InputSystem::InputSystem(window::Window& window)
{    
    windowImpl = window.getWindowImpl();
    glfwSetInputMode(windowImpl, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetKeyCallback(windowImpl, key_callback);
    glfwSetMouseButtonCallback(windowImpl, mouse_button_callback);
    glfwSetCursorPosCallback(windowImpl, cursor_position_callback);

    setInitialCursorState();
}

void InputSystem::update(ecs::World& world, float deltaTime)
{
    input::clearAllKeys();
    input::clearMouseOffset();

    glfwPollEvents();

    UpdateKeyInput();
    UpdateCursorVisibility();
    UpdateMouse();

}

END_NAMESPACE1
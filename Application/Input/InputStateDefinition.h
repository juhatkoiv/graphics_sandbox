#pragma once

#include "World/Ray.h"
#include "Precompiled/PrecompilationMacros.h"

#include <unordered_set>
#include <queue>

BEGIN_NAMESPACE1(input)

enum class InputEvent
{
    KEY_ESC,
    KEY_SPACE,
    KEY_TAB,
    MOVE_FORWARD,
    MOVE_BACKWARDS,
    STRAFE_LEFT,
    STRAFE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    KEY_F5,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT
};

struct InputState
{
    std::unordered_set<InputEvent> keyInputs{};
    std::queue<world::Ray> rays{};
    glm::vec2 lastMousePos{};
    glm::vec2 lastMouseOffset{};
    glm::vec2 inputCursorPos{};
    bool firstMouse = true;
    bool cursorVisibleChanged = false;
};


END_NAMESPACE1

#include "Precompiled/Precompiled.h"

#include "InputStateMutable.h"

BEGIN_NAMESPACE1(input)

const InputState& getInputState()
{
    return getInputStateMutable();
}

glm::vec2 getLastMouseOffset()
{
    return getInputState().lastMouseOffset;
}

bool consumeKey(InputEvent inputEvent)
{
    InputState& state = getInputStateMutable();
    if (state.keyInputs.count(inputEvent))
    {
        state.keyInputs.erase(inputEvent);
        return true;
    }
    return false;
}

glm::vec2 getLastMousePosition()
{
    return getInputState().lastMousePos;
}

bool popRay(world::Ray& ray)
{
    InputState& state = getInputStateMutable();
    if (state.rays.empty())
        return false;

    ray = state.rays.front();
    state.rays.pop();
    return true;
}

END_NAMESPACE1
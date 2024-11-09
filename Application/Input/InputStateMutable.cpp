#include "Precompiled/Precompiled.h"
#include "InputStateMutable.h"

BEGIN_NAMESPACE1(input)

InputState & getInputStateMutable()
{
    static InputState inputState{};
    return inputState;
}

void keyInputFound(InputEvent inputEvent)
{
    getInputStateMutable().keyInputs.insert(inputEvent);
}

void setFoundRay(const world::Ray& ray)
{
    InputState& state = getInputStateMutable();
    state.rays.push(ray);
}

void clearAllKeys()
{
    getInputStateMutable().keyInputs.clear();
}

void clearMouseOffset()
{
    getInputStateMutable().lastMouseOffset = glm::vec2{};
}

END_NAMESPACE1

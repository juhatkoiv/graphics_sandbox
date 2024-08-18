#pragma once

#include "Precompiled/PrecompilationMacros.h"
BEGIN_NAMESPACE1(app)

enum class AppState : unsigned
{
    None = 0,
    CursorVisible = (1 << 0),
    InstructionsVisible = (1 << 1),
    FpsVisible = (1 << 2),
    Quits = (1 << 3),
    ToggleSceneCamera = (1 << 4),
    ToggleEditorGui = (1 << 5),
};

inline AppState operator|(AppState one, AppState other)
{
    return static_cast<AppState>(static_cast<unsigned>(one) | static_cast<unsigned>(other));
}

inline AppState operator&(AppState one, AppState other)
{
    return static_cast<AppState>(static_cast<unsigned>(one) & static_cast<unsigned>(other));
}

inline AppState operator^(AppState one, AppState other)
{
    return static_cast<AppState>(static_cast<unsigned>(one) ^ static_cast<unsigned>(other));
}

inline AppState operator|=(AppState& one, AppState other)
{
    one = operator|(one, other);
    return one;
}

inline AppState operator&=(AppState& one, AppState other)
{
    one = operator&(one, other);
    return one;
}

inline AppState operator^=(AppState& one, AppState other)
{
    one = operator^(one, other);
    return one;
}

inline AppState& getAppStateMutable() 
{
    static AppState stateFlags = AppState::None;
    return stateFlags;
}

inline const AppState getAppState() 
{
    return getAppStateMutable();
}

inline void ToggleState(AppState flag)
{
    getAppStateMutable() ^= flag;
}

inline bool hasState(AppState flag)
{
    return (getAppStateMutable() & flag) != AppState::None;
}
END_NAMESPACE1
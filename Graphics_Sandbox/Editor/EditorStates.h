#pragma once

#include <set>

BEGIN_NAMESPACE1(editor)

struct EditorState
{
    enum class Flags
    {
        None = 0,
        CursorVisible = 1,
        GameStarted = 2,
        GameRunning = 3,
        InstructionsVisible = 4,
        FpsVisible = 5,
        Quits = 6,
        GameStopped = 7,
        ToggleSceneCamera = 8,
        ToggleEditorGui = 9,
    };

    static void SetState(Flags flag)
    {
        _flags.insert(flag);
    }

    static void RemoveState(Flags flag)
    {
        _flags.erase(flag);
    }

    static bool IsStateSet(Flags flag)
    {
        return _flags.find(flag) != _flags.end();
    }

    static inline std::set<Flags> _flags;
};

END_NAMESPACE1
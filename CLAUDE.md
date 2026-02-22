# CLAUDE.md — Graphics Sandbox

## Project Overview

C++ graphics sandbox with an OpenGL backend. Build target is Windows (Visual Studio 2022 / MSVC). The project uses an ECS architecture with a command-driven rendering pipeline.

## Build System

- **IDE**: Visual Studio 2022 (`Graphics_Sandbox.sln`, `Application.vcxproj`)
- **Compiler**: MSVC (v17.9)
- **Platforms**: x64 (primary), x86
- **Configurations**: Debug, Release
- No root-level CMakeLists.txt; CMake exists only in sub-dependencies.

## Key Dependencies

| Library | Purpose |
|---------|---------|
| GLFW | Window and context management |
| GLM | Math (vectors, matrices) |
| ImGui | Editor UI |
| Assimp | Mesh/model loading |
| shaderc | GLSL → SPIRV compilation |
| xxHash | Hashing utilities |
| Tracy | Frame profiler |
| stb | Image loading |

## Directory Structure

```
Graphics_Sandbox.sln          (VS solution — broken until CMake task)
Application/
├── AppData/          # App configuration
├── AssetLoading/     # Texture/mesh loaders (Assimp, stb)
├── Containers/       # Custom containers: DynamicVector, SparseMap, LinearMap, SmallVector
├── ECS/              # Entity-Component-System
├── Editor/           # ImGui-based editor
├── GL/               # OpenGL wrappers
├── Precompiled/      # Precompiled headers and shared macros
├── Profiling/        # Tracy integration
├── Rendering/        # Rendering pipeline
├── Resources/        # Resource system (textures, meshes, shaders)
├── Scene/            # Scene management
├── data/shaders/     # GLSL shader sources (.vert/.frag) and compiled SPIRV (.spv)
├── MainLoop.h/cpp    # Top-level loop: initialize → run → cleanup
└── Application.h     # Base application interface
penguins-skybox-pack/         # Asset pack (not a library)
third_party/
├── imgui/            # Source + vcxproj (ImGui)
├── Hashing/          # Source + vcxproj + xxHash/ (xxHash wrapper)
├── tracy/            # Full Tracy source tree (CMakeLists.txt)
├── assimp/           # include/assimp/, lib/, bin/ (prebuilt MSVC)
├── glfw/             # include/GLFW/, lib/ (prebuilt MSVC)
├── glew/             # include/GL/{glew.h,...}, lib/glew32.lib (prebuilt)
├── glad/             # include/GL/glad/, include/GL/KHR/ (header + .c)
├── shaderc/          # include/shaderc/, lib/, bin/ (prebuilt MSVC)
├── glm/              # include/glm/ (header-only)
├── nlohmann/         # include/nlohmann/ (header-only)
├── stb/              # include/stb/ (header-only)
├── glslang/          # include/glslang/ (headers)
├── spirv_cross/      # include/spirv_cross/ (headers)
├── spirv-tools/      # include/spirv-tools/ (headers)
├── spirv-headers/    # include/ (GLSL.std.450.h, spirv.h, ...)
└── OBJLoader/        # include/OBJLoader/ (header-only)
```

## Coding Conventions

### Macros (defined in `Precompiled/PrecompilationMacros.h`)

```cpp
// Namespace declaration
BEGIN_NAMESPACE1(rendering)   // → namespace rendering {
END_NAMESPACE1                // → }

BEGIN_NAMESPACE2(ecs, detail) // → namespace ecs { namespace detail {
END_NAMESPACE2                // → } }

// Forward declarations
DECLARE1(class, rendering, GfxWorker)   // → namespace rendering { class GfxWorker; }
DECLARE2(struct, ecs, detail, Foo)

// Copy/move suppression
NON_COPYABLE(MyClass)   // deletes copy ctor and copy-assign
NON_MOVABLE(MyClass)    // deletes move ctor and move-assign
NON_TRANSIENT(MyClass)  // both of the above
```

### General Style
- `_camelCase` private member variables with leading underscore
- Structs with default-initialized members (e.g., `= nullptr`, `= {}`, `= id::InvalidTextureId`)
- ID types: `id::TextureId`, `id::MeshId`, `id::ShaderId` with sentinel `id::InvalidXxxId`
- Components inherit from `Component<cid::COMPONENT_ID_ENUM>`

## Architecture

### Main Loop (`MainLoop`)

Owns all top-level systems as `unique_ptr` members:

```
MainLoop
├── Window          (GLFW)
├── GfxDevice       (OpenGL implementation)
├── GfxWorker       (renders + processes commands)
├── ResourceSystem  (loads/stores assets)
├── World           (ECS)
├── SystemContainer (ECS systems lifecycle)
├── Application     (editor / app logic)
└── AppData
```

Flow:
```
initialize() → generateSpirv() → run():
  loop:
    update(dt)   → ECS systems (Input, Camera, etc.)
    render(dt)   → Renderer builds commands → GfxWorker → RenderGraph → GPU
```

### ECS (`Application/ECS/`)

- `World` manages entities and components.
- Storage: `ComponentLookups` (SoA arrays), `ComponentMask2` (bitset per entity), `ComponentBucket`.
- Components inherit `Component<cid::ENUM_ID>` and use `DECLARE_SERIALIZABLE(Type, fields...)` for serialization.
- Key components: `Transform`, `Camera`, `Light`, `Mesh`, `Material`.

### Render Command System (`Application/Rendering/RenderCommands.h`)

Two coexisting implementations — the old one is being replaced by the new:

**Old** (`RenderCommand`) — union-based, dispatched via `CommandType` enum:
```cpp
struct RenderCommand {
    CommandType type;
    unsigned id, resourceSlot;
    union { CreateTextureCmd, SetLightCmd, ... };
};
```

**New** (`RenderCommand2`) — `std::variant`-based, type-safe:
```cpp
using CommandType2 = std::variant<SubmitToEffectQueueCmd2, CreateTextureCmd2, SetLightCmd2, ...>;
struct RenderCommand2 {
    CommandType2 commandType;
    unsigned id, resourceSlot;
};
```

When adding new command types, add a `CmdFoo` struct (old path) and a corresponding `CmdFoo2` struct + add it to `CommandType2` (new path). Dispatch is via `std::visit` on `RenderCommand2`.

### GfxWorker (`Application/Rendering/GfxWorker.h`)

- Owns `GfxDevice` (OpenGL implementation).
- `update(RenderCommandQueue&)` — drains the queue, dispatches to private handlers.
- `render()` — executes the render graph.
- `compileShaders(GfxShaderArgs&)` — uploads pre-compiled SPIRV to the device.
- Non-copyable (`NonCopyable` base or `NON_COPYABLE` macro).

### Shader Pipeline

1. GLSL sources live in `Application/data/shaders/` (`.vert`, `.frag`).
2. `ShaderCompilation.h/cpp` uses **shaderc** to compile GLSL → SPIRV.
3. `MainLoop::generateSpirv()` runs this during startup (async WIP).
4. `ResourceSystem::getShaderArgs()` returns `GfxShaderArgs` with compiled SPIRV binaries.
5. `GfxWorker::compileShaders(GfxShaderArgs)` uploads to the GPU.

`GfxShaderArgs` holds:
```cpp
struct GfxShaderArgs {
    std::vector<id::ShaderId>           shaderIds;
    std::vector<std::string>            shaderNames;
    std::vector<std::vector<uint32_t>>  vertexShaderData;   // SPIRV
    std::vector<std::vector<uint32_t>>  fragmentShaderData; // SPIRV
};
```
> File source paths were removed from this struct (commit `009d317`). Do not re-add them.

### Resource System (`Application/Resources/`)

- `ResourceSystem` is the central asset manager.
- Provides name-to-ID mappings and typed handles (`TextureId`, `MeshId`, `ShaderId`).
- Resources stored in `ResourceContainer`.
- `getShaderArgs()` returns compiled shader data for startup.

## Active Refactoring (branch: `shader_loading`)

- Migrating from `RenderCommand` (union) → `RenderCommand2` (variant). Both exist during transition.
- `GfxWorker` now has `executeRenderCommand2()` alongside `executeRenderCommand()`.
- Async SPIRV generation infrastructure in `MainLoop::generateSpirv()` is WIP.
- `Light` component serializes `lightColor`, `intensity`, `attenuation` (not `type`).

## Plan - TODO:
1. ~~Improve dependency management~~ **DONE** — All external deps now live under `third_party/` with per-library subdirectories. VS project files are broken (fixed in TODO 3).
2. Prebuilt libraries need their linux equivalent. Alternatives are:
i) build the projects from source ( preferred ) - May need help finding them.
ii) find linux packages.
If glfw proves to be an issue, we can think about replacing that with something else or even explicit window / context management. Later we'll do Vulkan support, where explicit context management may be useful.
3. Add linux dev support. On linux I'll use vscode and clang to build and run the project. For this I need to convert the Visual Studio solution to cmake project which should also work after the CMake configuaration.
4. Continue work on the refactoring.
* Continue what was already stated.
* Refactor architecture: Data driven shader args -> Material system.
* Refactor architecture: GfxWorker and GfxDevice doesn't have to know about ID:s. 
* Optimize visibility queries in Renderer.
* Refactor GfxWorker to: Dispatch buffers once and update if things change. This is the most common case, but need 

cmake_minimum_required(VERSION 3.16)
project(Application CXX C)

add_executable(Application
    Main.cpp
    MainLoop.cpp
    AppContext.cpp
    CommanLineArgsParser.cpp

    Precompiled/Logger.cpp

    GL/glad.c

    AppData/AppData.cpp

    AssetLoading/AssimpMeshLoader.cpp
    AssetLoading/AssimpVertexParser.cpp
    AssetLoading/JsonFileSystem.cpp
    AssetLoading/TextureLoader.cpp

    ECS/World.cpp
    ECS/Renderer.cpp
    ECS/Transform.cpp
    ECS/ModelSerializer.cpp
    ECS/System/CameraSystem.cpp
    ECS/System/InputSystem.cpp
    ECS/Serialization/GlmSerializationDefines.cpp

    Editor/Editor.cpp
    Editor/EditorManager.cpp
    Editor/EditorMenus.cpp
    Editor/EditorRenderer.cpp
    Editor/EditorTransform.cpp
    Editor/EditorMenus/ApplicationStatusMenu.cpp
    Editor/EditorMenus/InstructionsMenu.cpp
    Editor/EditorMenus/RenderGraphMenu.cpp
    Editor/EditorMenus/RenderingSettingsMenu.cpp
    Editor/EditorMenus/SceneConfigMenu.cpp

    Hash/Hash.cpp

    Input/InputState.cpp
    Input/InputStateMutable.cpp

    Rendering/Defaults.cpp
    Rendering/FeatureFilters.cpp
    Rendering/FrameBuilder.cpp
    Rendering/FrameResources.cpp
    Rendering/GfxDeviceFactory.cpp
    Rendering/GfxWorker.cpp
    Rendering/GraphFilters.cpp
    Rendering/PassResources.cpp
    Rendering/RenderFeature.cpp
    Rendering/RenderGraph.cpp
    Rendering/RenderQueue.cpp
    Rendering/ShaderBase.cpp
    Rendering/ShaderProgram.cpp
    Rendering/SpatialPartition.cpp
    Rendering/VertexBuffer.cpp
    Rendering/OpenGL/GfxDeviceOpenGL.cpp
    Rendering/OpenGL/GLConstBuffer.cpp
    Rendering/OpenGL/GLFunctions.cpp
    Rendering/OpenGL/RenderFunctionsGL.cpp
    Rendering/ShaderCompilation/ShaderCompilation.cpp
    Rendering/Serialization/RendererSerializationDefines.cpp

    Resources/ResourceSystem.cpp

    Scene/SceneData.cpp

    Screen/Screen.cpp
    Screen/ScreenMutable.cpp

    Window/Window.cpp

    World/Ray.cpp
)

# glad.c must not use precompiled headers
set_source_files_properties(GL/glad.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_include_directories(Application PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/glad/include
    ${CMAKE_SOURCE_DIR}/third_party/glm/include
    ${CMAKE_SOURCE_DIR}/third_party/nlohmann/include
    ${CMAKE_SOURCE_DIR}/third_party/stb/include
    ${CMAKE_SOURCE_DIR}/third_party/OBJLoader/include
    ${CMAKE_SOURCE_DIR}/third_party/spirv_cross/include
    ${CMAKE_SOURCE_DIR}/third_party/glslang/include
    ${CMAKE_SOURCE_DIR}/third_party/spirv-tools/include
    ${CMAKE_SOURCE_DIR}/third_party/spirv-headers/include
)

target_link_libraries(Application PRIVATE
    imgui
    xxhash
    Tracy::TracyClient
    glfw
    assimp::assimp
    GLEW::GLEW
    unofficial::shaderc::shaderc
)

if(WIN32)
    target_link_libraries(Application PRIVATE opengl32 gdi32 user32)
else()
    target_link_libraries(Application PRIVATE GL dl pthread)
endif()

target_precompile_headers(Application PRIVATE Precompiled/Precompiled.h)

if(MSVC)
    target_compile_options(Application PRIVATE /W4 /WX /arch:AVX2)
else()
    target_compile_options(Application PRIVATE -Wall -Wextra -mavx2)
endif()

# Copy shader data to build output directory
add_custom_command(TARGET Application POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:Application>/data
)

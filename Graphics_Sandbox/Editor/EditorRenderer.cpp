#include "Precompiled/Precompiled.h"
#include "EditorRenderer.h"

#include "Window/Window.h"
#include "Scene/SceneData.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl3.h"

BEGIN_NAMESPACE1(editor)

EditorRenderer::EditorRenderer(window::Window &window)
{
	IMGUI_CHECKVERSION();
	auto* imGuiContext = ImGui::CreateContext();
	UNUSED_VAR(imGuiContext);
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.WantCaptureMouse = false;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.getWindowImpl(), true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

EditorRenderer::~EditorRenderer()
{
	// Glfw cleanup done elsewhere
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void EditorRenderer::RenderEditorGUI(scene::SceneData &scene) const
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	for (auto &menu : menus) 
	{
		menu->Draw(scene);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorRenderer::AddMenu(std::shared_ptr<IMenu> menu)
{
	if (!menu)
		return;

	menus.push_back(menu);
}

END_NAMESPACE1
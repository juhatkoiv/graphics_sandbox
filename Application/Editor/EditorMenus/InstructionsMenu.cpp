#include "Precompiled/Precompiled.h"
#include "InstructionsMenu.h"

#include "Editor/EditorManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

BEGIN_NAMESPACE1(editor);

namespace 
{
	EditorManager* _editorManager = nullptr;
}

InstructionsMenu::InstructionsMenu(EditorManager* editorManager)
{
	assert(editorManager);

	_editorManager = editorManager;
}

void InstructionsMenu::Draw(scene::SceneData& scene)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoMove;

	bool open_ptr = true;
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("Inputs", &open_ptr, window_flags);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
	ImGui::Text("Space - Toggle Cursor / Rotate Camera ");
	ImGui::Text("F10 - Toggle instructions");
	ImGui::Text("F11 - Toggle Gui");
	ImGui::Text("ESC - Quit");
	ImGui::PopStyleColor();
	ImGui::End();
}

END_NAMESPACE1;
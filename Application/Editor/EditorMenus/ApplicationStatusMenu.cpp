#include "Precompiled/Precompiled.h"
#include "ApplicationStatusMenu.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

BEGIN_NAMESPACE1( editor )

ApplicationStatusMenu::ApplicationStatusMenu( EditorManager* editorManager ) {}

ApplicationStatusMenu::~ApplicationStatusMenu() {}

void ApplicationStatusMenu::Draw( scene::SceneData& scene )
{
	ImGui::Begin( "Application State" );
	
	if (app::hasState( app::AppState::CompilingShaders )) 
	{
		ImGui::Text( "Compiling SPV Shaders..." );
	}
	else if (app::hasState( app::AppState::GeneratingSpirv ))
	{
		ImGui::Text( "Generating SPIR-V from GLSL sources..." );
	}
	else
	{
		ImGui::Text( "Running..." );
	}
	
	ImGui::End();
}

END_NAMESPACE1

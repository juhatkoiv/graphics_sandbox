#include "Precompiled/Precompiled.h"
#include "RenderGraphMenu.h"

#include "Rendering/Serialization/RendererJsons.h"
#include "Editor/EditorManager.h"
#include <imgui/imgui.h>

BEGIN_NAMESPACE1(editor)

namespace 
{
	EditorManager* _editorManager;
	
	static void renderRenderGraph()
	{
		ImGui::Begin( "Render Graph" );
		const RenderGraphJson& graph = _editorManager->getRenderGraph();
		for ( const auto& queue : graph.queues )
		{
			ImGui::Text( "\tQueue: %s", queue.queueName.c_str() );
			for ( const auto& feature : queue.features )
			{
				ImGui::Text( "\t\tFeature: %s", feature.featureName.c_str() );
				for ( const auto& pass : feature.passes )
				{
					ImGui::Text( "\t\t\tPass: (%d): %s", (int)pass.passIndex, pass.passName.c_str() );
				}
			}
		}
		ImGui::End();
	}
}

RenderGraphMenu::RenderGraphMenu( EditorManager* editorManager )
{
	_editorManager = editorManager;
}

void RenderGraphMenu::Draw( scene::SceneData& scene )
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;

	bool open_ptr = true;
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("Render Graph", &open_ptr, window_flags);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
	ImGui::PushStyleColor( ImGuiCol_WindowBg, IM_COL32( 130, 170, 140, 180) );

	renderRenderGraph();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::End();
}

END_NAMESPACE1
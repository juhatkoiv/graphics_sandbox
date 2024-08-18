#include "Precompiled/Precompiled.h"
#include "RenderingSettingsMenu.h"

#include "Editor/EditorManager.h"
#include "Rendering/RenderEffect.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <sstream>


BEGIN_NAMESPACE1(editor);

namespace 
{
	EditorManager* _editorManager = nullptr;
	rendering::PostProcessingEffects currentEffect = rendering::PostProcessingEffects::Nothing;
	
	const char* fsEffectEnumStr(rendering::PostProcessingEffects effect) 
	{
		static std::unordered_map<rendering::PostProcessingEffects, const char*> enumTextMap{};
		if (enumTextMap.empty())
		{
			enumTextMap[rendering::PostProcessingEffects::Nothing] = "Nothing";
			enumTextMap[rendering::PostProcessingEffects::Inverted] = "Inverted";
			enumTextMap[rendering::PostProcessingEffects::Blur] = "Blur";
			enumTextMap[rendering::PostProcessingEffects::Pixelation] = "Pixelation";
			enumTextMap[rendering::PostProcessingEffects::ColorCorrection] = "ColorCorrection";
			enumTextMap[rendering::PostProcessingEffects::ChromeAberration] = "ChromeAberration";
		}
		return enumTextMap[effect];
	}

	const char* clearModeEnumStr(rendering::ClearMode mode)
	{
		static std::unordered_map<rendering::ClearMode, const char*> enumTextMap{};
		if (enumTextMap.empty())
		{
			enumTextMap[rendering::ClearMode::SolidColor] = "Solid Color";
			enumTextMap[rendering::ClearMode::Skybox] = "Skybox";
		}
		return enumTextMap[mode];
	}

	static void renderLigtingSettings() 
	{
		static rendering::LightingSettings lightingSettings
		{
			.exposure = 1.0f,
			.gamma = 1.0f
		};

		ImGui::Text("Ligthing Settings");
		ImGui::DragFloat("Exposure", &lightingSettings.exposure, 0.1f, 0.0f, 10.0f);
		ImGui::DragFloat("Gamma Correction", &lightingSettings.gamma, 0.1f, 0.0f, 10.0f);

		_editorManager->setLightingSettings(lightingSettings);
	}

	void renderBlurSettings() 
	{
		static rendering::FullScreenBlurSettings blurSettings
		{
			.kernelSize = 21,
			.sigma = 5.0f
		};

		ImGui::Text("Blur Settings");
		ImGui::DragInt("Kernel Size", &blurSettings.kernelSize, 1, 1, 50);
		ImGui::DragFloat("Sigma", &blurSettings.sigma, 0.1f, 0.0f, 200.0f);

		_editorManager->setBlurSettings(blurSettings);
	}

	void renderPixelationSettings()
	{
		static rendering::PixelatedSettings pixelatedSettings
		{
			.fragmentScale = 1.0f
		};

		ImGui::Text( "Pixelation Settings" );
		ImGui::DragFloat( "Fragment scale", &pixelatedSettings.fragmentScale, 1, 10, 2000 );

		_editorManager->setPixelationSettings( pixelatedSettings);
	}

	void renderColorCorrectionSettings()
	{
		static rendering::ColorCorrectionSettings colorCorrectionSettings
		{
			.colorCorrection = glm::vec3(1.0f, 1.0f, 1.0f)
		};

		ImGui::Text("Color Correction Settings");
		ImGui::ColorEdit3("Color Correction", &colorCorrectionSettings.colorCorrection[0]);

		_editorManager->setColorCorrectionSettings(colorCorrectionSettings);
	}

	void renderChromeAberrationSettings()
	{
		static rendering::ChromeAberrationSettings chromeAberrationSettings
		{
			.aberration = 0.0f
		};

		ImGui::Text( "Chrome Aberration Settings" );
		ImGui::DragFloat( "Aberration", &chromeAberrationSettings.aberration, 0.001f, 0.0f, 3.14f );

		_editorManager->setChromeAberrationSettings( chromeAberrationSettings );

	}

	void renderSelectFullScreenEffect() 
	{
		ImGui::Text("Full Screen Effects");

		if (ImGui::BeginCombo("Screen Effects ", fsEffectEnumStr(currentEffect)))
		{
			int maxI = static_cast<int>(rendering::PostProcessingEffects::COUNT);
			for (int i = 0; i < maxI; i++)
			{
				auto effect = static_cast<rendering::PostProcessingEffects>(i);
				bool isSelected = (currentEffect == effect);
				if (ImGui::Selectable(fsEffectEnumStr(effect), isSelected))
				{
					currentEffect = effect;
					_editorManager->setFullScreenEffect(currentEffect);
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	void renderClearModeSettingsPanel()
	{
		static rendering::ClearSettings clearSettings
		{
			.mode = rendering::ClearMode::SolidColor,
			.color = glm::vec4(0.2f, 0.3f, 0.2f, 1.0f)
		};

		ImGui::Text("Clear Mode Settings");

		if (ImGui::BeginCombo("Clear Mode", clearModeEnumStr(clearSettings.mode)))
		{
			int maxI = static_cast<int>(rendering::ClearMode::COUNT);
			for (int i = 0; i < maxI; i++)
			{
				auto mode = static_cast<rendering::ClearMode>(i);
				bool isSelected = (clearSettings.mode == mode);
				if (ImGui::Selectable(clearModeEnumStr(mode), isSelected))
				{
					clearSettings.mode = mode;
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (clearSettings.mode == rendering::ClearMode::SolidColor)
		{
			ImGui::ColorEdit4("Clear Color", &clearSettings.color[0]);
		}

		_editorManager->setClearSettings(clearSettings);
	}

	void renderSettingsPanel() 
	{
		ImGui::Begin("Rendering Settings");

		renderLigtingSettings();
		renderBlurSettings();
		renderPixelationSettings();
		renderColorCorrectionSettings();
		renderChromeAberrationSettings();
		renderSelectFullScreenEffect();
		renderClearModeSettingsPanel();

		ImGui::End();

	}
}

RenderingSettingsMenu::RenderingSettingsMenu(EditorManager* editorManager)
{
	_editorManager = editorManager;
}

RenderingSettingsMenu::~RenderingSettingsMenu() {}

void RenderingSettingsMenu::Draw(scene::SceneData& scene)
{
	renderSettingsPanel();
}

END_NAMESPACE1;
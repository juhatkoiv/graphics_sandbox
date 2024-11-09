#include "Precompiled.h"
#include "GuardsmanGame.h"

#include "GameInputSystem.h"
#include "ECS/DataBases.h"

#include "Scene/SceneData.h"

#include <queue>

BEGIN_NAMESPACE1(guardsman)

namespace
{
	namespace sceneNames 
	{
		static constexpr const char* MAIN_SCENE = "mainScene"; // hub
		static constexpr const char* BATTLE_SCENE = "battleScene"; // battle
		static constexpr const char* GARDEN_SCENE = "gardenScene"; // garden
		static constexpr const char* MINING_SCENE = "miningScene"; // mining
		static constexpr const char* LAB_SCENE = "labScene"; // lab
		static constexpr const char* TEAM_SCENE = "teamScene"; // whole team
		static constexpr const char* CHARACTER_SCENE = "characterScene"; // single character
	}
}

GuardsmanGame::GuardsmanGame(Args& args, window::Window& window)
	: _inputSystem(window)
{


}

GuardsmanGame::~GuardsmanGame()
{
}

void GuardsmanGame::Update(float deltaTime)
{
	// handle input 
	// >> dispatch to message system;
	// >> charactercontroller ?
	std::vector<input::ScreenInputOperation> inputs;
	_inputSystem.FlushInputOperations(inputs);

	for (int i = 0; i < inputs.size(); i++)
	{
		const auto& input = inputs[i];
		// >> does what?
		// selects character?
		// se
	}
}

void GuardsmanGame::PostUpdate(float deltaTime)
{

}

END_NAMESPACE1
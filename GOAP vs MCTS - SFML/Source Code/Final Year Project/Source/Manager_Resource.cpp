#include "Manager_Resource.h"

ResourceManager* ResourceManager::globalVariablesInstance = nullptr;

ResourceManager::ResourceManager()
{
	createStateNames();

	FONT.loadFromFile("content\\fonts\\kenvector_future.TTF");

	// Art
	t_map.loadFromFile("content\\game\\grass.png");

	t_blue_builder.loadFromFile("content\\game\\blue_builder.png");
	t_blue_soldier.loadFromFile("content\\game\\blue_soldier.png");
	t_blue_archer.loadFromFile("content\\game\\blue_archer.png");
	t_blue_wall.loadFromFile("content\\game\\blue_wall.png");
	t_blue_castle.loadFromFile("content\\game\\blue_castle.png");
	t_blue_resource.loadFromFile("content\\game\\blue_resource.png");

	t_red_builder.loadFromFile("content\\game\\red_builder.png");
	t_red_soldier.loadFromFile("content\\game\\red_soldier.png");
	t_red_archer.loadFromFile("content\\game\\red_archer.png");
	t_red_wall.loadFromFile("content\\game\\red_wall.png");
	t_red_castle.loadFromFile("content\\game\\red_castle.png");
	t_red_resource.loadFromFile("content\\game\\red_resource.png");

	// Audio
	if (!music.openFromFile("content\\audio\\music.ogg")) {}
	music.setVolume(50);
	music.setLoop(true);

	// Sounds
	if (!b_builder.loadFromFile("content\\audio\\builder.ogg")) {}
	s_builder.setBuffer(b_builder);

	if (!b_soldier.loadFromFile("content\\audio\\soldier.ogg")) {}
	s_soldier.setBuffer(b_soldier);

	if (!b_archer.loadFromFile("content\\audio\\archer.ogg")) {}
	s_archer.setBuffer(b_archer);

	if (!b_walk.loadFromFile("content\\audio\\walk.ogg")) {}
	s_walk.setBuffer(b_walk);

	if (!b_menu.loadFromFile("content\\audio\\menu.wav")) {}
	s_menu.setBuffer(b_menu);

	if (!b_enter.loadFromFile("content\\audio\\enter.wav")) {}
	s_enter.setBuffer(b_enter);
}


ResourceManager::~ResourceManager()
{
}

ResourceManager * ResourceManager::GetInstance()
{
	if (globalVariablesInstance == nullptr)
	{
		globalVariablesInstance = new ResourceManager();
	}
	return globalVariablesInstance;
}

void ResourceManager::createStateNames()
{
	stateNames[ActionState::Idle] = "Idle";
	stateNames[ActionState::Idle_Builder] = "Idle Builder";

	stateNames[ActionState::Move_To_Castle] = "Move Castle";
	stateNames[ActionState::Attack_Castle] = "Attack Castle";
	stateNames[ActionState::Castle_Dead] = "Castle Dead";

	stateNames[ActionState::Move_To_Enemy] = "Move To Enemy";
	stateNames[ActionState::Attack_Enemy] = "Attack Enemy";
	stateNames[ActionState::Enemy_Dead] = "Enemy Dead";

	stateNames[ActionState::Move_To_Structure] = "Move To Structure";
	stateNames[ActionState::Attack_Structure] = "Attack Structure";
	stateNames[ActionState::Structure_Dead] = "Structure Dead";

	stateNames[ActionState::Move_To_Base] = "Run To Base";
	stateNames[ActionState::Base_Reached] = "Base Reached";

	stateNames[ActionState::Move_To_Resource] = "Move To Resource";
	stateNames[ActionState::Gather_Resources] = "Gather Resources";
	stateNames[ActionState::Resources_Collected] = "Resources Collected";

	stateNames[ActionState::Move_To_Site] = "Move To Site";
	stateNames[ActionState::Build_Structure] = "Build Structure";
	stateNames[ActionState::Structure_Built] = "Structure_Built";
}
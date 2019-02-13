#pragma once

// Game Scenes - for changing scene 
enum class GameScene {
	SPLASH,
	MAINMENU,
	GAME,
	INSTRUCTIONS,
};

// Menu Options - for selecting scene in Main Menu
enum MenuOptions {
	GAME,
	INSTRUCTIONS,
	QUIT
};

// Actions Units can perform
enum ActionState {
	Idle, 
	Idle_Builder,

	Move_To_Castle,
	Attack_Castle,
	Castle_Dead, //

	Move_To_Enemy,
	Attack_Enemy,
	Enemy_Dead, //

	Move_To_Structure,
	Attack_Structure,
	Structure_Dead, //

	Move_To_Base,
	Base_Reached, //

	Move_To_Resource, 
	Gather_Resources,
	Resources_Collected, //

	Move_To_Site,
	Build_Structure,
	Structure_Built,
};

// World States that affect Unit actions
enum class WorldState {
	Engage_Castle,
	Engage_Enemies,
	Engage_Structure,

	Defend_Castle,
	Build_Structures,

	Need_Resources,
	Wait
};

// Unit Type
enum class UnitType {
	SOLDIER,
	ARCHER,
	BUILDER
};

// Structure Type
enum class StructureType {
	CASTLE,
	WALL,
	RESOURCE
};

// Team side in game
enum class Team {
	LEFT,
	RIGHT
};

// Current Unit Target - for attack
enum class Target {
	CASTLE_T,
	ENEMY_T,
	WALL_T,

	RESOURCE_T,
	SITE_T,

	NO_T
};

// Units - for Build Queue
enum class BuildQueue {
	Defence_Archer,
	Defence_Soldier,
	Offence_Archer,
	Offence_Soldier,
	Builder
};
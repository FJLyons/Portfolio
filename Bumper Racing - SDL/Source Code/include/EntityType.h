#pragma once

typedef unsigned short uint16;

enum class EntityType : uint16
{ //draw order is determined by the below order
	Tile = 0x0001,			//0000000000000001
	Obstacle = 0x0002,		//0000000000000010
	Checkpoint = 0x0004,	//0000000000000100
	Radar = 0x0008,
	PowerUp = 0x00010,		//0000000000001000
	Weapon = 0x00020,
	AI = 0x0040,
	Player = 0x0080,
	RemotePlayer = 0x0100,
	Bullet = 0x0200,
	Flag = 0x0400,
	Point = 0x0800,
	Particle = 0x1000,
	UI = 0x2000,
}; //16 max

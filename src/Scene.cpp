#include "Scene.h"
#include <stdio.h>
#include "Globals.h"
#include <iostream>
#include "Game.h"
#include "StaticImage.h"
#include "TileMap.h"
#include "ResourceManager.h"



Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	enemy = nullptr;
	nLevel = 0;


	
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
	//if (enemy != nullptr)
	//{
	//	enemy->Release();
	//	delete enemy;
	//	enemy = nullptr;
	//}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	for (Entity* objEn : objsEn) 
	{
		delete objEn;
	}
	objects.clear();
	objsEn.clear();
}
AppStatus Scene::Init()
{

	doorOpen = false;
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::EROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::EROR;
	}
	//Create enemy
	enemy = new Enemy({ 0,0 }, State::IDLE, Look::RIGHT);
	if (enemy == nullptr)
	{
		LOG("Failed to allocate memory for Enemy");
		return AppStatus::EROR;
	}
	//Initialise enemy
	if (enemy->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy");
		return AppStatus::EROR;
	}
	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::EROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::EROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::EROR;
	}

	//Create text font 1
	myfont = new Text();
	if (myfont == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::EROR;
	}
	//Initialise text font 1
	if (myfont->Initialise(Resource::IMG_FONT, "images/myfont.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::EROR;
	}

	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);
	enemy->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				133, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 15, 14, 103, 135, 15, 14, 101, 133, 14, 15, 103, 135, 14, 15, 103,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 22, 0, 0, 0, 0, 0, 0, 88, 130, 0, 0, 0, 0, 0, 0, 0,
				131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 89, 131, 0, 0, 0, 0, 0, 0, 0,
				130, 0, 0, 0, 0, 0, 0, 0, 108, 25, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88, 130, 0, 0, 0, 0, 0, 0, 0,
				130, 0, 0, 0, 0, 0, 0, 0, 107, 302, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666,
				131, 0, 0, 87, 1, 1, 1, 1, 86, 1, 128, 1, 1, 1, 1, 1, 1, 1, 1, 1, 129, 0, 0, 0, 0, 0, 0, 87, 1, 1, 1, 1,
				132, 0, 0, 14, 13, 19, 15, 103, 135, 15, 14, 103, 135, 15, 14, 103, 135, 15, 19, 13, 14, 0, 0, 0, 0, 0, 0, 14, 13, 19, 15, 101,
				130, 0, 0, 0, 0, 20, 0, 21, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 89,
				130, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 119, 48, 153, 0, 0, 0, 20, 0, 88,
				131, 0, 0, 0, 0, 20, 0, 0, 0, 0, 108, 25, 140, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 120, 49, 154, 0, 0, 0, 20, 0, 89,
				130, 0, 0, 0, 0, 20, 0, 0, 0, 0, 107, 301, 139, 0, 0, 0, 0, 0, 20, 0, 500, 0, 303, 121, 52, 155, 0, 0, 0, 20, 0, 88,
				134, 1, 1, 1, 1, 2, 1, 1, 1, 1, 86, 1, 128, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 111, 39, 143, 1, 1, 1, 2, 1, 102,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 307, 0, 180, 180, 180, 180, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 180, 180, 0, 0,
		};
		nLevel = 1;
		thelevel = 1;
		player->InitScore();
		doorOpen = false;
	}
	else if (stage == 2)
	{
		map = new int[size] {
	
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				133, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 105, 23, 137, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 106, 24, 138, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 108, 25, 140, 0, 0, 0, 0, 666,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 107, 301, 139, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 87, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 86, 1, 128, 1, 1, 1, 1, 1,
				667, 0, 0, 1000, 0, 0, 0, 0, 135, 14, 19, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 15, 14, 103, 135, 15, 14, 103,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 666,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 129, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 87, 1, 1, 1,
				133, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 104, 16, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 16, 136, 15, 14, 103,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45,
				45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44,
				44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45,
				45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44, 45, 44,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 307, 0, 180, 180, 180, 180, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 180, 180, 0, 0,
		};
		nLevel = 2;
		thelevel = 2;
		doorOpen = false;
	}
	else if(stage == 3)
	{
		map = new int[size]
		{
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				135, 15, 14, 103, 135, 15, 14, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 130, 135, 14, 15, 103, 135, 14, 15, 103,
				0, 0, 0, 21, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 666,
				0, 0, 0, 0, 0, 108, 25, 140, 0, 0, 0, 0, 0, 0, 0, 0, 87, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
				0, 1000, 0, 0, 0, 107, 0, 139, 0, 0, 0, 0, 0, 0, 0, 0, 135, 14, 15, 103, 135, 19, 15, 103, 135, 14, 15, 103, 135, 19, 15, 103,
				1, 1, 1, 1, 1, 86, 1, 128, 1, 1, 1, 1, 129, 0, 0, 0, 50, 51, 0, 0, 0, 20, 0, 21, 22, 0, 0, 0, 0, 20, 0, 0,
				135, 14, 15, 103, 135, 14, 15, 103, 135, 15, 19, 13, 14, 0, 0, 0, 7, 8, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 9, 10, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 20, 0, 666,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 11, 12, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0,
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 129, 0, 0, 0, 87, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
				133, 14, 15, 103, 135, 14, 15, 103, 135, 15, 14, 103, 135, 19, 14, 103, 0, 0, 0, 14, 13, 14, 15, 103, 135, 15, 19, 103, 135, 15, 14, 103,
				130, 0, 0, 0, 0, 0, 0, 21, 22, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0,
				130, 0, 0, 119, 48, 153, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0,
				130, 0, 0, 120, 49, 154, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 108, 25, 140, 0, 0, 0, 0, 20, 0, 0, 0, 0, 666,
				131, 0, 303, 121, 52, 155, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 107, 301, 139, 0, 0, 0, 0, 20, 0, 0, 0, 0, 666,
				134, 1, 1, 111, 39, 143, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 86, 1, 128, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31,
				31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30,
				888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 0, 307, 0, 180, 180, 180, 180, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 180, 180, 0, 0,
		};
		nLevel = 3;
		thelevel = 3;
		drawKeyLock = true;
		doorOpen = false;
	}
	else if (stage == 4)
	{
		map = new int[size]
		{
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 122, 210, 210, 210, 210, 210, 210, 210, 210, 210, 210, 156, 0, 0, 0, 0, 0, 0,
				135, 15, 14, 103, 135, 15, 14, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 14, 15, 103, 135, 15, 14, 103, 135, 15, 14, 101,
				0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 22, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88,
				667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 118, 60, 150, 0, 0, 0, 0, 0, 0, 0, 0, 118, 60, 150, 0, 0, 0, 0, 89,
				1, 1, 1, 1, 1, 1, 1, 129, 0, 0, 0, 0, 0, 117, 59, 149, 0, 0, 0, 0, 0, 0, 0, 0, 117, 59, 149, 0, 0, 0, 0, 88,
				135, 15, 19, 103, 135, 15, 14, 103, 0, 0, 0, 0, 0, 116, 668, 148, 0, 0, 0, 0, 0, 0, 0, 0, 116, 668, 148, 0, 0, 0, 0, 88,
				0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 87, 1, 109, 1, 141, 1, 1, 1, 1, 1, 1, 1, 1, 109, 1, 141, 1, 129, 0, 0, 89,
				0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 14, 13, 14, 15, 103, 135, 14, 19, 103, 135, 14, 15, 103, 135, 15, 14, 13, 14, 0, 0, 100,
				0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 88,
				0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 48, 153, 88,
				1, 1, 2, 1, 1, 1, 1, 129, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 49, 154, 88,
				135, 15, 14, 103, 135, 19, 14, 103, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 303, 121, 52, 155, 89,
				0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 87, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 111, 39, 143, 102,
				0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 135, 15, 14, 103, 135, 19, 14, 101, 30, 31, 30, 31, 30, 31, 30, 31,
				0, 0, 0, 0, 0, 20, 0, 0, 108, 25, 140, 0, 0, 0, 0, 0, 0, 0, 0, 21, 22, 20, 0, 88, 31, 30, 31, 30, 31, 30, 31, 30,
				0, 0, 0, 0, 0, 20, 0, 0, 107, 301, 139, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 89, 30, 31, 30, 31, 30, 31, 30, 31,
				1, 1, 1, 1, 1, 2, 1, 1, 86, 1, 128, 1, 1, 1, 1, 129, 0, 0, 0, 0, 0, 20, 0, 88, 31, 30, 31, 30, 31, 30, 31, 30,
				133, 14, 15, 103, 135, 14, 15, 103, 135, 15, 14, 103, 135, 19, 14, 103, 0, 0, 0, 0, 0, 20, 0, 88, 30, 31, 30, 31, 30, 31, 30, 31,
				130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 20, 0, 89, 31, 30, 31, 30, 31, 30, 31, 30,
				131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 500, 20, 0, 88, 30, 31, 30, 31, 30, 31, 30, 31,
				134, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 102, 31, 30, 31, 30, 31, 30, 31, 30,
				888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 888, 0, 307, 0, 180, 180, 180, 180, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 180, 180, 180, 0, 0,

		};
		nLevel = 4;
		thelevel = 4;
		doorOpen = false;
		drawKeyLock = true;
		drawKid = true;
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::EROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PLAYER)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				player->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::ENEMY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				enemy->SetPos(pos);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_APPLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHILI)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHILI);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::MONEYBAG)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::MONEYBAG);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::KEY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::KEY);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::KEYHOLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::KEYHOLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::NEXTLEVEL)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::NEXTLEVEL);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::PREVLEVEL)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::PREVLEVEL);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::VICTORY)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::VICTORY);
				objects.push_back(obj);
				map[i] = 0;
			}

			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Debug() 
{
	if (IsKeyPressed(KEY_P)) 
	{
		if (player->IsLookingLeft()) 
		{
			Object* obj;
			obj = new Object({ player->pos.x - 16 , player->pos.y - 4 }, ObjectType::POTION);
			objects.push_back(obj);
		}
		else 
		{
			Object* obj;
			obj = new Object({ player->pos.x + 12 , player->pos.y - 4 }, ObjectType::POTION);
			objects.push_back(obj);
		}

	}
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;


	Debug();

	if (doorOpen && thelevel == 3 && drawKid)
	{
		drawKid = false;
		Object* obj;
		obj = new Object({ 24 + 8, 123 + 12}, ObjectType::KID);
		objects.push_back(obj);
	}

	if (doorOpen && thelevel == 4 && drawKid)
	{
		drawKid = false;
		Object* obj;
		obj = new Object({ 225 + 6, 125 - 32 + 8 }, ObjectType::KID);
		objects.push_back(obj);
	}

	if (doorOpen && thelevel == 1 && drawPotion)
	{
		drawPotion = false;
		Object* obj;
		obj = new Object({ 192,143 - 8 }, ObjectType::POTION);
		objects.push_back(obj);
	}
	if (drawGameKey && thelevel == 3)
	{

		if (player->pos.y < 120) 
		{
			drawGameKey = false;
			Object* obj;
			obj = new Object({ 48, 54}, ObjectType::KEY);
			objects.push_back(obj);
		}
		else 
		{
			Object* obj;
			obj = new Object({ 160, 134 }, ObjectType::KEY);
			objects.push_back(obj);
		}

	}
	if (drawGameKey && thelevel == 4) 
	{
		drawGameKey = false;
		Object* obj;
		obj = new Object({ 48 + 24 , 154 - 18 }, ObjectType::KEY);
		objects.push_back(obj);
	}


	if (timer > 0) 
	{
		timer--;

	}

	if (timer < 1 && timer > -1) 
	{
		objsEn.clear();
		timer = -1;
		if (player->IsLookingRight()) 
		{
			player->SetAnimation((int)PlayerAnim::IDLE_RIGHT);
		}
		else 
		{
			player->SetAnimation((int)PlayerAnim::IDLE_LEFT);
		}
	}

	if (attackHitbox) 
	{
		if (player->IsLookingLeft()) 
		{
			Object* obj;
			obj = new Object({ player->pos.x - 16 , player->pos.y - 4 }, ObjectType::ATTACKHITBOX);
			objsEn.push_back(obj);
			timer = 10;


		}
		if (player->IsLookingRight())
		{
			Object* obj;
			obj = new Object({ player->pos.x + 8 , player->pos.y - 4 }, ObjectType::ATTACKHITBOX);
			objsEn.push_back(obj);
			timer = 10;

		}
		attackHitbox = false;

	}

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);
	else if (IsKeyPressed(KEY_THREE))	LoadLevel(3);
	else if (IsKeyPressed(KEY_FOUR))	LoadLevel(4);

	if (player->GetLives() <= 0) 
	{
		death = true;
	}

	level->Update();
	player->Update();
	enemy->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
		enemy->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemy->DrawDebug(RED);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	enemy->Release();
	ClearLevel();
}

void Scene::CheckCollisions()
{
	AABB player_box, obj_box, enemy_box;
	bool myNextLevel = false;
	bool myPrevLevel = false;
	player_box = player->GetHitbox();
	enemy_box = enemy->GetHitbox();
	auto it = objects.begin();
	auto itEn = objsEn.begin();


	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();

		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());

			if ((*it)->NextLevel()) 
			{
				myNextLevel = true;
			}
			if ((*it)->PrevLevel()) 
			{
				myPrevLevel = true;
			}
			if ((*it)->isKey()) 
			{
				if ( !player->hasKey) 
				{
					player->IncrScore(200);

					Sound key = LoadSound("sounds/PickUpKey.wav");
					PlaySound(key);

					drawKey = true;
					player->hasKey = true;
				}
				else 
				{
					drawGameKey = true;
				}

			}
			if ((*it)->isMoney())
			{
				Sound key = LoadSound("sounds/PickUpKey.wav");
				PlaySound(key);
				drawItem = true;

			}
			if ((*it)->isVICTORY()) 
			{
				victory = true;
			}


			if ((*it)->isKeyHole()) 
			{
				if (drawKey) 
				{
					drawKeyLock = false;
				}

				drawKey = false;


			}
			if ((*it)->isPotion()) 
			{
				player->IncrScore(500);
				player->exp = 10;
				Sound key = LoadSound("sounds/PickUpKey.wav");
				PlaySound(key);
			}
			
			if ((*it)->isKid()) 
			{
				Sound key = LoadSound("sounds/PickUpKey.wav");
				PlaySound(key);
				player->hasKey = false;
				player->numKids++;
				numKids++;
			}

			delete* it;
	
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);

		}
		else
		{
			//Move to the next object
			++it; 
		}
	}

	AABB enemyBox;
	enemyBox = enemy->GetHitbox();
	if (player_box.TestAABB(enemyBox) && enemy->IsAlive()) 
	{
		player->TakeDamage();
	}

	while (itEn != objsEn.end())
	{
		obj_box = (*itEn)->GetHitbox();

		if (enemyBox.TestAABB(obj_box))
		{
			
			if ((*itEn)->isAttackHitbox()) 
			{
				enemy->hp--;
			}

			delete* itEn;

			//Erase the object from the vector and get the iterator to the next valid element
			itEn = objsEn.erase(itEn);

		}
		else
		{
			//Move to the next object
			++itEn;
		}
	}



	if (enemy->hp == 0) 
	{
		/*delete enemy;
		enemy = NULL;*/
		enemy->SetAlive(false);
	}


	if (myNextLevel) 
	{
		LoadLevel(nLevel + 1);
	}
	if (myPrevLevel)
	{
		LoadLevel(nLevel - 1);
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	for (Object* objEn : objsEn)
	{
		delete objEn;
	}
	objsEn.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
	for (Object* objEn : objsEn)
	{
		objEn->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
	for (Object* objEn : objsEn)
	{
		objEn->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	//DrawText(TextFormat("SCORE : %d", player->GetScore()), 10, 4, 8, LIGHTGRAY);
	//DrawText(TextFormat("LIVES : %d", player->GetLives()), 200, 4, 8, LIGHTGRAY);

	int n = 11;

	myfont->Draw(10, 0,TextFormat("HI-%06d",player->GetScore()));
	myfont->Draw(10, 8,TextFormat("1P-%06d",player->GetScore()));

	//myfont->Draw(0, 60, TextFormat("NUMKIDS: %d", player->numKids));

	myfont->Draw(88, 0, TextFormat("VIT"));
	myfont->Draw(88, 8, TextFormat("EXP"));

	for (size_t i = 0; i < player->GetLives() ; i++)
	{
		int xOffset = 120 + i * n;
		myfont->Draw(xOffset , 0, TextFormat("#"), VITBLUE);
		myfont->Draw(xOffset + 8, 0, TextFormat("$"), VITBLUE);
	}

	if (player->GetLives() == 7) 
	{
		myfont->Draw(197, 0, TextFormat("$"), VITBLUE);
	}

	int m = 8;


	for (size_t i = 0; i < player->GetExp(); i++)
	{
		int xOffset = 120 + i * m;
		myfont->Draw(xOffset, 8, TextFormat("#"), EXPGREEN);
		//myfont->Draw(xOffset + 8, 8, TextFormat("%"), GREEN);
	}


	//myfont->Draw(120  /*+i * n*/, 0, TextFormat("#"), VITBLUE);
	//myfont->Draw(120 + 3  /*+i * n*/, 0, TextFormat("#"), VITBLUE);


	myfont->Draw(210, 0, TextFormat("SCENE"));
	myfont->Draw(210, 8, TextFormat("01-0%d", thelevel));

	for (size_t i = 0; i < 400; i++)
	{
		myfont->Draw(-1 + i, 172, TextFormat("-"));
	}

	myfont->Draw(208, 176, TextFormat("-"));

	if (doorOpen && thelevel == 1)
	{
		player->hasKey = false;
	}


	

	//DrawText(TextFormat("TIMER : %d", player->attack_timer), 100, 4, 8, LIGHTGRAY);
}
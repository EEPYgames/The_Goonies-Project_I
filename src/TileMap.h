#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>


enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,
	FALSEAIR = 888,

	// 0 < id < 50: static tiles
	//BLOCK_SQUARE1_TL = 1, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
	//BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
	//BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
	//BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	//BLOCK_BEAM_L, BLOCK_BEAM_R,
	NEXTLEVEL = 666,
	PREVLEVEL = 667,
	VICTORY = 668,
	KEYHOLE = 303,

	A1 = 1, A3 = 3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17,
	A18, A21 = 21, A22, A23, A24, A25, A26, A27, A28, A29, A32 = 32, A33,
	A34, A35, A36, A37, A38, A39, A40, A41, A42, A43, A44, A45, A46, A47, A48, A49,
	A50, A51, A52, A53, A54, A55, A56, A57, A58, A59, A60, A61, A62, A63, A64 = 64, A65,
	A66, A67, A68, A69, A70, A71, A72, A73, A74, A75, A76, A77, A78, A79, A80, A81,
	A82, A83, A84, A85, A86, A87, A88, A89, A90, A91, A92, A93, A94, A95, A96, A97,
	A98, A99, A100, A101, A102, A103, A104, A105, A106, A107, A108, A109, A110, A111,
	A112, A113, A114, A115, A116, A117, A118, A119, A120, A121, A122, A123, A124, A125,
	A126, A127, A128, A129, A130, A131, A132, A133, A134, A135, A136, A137, A138, A139,
	A140, A141, A142, A143, A144, A145, A146, A147, A148, A149, A150, A151, A152, A153,
	A154, A155, A156, A157, A158, A159, A160, A161,

	A179=179, A180, A181, A182,

	A201 = 201, A202, A203, A204, A205, A206, A207, A208, A209, A210, A211,	A212, A213,
	A214, A215, A216, A217, A218,

	P1 = 30, P2 = 31,

	LADDER_BOTTOM = 2, LADDER = 20, LADDER_TOP = 19,
	LOCK_RED = 3000, LOCK_YELLOW,
	LASER_L = 4000, LASER_R,

	//Items

	KEY = 301,
	MONEYBAG = 302,
	KIDHEAD = 307,

	//Special tiles
	DOOR = 5000,
	KEY_RED = 6000, YELLOW_KEY, ITEM_APPLE, ITEM_CHILI,
	LASER = 7000, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,

	// id >= 100: entities' initial locations
	PLAYER = 1000,
	ENEMY = 500,

	//Intervals
	STATIC_FIRST = A1,
	STATIC_LAST = LASER_R,
	SOLID_FIRST = A7,
	SOLID_LAST = A161,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = LASER,
	ENTITY_FIRST = ENEMY,
	ENTITY_LAST = PLAYER,

	SOLID2 = A17, 

	SOLID3 = A22, 
	SOLID4 = A22,

	SOLID5 = P1,
	SOLID6 = P2,

	SOLID7 = A100, 
	SOLID8 = A104,

	SOLID9 = A88,
	SOLID10 = A89,

	SOLID11 = A130,
	SOLID12 = A136,

	SOLID13 = 888, 
	SOLID14 = 888,

	KID, POTION

};

class TileMap
{
public:

	TileMap();
	~TileMap();




	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;


	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	bool TestCollisionCeiling(const AABB& box, int* py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

	bool TestOnLevel(const AABB& box, int* px) const;


private:

	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;
	bool IsChangeLevel(Tile tile)const;
	void drawkid();
	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite *laser;
	
	//Tile sheet
	const Texture2D *img_tiles;
};


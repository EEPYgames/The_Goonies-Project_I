#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>
#include "StaticImage.h"
#include <iostream>
#include "Scene.h"

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	laser = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (laser != nullptr)
	{
		laser->Release();
		delete laser;
		laser = nullptr;
	}
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	//dict_rect[(int)Tile::BLOCK_SQUARE1_TL] = { 0,  0, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE1_TR] = { n,  0, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE1_BL] = { 0, n, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE1_BR] = { n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE2_TL] = { 2 * n,  0, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE2_TR] = { 3 * n,  0, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE2_BL] = { 2 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_SQUARE2_BR] = { 3 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_VERT2_T] = { 4 * n,  0, n, n };
	//dict_rect[(int)Tile::BLOCK_VERT2_B] = { 4 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_HORIZ2_L] = { 5 * n, 0, n, n };
	//dict_rect[(int)Tile::BLOCK_HORIZ2_R] = { 6 * n, 0, n, n };
	//dict_rect[(int)Tile::BLOCK_BLUE] = { 7 * n, 0, n, n };
	//dict_rect[(int)Tile::BLOCK_HORIZ3_L] = { 5 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_HORIZ3_M] = { 6 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_HORIZ3_R] = { 7 * n, n, n, n };
	//dict_rect[(int)Tile::BLOCK_BEAM_L] = { 0, 2 * n, n, n };
	//dict_rect[(int)Tile::BLOCK_BEAM_R] = { n, 2 * n, n, n };


	dict_rect[(int)Tile::A1] = { 0,  2 * n, n, n };
	dict_rect[(int)Tile::A3] = { n * 2, 2 * n, n, n };
	dict_rect[(int)Tile::A4] = { n * 3, 2 * n, n, n };
	dict_rect[(int)Tile::A5] = { n * 4, 2 * n, n, n };
	dict_rect[(int)Tile::A6] = { n * 5, 2 * n, n, n };
	dict_rect[(int)Tile::A7] = { n * 6, 2 * n, n, n };
	dict_rect[(int)Tile::A8] = { n * 7, 2 * n, n, n };
	dict_rect[(int)Tile::A9] = { n * 8, 2 * n, n, n };
	dict_rect[(int)Tile::A10] = { n * 9, 2 * n, n, n };
	dict_rect[(int)Tile::A11] = { n * 10, 2 * n, n, n };
	dict_rect[(int)Tile::A12] = { n * 11, 2 * n, n, n };
	dict_rect[(int)Tile::A13] = { n * 12, 2 * n, n, n };
	dict_rect[(int)Tile::A14] = { n * 13, 2 * n, n, n };
	dict_rect[(int)Tile::A15] = { n * 14, 2 * n, n, n };
	dict_rect[(int)Tile::A16] = { n * 15, 2 * n, n, n };
	dict_rect[(int)Tile::A17] = { n * 16, 2 * n, n, n };
	dict_rect[(int)Tile::A18] = { n * 18, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP] = { 19 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER] = { 20 * n, 2 * n, n, n };
	dict_rect[(int)Tile::A21] = { n * 21, 2 * n, n, n };
	dict_rect[(int)Tile::A22] = { n * 22, 2 * n, n, n };
	dict_rect[(int)Tile::A23] = { n * 23, 2 * n, n, n };
	dict_rect[(int)Tile::A24] = { n * 24, 2 * n, n, n };
	dict_rect[(int)Tile::A25] = { n * 25, 2 * n, n, n };
	dict_rect[(int)Tile::A26] = { n * 26, 2 * n, n, n };
	dict_rect[(int)Tile::A27] = { n * 27, 2 * n, n, n };
	dict_rect[(int)Tile::A28] = { n * 28, 2 * n, n, n };
	dict_rect[(int)Tile::A29] = { n * 29, 2 * n, n, n };
	dict_rect[(int)Tile::P1] = { 30 * n,  2 * n, n, n };
	dict_rect[(int)Tile::P2] = { 31 * n,  2 * n, n, n };
	dict_rect[(int)Tile::A32] = { n * 0, 3 * n, n, n };
	dict_rect[(int)Tile::A33] = { n * 1, 3 * n, n, n };
	dict_rect[(int)Tile::A34] = { n * 2, 3 * n, n, n };
	dict_rect[(int)Tile::A35] = { n * 3, 3 * n, n, n };
	dict_rect[(int)Tile::A36] = { n * 4, 3 * n, n, n };
	dict_rect[(int)Tile::A37] = { n * 5, 3 * n, n, n };
	dict_rect[(int)Tile::A38] = { n * 7, 3 * n, n, n };
	dict_rect[(int)Tile::A39] = { n * 9, 3 * n, n, n };
	dict_rect[(int)Tile::A40] = { n * 10, 3 * n, n, n };
	dict_rect[(int)Tile::A41] = { n * 11, 3 * n, n, n };
	dict_rect[(int)Tile::A42] = { n * 12, 3 * n, n, n };
	dict_rect[(int)Tile::A43] = { n * 13, 3 * n, n, n };
	dict_rect[(int)Tile::A44] = { n * 14, 3 * n, n, n };
	dict_rect[(int)Tile::A45] = { n * 15, 3 * n, n, n };
	dict_rect[(int)Tile::A46] = { n * 20, 3 * n, n, n };
	dict_rect[(int)Tile::A47] = { n * 21, 3 * n, n, n };
	dict_rect[(int)Tile::A48] = { n * 22, 3 * n, n, n };
	dict_rect[(int)Tile::A49] = { n * 23, 3 * n, n, n };
	dict_rect[(int)Tile::A50] = { n * 24, 3 * n, n, n };
	dict_rect[(int)Tile::A51] = { n * 25, 3 * n, n, n };
	dict_rect[(int)Tile::A52] = { n * 26, 3 * n, n, n };
	dict_rect[(int)Tile::A53] = { n * 27, 3 * n, n, n };
	dict_rect[(int)Tile::A54] = { n * 28, 3 * n, n, n };
	dict_rect[(int)Tile::A55] = { n * 29, 3 * n, n, n };
	dict_rect[(int)Tile::A56] = { n * 30, 3 * n, n, n };
	dict_rect[(int)Tile::A57] = { n * 31, 3 * n, n, n };
	dict_rect[(int)Tile::A58] = { 0, 4 * n, n, n };
	dict_rect[(int)Tile::A59] = { n , 4 * n, n, n };
	dict_rect[(int)Tile::A60] = { n * 2, 4 * n, n, n };
	dict_rect[(int)Tile::A61] = { n * 3, 4 * n, n, n };
	dict_rect[(int)Tile::A61] = { n * 4, 4 * n, n, n };
	dict_rect[(int)Tile::A63] = { n * 5, 4 * n, n, n };


	//dict_rect[(int)Tile::A64] = { n * 6, 5 * n, n, n };
	dict_rect[(int)Tile::A65] = { n * 7, 5 * n, n, n };
	dict_rect[(int)Tile::A66] = { n * 8, 5 * n, n, n };
	dict_rect[(int)Tile::A67] = { n * 9, 5 * n, n, n };
	dict_rect[(int)Tile::A68] = { n * 10, 5 * n, n, n };
	dict_rect[(int)Tile::A69] = { n * 11, 5 * n, n, n };
	dict_rect[(int)Tile::A70] = { n * 12, 5 * n, n, n };
	dict_rect[(int)Tile::A71] = { n * 13, 5 * n, n, n };
	dict_rect[(int)Tile::A72] = { n * 14, 5 * n, n, n };
	dict_rect[(int)Tile::A73] = { n * 15, 5 * n, n, n };
	dict_rect[(int)Tile::A74] = { n * 16, 5 * n, n, n };
	dict_rect[(int)Tile::A75] = { n * 17, 5 * n, n, n };
	dict_rect[(int)Tile::A76] = { n * 18, 5 * n, n, n };
	dict_rect[(int)Tile::A77] = { n * 19, 5 * n, n, n };
	dict_rect[(int)Tile::A78] = { n * 20, 5 * n, n, n };
	dict_rect[(int)Tile::A79] = { n * 21, 5 * n, n, n };
	dict_rect[(int)Tile::A80] = { n * 22, 5 * n, n, n };
	dict_rect[(int)Tile::A81] = { n * 23, 5 * n, n, n };
	dict_rect[(int)Tile::A82] = { n * 24, 5 * n, n, n };
	dict_rect[(int)Tile::A83] = { n * 25, 5 * n, n, n };
	dict_rect[(int)Tile::A84] = { n * 26, 5 * n, n, n };
	dict_rect[(int)Tile::A85] = { n * 27, 5 * n, n, n };
	dict_rect[(int)Tile::A86] = { n * 28, 5 * n, n, n };
	dict_rect[(int)Tile::A87] = { n * 29, 5 * n, n, n };
	dict_rect[(int)Tile::A88] = { n * 30, 5 * n, n, n };
	dict_rect[(int)Tile::A89] = { n * 31, 5 * n, n, n };
	dict_rect[(int)Tile::A90] = { n * 32, 5 * n, n, n };
	dict_rect[(int)Tile::A91] = { n * 33, 5 * n, n, n };
	dict_rect[(int)Tile::A92] = { n * 34, 5 * n, n, n };
	dict_rect[(int)Tile::A93] = { n * 35, 5 * n, n, n };
	dict_rect[(int)Tile::A94] = { n * 36, 5 * n, n, n };
	dict_rect[(int)Tile::A95] = { n * 37, 5 * n, n, n };
	dict_rect[(int)Tile::A96] = { n * 38, 5 * n, n, n };
	dict_rect[(int)Tile::A97] = { n * 39, 5 * n, n, n };
	dict_rect[(int)Tile::A98] = { n * 40, 5 * n, n, n };
	dict_rect[(int)Tile::A99] = { n * 41, 5 * n, n, n };
	dict_rect[(int)Tile::A100] = { n * 0, 6 * n, n, n };
	dict_rect[(int)Tile::A101] = { n * 1, 6 * n, n, n };
	dict_rect[(int)Tile::A102] = { n * 2, 6 * n, n, n };
	dict_rect[(int)Tile::A103] = { n * 3, 6 * n, n, n };
	dict_rect[(int)Tile::A104] = { n * 4, 6 * n, n, n };
	dict_rect[(int)Tile::A105] = { n * 5, 6 * n, n, n };
	dict_rect[(int)Tile::A106] = { n * 6, 6 * n, n, n };
	dict_rect[(int)Tile::A107] = { n * 7, 6 * n, n, n };
	dict_rect[(int)Tile::A108] = { n * 8, 6 * n, n, n };
	dict_rect[(int)Tile::A109] = { n * 9, 6 * n, n, n };
	dict_rect[(int)Tile::A110] = { n * 10, 6 * n, n, n };
	dict_rect[(int)Tile::A111] = { n * 11, 6 * n, n, n };
	dict_rect[(int)Tile::A112] = { n * 12, 6 * n, n, n };
	dict_rect[(int)Tile::A113] = { n * 13, 6 * n, n, n };
	dict_rect[(int)Tile::A114] = { n * 14, 6 * n, n, n };
	dict_rect[(int)Tile::A115] = { n * 15, 6 * n, n, n };
	dict_rect[(int)Tile::A116] = { n * 16, 6 * n, n, n };
	dict_rect[(int)Tile::A117] = { n * 17, 6 * n, n, n };
	dict_rect[(int)Tile::A118] = { n * 18, 6 * n, n, n };
	dict_rect[(int)Tile::A119] = { n * 21, 6 * n, n, n };
	dict_rect[(int)Tile::A120] = { n * 22, 6 * n, n, n };
	dict_rect[(int)Tile::A121] = { n * 23, 6 * n, n, n };
	dict_rect[(int)Tile::A122] = { n * 24, 6 * n, n, n };
	dict_rect[(int)Tile::A123] = { n * 25, 6 * n, n, n };
	dict_rect[(int)Tile::A124] = { n * 26, 6 * n, n, n };
	dict_rect[(int)Tile::A125] = { n * 27, 6 * n, n, n };
	dict_rect[(int)Tile::A126] = { n * 28, 6 * n, n, n };
	dict_rect[(int)Tile::A127] = { n * 29, 6 * n, n, n };
	dict_rect[(int)Tile::A128] = { n * 30, 6 * n, n, n };
	dict_rect[(int)Tile::A129] = { n * 31, 6 * n, n, n };
	dict_rect[(int)Tile::A130] = { n * 0, 7 * n, n, n };
	dict_rect[(int)Tile::A131] = { n * 1, 7 * n, n, n };
	dict_rect[(int)Tile::A132] = { n * 2, 7 * n, n, n };
	dict_rect[(int)Tile::A133] = { n * 3, 7 * n, n, n };
	dict_rect[(int)Tile::A134] = { n * 4, 7 * n, n, n };
	dict_rect[(int)Tile::A135] = { n * 5, 7 * n, n, n };
	dict_rect[(int)Tile::A136] = { n * 6, 7 * n, n, n };
	dict_rect[(int)Tile::A137] = { n * 7, 7 * n, n, n };
	dict_rect[(int)Tile::A138] = { n * 8, 7 * n, n, n };
	dict_rect[(int)Tile::A139] = { n * 9, 7 * n, n, n };
	dict_rect[(int)Tile::A140] = { n * 10, 7 * n, n, n };
	dict_rect[(int)Tile::A141] = { n * 11, 7 * n, n, n };
	dict_rect[(int)Tile::A142] = { n * 12, 7 * n, n, n };
	dict_rect[(int)Tile::A143] = { n * 13, 7 * n, n, n };
	dict_rect[(int)Tile::A144] = { n * 14, 7 * n, n, n };
	dict_rect[(int)Tile::A145] = { n * 15, 7 * n, n, n };
	dict_rect[(int)Tile::A146] = { n * 16, 7 * n, n, n };
	dict_rect[(int)Tile::A147] = { n * 17, 7 * n, n, n };
	dict_rect[(int)Tile::A148] = { n * 18, 7 * n, n, n };
	dict_rect[(int)Tile::A149] = { n * 19, 7 * n, n, n };
	dict_rect[(int)Tile::A150] = { n * 20, 7 * n, n, n };
	dict_rect[(int)Tile::A151] = { n * 21, 7 * n, n, n };
	dict_rect[(int)Tile::A152] = { n * 22, 7 * n, n, n };
	dict_rect[(int)Tile::A153] = { n * 23, 7 * n, n, n };
	dict_rect[(int)Tile::A154] = { n * 24, 7 * n, n, n };
	dict_rect[(int)Tile::A155] = { n * 25, 7 * n, n, n };
	dict_rect[(int)Tile::A156] = { n * 26, 7 * n, n, n };
	dict_rect[(int)Tile::A157] = { n * 27, 7 * n, n, n };
	dict_rect[(int)Tile::A158] = { n * 28, 7 * n, n, n };
	dict_rect[(int)Tile::A159] = { n * 29, 7 * n, n, n };
	dict_rect[(int)Tile::A160] = { n * 30, 7 * n, n, n };
	dict_rect[(int)Tile::A161] = { n * 31, 7 * n, n, n };

	dict_rect[(int)Tile::A179] = { n * 5, 9 * n, n, n };
	dict_rect[(int)Tile::A180] = { n * 5, 10 * n, n, n };
	dict_rect[(int)Tile::A181] = { n * 6, 9 * n, n, n };
	dict_rect[(int)Tile::A182] = { n * 6, 10 * n, n, n };

	dict_rect[(int)Tile::A201] = { n * 6, 8 * n, n, n };
	dict_rect[(int)Tile::A202] = { n * 7, 8 * n, n, n };
	dict_rect[(int)Tile::A203] = { n * 8, 8 * n, n, n };
	dict_rect[(int)Tile::A204] = { n * 9, 8 * n, n, n };
	dict_rect[(int)Tile::A205] = { n * 10, 8 * n, n, n };
	dict_rect[(int)Tile::A206] = { n * 11, 8 * n, n, n };
	dict_rect[(int)Tile::A207] = { n * 12, 8 * n, n, n };
	dict_rect[(int)Tile::A208] = { n * 13, 8 * n, n, n };
	dict_rect[(int)Tile::A209] = { n * 14, 8 * n, n, n };
	dict_rect[(int)Tile::A210] = { n * 15, 8 * n, n, n };

	dict_rect[(int)Tile::KIDHEAD] = { n * 6, 11 * n, n, n };

	dict_rect[(int)Tile::FALSEAIR] = { 0, 0, n, n };
	dict_rect[(int)Tile::LADDER_BOTTOM] = { n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER] = { 20 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LADDER_TOP] = { 19 * n, 2 * n, n, n };


	dict_rect[(int)Tile::LOCK_RED] = { 6 * n, 2 * n, n, n };
	dict_rect[(int)Tile::LOCK_YELLOW] = { 7 * n, 2 * n, n, n };

	dict_rect[(int)Tile::LASER_L] = { 0, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_R] = { 4 * n, 6 * n, n, n };
	
	dict_rect[(int)Tile::LASER_FRAME0] = { 1 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME1] = { 2 * n, 6 * n, n, n };
	dict_rect[(int)Tile::LASER_FRAME2] = { 3 * n, 6 * n, n, n };

	//dict_rect[(int)Tile::MONEYBAG] = { n , 11 * n, n, n };
}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/GooniesTilemap.png") != AppStatus::OK)
	{
		return AppStatus::EROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	laser = new Sprite(img_tiles);
	if (laser == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::EROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::EROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::Update()
{
	laser->Update();
	

}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y);
			return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID2 
		|| Tile::SOLID3 <= tile && tile <= Tile::SOLID4
		|| Tile::SOLID5 <= tile && tile <= Tile::SOLID6
		|| Tile::SOLID7 <= tile && tile <= Tile::SOLID8
		|| Tile::SOLID9 <= tile && tile <= Tile::SOLID10
		|| Tile::SOLID11 <= tile && tile <= Tile::SOLID12
		|| Tile :: SOLID13 <= tile && tile <= Tile::SOLID14);
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::LADDER_TOP;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::LADDER || tile == Tile::LADDER_BOTTOM ;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestCollisionCeiling (const AABB& box, int* py) const
{
	Point p(box.pos.x, *py - 4);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE + 1;
		return true;
	}
	return false;
}
;
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}
bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;
	
	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width-1;
	bottom = box.pos.y + box.height-1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadder(tile1) && IsTileLadder(tile2))
	{
		*px = GetLadderCenterPos(left, bottom)- box.width/4;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2, tile3;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	tile3 = GetTileIndex((tx1 + tx2) / 2, ty);

	//if (IsTileLadderTop(tile1) && IsTileLadderTop(tile2))
	//{
	//	*px = GetLadderCenterPos(left, bottom) - box.width;
	//	return true;
	//}

	if (IsTileLadderTop(tile3))
	{
		*px = GetLadderCenterPos(left, bottom);
		//- box.width;
		return true;
	}

	return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;
	
	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	//if (tile == Tile::LADDER_L || tile == Tile::LADDER_TOP_L)		return tx * TILE_SIZE + TILE_SIZE;
	//else if (tile == Tile::LADDER_R || tile == Tile::LADDER_TOP_R)	return tx * TILE_SIZE;
	//else
	//{
	//	LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
	//	return 0;
	//}


	return tx * TILE_SIZE+ TILE_SIZE/3;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::LASER)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}

	if (drawItem) 
	{
		const int n = TILE_SIZE;
		rc = { 2*n, 12 * n, 16, 16 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(0, 176);
	}
	if (drawKey) 
	{
		const int n = TILE_SIZE;
		rc = { 0, 11 * n, 8, 8 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(200, 184);
	}


	
	if (thelevel == 1) 
	{
		if (drawKeyLock) 
		{
			const int n = TILE_SIZE;
			rc = { 2 * n, 11 * n, 8, 8 };
			ResourceManager& data = ResourceManager::Instance();
			RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
			render->Draw(176, 128);
		}

	}
	else if (thelevel == 3) 
	{
		if (drawKeyLock)
		{
			const int n = TILE_SIZE;
			rc = { 2 * n, 11 * n, 8, 8 };
			ResourceManager& data = ResourceManager::Instance();
			RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
			render->Draw(16, 128);
		}
	}
	else if (thelevel == 4) 
	{
		if (drawKeyLock)
		{
			const int n = TILE_SIZE;
			rc = { 2 * n, 11 * n, 8, 8 };
			ResourceManager& data = ResourceManager::Instance();
			RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
			render->Draw(228 - 16 + 4, 116 - 28 + 8);
		}
	}


	if (doorOpen && thelevel == 1) 
	{
		const int n = TILE_SIZE;
		rc = { 30 * n, 0 * n, 16, 16 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(185, 125);
		render->Draw(191, 125);
		render->Draw(185, 124);
		render->Draw(191, 124);
	}
	if (doorOpen && thelevel == 3)
	{
		const int n = TILE_SIZE;
		rc = { 30 * n, 0 * n, 16, 16 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(25, 125);
		render->Draw(31, 125);
		render->Draw(25, 124);
		render->Draw(31, 124);
	}
	if (doorOpen && thelevel == 4)
	{
		const int n = TILE_SIZE;
		rc = { 30 * n, 0 * n, 16, 16 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(225, 125 - 32);
		render->Draw(231, 125 - 32);
		render->Draw(225, 124 - 32);
		render->Draw(231, 124 - 32);
	}

	if (numKids == 1)
	{
		const int n = TILE_SIZE;
		rc = { 6 * n, 11 * n, 8, 8 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(200 + 16, 184 - 8);
	}
	else if (numKids == 2)
	{
		const int n = TILE_SIZE;
		rc = { 6 * n, 11 * n, 8, 8 };
		ResourceManager& data = ResourceManager::Instance();
		RenderComponent* render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
		render->Draw(200 + 16, 184 - 8);
		render->Draw(200 + 16 + 8, 184 - 8);
	}

}

bool TileMap::IsChangeLevel(Tile tile) const
{
	return tile == Tile::NEXTLEVEL;
}

bool TileMap::TestOnLevel(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);

	if (IsChangeLevel(tile1) || IsChangeLevel(tile2))
	{
		return true;
	}
	return false;
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	laser->Release();

	dict_rect.clear();
}
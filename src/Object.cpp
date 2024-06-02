#include "Object.h"
#include "StaticImage.h"
#include "TileMap.h"

Object::Object(const Point& p, ObjectType t) : Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::APPLE: rc = {4*n, 3*n, n, n}; break;
		case ObjectType::CHILI: rc = {5*n, 3*n, n, n}; break;
		case ObjectType::MONEYBAG: rc = { n , 11 * n, n, n }; break;
		case ObjectType::KEY: rc = { 0 , 11 * n, n, n }; break;
		case ObjectType::KEYHOLE: rc = { 2*n , 11 * n, n, n }; break;
		case ObjectType :: NEXTLEVEL:  rc = { 0 , 0, n, n }; break;
		case ObjectType::PREVLEVEL:  rc = { 0 , 0, n, n }; break;
		case ObjectType::VICTORY:  rc = { 0 , 0, n, n }; break;
		case ObjectType::MOONYELLOWTHINGIE: rc = { n , 11 * n, n, n }; break;
		case ObjectType::KID: rc = { 3*n , 9 * n, n, 16 }; break;
		case ObjectType::POTION: rc = { 3 * n , 11 * n, n, n }; break;
		case ObjectType::ATTACKHITBOX: rc = { 8 * n , 11 * n, n, n }; break;
		default: LOG("Internal error: object creation of invalid type");
	}



	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
int Object::Points() const
{
	if (type == ObjectType::APPLE)		return POINTS_APPLE;
	else if (type == ObjectType::CHILI)	return POINTS_CHILI;

	else if (type == ObjectType::MONEYBAG)	return POINTS_MONEY;
	else if (type == ObjectType::KEY)	return POINTS_KEY;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}
bool Object::NextLevel() const
{
	return type == ObjectType::NEXTLEVEL;
}
bool Object::PrevLevel() const
{
	return type == ObjectType::PREVLEVEL;
}
bool Object::isKeyHole() const
{
	return type == ObjectType::KEYHOLE;
}
bool Object::isKey() const
{
	return type == ObjectType::KEY;
}
bool Object::isMoney() const
{
	return type == ObjectType::MONEYBAG;
}
bool Object::isVICTORY() const
{
	return type == ObjectType::VICTORY;
}
bool Object::isKid() const
{
	return type == ObjectType::KID;
}
bool Object::isPotion() const
{
	return type == ObjectType::POTION;
}
bool Object::isAttackHitbox() const
{
	return type == ObjectType::ATTACKHITBOX;
}
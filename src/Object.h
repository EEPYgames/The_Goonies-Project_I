#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	8
#define OBJECT_FRAME_SIZE		8

#define POINTS_APPLE	10
#define POINTS_CHILI	20
#define POINTS_MONEY	0
#define POINTS_KEY 0

enum class ObjectType { APPLE, CHILI, MONEYBAG, KEY, NEXTLEVEL, PREVLEVEL, KEYHOLE, VICTORY, 
MOONYELLOWTHINGIE, KID, POTION, ATTACKHITBOX};

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;
	bool NextLevel() const;
	bool PrevLevel()const;
	bool isKeyHole()const;
	bool isKey()const;
	bool isMoney()const;
	bool isVICTORY()const;
	bool isKid()const;
	bool isPotion()const;
	bool isAttackHitbox()const;

	Rectangle rc;
private:
	ObjectType type;
};


#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 32x32
#define PLAYER_FRAME_SIZE		16

//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	5
#define PLAYER_PHYSICAL_HEIGHT	16

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1
//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(4*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		6

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, JUMPING, FALLING, CLIMBING, DEAD, ATTACKING };
enum class Look { RIGHT, LEFT };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING, CLIMBING_PRE_TOP, CLIMBING_TOP,
	SHOCK_LEFT, SHOCK_RIGHT,
	TELEPORT_LEFT, TELEPORT_RIGHT, ATTACK_RIGHT, ATTACK_LEFT,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	bool drawItem = false;

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();
	void InitLives();
	void TakeDamage();
	int GetLives();
	int GetExp();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	bool hasKey;

	int timer = 0;
	int attack_timer = 0;
	int expTimer = 0;
	int exp = 0;
	int numKids = 0;

	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	void SetAnimation(int id);

private:


	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicClimbing();
	void Debug();

	//Animation management

	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void StartClimbingUp();
	void StartClimbingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void Attack();
	void AttackRight();
	void AttackLeft();
	bool AttackingRight;
	bool AttackingLeft;

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;

	TileMap *map;

	int score;
	int lives = 7;


};


#define ENEMY_FRAME_SIZE		10
//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	8
#define ENEMY_PHYSICAL_HEIGHT	8

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED			2

enum class EnemyAnim { WALKINGLEFT, WALKINGRIGHT, NUM_ANIMATIONS };
 class Enemy : public Entity
{
public:
	Enemy(const Point& p, State s, Look view);
	~Enemy();
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	int hp = 1;

private:
	State state;
	Look look;
	TileMap* map;

	EnemyAnim GetAnimation();
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	void Move();
    bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	void SetAnimation(int id);
	void WalkRight();
	void WalkLeft();
	void fall();

};


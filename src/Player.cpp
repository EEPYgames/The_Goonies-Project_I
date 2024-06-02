#pragma once
#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "Scene.h"
#include <raymath.h>
#include <iostream>
#include "StaticImage.h"
using namespace std;

Player::Player(const Point& p, State s, Look view) :
Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = PLAYER_JUMP_DELAY;
	map = nullptr;
	score = 0;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/PlayerGoonies.png") != AppStatus::OK)
	{
		return AppStatus::EROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::EROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 0, 2 * n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)i*n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)i*n, 2*n, n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_RIGHT, { 0, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::FALLING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::FALLING_LEFT, { 0, 2 * n, n, n });


	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_RIGHT, { 2*n, n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::JUMPING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::JUMPING_LEFT, { 2*n, 2*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_RIGHT, { n, 5*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::LEVITATING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::LEVITATING_LEFT, { n, 5*n, -n, n });

	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING, ANIM_LADDER_DELAY);
	for (i = 0; i < 1; ++i)
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { 0, 0, n, n });
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING, { n, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_PRE_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_PRE_TOP, { 0, 0, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::CLIMBING_TOP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::CLIMBING_TOP, { 0, 0, n, n });
		
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACK_RIGHT, { 0, 3*n, n, n });
	sprite->SetAnimationDelay((int)PlayerAnim::ATTACK_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::ATTACK_LEFT, { n, 3 * n, n, n });


	sprite->SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	timer = 0;
	return AppStatus::OK;
}
void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
int Player::GetScore()
{
	return score;
}
void Player::InitLives()
{
	lives = 7;
}
int Player::GetLives()
{
	return lives;
}
int Player::GetExp()
{
	return exp;
}


void Player::TakeDamage()
{
	Sound hit = LoadSound("sounds/Hit.wav");
	if (timer <= 0) 
	{
		lives -= 1;
		PlaySound(hit);
		timer = 100;
	}

	if (lives < 0) 
	{
		lives = 0;
	}
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsAscending() const
{
	return dir.y < -PLAYER_LEVITATING_SPEED;
}
bool Player::IsLevitating() const
{
	return abs(dir.y) <= PLAYER_LEVITATING_SPEED;
}
bool Player::IsDescending() const
{
	return dir.y > PLAYER_LEVITATING_SPEED;
}
bool Player::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Player::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE/2;
}
void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = State::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else					SetAnimation((int)PlayerAnim::IDLE_LEFT);
}
void Player::StartWalkingLeft()
{
	state = State::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = State::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartFalling()
{
	dir.y = PLAYER_SPEED;
	state = State::FALLING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
	else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
}
void Player::StartJumping()
{
	Sound jump = LoadSound("sounds/Jump.wav");
	PlaySound(jump);
	dir.y = -PLAYER_JUMP_FORCE;
	state = State::JUMPING;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
	else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
	jump_delay = PLAYER_JUMP_DELAY;
}
void Player::StartClimbingUp()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::StartClimbingDown()
{
	state = State::CLIMBING;
	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
	pos.y += PLAYER_LADDER_SPEED;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetManualMode();
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case State:: ATTACKING:  SetAnimation((int)PlayerAnim::ATTACK_RIGHT);break;
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break; 
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_RIGHT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		case State::ATTACKING:  SetAnimation((int)PlayerAnim::ATTACK_LEFT);break;
		case State::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
		case State::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
		case State::JUMPING: SetAnimation((int)PlayerAnim::JUMPING_LEFT); break;
		case State::FALLING: SetAnimation((int)PlayerAnim::FALLING_LEFT); break;
	}
}
void Player::Debug() 
{
	if (IsKeyPressed(KEY_E))
	{
		if (IsKeyDown(KEY_LEFT_SHIFT)) 
		{
			exp = 10;
		}
		else 
		{
			exp++;
		}

	}
	if (IsKeyPressed(KEY_BACKSPACE)) 
	{
		lives--;
	}
	if (IsKeyPressed(KEY_H)) 
	{
		if (IsKeyDown(KEY_LEFT_SHIFT)) 
		{
			lives = 7;
		}
		else 
		{
			if (lives < 7)
			{
				lives++;
			}
		}

	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();
	Attack();
	Debug();


	if (thelevel == 1 && pos.x == 176 && pos.y == 143 && hasKey) 
	{
		drawKeyLock = false;
		drawKey = false;
		doorOpen = true;
		
	}
	if (thelevel == 3 && pos.x == 18 && pos.y == 143 && hasKey)
	{
		drawKeyLock = false;
		drawKey = false;
		doorOpen = true;
	}
	if (thelevel == 4 && pos.x == 228 - 16 + 4 && pos.y == 111 && hasKey)
	{
		drawKeyLock = false;
		drawKey = false;
		doorOpen = true;
	}

	int deltaTime = GetFrameTime() * 100;

	if (timer <= 100 && timer >= 0) 
	{
		timer -= deltaTime;
	}
	if (expTimer <= 10000 && expTimer >= 0) 
	{
		expTimer -= deltaTime;
	}

	if (attack_timer <= 100 && attack_timer >= 0) 
	{
		if (IsLookingRight())
		{
			AttackingRight = true;
		}
		if (IsLookingLeft())
		{
			AttackingLeft = true;
		}
		attack_timer -= deltaTime;

	}
	else 
	{
		AttackingLeft = false;
		AttackingRight = false;
	}

	//if (AttackingRight) 
	//{
	//	attack_timer = 100;
	//	

	//	if (attack_timer <= 100 && attack_timer >= 0)
	//	{
	//		SetAnimation((int)PlayerAnim::ATTACK_RIGHT);
	//	}
	//	else
	//	{
	//		SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	//		AttackingRight = false;
	//	}
	//}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;

	//We can only go up and down while climbing
	if (state == State::CLIMBING)	return;

	if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) )
	{
		pos.x += -PLAYER_SPEED ;
		if (state == State::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED ;
		if (state == State::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == State::WALKING) Stop();
		}
	}
	else
	{
		if (state == State::WALKING) Stop();
	}
}
void Player::MoveY()
{
	AABB box;

	if (state == State::JUMPING)
	{
		LogicJumping();
	}
	else if (state == State::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += PLAYER_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == State::FALLING) Stop();

			if (IsKeyDown(KEY_UP))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x))
					StartClimbingUp();
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				state = State::CLIMBING;
				box = GetHitbox();
				box.pos.y++;
				if (map->TestOnLadderTop(box, &pos.x))
				{
					StartClimbingDown();
					pos.y += PLAYER_LADDER_SPEED;

				}
			}
			else if (IsKeyPressed(KEY_SPACE))
			{
				StartJumping();
			}
		}
		else
		{
			if (state != State::FALLING) StartFalling();
		}
	}
}

void Player::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = PLAYER_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > PLAYER_JUMP_FORCE)
		{
			dir.y = PLAYER_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::JUMPING_RIGHT);
				else					SetAnimation((int)PlayerAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)PlayerAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)PlayerAnim::FALLING_RIGHT);
				else					SetAnimation((int)PlayerAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Player::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_UP))
	{
		pos.y -= PLAYER_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	if (map->TestOnLadderTop(box, &tmp))
	{
		if (IsInSecondHalfTile())		SetAnimation((int)PlayerAnim::CLIMBING_PRE_TOP);
		else if (IsInFirstHalfTile())	SetAnimation((int)PlayerAnim::CLIMBING_TOP);
		else	LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	}
	else if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else if (!map->TestOnLadder(box, &tmp))
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (GetAnimation() != PlayerAnim::CLIMBING)	SetAnimation((int)PlayerAnim::CLIMBING);

	}
}
void Player::Attack() 
{
	
	if (IsKeyPressed(KEY_LEFT_CONTROL)) 
	{
		if (IsLookingRight()) 
		{
			AttackRight();
			if (AttackingRight) 
			{
				SetAnimation((int)PlayerAnim::ATTACK_RIGHT);
				attackHitbox = true;
			}
			else 
			{
				SetAnimation((int)PlayerAnim::IDLE_RIGHT);
			}
		}
		else 
		{
			AttackLeft();
			if (AttackingLeft)
			{
				SetAnimation((int)PlayerAnim::ATTACK_LEFT);
				attackHitbox = true;
			}
			else
			{
				SetAnimation((int)PlayerAnim::IDLE_LEFT);
			}
		}

	}

	if (exp > 10) 
	{
		exp = 0;

		if (lives < 7) 
		{
			lives++;
		}

	}

}



void Player::AttackRight() 
{
	attack_timer = 100;

}
void Player::AttackLeft() 
{
	attack_timer = 100;
}
void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}

Enemy::Enemy(const Point& p, State s, Look view) :
Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ENEMY, "images/enemy.png") != AppStatus::OK)
	{
		return AppStatus::EROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ENEMY));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy sprite");
		return AppStatus::EROR;
	}

	Sprite* sprita = dynamic_cast<Sprite*>(render);
	sprita->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprita->SetAnimationDelay((int)EnemyAnim::WALKINGRIGHT, ANIM_DELAY);
	//for (i = 0; i < 1; ++i)
	sprita->AddKeyFrame((int)EnemyAnim::WALKINGRIGHT, { 0, 0, n, n });
	sprita->AddKeyFrame((int)EnemyAnim::WALKINGRIGHT, { n, 0, n, n });

	sprita->SetAnimationDelay((int)EnemyAnim::WALKINGLEFT, ANIM_DELAY);
	//for (i = 0; i < 1; ++i)
	sprita->AddKeyFrame((int)EnemyAnim::WALKINGLEFT, { 0, n, n, n });
	sprita->AddKeyFrame((int)EnemyAnim::WALKINGLEFT, { n, n, n, n });

	sprita->SetAnimation((int)EnemyAnim::WALKINGRIGHT);

	look = Look::LEFT;
	return AppStatus::OK;
}
void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18 * 16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ENEMY);

	render->Release();
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Enemy::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Enemy::IsLookingLeft() const
{
	return look == Look::LEFT;
}
void Enemy::Update()
{
	//Enemy doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	if (IsAlive()) 
	{
		Move();
		Sprite* sprita = dynamic_cast<Sprite*>(render);
		sprita->Update();
	}



}
bool Enemy::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Enemy::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprita = dynamic_cast<Sprite*>(render);
	sprita->SetAnimation(id);
}
EnemyAnim Enemy::GetAnimation()
{
	Sprite* sprita = dynamic_cast<Sprite*>(render);
	return (EnemyAnim)sprita->GetAnimation();
}
void Enemy::WalkRight()
{
		look = Look::RIGHT;
		SetAnimation((int)EnemyAnim::WALKINGLEFT);
		pos.x += 1;
}
void Enemy::WalkLeft()
{
		look = Look::LEFT;
		SetAnimation((int)EnemyAnim::WALKINGRIGHT);
		pos.x -= 1;
}
void Enemy::fall() 
{
	AABB box;
	box = GetHitbox();

	pos.y += 1;

	if (map->TestCollisionGround(box, &pos.y)) 
	{
		dir = { 0,0 };
	}
}
void Enemy::Move() 
{
	AABB box;
	int prev_x = pos.x;

	if (IsLookingRight()) 
	{
		WalkRight();
	}
	if (IsLookingLeft())
	{
		WalkLeft();
	}

	box = GetHitbox();
	if (map->TestCollisionWallLeft(box))
	{
		pos.x = prev_x + 4;
		WalkRight();
	}
	if (map->TestCollisionWallRight(box)) 
	{
		pos.x = prev_x - 4;
		WalkLeft();
	}
}	
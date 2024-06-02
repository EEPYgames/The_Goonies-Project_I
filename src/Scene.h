#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"
#include "Entity.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    AppStatus LoadLevel(int stage);
    bool death = false;
    bool victory = false;

    Player* player;

private:

  
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;
    void Debug();

    void RenderGUI() const;

    Enemy* enemy;
    TileMap *level;
    int nLevel;
    std::vector<Object*> objects;
    std::vector<Object*> objsEn;

    Camera2D camera;
    DebugMode debug;

    Text* myfont;
};


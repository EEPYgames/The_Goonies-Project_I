#pragma once
#include "Globals.h"
#include "Scene.h"


enum class GameState { MAIN_MENU, PLAYING, DEATHSCREEN, VICTORY, SETTINGS, CREDITS, INITIALSCREEN };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    GameState state;
private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();


    Scene *scene;
    const Texture2D *img_menu;
    const Texture2D* img_deathscreen;
    const Texture2D* img_victory;
    const Texture2D* img_initialScreen;
    Music music;
    Music death;
    Music victory;
    Music menu;

    Image inScreen;
    Texture2D inScreenTex;

    Text* myfont;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;

    float timePlayed = 0;
    float timePlayedA = 0;
};
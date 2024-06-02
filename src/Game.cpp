#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>
#include "Player.h"
#include <iostream>

Game::Game()
{
    state = GameState::INITIALSCREEN;
    scene = nullptr;
    img_menu = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "The Goonies");
    ToggleFullscreen();
    InitAudioDevice();

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::EROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::EROR;
    }
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
    //Set the target frame rate for the application
    SetTargetFPS(60);
    SetMasterVolume(1);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "images/GooniesTitle.png") != AppStatus::OK)
    {
        return AppStatus::EROR;
    }
    if (data.LoadTexture(Resource::IMG_DEATHSCREEN, "images/gameover.PNG") != AppStatus::OK)
    {
        return AppStatus::EROR;
    }
    if (data.LoadTexture(Resource::IMG_VICTORYSCREEN, "images/victory.PNG") != AppStatus::OK)
    {
        return AppStatus::EROR;
    }
    if (data.LoadTexture(Resource::IMG_INITIALSCREEN, "images/InitialScreen.png") != AppStatus::OK)
    {
        return AppStatus::EROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    img_deathscreen = data.GetTexture(Resource::IMG_DEATHSCREEN);
    img_victory = data.GetTexture(Resource::IMG_VICTORYSCREEN);
    img_initialScreen = data.GetTexture(Resource::IMG_INITIALSCREEN);

    music = LoadMusicStream("music/02_-_Goonies_-_MSX_-_The_Goonies_'R'_Good_Enough.ogg");
    menu = LoadMusicStream("music/01_-_Goonies_-_MSX_-_Title.ogg");
    victory = LoadMusicStream("music/08_-_Goonies_-_MSX_-_Stage_Clear.ogg");
    death = LoadMusicStream("music/07_-_Goonies_-_MSX_-_Game_Over.ogg");

    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();

    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::EROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::EROR;
    }



    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if(WindowShouldClose()) return AppStatus::QUIT;
    switch (state)
    {
        case GameState::INITIALSCREEN:
            PlayMusicStream(menu);
            UpdateMusicStream(menu);
            if (IsKeyPressed(KEY_SPACE)) { state = GameState::MAIN_MENU; }
            break;
        case GameState::MAIN_MENU: 

            UpdateMusicStream(menu);

            if (IsKeyPressed(KEY_ESCAPE))  return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                if(BeginPlay() != AppStatus::OK) return AppStatus::EROR;
                state = GameState::PLAYING;
                StopMusicStream(menu);
                PlayMusicStream(music);
            }
            break;

        case GameState::PLAYING:  
            if (IsKeyPressed(KEY_ESCAPE))
            {
                StopMusicStream(music);
                FinishPlay();
                state = GameState::MAIN_MENU;
            }
            else if (scene->death) 
            {
                state = GameState::DEATHSCREEN;
            }
            else if (scene->victory) 
            {
                state = GameState::VICTORY;
            }
            else
            {
                //Game logic
                scene->Update();
                UpdateMusicStream(music);
            }
            break;
        case GameState:: DEATHSCREEN:

            if (timePlayed < 9.6 && timePlayed > -1) 
            {
                PlayMusicStream(death);
                UpdateMusicStream(death);
            }

            timePlayed = GetMusicTimePlayed(death) / GetMusicTimeLength(death) * 10;

            if (timePlayed > 9.6) 
            {
                timePlayed = -1;
            }
            std::cout << timePlayed << std::endl;

            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::MAIN_MENU;
            }
            break;
        case GameState::VICTORY:


            if (timePlayedA < 9.6 && timePlayedA > -1)
            {
                PlayMusicStream(victory);
                UpdateMusicStream(victory);
            }


            timePlayedA = GetMusicTimePlayed(victory) / GetMusicTimeLength(victory) * 10;

            if (timePlayedA > 9.6)
            {
                timePlayedA = -1;
            }
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::MAIN_MENU;
            }
            break;

    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::MAIN_MENU:

            DrawTexture(*img_menu, 0, 0, WHITE);
            break;
        case GameState::INITIALSCREEN:
               
           /* DrawTexturePro(*img_initialScreen, { 0,0,256, 192 }, {256/2,192/2,(float)img_initialScreen->width,(float)img_initialScreen->height},
                {(float)img_initialScreen->width/2,(float)img_initialScreen->height/2}, 0, WHITE);*/
            DrawTexture(*img_initialScreen, 0, 0, WHITE);


            break;
        case GameState::PLAYING:
            scene->Render();
            break;
        case GameState::DEATHSCREEN:
                
            if (scene->death)
            {
                myfont->Draw(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT / 2, TextFormat("GAME OVER"));
            }
            break;
        case GameState::VICTORY:

            if (scene->victory)
            {
                ClearBackground(MYBLACK);
                DrawTexture(*img_victory, 7, 20, WHITE);

            }
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
    CloseAudioDevice();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_DEATHSCREEN);
    data.ReleaseTexture(Resource::IMG_INITIALSCREEN);

    UnloadRenderTexture(target);
}
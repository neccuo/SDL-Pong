#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include <unordered_set>

#include "GameObject.h"
#include "Utils.h"

extern "C" 
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

// Screen dimension constants
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 600;

const int FONT_SIZE = 50;

bool PAUSED = false;

bool GAME_END = false;

int P1_SCORE = 99;
int P2_SCORE = 99;

TTF_Font* g_font;

SDL_Window* window;
SDL_Renderer* renderer;

// For scoreboard textures
SDL_Surface* sb1Surface;
SDL_Surface* sb2Surface;

// For announcer textures
SDL_Surface* annSurface;


// DYNAMIC ALLOC YAP VE GLOBAL KULLAN
GameObject* p1 = nullptr;
GameObject* p2 = nullptr;

GameObject* net = nullptr;
GameObject* bx0 = nullptr;
GameObject* bx1 = nullptr;
GameObject* by0 = nullptr;
GameObject* by1 = nullptr;

GameObject* ball = nullptr;
GameObject* scoreboard_p1 = nullptr;
GameObject* scoreboard_p2 = nullptr;

GameObject* announceText = nullptr;

SDL_Rect* p1_r = nullptr;
SDL_Rect* p2_r = nullptr;

SDL_Rect* ball_r = nullptr;

SDL_Rect* border_x0_r = nullptr;
SDL_Rect* border_x1_r = nullptr;
SDL_Rect* border_y0_r = nullptr;
SDL_Rect* border_y1_r = nullptr;

std::vector<GameObject*> objectList;

Vector2 initial_ball_velo(-0.5f, 0.0f);

// Create a set to store the keys that are currently being held down
std::unordered_set<SDL_Keycode> held_keys;

void scoreUpdate(SDL_Surface* &surface, GameObject &obj, int score)
{
    if(score > 9) score = 9;
    else if(score < 0) score = 0;
    SDL_FreeSurface(surface);
    std::string str = std::to_string(score);

    surface = TTF_RenderText_Solid(g_font, str.c_str(), {0xff, 0xff, 0xff});
    if (!surface) 
    {
        SDL_Log("SDL_Surface could not initialize! SDL_Error: %s", SDL_GetError());
        exit(0);
    }
    obj.SetTextureBySurface(surface);
}

void ClearGameObjects()
{
    for(int i = 0; i < objectList.size(); ++i)
    {
        delete objectList[i];
    }
    objectList.clear();
}

void InitGameObj(GameObject* &obj, float x, float y, int w, int h, float v_x=0.0f, float v_y=0.0f)
{
    if(obj != nullptr)
        delete obj;
    obj = new GameObject(renderer, x, y, w, h, v_x, v_y);
    if(obj)
        objectList.push_back(obj);

}

void StartGame()
{
    // if(!objectList.empty())
    // {
    //     std::cout << "objectList is not empty" << std::endl;
    // }
    InitGameObj(p1, 30.0f, 150.0f, 10, 200);
    InitGameObj(p2, SCREEN_WIDTH - 30.0f, 150.0f, 10, 200);

    InitGameObj(net, SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 10, SCREEN_HEIGHT);
    InitGameObj(bx0, 
        SCREEN_WIDTH/2.0f, 
        0.0f, 
        SCREEN_WIDTH, 
        4);
    InitGameObj(bx1, 
        SCREEN_WIDTH/2.0f, 
        SCREEN_HEIGHT, 
        SCREEN_WIDTH, 
        4);
    InitGameObj(by0, 
        0.0f, 
        SCREEN_HEIGHT/2.0f, 
        4, 
        SCREEN_HEIGHT);
    InitGameObj(by1, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT/2.0f, 
        4, 
        SCREEN_HEIGHT);

    InitGameObj(ball, 
        SCREEN_WIDTH / 2.0f, 
        SCREEN_HEIGHT / 2.0f,
        10,
        10);
    InitGameObj(scoreboard_p1, 
        (SCREEN_WIDTH / 2.0f) - (FONT_SIZE),
        50,
        FONT_SIZE,
        FONT_SIZE);
    InitGameObj(scoreboard_p2, 
        (SCREEN_WIDTH / 2.0f) + (FONT_SIZE),
        50,
        FONT_SIZE,
        FONT_SIZE);
    InitGameObj(announceText, 
        (SCREEN_WIDTH / 4.0f),
        (SCREEN_HEIGHT / 2.0f),
        FONT_SIZE*10,
        FONT_SIZE);

    p1_r = p1->GetRectRef();
    p2_r = p2->GetRectRef();
    ball_r = ball->GetRectRef();
    border_x0_r = bx0->GetRectRef();
    border_x1_r = bx1->GetRectRef();
    border_y0_r = by0->GetRectRef();
    border_y1_r = by1->GetRectRef();

    ball->SetVelocity(initial_ball_velo);

    P1_SCORE = 0;
    P2_SCORE = 0;

    scoreUpdate(sb1Surface, *scoreboard_p1, P1_SCORE);
    scoreUpdate(sb2Surface, *scoreboard_p2, P2_SCORE);

    PAUSED = false;
    
}

void PauseSet(bool isPaused)
{
    PAUSED = isPaused;
}

// Function to check if a key is being held down
bool is_key_down(SDL_Keycode key)
{
    return held_keys.count(key) > 0;
}

int init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) 
    {
        SDL_Log("SDL_TTF could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // Create font
    g_font = TTF_OpenFont("fonts/ArcadeNormal-ZDZ.ttf", FONT_SIZE);
    if (!g_font) 
    {
        SDL_Log("Font could not loaded! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) 
    {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) 
    {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }

}

void announceUpdate(SDL_Surface* &surface, GameObject &obj, std::string text)
{
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(g_font, text.c_str(), {0xdf, 0xdf, 0xdf});
    if (!surface) 
    {
        SDL_Log("SDL_Surface could not initialize! SDL_Error: %s", SDL_GetError());
        exit(0);
    }
    obj.SetTextureBySurface(surface);
}

int main(int argc, char* args[]) 
{
    if(init() == 1)
        return 1;    

    StartGame();

    // Main loop
    bool quit = false;
    SDL_Event event;

    srand(time(NULL));

    // DELTA
    Uint32 NOW = SDL_GetPerformanceCounter();
    Uint32 LAST = 0;
    float DELTA_TIME = 0.0f;

    while (!quit) 
    {
        LAST = NOW;
        // Event handling
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                quit = true;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                if(event.key.keysym.sym == SDLK_p)
                {
                    PauseSet(!PAUSED);
                }
                else
                {
                    PauseSet(false);
                    held_keys.insert(event.key.keysym.sym);
                }
            }
            else if(event.type == SDL_KEYUP)
            {
                held_keys.erase(event.key.keysym.sym);
            }
        }

        NOW = SDL_GetPerformanceCounter();

        DELTA_TIME = (float)((NOW - LAST)*1000 / (float)SDL_GetPerformanceFrequency() );

        if(GAME_END && !PAUSED)
        {
            GAME_END = false;
            StartGame();
            continue;
        }

        if(P2_SCORE == 2 && !GAME_END)
        {
            announceText->SetPosition(3.0f*SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f);
            announceUpdate(annSurface, *announceText, "P2 WON, Press any button");
            PauseSet(true);
            GAME_END = true;
        }
        else if(P1_SCORE == 2 && !GAME_END)
        {
            announceText->SetPosition(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f);
            announceUpdate(annSurface, *announceText, "P1 WON, Press any button");
            PauseSet(true);
            GAME_END = true;
        }

        
        // COLLISION REALM
        bool p1_col = SDL_HasIntersection(ball_r, p1_r);
        bool p2_col = SDL_HasIntersection(ball_r, p2_r);

        bool p1_up_wall_col = SDL_HasIntersection(p1_r, border_x0_r);
        bool p1_down_wall_col = SDL_HasIntersection(p1_r, border_x1_r);

        bool p2_up_wall_col = SDL_HasIntersection(p2_r, border_x0_r);
        bool p2_down_wall_col = SDL_HasIntersection(p2_r, border_x1_r);


        bool up_col = SDL_HasIntersection(ball_r, border_x0_r);
        bool down_col = SDL_HasIntersection(ball_r, border_x1_r);

        bool p2_score = SDL_HasIntersection(ball_r, border_y0_r);
        bool p1_score = SDL_HasIntersection(ball_r, border_y1_r);

        auto velo = ball->GetVelocity();

        if((p1_col && velo.x < 0) || (p2_col && velo.x > 0))
        {
            velo.x = RelativeRNG(velo.x, 40, 80);
            velo.y = RelativeRNG(velo.y, 20, 40);
            // std::cout << velo << std::endl;

            ball->SetVelocity(velo.x * (-1.0f), velo.y);
        }
        if((up_col && velo.y < 0) || (down_col && velo.y > 0))
        {
            ball->SetVelocity(velo.x, velo.y * (-1.0f));
        }
        if(p2_score)
        {
            std::cout << "P2 SCORE!" << std::endl;
            ball->SetPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
            ball->SetVelocity(initial_ball_velo);
            scoreUpdate(sb2Surface, *scoreboard_p2, ++P2_SCORE);
        }
        else if(p1_score)
        {
            std::cout << "P1 SCORE!" << std::endl;
            ball->SetPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
            ball->SetVelocity(initial_ball_velo.x*-1, initial_ball_velo.y);
            scoreUpdate(sb1Surface, *scoreboard_p1, ++P1_SCORE);
        }

        // INPUTS
        p1->SetVelocity(0.0f, 0.0f);
        p2->SetVelocity(0.0f, 0.0f);

        // PLAYER INPUT

        // GO UP
        if(!p1_up_wall_col && is_key_down(SDLK_w))
        {
            p1->SetVelocity(0.0f, -0.5f);
        }

        // GO DOWN
        if(!p1_down_wall_col && is_key_down(SDLK_s))
        {
            p1->SetVelocity(0.0f, 0.5f);
        }
        if(is_key_down(SDLK_r))
        {
            StartGame();
            continue;
        }

        // AI INPUT
        float ball_x = ball->GetPosition().x;
        float ball_y = ball->GetPosition().y;

        float p2_x = p2->GetPosition().x;
        float p2_y = p2->GetPosition().y;

        float dist = std::abs(ball_x-p2_x);

        // GO UP
        if(!p2_up_wall_col && p2_y > ball_y && dist < 500.0f)
            p2->SetVelocity(0.0f, -0.5f);
        // GO DOWN
        else if(!p2_down_wall_col && p2_y < ball_y && dist < 500.0f)
            p2->SetVelocity(0.0f, 0.5f);

        if(!PAUSED)
        {
            ball->Update(DELTA_TIME);

            p1->Update(DELTA_TIME);
            p2->Update(DELTA_TIME);
        }

        // RENDER

        // FIRST:::Clear the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render border
        bx0->Render();
        bx1->Render();
        by0->Render();
        by1->Render();

        // Render player rec
        p1->Render();
        p2->Render();

        // Render net
        net->Render();

        // Render ball
        ball->Render();

        // Render texture
        scoreboard_p1->RenderTex();
        scoreboard_p2->RenderTex();

        announceText->RenderTex();

        // Update the screen
        SDL_RenderPresent(renderer);
    }
    // Clear every active game object
    // ClearGameObjects();

    // Cleanup and exit
    SDL_FreeSurface(sb1Surface);
    SDL_FreeSurface(sb2Surface);
    SDL_FreeSurface(annSurface);

    TTF_CloseFont(g_font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

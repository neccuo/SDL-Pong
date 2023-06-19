#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>

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

SDL_Window* window;
SDL_Renderer* renderer;

// Create a set to store the keys that are currently being held down
std::unordered_set<SDL_Keycode> held_keys;

// Function to check if a key is being held down
bool is_key_down(SDL_Keycode key)
{
    return held_keys.count(key) > 0;
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    window = SDL_CreateWindow("SDL Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return 1;
    }

    // PLAYER OBJECTS;
    GameObject p1(renderer, 30.0f, 150.0f, 10, 200);
    
    // SDL_Rect p1_colr;
    // SDL_Rect t_p1r = *(p1.GetRectRef());
    // p1_colr.x = t_p1r.x + (t_p1r.w/2);
    // p1_colr.y = t_p1r.y - (t_p1r.h/2);
    // p1_colr.x = t_p1r.x;
    // p1_colr.y = t_p1r.y;
    // p1_colr.w = 1;
    // p1_colr.h = t_p1r.h;

    // p1.SetCollider(p1_colr);

    GameObject p2(renderer, SCREEN_WIDTH - 30.0f, 150.0f, 10, 200);

    // ENVIRONMENT
    GameObject net(renderer, SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f, 10, SCREEN_HEIGHT);
    GameObject bx0(renderer, 
        SCREEN_WIDTH/2.0f, 
        0.0f, 
        SCREEN_WIDTH, 
        4
    );
    GameObject bx1(renderer, 
        SCREEN_WIDTH/2.0f, 
        SCREEN_HEIGHT, 
        SCREEN_WIDTH, 
        4
    );
    GameObject by0(renderer, 
        0.0f, 
        SCREEN_HEIGHT/2.0f, 
        4, 
        SCREEN_HEIGHT
    );
    GameObject by1(renderer, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT/2.0f, 
        4, 
        SCREEN_HEIGHT
    );

    // BALL
    GameObject ball(renderer, 
        SCREEN_WIDTH / 2.0f, 
        SCREEN_HEIGHT / 2.0f,
        10,
        10,
        -0.3f,
        0.3f
    );

    // params
    SDL_Rect* p1_r = p1.GetRectRef();
    SDL_Rect* p2_r = p2.GetRectRef();

    SDL_Rect* ball_r = ball.GetRectRef();

    SDL_Rect* border_x0_r = bx0.GetRectRef();
    SDL_Rect* border_x1_r = bx1.GetRectRef();
    SDL_Rect* border_y0_r = by0.GetRectRef();
    SDL_Rect* border_y1_r = by1.GetRectRef();

    // Main loop
    bool quit = false;
    SDL_Event event;

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
                held_keys.insert(event.key.keysym.sym);
            }
            else if(event.type == SDL_KEYUP)
            {
                held_keys.erase(event.key.keysym.sym);
            }
        }

        NOW = SDL_GetPerformanceCounter();

        DELTA_TIME = (float)((NOW - LAST)*1000 / (float)SDL_GetPerformanceFrequency() );

        // COLLISION DETECTION
        bool player_collision = SDL_HasIntersection(ball_r, p1_r) || 
                                SDL_HasIntersection(ball_r, p2_r);

        bool wall_collision = SDL_HasIntersection(ball_r, border_x0_r) ||
                              SDL_HasIntersection(ball_r, border_x1_r);

        bool left_col = SDL_HasIntersection(ball_r, border_y0_r);
        bool right_col = SDL_HasIntersection(ball_r, border_y1_r);

        if(wall_collision)
        {
            auto velo = ball.GetVelocity();
            ball.SetVelocity(velo.x, velo.y * (-1.0f));
        }
        if(player_collision)
        {
            auto velo = ball.GetVelocity();
            ball.SetVelocity(velo.x * (-1.0f), velo.y);
        }
        if(left_col || right_col)
        {
            ball.SetPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
        }

        // INPUTS
        p1.SetVelocity(0.0f, 0.0f);
        p2.SetVelocity(0.0f, 0.0f);

        if(is_key_down(SDLK_w))
        {
            p1.SetVelocity(0.0f, -0.5f);
            p2.SetVelocity(0.0f, -0.5f);
        }
        if(is_key_down(SDLK_s))
        {
            p1.SetVelocity(0.0f, 0.5f);
            p2.SetVelocity(0.0f, 0.5f);
        }

        bx0.Update(DELTA_TIME);
        bx1.Update(DELTA_TIME);
        by0.Update(DELTA_TIME);
        by1.Update(DELTA_TIME);

        ball.Update(DELTA_TIME);
        net.Update(DELTA_TIME);
        p1.Update(DELTA_TIME);
        p2.Update(DELTA_TIME);


        // RENDER

        // FIRTS:::Clear the screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render border
        bx0.Render();
        bx1.Render();
        by0.Render();
        by1.Render();

        // Render player rec
        p1.Render();
        p2.Render();

        // Render net
        net.Render();

        // Render ball
        ball.Render();

        // Update the screen
        SDL_RenderPresent(renderer);
    }


    // Cleanup and exit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

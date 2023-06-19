#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "Utils.h"

// COORDINATES ARE CENTERED
class GameObject 
{
public:
    // GameObject(SDL_Renderer* renderer, float x, float y, float width, float height, Vector2 velocity)
    //     : m_renderer(renderer), m_x(x), m_y(y), m_width(width), m_height(height), m_velocity(velocity) {}

    GameObject(SDL_Renderer *renderer, float x, float y);

    GameObject(SDL_Renderer *renderer, float x, float y, int w, int h);

    GameObject(SDL_Renderer *renderer, float x, float y, int w, int h, float v_x, float v_y);

    ~GameObject();

    void Update(float deltaTime);

    void Render();

    Vector2 GetVelocity();

    SDL_Rect* GetRectRef();

    // SDL_Rect *GetColliderRef();

    void SetPosition(float x, float y);

    void SetScale(int x, int y);

    void SetVelocity(float x, float y);

    // void SetCollider(SDL_Rect rect);


private:
    void UpdateRect();

    // void UpdateCollider();

    SDL_Renderer* m_renderer;
    float m_x;
    float m_y;
    int m_width;
    int m_height;
    Vector2 m_velocity;

    SDL_Rect* m_rect;
    SDL_Rect* m_collider = nullptr;
};


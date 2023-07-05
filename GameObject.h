#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "Utils.h"

// COORDINATES ARE CENTERED
class GameObject 
{
public:
    GameObject(SDL_Renderer *renderer, float x, float y);

    GameObject(SDL_Renderer *renderer, float x, float y, int w, int h);

    GameObject(SDL_Renderer *renderer, float x, float y, int w, int h, float v_x, float v_y);

    ~GameObject();

    void Update(float deltaTime);

    void Render();

    void RenderTex();

    Vector2 GetPosition();

    Vector2 GetVelocity();

    SDL_Rect* GetRectRef();

    void SetPosition(float x, float y);

    void SetScale(int x, int y);

    void SetVelocity(float x, float y);

    void SetVelocity(Vector2 v2);

    void SetTexture(SDL_Texture* &tex);

    void SetTextureBySurface(SDL_Surface* &surf);

private:
    void UpdateRect();

    SDL_Renderer* m_renderer;
    float m_x;
    float m_y;
    int m_width;
    int m_height;
    Vector2 m_velocity;

    SDL_Rect* m_rect;
    SDL_Rect* m_collider = nullptr;

    SDL_Texture* m_texture = nullptr;

};


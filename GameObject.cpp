#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* renderer, float x, float y)
{
    m_renderer = renderer;
    SetPosition(x, y);
    SetScale(1.0f, 1.0f);
    SetVelocity(0.0f, 0.0f);

    m_rect = new SDL_Rect();
}

GameObject::GameObject(SDL_Renderer* renderer, float x, float y, int w, int h)
{
    m_renderer = renderer;
    SetPosition(x, y);
    SetScale(w, h);
    SetVelocity(0.0f, 0.0f);

    m_rect = new SDL_Rect();
}

GameObject::GameObject(SDL_Renderer* renderer, float x, float y, int w, int h, float v_x, float v_y)
{
    m_renderer = renderer;
    SetPosition(x, y);
    SetScale(w, h);
    SetVelocity(v_x, v_y);

    m_rect = new SDL_Rect();
}

GameObject::~GameObject()
{
    delete m_rect;
}

void GameObject::Update(float deltaTime)
{
    float newX = m_x + m_velocity.x * deltaTime;
    float newY = m_y + m_velocity.y * deltaTime;

    SetPosition(newX, newY);

    UpdateRect();
    // UpdateCollider();
}

void GameObject::Render()
{
    // Render the game object on the screen
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(m_renderer, m_rect);
}

Vector2 GameObject::GetVelocity()
{
    return m_velocity;
}

SDL_Rect* GameObject::GetRectRef()
{
    return m_rect;
}

// SDL_Rect* GameObject::GetColliderRef()
// {
//     return m_collider;
// }

void GameObject::SetPosition(float x, float y) 
{
    m_x = x;
    m_y = y;
}

void GameObject::SetScale(int x, int y) 
{
    m_width = x;
    m_height = y;
}

void GameObject::SetVelocity(float x, float y) 
{
    m_velocity.x = x;
    m_velocity.y = y;
}

// FIND A WAY TO MOVE THE COLLIDER ALSO
// void GameObject::SetCollider(SDL_Rect rect)
// {
//     if (m_collider == nullptr)
//     {
//         m_collider = new SDL_Rect(rect);
//     }
//     else
//     {
//         *m_collider = rect;
//     }
// }

// PRIVATE REALM
void GameObject::UpdateRect()
{
    m_rect->x = static_cast<int>(m_x) - (m_width/2);
    m_rect->y = static_cast<int>(m_y) - (m_height/2);
    m_rect->w = static_cast<int>(m_width);
    m_rect->h = static_cast<int>(m_height);
}

// void GameObject::UpdateCollider()
// {
//     if(m_collider == nullptr)
//         return;
//     m_collider->x = m_rect->x;
//     m_collider->y = m_rect->y;
// }
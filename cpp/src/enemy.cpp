#include "enemy.hpp"

Enemy::Enemy (Vector3 initial_position)
{
    position = initial_position;
    velocity = Vector3Zero();
    rotation = QuaternionIdentity();
}

Enemy::~Enemy ()
{
}

void Enemy::draw ()
{
    if (is_hit)
    {
        DrawSphere(position, 1.0f, RED);
    }
    else
    {
        DrawSphere(position, 1.0f, GREEN);
    }
}
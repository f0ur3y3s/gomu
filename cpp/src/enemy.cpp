#include "enemy.hpp"

Enemy::Enemy (Vector3 initial_position)
{
    position     = initial_position;
    velocity     = Vector3Zero();
    rotation     = QuaternionIdentity();
    bounding_box = {
        Vector3 { -1.0f, -1.0f, -1.0f },
        Vector3 { 1.0f, 1.0f, 1.0f },
    };
}

Enemy::~Enemy ()
{
}

void Enemy::update (float delta_time)
{
    if (is_hit)
    {
        health -= 10.0f * delta_time;
        health = Clamp(health, 0.0f, 100.0f);

        if (health <= 0.0f)
        {
            // destroy
        }
    }
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

    BoundingBox calculated_bounding_box = {
        Vector3Subtract(position, Vector3One()),
        Vector3Add(position, Vector3One()),
    };
    DrawBoundingBox(calculated_bounding_box, PURPLE);
}

void Enemy::draw_health (Camera3D camera)
{
    // TODO: There is a bug here where enemies behind the camera are still
    // rendered
    Vector2 screen_position = GetWorldToScreen(position, camera);

    DrawText(TextFormat("Enemy: %d / 100", (int)health),
             (int)screen_position.x - MeasureText("Enemy: 100/100", 20) / 2,
             (int)screen_position.y,
             20,
             FOREGROUND);
}
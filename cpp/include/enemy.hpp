#include "raylib.h"
#include "raymath.h"

#include "actor.hpp"
#include "movement.h"

class Enemy : public Actor
{
public:
    Enemy (Vector3 initial_position);
    ~Enemy ();

    void    draw ();
    void    update (float delta_time);
    void    reset ();
    void    shoot ();
    Vector3 get_position ();
    bool    is_hit = false;

private:
    float health = 100.0f;
};
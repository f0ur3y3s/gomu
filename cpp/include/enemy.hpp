#include "raylib.h"
#include "raymath.h"

#include "actor.hpp"
#include "movement.h"
#include "uju_utils.hpp"

class Enemy : public Actor
{
public:
    Enemy (Vector3 initial_position);
    ~Enemy ();

    void    draw ();
    void    update (float delta_time);
    void    reset ();
    void    shoot ();
    void    draw_health (Camera3D camera);
    Vector3 get_position ();
    bool    is_hit     = false;
    bool    show_stats = false;
    bool    render     = false;

    BoundingBox bounding_box = { 0 };

private:
    float health = 100.0f;
};
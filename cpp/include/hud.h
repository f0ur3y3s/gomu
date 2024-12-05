#ifndef UJU_HUD_H
#define UJU_HUD_H

#include "raylib.h"
#include "raymath.h"
#include "uju_utils.h"
#include "ship.h"

class HUD
{
public:
    HUD (float screen_width, float screen_height, float aim_deadzone);

    void draw (Vector2 mouse_delta);
    void update (Ship & p_ship);

private:
    float   radius        = 0;
    float   deadzone      = 0;
    Vector2 screen_center = { 0, 0 };
    void    draw_health ();
    void    draw_energy ();
    float   health;
    float   energy;
    float   bar_offset    = 10.0f;
    float   bar_thickness = 20.0f;
    float   bar_size      = 90.0f;
};

#endif
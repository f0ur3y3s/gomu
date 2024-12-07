#ifndef UJU_HUD_H
#define UJU_HUD_H

#include "raylib.h"
#include "raymath.h"
#include "uju_utils.hpp"
#include "ship.hpp"

class HUD
{
public:
    HUD (float screen_width, float screen_height, float aim_deadzone);

    void draw (Vector2 mouse_delta);
    void update (Ship & p_ship);
    void update (Ship & p_ship, bool is_target_in_aim);

private:
    void    draw_health ();
    void    draw_energy ();
    float   radius        = 0;
    float   deadzone      = 0;
    Vector2 screen_center = { 0, 0 };
    float   health        = 100.0f;
    float   engine_energy = 100.0f;
    float   bar_offset    = 10.0f;
    float   bar_thickness = 20.0f;
    float   bar_size      = 90.0f;
    bool    show_aim      = false;
};

#endif
#include "hud.hpp"

HUD::HUD (float screen_width, float screen_height, float aim_deadzone)
{
    deadzone = aim_deadzone;
    float max_size
        = screen_width > screen_height ? screen_height : screen_width;
    radius        = max_size / 3.0f;
    screen_center = Vector2 { screen_width / 2.0f, screen_height / 2.0f };
}

void HUD::draw_health ()
{
    float start_angle = 135.0f;
    float bar_size    = 90.0f;

    Color color_array[] = {
        RED, ORANGE, YELLOW, GREEN, DARKGREEN,
    };
    const int color_array_size = sizeof(color_array) / sizeof(Color);

    int health_index   = (int)(health / 100.0f * color_array_size) - 1;
    health_index       = Clamp(health_index, 0, color_array_size - 1);
    Color health_color = color_array[health_index];

    float health_size = bar_size * (health / 100.0f);
    health_size       = Clamp(health_size, 0.0f, bar_size);

    DrawRing(Vector2 { screen_center.x, screen_center.y },
             radius + bar_offset,
             radius + bar_offset + bar_thickness,
             start_angle,
             start_angle + health_size,
             100,
             Fade(health_color, 0.5f));
    DrawRingLines(Vector2 { screen_center.x, screen_center.y },
                  radius + bar_offset,
                  radius + bar_offset + bar_thickness,
                  start_angle,
                  start_angle + bar_size,
                  100,
                  Fade(FOREGROUND, 0.5f));
}

void HUD::draw_engine_energy ()
{
    float start_angle = 0.0f;
    float bar_size    = 45.0f;

    float energy_size = bar_size * (engine_energy / 100.0f);
    energy_size       = Clamp(energy_size, 0.0f, bar_size);

    DrawRing(Vector2 { screen_center.x, screen_center.y },
             radius + bar_offset,
             radius + bar_offset + bar_thickness,
             start_angle - energy_size, // 315 - 90
             start_angle,               // 315 + 90
             100,
             Fade(BLUE, 0.5f));

    DrawRingLines(Vector2 { screen_center.x, screen_center.y },
                  radius + bar_offset,
                  radius + bar_offset + bar_thickness,
                  start_angle - bar_size,
                  start_angle,
                  100,
                  Fade(FOREGROUND, 0.5f));
}

void HUD::draw_weapon_energy ()
{
    float start_angle = 0.0f;
    float bar_size    = 45.0f;

    float energy_size = bar_size * (weapon_energy / 100.0f);
    energy_size       = Clamp(energy_size, 0.0f, bar_size);

    DrawRing(Vector2 { screen_center.x, screen_center.y },
             radius + bar_offset,
             radius + bar_offset + bar_thickness,
             start_angle,
             start_angle + energy_size,
             100,
             Fade(RED, 0.5f));

    DrawRingLines(Vector2 { screen_center.x, screen_center.y },
                  radius + bar_offset,
                  radius + bar_offset + bar_thickness,
                  start_angle,
                  start_angle + bar_size,
                  100,
                  Fade(FOREGROUND, 0.5f));
}

void HUD::draw (Vector2 mouse_delta)
{
    // reticle
    DrawRing(
        screen_center, radius, radius - 1, 0, 360, 100, Fade(FOREGROUND, 0.5f));
    DrawRing(screen_center,
             deadzone,
             deadzone - 1,
             0,
             360,
             100,
             Fade(FOREGROUND, 0.5f));

    // crosshair
    DrawLineEx(Vector2 { mouse_delta.x - 10, mouse_delta.y },
               Vector2 { mouse_delta.x + 10, mouse_delta.y },
               2,
               Fade(FOREGROUND, 0.5f));
    DrawLineEx(Vector2 { mouse_delta.x, mouse_delta.y - 10 },
               Vector2 { mouse_delta.x, mouse_delta.y + 10 },
               2,
               Fade(FOREGROUND, 0.5f));

    // only if aim is colliding
    if (show_aim)
    {
        DrawRingLines(mouse_delta, 15, 15 - 1, 0, 360, 100, FOREGROUND);
    }

    draw_health();
    draw_engine_energy();
    draw_weapon_energy();
}

void HUD::update (Ship & p_ship)
{
    Vector3 stats = p_ship.get_stats();
    health        = stats.x;
    engine_energy = stats.y;
    weapon_energy = stats.z;
    // health        = p_ship.health;
    // engine_energy = p_ship.engine_energy;
    // weapon_energy = p_ship.weapon_energy;
    // if (p_ship.aim_colliding)
    // {
    //     show_aim = true;
    // }
    // else
    // {
    //     show_aim = false;
    // }
}

// void HUD::update (Ship & p_ship, bool is_target_in_aim)
// {
//     // none for now
// }
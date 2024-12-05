#include "hud.h"

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

void HUD::draw_energy ()
{
    float start_angle = 45.0f;

    float energy_size = bar_size * (energy / 100.0f);
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

void HUD::draw (Vector2 mouse_delta)
{
    // reticle
    DrawRing(screen_center, radius, radius - 1, 0, 360, 100, FOREGROUND);
    // DrawCircleLines(
    //     screen_center.x, screen_center.y, deadzone, Fade(FOREGROUND, 0.5f));
    DrawRing(screen_center, deadzone, deadzone - 1, 0, 360, 100, FOREGROUND);

    // mouse
    DrawCircleV(mouse_delta, 10, RED);

    draw_health();
    draw_energy();
}

void HUD::update (Ship & p_ship)
{
    health = p_ship.health;
    energy = p_ship.energy;
}
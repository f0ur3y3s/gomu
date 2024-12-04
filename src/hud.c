#include "hud.h"

void draw_reticle (const int   screen_width,
                   const int   screen_height,
                   const float deadzone)
{
    float max_size
        = screen_width > screen_height ? screen_height : screen_width;
    int center_x = screen_width / 2;
    int center_y = screen_height / 2;

    DrawCircleLines(center_x, center_y, max_size / 3.0f, Fade(RED, 0.5f));
    DrawCircleLines(center_x, center_y, deadzone, Fade(RED, 0.5f));
}

void draw_mouse (Vector2 mouse_delta)
{
    DrawCircleV(mouse_delta, 10, RED);
}

void draw_health (const int   screen_width,
                  const int   screen_height,
                  const float health)
{
    float offset    = 10.0f;
    float thickness = 20.0f;
    float max_size
        = screen_width > screen_height ? screen_height : screen_width;
    int center_x = screen_width / 2;
    int center_y = screen_height / 2;

    Color color_array[] = {
        RED, ORANGE, YELLOW, GREEN, DARKGREEN,
    };

    // normalize health from 0-100 to 0-number of colors
    const int color_array_size = sizeof(color_array) / sizeof(Color);
    int       health_index     = (int)(health / 100.0f * color_array_size) - 1;
    health_index               = Clamp(health_index, 0, color_array_size - 1);
    Color health_color         = color_array[health_index];

    // calculate size of health ring from 135-225
    float health_size = 90.0f * (health / 100.0f);
    health_size       = Clamp(health_size, 0.0f, 90.0f);

    printf("Health: %f\n", health_size);
    DrawRing((Vector2) { center_x, center_y },
             max_size / 3.0f + offset,
             max_size / 3.0f + offset + thickness,
             135,
             135 + health_size,
             100,
             Fade(health_color, 0.5f));

    DrawRingLines((Vector2) { center_x, center_y },
                  max_size / 3.0f + offset,
                  max_size / 3.0f + offset + thickness,
                  135,
                  225,
                  100,
                  BLACK);
}

void draw_speed (const int   screen_width,
                 const int   screen_height,
                 const float speed)
{
}

void draw_energy (const int   screen_width,
                  const int   screen_height,
                  const float energy)
{
}
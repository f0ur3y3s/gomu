#ifndef UJU_UI_H
#define UJU_UI_H

#include "raylib.h"
#include "raymath.h"

typedef struct ui_t
{
    Vector2 screen_center;
    float   deadzone;
} ui_t;

bool draw_axis (Model * p_model, const Vector3 * p_position);
bool draw_reticle (const int   screen_width,
                   const int   screen_height,
                   const float deadzone);

#endif
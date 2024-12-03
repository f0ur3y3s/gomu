#ifndef UJU_CONTROLS_H
#define UJU_CONTROLS_H

#include "raylib.h"

typedef struct mouse_t
{
    Vector2 aim;
    Vector2 base_pos;
    Vector2 cur_pos;
    Vector2 delta;
} mouse_t;

typedef struct controls_t
{
    mouse_t mouse;
} controls_t;

bool controls_init (controls_t * p_controls);
bool controls_update (controls_t * p_controls);

#endif
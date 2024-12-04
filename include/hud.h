#ifndef UJU_HUD_H
#define UJU_HUD_H

#include "raylib.h"
#include "raymath.h"
// functions to draw hud
// reticle
// aim
// health
// speed
void draw_reticle (const int   screen_width,
                   const int   screen_height,
                   const float deadzone);

void draw_mouse (Vector2 mouse_delta);

void draw_health (const int   screen_width,
                  const int   screen_height,
                  const float health);

void draw_speed (const int   screen_width,
                 const int   screen_height,
                 const float speed);

void draw_energy (const int   screen_width,
                  const int   screen_height,
                  const float energy);
#endif
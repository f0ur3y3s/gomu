#ifndef UJU_UTILS_H
#define UJU_UTILS_H

#include "raylib.h"
#include "raymath.h"

#define FOREGROUND WHITE

float   float_damp (float from, float to, float speed, float delta_time);
Vector3 vector3_damp (Vector3 from, Vector3 to, float speed, float delta_time);
Quaternion quaternion_damp (Quaternion from,
                            Quaternion to,
                            float      speed,
                            float      delta_time);

void draw_axis (const Vector3 * p_position);

#endif
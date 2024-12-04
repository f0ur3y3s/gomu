#ifndef UJU_UTILS_H
#define UJU_UTILS_H

#include "raylib.h"
#include "raymath.h"

inline float float_damp (float from, float to, float speed, float delta_time)
{
    return Lerp(from, to, 1.0f - expf(-speed * delta_time));
}

inline Vector3 vector3_damp (Vector3 from,
                             Vector3 to,
                             float   speed,
                             float   delta_time)
{
    return (Vector3) { Lerp(from.x, to.x, 1.0f - expf(-speed * delta_time)),
                       Lerp(from.y, to.y, 1.0f - expf(-speed * delta_time)),
                       Lerp(from.z, to.z, 1.0f - expf(-speed * delta_time)) };
}

inline Quaternion quaternion_damp (Quaternion from,
                                   Quaternion to,
                                   float      speed,
                                   float      delta_time)
{
    return QuaternionSlerp(from, to, 1.0f - expf(-speed * delta_time));
}

void draw_axis (const Vector3 * p_position);

#endif UJU_UTILS_H

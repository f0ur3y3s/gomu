#include "uju_utils.h"

float float_damp (float from, float to, float speed, float delta_time)
{
    return Lerp(from, to, 1.0f - expf(-speed * delta_time));
}

Vector3 vector3_damp (Vector3 from, Vector3 to, float speed, float delta_time)
{
    return (Vector3) { Lerp(from.x, to.x, 1.0f - expf(-speed * delta_time)),
                       Lerp(from.y, to.y, 1.0f - expf(-speed * delta_time)),
                       Lerp(from.z, to.z, 1.0f - expf(-speed * delta_time)) };
}

Quaternion quaternion_damp (Quaternion from,
                            Quaternion to,
                            float      speed,
                            float      delta_time)
{
    return QuaternionSlerp(from, to, 1.0f - expf(-speed * delta_time));
}

void draw_axis (const Vector3 * p_position)
{
    Vector3 origin = *p_position;

    float axis_length = 2.0f;

    Vector3 x_end = { origin.x + axis_length, origin.y, origin.z };
    DrawLine3D(origin, x_end, RED);

    Vector3 y_end = { origin.x, origin.y + axis_length, origin.z };
    DrawLine3D(origin, y_end, GREEN);

    Vector3 z_end = { origin.x, origin.y, origin.z + axis_length };
    DrawLine3D(origin, z_end, BLUE);
}
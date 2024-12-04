#include "uju_utils.h"

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
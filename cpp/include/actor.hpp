#ifndef UJU_ACTOR_H
#define UJU_ACTOR_H

#include "raylib.h"
#include "raymath.h"

class Actor
{
public:
    Actor ();

    Vector3 get_forward ();
    Vector3 get_back ();
    Vector3 get_left ();
    Vector3 get_right ();
    Vector3 get_up ();
    Vector3 get_down ();
    Vector3 get_position ();

    Vector3    transform_point (Vector3 point);
    void       rotate_local_euler (Vector3 axis, float degrees);
    Vector3    position;
    Vector3    velocity;
    Quaternion rotation;
    // BoundingBox get_bounding_box ();

private:
    // none for now
    BoundingBox bounding_box = { 0 };
};

#endif
#ifndef UJU_ACTOR_H
#define UJU_ACTOR_H

#include "raylib.h"
#include "raymath.h"

typedef struct actor_t
{
    Vector3    position;
    Vector3    velocity;
    Quaternion rotation;
} actor_t;

Vector3 actor_get_forward (actor_t * p_actor);
Vector3 actor_get_back (actor_t * p_actor);
Vector3 actor_get_left (actor_t * p_actor);
Vector3 actor_get_right (actor_t * p_actor);
Vector3 actor_get_up (actor_t * p_actor);
Vector3 actor_get_down (actor_t * p_actor);
void actor_rotate_local_euler (actor_t * p_actor, Vector3 axis, float degrees);
Vector3 actor_transform_point (actor_t * p_actor, Vector3 point);

#endif
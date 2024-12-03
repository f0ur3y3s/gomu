#include "actor.h"

Vector3 actor_get_forward (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, 1 }, p_actor->rotation);
}

Vector3 actor_get_back (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, -1 }, p_actor->rotation);
}

Vector3 actor_get_left (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 1, 0, 0 }, p_actor->rotation);
}

Vector3 actor_get_right (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { -1, 0, 0 }, p_actor->rotation);
}

Vector3 actor_get_up (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 1, 0 }, p_actor->rotation);
}

Vector3 actor_get_down (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, -1, 0 }, p_actor->rotation);
}

void actor_rotate_local_euler (actor_t * p_actor, Vector3 axis, float degrees)
{
    float converted_degrees = degrees * DEG2RAD;
    p_actor->rotation       = QuaternionMultiply(
        p_actor->rotation, QuaternionFromAxisAngle(axis, converted_degrees));
}
#include "actor.h"

Vector3 get_forward (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, 1 }, p_actor->rotation);
}

Vector3 get_back (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, -1 }, p_actor->rotation);
}

Vector3 get_left (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 1, 0, 0 }, p_actor->rotation);
}

Vector3 get_right (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { -1, 0, 0 }, p_actor->rotation);
}

Vector3 get_up (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 1, 0 }, p_actor->rotation);
}

Vector3 get_down (actor_t * p_actor)
{
    return Vector3RotateByQuaternion((Vector3) { 0, -1, 0 }, p_actor->rotation);
}

void rotate_local_euler (actor_t * p_actor, Vector3 axis, float degrees)
{
    float converted_degrees = degrees * DEG2RAD;
    p_actor->rotation       = QuaternionMultiply(
        p_actor->rotation, QuaternionFromAxisAngle(axis, converted_degrees));
}
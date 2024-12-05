#include "actor.h"

Actor::Actor ()
{
    position = Vector3Zero();
    velocity = Vector3Zero();
    rotation = QuaternionIdentity();
}

Vector3 Actor::get_forward ()
{
    return Vector3RotateByQuaternion(Vector3 { 0, 0, 1 }, rotation);
}

Vector3 Actor::get_back ()
{
    return Vector3RotateByQuaternion(Vector3 { 0, 0, -1 }, rotation);
}

Vector3 Actor::get_left ()
{
    return Vector3RotateByQuaternion(Vector3 { 1, 0, 0 }, rotation);
}

Vector3 Actor::get_right ()
{
    return Vector3RotateByQuaternion(Vector3 { -1, 0, 0 }, rotation);
}

Vector3 Actor::get_up ()
{
    return Vector3RotateByQuaternion(Vector3 { 0, 1, 0 }, rotation);
}

Vector3 Actor::get_down ()
{
    return Vector3RotateByQuaternion(Vector3 { 0, -1, 0 }, rotation);
}

void Actor::rotate_local_euler (Vector3 axis, float degrees)
{
    float converted_degrees = degrees * DEG2RAD;
    rotation                = QuaternionMultiply(
        rotation, QuaternionFromAxisAngle(axis, converted_degrees));
}

Vector3 Actor::transform_point (Vector3 point)
{
    Matrix new_position = MatrixTranslate(position.x, position.y, position.z);
    Matrix new_rotation = QuaternionToMatrix(rotation);
    Matrix applied      = MatrixMultiply(new_rotation, new_position);
    return Vector3Transform(point, applied);
}

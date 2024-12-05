#include "uju_camera.h"

UJUCamera::UJUCamera (Vector3 position, Vector3 target, Vector3 up)
{
    camera.position   = position;
    camera.target     = target;
    camera.up         = up;
    camera.fovy       = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

UJUCamera::~UJUCamera ()
{
}

void UJUCamera::follow (Ship & p_ship, float delta_time)
{
    Vector3 position      = p_ship.transform_point(Vector3 { 0, 1, -3 });
    Vector3 ship_forwards = Vector3Scale(p_ship.get_forward(), 25.0f);
    Vector3 target        = Vector3Add(p_ship.position, ship_forwards);
    Vector3 up            = p_ship.get_up();

    move(position, target, up, delta_time);
}

void UJUCamera::move (Vector3 position,
                      Vector3 target,
                      Vector3 up,
                      float   delta_time)
{
    smooth_position = vector3_damp(smooth_position, position, 25, delta_time);
    smooth_target   = vector3_damp(smooth_target, target, 10, delta_time);
    smooth_up       = vector3_damp(smooth_up, up, 5, delta_time);

    set_position(smooth_position, smooth_target, smooth_up);
}

void UJUCamera::set_position (Vector3 position, Vector3 target, Vector3 up)
{
    camera.position = position;
    camera.target   = target;
    camera.up       = up;
}

Vector3 UJUCamera::get_position ()
{
    return camera.position;
}

void UJUCamera::draw_begin ()
{
    BeginMode3D(camera);
}

void UJUCamera::draw_end ()
{
    EndMode3D();
}
#include "camera.h"

camera_t * camera_init (Vector3 position, Vector3 target, Vector3 up)
{
    camera_t * p_camera = calloc(1, sizeof(camera_t));

    if (NULL == p_camera)
    {
        return NULL;
    }

    p_camera->camera = (Camera3D) {
        .position   = position,
        .target     = target,
        .up         = up,
        .fovy       = 45.0f,
        .projection = CAMERA_PERSPECTIVE,
    };

    p_camera->smooth_position = Vector3Zero();
    p_camera->smooth_target   = Vector3Zero();
    p_camera->smooth_up       = Vector3Zero();

    return p_camera;
}

void camera_follow (camera_t * p_camera, ship_t * p_ship)
{
    Vector3 position
        = actor_transform_point(&p_ship->actor, (Vector3) { 0, 1, -3 });
    Vector3 ship_forwards
        = Vector3Scale(actor_get_forward(&p_ship->actor), 25.0f);
    Vector3 target = Vector3Add(p_ship->actor.position, ship_forwards);
    Vector3 up     = actor_get_up(&p_ship->actor);

    camera_move(p_camera, position, target, up);
}

void camera_move (camera_t * p_camera,
                  Vector3    position,
                  Vector3    target,
                  Vector3    up)
{
    // p_camera->camera.position = vector3_damp(
    //     p_camera->camera.position, position, 15.0f, GetFrameTime());
    // p_camera->camera.target
    //     = vector3_damp(p_camera->camera.target, target, 5.0f,
    //     GetFrameTime());
    p_camera->camera.position = position;
    p_camera->camera.target   = target;
    p_camera->camera.up
        = vector3_damp(p_camera->camera.up, up, 5.0f, GetFrameTime());
}

void camera_set_position (camera_t * p_camera,
                          Vector3    position,
                          Vector3    target,
                          Vector3    up)
{
    p_camera->camera.position = position;
    p_camera->camera.target   = target;
    p_camera->camera.up       = up;
}

void camera_teardown (camera_t * p_camera)
{
    free(p_camera);
    p_camera = NULL;
}

Vector3 camera_get_position (camera_t * p_camera)
{
    return p_camera->camera.position;
}

void camera_begin (camera_t * p_camera)
{
    BeginMode3D(p_camera->camera);
}

void camera_end (void)
{
    EndMode3D();
}
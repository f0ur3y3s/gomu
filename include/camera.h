#ifndef UJU_CAMERA_H
#define UJU_CAMERA_H

#include "raylib.h"
#include "raymath.h"
#include "uju_utils.h"
#include "ship.h"

typedef struct camera_t
{
    Camera3D camera;
    Vector3  smooth_position;
    Vector3  smooth_target;
    Vector3  smooth_up;
} camera_t;

camera_t * camera_init (Vector3 position, Vector3 target, Vector3 up);
void       camera_follow (camera_t * p_camera, ship_t * p_ship);
void       camera_move (camera_t * p_camera,
                        Vector3    position,
                        Vector3    target,
                        Vector3    up);
void       camera_set_position (camera_t * p_camera,
                                Vector3    position,
                                Vector3    target,
                                Vector3    up);

void    camera_teardown (camera_t * p_camera);
Vector3 camera_get_position (camera_t * p_camera);
void    camera_begin (camera_t * p_camera);
void    camera_end (void);

#endif
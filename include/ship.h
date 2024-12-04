#ifndef UJU_SHIP_H
#define UJU_SHIP_H

#include <stdlib.h>
#include "actor.h"
#include "uju_utils.h"

typedef struct movement_t
{
    float max_speed;
    float engine_accel;
    float thruster_accel;
} movement_t;

typedef struct ship_t
{
    Model      model;
    actor_t    actor;
    movement_t movement_stat;
    float      health;
    float      smooth_forward;
    float      smooth_left;
    float      smooth_up;
    float      smooth_pitch_down;
    float      smooth_roll_right;
    float      smooth_yaw_left;
    float      input_forward;
    float      input_left;
    float      input_up;
    float      input_pitch_down;
    float      input_roll_right;
    float      input_yaw_left;

} ship_t;

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    const char * p_texture_path,
                    movement_t * p_movement_stats);
void     ship_draw (ship_t * p_ship);
void     ship_update (ship_t * p_ship, float delta_time);
void     ship_teardown (ship_t * p_ship);
void     ship_reset (ship_t * p_ship);

#endif
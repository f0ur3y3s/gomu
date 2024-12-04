#ifndef UJU_SHIP_H
#define UJU_SHIP_H

#include <stdlib.h>
#include "actor.h"
#include "uju_utils.h"

// typedef struct movement_t
// {
//     float max_speed;
//     float engine_accel;
//     float thruster_accel;
// } movement_t;

typedef struct movement_t
{
    float max_speed;
    float throttle_response;
    float turn_rate;
    float turn_response;
} movement_t;

typedef struct delta_t
{
    float forward;
    float left;
    float up;
    float pitch_down;
    float roll_right;
    float yaw_left;
} delta_t;

typedef struct ship_t

{
    Model      model;
    actor_t    actor;
    movement_t movement_stat;
    delta_t    input_delta;
    delta_t    smooth_delta;
    float      visual_bank;
    float      health;
    float      energy;
    float      boost_recharge_timer;
    bool       is_boosted;
} ship_t;

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    const char * p_texture_path,
                    movement_t * p_movement_stats);
void     ship_draw (ship_t * p_ship);
void     ship_update (ship_t * p_ship, float delta_time);
void     ship_teardown (ship_t * p_ship);
void     ship_reset (ship_t * p_ship);
void     delta_reset (delta_t * p_delta);

#endif
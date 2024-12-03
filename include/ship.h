#ifndef UJU_SHIP_H
#define UJU_SHIP_H

#include "actor.h"

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
} ship_t;

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    const char * p_texture_path,
                    movement_t * p_movement_stats);
bool     ship_draw (ship_t * p_ship);
bool     ship_update (ship_t * p_ship);
bool     ship_teardown (ship_t * p_ship);

#endif
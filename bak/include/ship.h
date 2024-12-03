#ifndef SHIP_H
#define SHIP_H

#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

typedef struct movement_t
{
    float max_speed;
    float engine_speed;
    float engine_accel;
    float thruster_speed;
    float thruster_accel;
} movement_t;

typedef struct ship_t

{
    Model      model;
    movement_t movement_stat;
    Matrix     rotation;
    Vector3    position;    // ship position xyz
    Vector3    translation; // thruster translation xyz
    float      scale;
} ship_t;

// create, destroy, update, draw

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    movement_t * p_movement_stats);
bool     ship_teardown (ship_t * p_ship);
bool     ship_draw (ship_t * p_ship);
bool     ship_update (ship_t * p_ship, const Matrix * p_rotation_matrix);

#endif
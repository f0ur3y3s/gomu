#ifndef UJU_SHIP_H
#define UJU_SHIP_H

#include "actor.h"

typedef struct ship_t
{
    Model   model;
    actor_t actor;
    float   max_speed;
    float   engine_accel;
    float   thruster_accel;
} ship_t;

#endif
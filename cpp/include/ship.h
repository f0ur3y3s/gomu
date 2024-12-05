#ifndef UJU_SHIP_H
#define UJU_SHIP_H

#include <stdlib.h>

#include "raylib.h"
#include "actor.h"
#include "uju_utils.h"

typedef struct movement_t
{
    float max_speed;
    float throttle_response;
    float turn_rate;
    float turn_response;
} movement_t;

class Delta
{
public:
    void  reset ();
    float forward    = 0.0f;
    float left       = 0.0f;
    float up         = 0.0f;
    float pitch_down = 0.0f;
    float roll_right = 0.0f;
    float yaw_left   = 0.0f;
};

void Delta::reset ()
{
    forward    = 0.0f;
    left       = 0.0f;
    up         = 0.0f;
    pitch_down = 0.0f;
    roll_right = 0.0f;
    yaw_left   = 0.0f;
}

class Ship : public Actor
{
public:
    Delta input_delta  = Delta();
    Delta smooth_delta = Delta();
    bool  is_boosted   = false;
    float health       = 100.0f;
    float energy       = 100.0f;

    Ship (Vector3      initial_position,
          const char * p_model_path,
          const char * p_texture_path,
          movement_t * p_movement_stats);
    ~Ship ();

    void draw ();
    void update (float delta_time);
    void reset ();

private:
    Model      model;
    Texture2D  texture;
    movement_t movement_stat;
    float      visual_bank          = 0.0f;
    float      boost_recharge_timer = 0.0f;
};

#endif
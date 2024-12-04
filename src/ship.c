#include "ship.h"

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    const char * p_texture_path,
                    movement_t * p_movement_stats)
{
    bool     status = false;
    ship_t * p_ship = calloc(1, sizeof(ship_t));

    if (NULL == p_ship)
    {
        goto EXIT;
    }

    p_ship->model = LoadModel(p_model_path);

    if (NULL != p_texture_path)
    {
        Texture2D texture = LoadTexture(p_texture_path);
        texture.mipmaps   = 0;
        SetTextureFilter(texture, TEXTURE_FILTER_POINT);
        p_ship->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }

    p_ship->movement_stat     = *p_movement_stats;
    p_ship->actor.position    = initial_position;
    p_ship->actor.velocity    = Vector3Zero();
    p_ship->actor.rotation    = QuaternionIdentity();
    p_ship->smooth_forward    = 0.0f;
    p_ship->smooth_left       = 0.0f;
    p_ship->smooth_up         = 0.0f;
    p_ship->smooth_pitch_down = 0.0f;
    p_ship->smooth_roll_right = 0.0f;
    p_ship->smooth_yaw_left   = 0.0f;
    p_ship->input_forward     = 0.0f;
    p_ship->input_left        = 0.0f;
    p_ship->input_up          = 0.0f;
    p_ship->input_pitch_down  = 0.0f;
    p_ship->input_roll_right  = 0.0f;
    p_ship->input_yaw_left    = 0.0f;

    status = true;

    if (!status)
    {
        free(p_ship);
        p_ship = NULL;
    }
EXIT:
    return p_ship;
}

void ship_update (ship_t * p_ship, float delta_time)
{
    p_ship->smooth_forward = float_damp(p_ship->smooth_forward,
                                        p_ship->input_forward,
                                        p_ship->movement_stat.engine_accel,
                                        delta_time);

    p_ship->smooth_left = float_damp(p_ship->smooth_left,
                                     p_ship->input_left,
                                     p_ship->movement_stat.thruster_accel,
                                     delta_time);

    p_ship->smooth_up = float_damp(p_ship->smooth_up,
                                   p_ship->input_up,
                                   p_ship->movement_stat.thruster_accel,
                                   delta_time);

    float speed_multiplier    = p_ship->smooth_forward > 0.0f ? 1.0f : 0.5f;
    float thruster_multiplier = 0.5f;

    Vector3 velocity = Vector3Zero();
    velocity         = Vector3Add(
        velocity,
        Vector3Scale(actor_get_forward(&p_ship->actor),
                     p_ship->movement_stat.max_speed * p_ship->smooth_forward
                         * speed_multiplier));
    velocity = Vector3Add(
        velocity,
        Vector3Scale(actor_get_left(&p_ship->actor),
                     (p_ship->movement_stat.max_speed / 2) * p_ship->smooth_left
                         * thruster_multiplier));
    velocity = Vector3Add(
        velocity,
        Vector3Scale(actor_get_up(&p_ship->actor),
                     (p_ship->movement_stat.max_speed / 2) * p_ship->smooth_up
                         * thruster_multiplier));

    p_ship->actor.velocity
        = vector3_damp(p_ship->actor.velocity, velocity, 0.1f, delta_time);
    p_ship->actor.position
        = Vector3Add(p_ship->actor.position,
                     Vector3Scale(p_ship->actor.velocity, delta_time));
    // p_ship->actor.position = Vector3Add(
    //     p_ship->actor.position,
    //     Vector3Scale(actor_get_forward(&p_ship->actor), smooth_forward));
    // p_ship->actor.position
    //     = Vector3Add(p_ship->actor.position,
    //                  Vector3Scale(actor_get_left(&p_ship->actor),
    //                  smooth_left));
    // p_ship->actor.position
    //     = Vector3Add(p_ship->actor.position,
    //                  Vector3Scale(actor_get_up(&p_ship->actor), smooth_up));
}

void ship_draw (ship_t * p_ship)
{
    DrawModel(p_ship->model, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, RAYWHITE);
    draw_axis(&((p_ship->actor).position));
}

void ship_teardown (ship_t * p_ship)
{
    UnloadModel(p_ship->model);
    free(p_ship);
}

void ship_reset (ship_t * p_ship)
{
    p_ship->actor.position    = Vector3Zero();
    p_ship->actor.velocity    = Vector3Zero();
    p_ship->actor.rotation    = QuaternionIdentity();
    p_ship->smooth_forward    = 0.0f;
    p_ship->smooth_left       = 0.0f;
    p_ship->smooth_up         = 0.0f;
    p_ship->smooth_pitch_down = 0.0f;
    p_ship->smooth_roll_right = 0.0f;
    p_ship->smooth_yaw_left   = 0.0f;
    p_ship->input_forward     = 0.0f;
    p_ship->input_left        = 0.0f;
    p_ship->input_up          = 0.0f;
    p_ship->input_pitch_down  = 0.0f;
    p_ship->input_roll_right  = 0.0f;
    p_ship->input_yaw_left    = 0.0f;
}
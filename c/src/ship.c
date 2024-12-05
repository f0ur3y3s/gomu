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

    p_ship->movement_stat        = *p_movement_stats;
    p_ship->actor.position       = initial_position;
    p_ship->actor.velocity       = Vector3Zero();
    p_ship->actor.rotation       = QuaternionIdentity();
    p_ship->visual_bank          = 0.0f;
    p_ship->health               = 100.0f;
    p_ship->energy               = 100.0f;
    p_ship->is_boosted           = false;
    p_ship->boost_recharge_timer = 0.0f;
    delta_reset(&(p_ship->input_delta));
    delta_reset(&(p_ship->smooth_delta));

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
    float boost = 1.0f;

    if (p_ship->is_boosted && p_ship->energy > 0.0f)
    {
        boost = 3.0f;
        p_ship->energy -= 10.0f * delta_time;
        p_ship->energy               = Clamp(p_ship->energy, 0.0f, 100.0f);
        p_ship->boost_recharge_timer = 0.0f;
    }
    else
    {
        p_ship->boost_recharge_timer += delta_time;

        if (p_ship->boost_recharge_timer >= 3.0f)
        {
            p_ship->energy += 20.0f * delta_time;
            p_ship->energy = Clamp(p_ship->energy, 0.0f, 100.0f);
        }
    }

    p_ship->smooth_delta.forward
        = float_damp(p_ship->smooth_delta.forward,
                     p_ship->input_delta.forward * boost,
                     p_ship->movement_stat.throttle_response,
                     delta_time);

    p_ship->smooth_delta.left
        = float_damp(p_ship->smooth_delta.left,
                     p_ship->input_delta.left,
                     p_ship->movement_stat.throttle_response,
                     delta_time);

    p_ship->smooth_delta.up
        = float_damp(p_ship->smooth_delta.up,
                     p_ship->input_delta.up,
                     p_ship->movement_stat.throttle_response,
                     delta_time);

    float speed_multiplier = p_ship->smooth_delta.forward > 0.0f ? 1.0f : 0.5f;

    Vector3 velocity = Vector3Zero();
    velocity         = Vector3Add(velocity,
                          Vector3Scale(actor_get_forward(&p_ship->actor),
                                       p_ship->movement_stat.max_speed
                                           * p_ship->smooth_delta.forward
                                           * speed_multiplier));
    velocity         = Vector3Add(velocity,
                          Vector3Scale(actor_get_up(&p_ship->actor),
                                       p_ship->movement_stat.max_speed * 0.5f
                                           * p_ship->smooth_delta.up));
    velocity         = Vector3Add(velocity,
                          Vector3Scale(actor_get_left(&p_ship->actor),
                                       p_ship->movement_stat.max_speed * 0.5f
                                           * p_ship->smooth_delta.left));

    p_ship->actor.velocity
        = vector3_damp(p_ship->actor.velocity, velocity, 2.5f, delta_time);
    p_ship->actor.position
        = Vector3Add(p_ship->actor.position,
                     Vector3Scale(p_ship->actor.velocity, delta_time));

    p_ship->smooth_delta.roll_right
        = float_damp(p_ship->smooth_delta.roll_right,
                     p_ship->input_delta.roll_right,
                     p_ship->movement_stat.turn_response,
                     delta_time);

    p_ship->smooth_delta.yaw_left
        = float_damp(p_ship->smooth_delta.yaw_left,
                     p_ship->input_delta.yaw_left,
                     p_ship->movement_stat.turn_response,
                     delta_time);

    p_ship->smooth_delta.pitch_down
        = float_damp(p_ship->smooth_delta.pitch_down,
                     p_ship->input_delta.pitch_down,
                     p_ship->movement_stat.turn_response,
                     delta_time);

    actor_rotate_local_euler(&p_ship->actor,
                             (Vector3) { 0, 0, 1 },
                             p_ship->smooth_delta.roll_right * delta_time
                                 * p_ship->movement_stat.turn_rate);
    actor_rotate_local_euler(&p_ship->actor,
                             (Vector3) { 0, 1, 0 },
                             p_ship->smooth_delta.yaw_left * delta_time
                                 * p_ship->movement_stat.turn_rate);
    actor_rotate_local_euler(&p_ship->actor,
                             (Vector3) { 1, 0, 0 },
                             p_ship->smooth_delta.pitch_down * delta_time
                                 * p_ship->movement_stat.turn_rate);

    actor_rotate_local_euler(&p_ship->actor,
                             (Vector3) { 0, 0, -1 },
                             p_ship->smooth_delta.yaw_left * delta_time * 0.5f
                                 * p_ship->movement_stat.turn_rate);

    float visual_bank = (-30 * DEG2RAD * p_ship->smooth_delta.yaw_left)
                        + (-15 * DEG2RAD * p_ship->smooth_delta.left);
    p_ship->visual_bank
        = float_damp(p_ship->visual_bank, visual_bank, 10.0f, delta_time);
    Quaternion visual_rotation
        = QuaternionFromAxisAngle((Vector3) { 0, 0, 1 }, p_ship->visual_bank);

    Matrix transform = MatrixTranslate(p_ship->actor.position.x,
                                       p_ship->actor.position.y,
                                       p_ship->actor.position.z);
    transform = MatrixMultiply(QuaternionToMatrix(visual_rotation), transform);
    transform
        = MatrixMultiply(QuaternionToMatrix(p_ship->actor.rotation), transform);
    p_ship->model.transform = transform;
}

void ship_draw (ship_t * p_ship)
{
    DrawModel(p_ship->model, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, RAYWHITE);
#ifdef DEBUG
    DrawLine3D(
        p_ship->actor.position,
        Vector3Add(p_ship->actor.position,
                   Vector3Scale(actor_get_forward(&p_ship->actor), 2.0f)),
        RED);
    DrawLine3D(p_ship->actor.position,
               Vector3Add(p_ship->actor.position,
                          Vector3Scale(actor_get_left(&p_ship->actor), 2.0f)),
               GREEN);
    DrawLine3D(p_ship->actor.position,
               Vector3Add(p_ship->actor.position,
                          Vector3Scale(actor_get_up(&p_ship->actor), 2.0f)),
               BLUE);
#endif
    // draw_axis(&(p_ship->actor.position));
}

void ship_teardown (ship_t * p_ship)
{
    UnloadModel(p_ship->model);
    free(p_ship);
}

void ship_reset (ship_t * p_ship)
{
    p_ship->actor.position       = Vector3Zero();
    p_ship->actor.velocity       = Vector3Zero();
    p_ship->actor.rotation       = QuaternionIdentity();
    p_ship->visual_bank          = 0.0f;
    p_ship->health               = 100.0f;
    p_ship->energy               = 100.0f;
    p_ship->is_boosted           = false;
    p_ship->boost_recharge_timer = 0.0f;

    delta_reset(&(p_ship->input_delta));
    delta_reset(&(p_ship->smooth_delta));
}

void delta_reset (delta_t * p_delta)
{
    *p_delta = (delta_t) { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
}
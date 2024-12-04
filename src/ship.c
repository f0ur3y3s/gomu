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

    if (IsModelValid(p_ship->model))
    {
        goto CLEANUP;
    }

    if (NULL != p_texture_path)
    {
        Texture2D texture = LoadTexture(p_texture_path);
        texture.mipmaps   = 0;
        SetTextureFilter(texture, TEXTURE_FILTER_POINT);
        p_ship->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }

    p_ship->movement_stat  = *p_movement_stats;
    p_ship->actor.position = initial_position;
    p_ship->actor.velocity = Vector3Zero();
    p_ship->actor.rotation = QuaternionIdentity();

    status = true;

CLEANUP:
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
    float smooth_forward = float_damp(p_ship->input_forward,
                                      p_ship->movement_stat.engine_accel,
                                      0.1f,
                                      delta_time);
    float smooth_left    = float_damp(p_ship->input_left,
                                   p_ship->movement_stat.engine_accel,
                                   0.1f,
                                   delta_time);
    float smooth_up      = float_damp(
        p_ship->input_up, p_ship->movement_stat.engine_accel, 0.1f, delta_time);
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
#include "ship.h"

ship_t * ship_init (Vector3      initial_position,
                    const char * p_model_path,
                    movement_t * p_movement_stats)
{
    ship_t * p_ship = calloc(1, sizeof(ship_t));

    if (p_ship)
    {
        p_ship->model         = LoadModel(p_model_path);
        p_ship->position      = initial_position;
        p_ship->translation   = (Vector3) { 0.0f, 0.0f, 0.0f };
        p_ship->rotation      = MatrixIdentity();
        p_ship->movement_stat = *p_movement_stats;
        p_ship->scale         = 1.0f;
    }

    return p_ship;
}

bool ship_teardown (ship_t * p_ship)
{
    if (p_ship)
    {
        UnloadModel(p_ship->model);
        free(p_ship);
        p_ship = NULL;
        return true;
    }

    return false;
}

bool ship_draw (ship_t * p_ship)
{
    if (p_ship)
    {
        DrawModel(p_ship->model, p_ship->position, p_ship->scale, WHITE);
        return true;
    }

    return false;
}
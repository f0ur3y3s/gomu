#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include "ui.h"
#include "ship.h"
#include "constants.h"

static float previous_scale_factor = 1.0f; // Store the previous scale factor

void calculate_aim (const int       screen_height,
                    const float     deadzone,
                    const Vector2 * p_screen_center,
                    Vector2 *       p_mouse_delta,
                    Vector2 *       p_out_aim)
{
    if ((NULL == p_screen_center) || (NULL == p_mouse_delta)
        || (NULL == p_out_aim))
    {
        return;
    }

    float distance = Vector2Distance(*p_mouse_delta, *p_screen_center);

    if ((deadzone > distance))
    {
        return; // Ignore small distances to avoid jittering
    }

    if (!IsCursorOnScreen())
    {
        p_out_aim->x = 0.0f;
        p_out_aim->y = 0.0f;
    }

    float radius = screen_height / 3.0f;

    // Clamp mouse position to circle's edge if outside the radius
    if (distance > radius)
    {
        Vector2 direction = Vector2Subtract(*p_mouse_delta, *p_screen_center);
        direction         = Vector2Normalize(direction);
        direction         = Vector2Scale(direction, radius);
        *p_mouse_delta    = Vector2Add(*p_screen_center, direction);
    }

    Vector2 aim = Vector2Subtract(*p_mouse_delta, *p_screen_center);
    aim         = Vector2Normalize(aim);

    // Adjust sensitivity and apply exponential factor
    float base_sensitivity = 0.05f; // Base sensitivity factor
    float exponent         = 2.0f;  // Exponential growth factor
    float scale_factor = powf(distance / radius, exponent) * base_sensitivity;
    if (distance > radius)
    {
        scale_factor = previous_scale_factor;
    }
    else
    {
        previous_scale_factor = scale_factor;
    }
    aim.x = aim.x * scale_factor * -1; // Invert x direction
    aim.y = aim.y * scale_factor * -1;

    p_out_aim->x += aim.x;
    p_out_aim->y += aim.y;

    printf("aim: %f, %f\n", p_out_aim->x, p_out_aim->y);
}

void update_ship_position (Vector3 * p_ship_position,
                           Matrix *  p_rotation_matrix)
{
    static float accel_speed  = 0.0f;  // Initialize acceleration speed
    const float  accel_factor = 0.01f; // Acceleration factor
    const float  max_speed    = 0.5f;  // Maximum speed
    const float  roll_speed = 0.05f; // Adjusted roll speed for smooth rotation

    // forward and backward movement
    if (IsKeyDown(KEY_W))
    {
        accel_speed += accel_factor;
        if (accel_speed > max_speed)
        {
            accel_speed = max_speed;
        }
        Vector3 forward = { p_rotation_matrix->m8,
                            p_rotation_matrix->m9,
                            p_rotation_matrix->m10 };
        *p_ship_position
            = Vector3Add(*p_ship_position, Vector3Scale(forward, accel_speed));
    }
    else if (IsKeyDown(KEY_S))
    {
        accel_speed -= accel_factor;
        if (accel_speed < -max_speed)
        {
            accel_speed = -max_speed;
        }
        Vector3 backward = { -p_rotation_matrix->m8,
                             -p_rotation_matrix->m9,
                             -p_rotation_matrix->m10 };
        *p_ship_position = Vector3Add(*p_ship_position,
                                      Vector3Scale(backward, -accel_speed));
    }
    else
    {
        // printf("accel_speed: %f\n", accel_speed);
        // get sign of accel_speed
        // bool is_neg = (*(unsigned int *)&accel_speed >> 31) & 1;
        float sign = accel_speed > 0.0f ? 1.0f : -1.0f;

        // decelerate
        if (accel_speed != 0.0f)
        {
            accel_speed -= sign * accel_factor;

            if (sign * accel_speed < 0.0f)
            {
                accel_speed = 0.0f;
            }

            Vector3 decel    = { sign * p_rotation_matrix->m8,
                                 sign * p_rotation_matrix->m9,
                                 sign * p_rotation_matrix->m10 };
            *p_ship_position = Vector3Add(
                *p_ship_position, Vector3Scale(decel, sign * accel_speed));
        }
    }

    // roll left and right
    // Roll left and right
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D))
    {
        p_ship_position->y += IsKeyDown(KEY_A) ? -roll_speed : roll_speed;
    }
}

int main (void)
{
    const int     screen_width  = 1920;
    const int     screen_height = 1080;
    const Vector2 screen_center = { screen_width / 2.0f, screen_height / 2.0f };

    InitWindow(screen_width, screen_height, "uju raylib");
    SetTargetFPS(60); // Set FPS

    Vector3  third_person_offset = { 0.0f, 20.0f, -30.0f };
    Camera3D camera              = { 0 };
    camera.position              = third_person_offset;
    camera.target                = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up                    = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy                  = 45.0f;              // Camera field-of-view Y
    camera.projection            = CAMERA_PERSPECTIVE; // Camera projection type

    Mesh  cylinder_mesh  = GenMeshCylinder(0.05f, 2.0f, 8);
    Model cylinder_model = LoadModelFromMesh(cylinder_mesh);

    Vector2 mouse_base_pos = GetMousePosition();
    Vector2 mouse_cur_pos  = GetMousePosition();
    Vector2 mouse_delta    = g_zero_vector2;

    movement_t movement_stats = { 1.0f, 0.0f, 0.1f, 0.0f, 0.01f };

    ship_t * p_ship
        = ship_init(g_zero_vector3,
                    ASSETS_PATH "Intergalactic_Spaceship-(Wavefront).obj",
                    &movement_stats);

    if (NULL)
    {
        goto UNLOAD_EXIT;
    }

    Vector2 aim = g_zero_vector2;
    HideCursor();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        {
            SetMousePosition(screen_center.x, screen_center.y);
            mouse_base_pos = GetMousePosition();
            aim            = g_zero_vector2;
        }

        if (IsKeyPressed(KEY_R))
        {
            SetMousePosition(screen_center.x, screen_center.y);
            mouse_base_pos   = GetMousePosition();
            aim              = g_zero_vector2;
            p_ship->position = g_zero_vector3;
        }

        mouse_cur_pos = GetMousePosition();
        mouse_delta   = Vector2Subtract(mouse_cur_pos, mouse_base_pos);
        mouse_delta   = Vector2Add(screen_center, mouse_delta);

        calculate_aim(
            screen_height, DEADZONE, &screen_center, &mouse_delta, &aim);

        camera.position = Vector3Add(p_ship->position, third_person_offset);
        camera.target   = p_ship->position;

        Matrix pitch_matrix = MatrixRotateX(aim.y);
        Matrix yaw_matrix   = MatrixRotateY(aim.x);

        Matrix rotation_matrix = MatrixMultiply(pitch_matrix, yaw_matrix);

        update_ship_position(&p_ship->position, &rotation_matrix);
        (p_ship->model).transform = rotation_matrix;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawGrid(10000, 1.0f);

        draw_axis(&cylinder_model, &g_zero_vector3);

        ship_draw(p_ship);
        EndMode3D();

        draw_reticle(screen_width, screen_height, DEADZONE);
        DrawCircleV(mouse_delta, 10, RED);

        DrawText("X: Red, Y: Green, Z: Blue", 10, 10, 20, DARKGRAY);
        DrawText(
            TextFormat("Mouse Delta: %f, %f", mouse_delta.x, mouse_delta.y),
            10,
            30,
            20,
            DARKGRAY);
        DrawFPS(10, 50);

        EndDrawing();
    }

UNLOAD_EXIT:
    UnloadModel(cylinder_model);
    ship_teardown(p_ship);

    CloseWindow();

    return 0;
}

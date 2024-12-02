#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#define DEADZONE 20.0f

void draw_axis (Model * p_model)
{
    if (NULL == p_model)
    {
        return;
    }

    // Transform and draw the X-axis cylinder (red)
    Matrix xAxisTransform = MatrixRotateZ(PI / 2.0f); // Rotate around Z-axis
    xAxisTransform        = MatrixMultiply(
        xAxisTransform, MatrixTranslate(0.0f, 0.0f, 0.0f)); // Move to X-axis
    p_model->transform = xAxisTransform;
    DrawModel(*p_model, Vector3Zero(), 1.0f, RED);

    // Transform and draw the Y-axis cylinder (green)
    Matrix yAxisTransform = MatrixTranslate(0.0f, 0.0f, 0.0f); // Move to Y-axis
    p_model->transform    = yAxisTransform;
    DrawModel(*p_model, Vector3Zero(), 1.0f, GREEN);

    // Transform and draw the Z-axis cylinder (blue)
    Matrix zAxisTransform = MatrixRotateX(PI / 2.0f); // Rotate around X-axis
    zAxisTransform        = MatrixMultiply(
        zAxisTransform, MatrixTranslate(0.0f, 0.0f, 0.0f)); // Move to Z-axis
    p_model->transform = zAxisTransform;
    DrawModel(*p_model, Vector3Zero(), 1.0f, BLUE);
}

void draw_reticle (const int   screen_width,
                   const int   screen_height,
                   const float deadzone)
{
    DrawCircleLines(screen_width / 2,
                    screen_height / 2,
                    screen_height / 3.0f,
                    Fade(RED, 0.5f));
    DrawCircleLines(
        screen_width / 2, screen_height / 2, deadzone, Fade(RED, 0.5f));
}

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
    aim.y = aim.y * scale_factor;

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
    const float  roll_speed   = 0.1f;

    // forward and backward movement
    if (IsKeyDown(KEY_W))
    {
        if (accel_speed < 0.0f) // If moving backward, reset speed
        {
            accel_speed = 0.0f;
        }
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
        if (accel_speed > 0.0f) // If moving forward, reset speed
        {
            accel_speed = 0.0f;
        }
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
        printf("accel_speed: %f\n", accel_speed);
        if (accel_speed > 0.0f)
        {
            accel_speed -= accel_factor;
            if (accel_speed < 0.0f)
            {
                accel_speed = 0.0f;
            }

            Vector3 backward = { -p_rotation_matrix->m8,
                                 -p_rotation_matrix->m9,
                                 -p_rotation_matrix->m10 };
            *p_ship_position = Vector3Add(*p_ship_position,
                                          Vector3Scale(backward, -accel_speed));
        }
        else if (accel_speed < 0.0f)
        {
            accel_speed += accel_factor;
            if (accel_speed > 0.0f)
            {
                accel_speed = 0.0f;
            }

            Vector3 forward  = { p_rotation_matrix->m8,
                                 p_rotation_matrix->m9,
                                 p_rotation_matrix->m10 };
            *p_ship_position = Vector3Add(*p_ship_position,
                                          Vector3Scale(forward, accel_speed));
        }
    }
}

int main (void)
{
    const int     screen_width  = 1920;
    const int     screen_height = 1080;
    const Vector2 screen_center = { screen_width / 2.0f, screen_height / 2.0f };
    InitWindow(screen_width, screen_height, "3D Axes with Thickness");
    SetTargetFPS(60); // Set FPS

    Camera3D camera   = { 0 };
    camera.position   = (Vector3) { 10.0f, 10.0f, 10.0f };
    camera.target     = (Vector3) { 0.0f, 0.0f, 0.0f };
    camera.up         = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy       = 45.0f;              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    Mesh  cylinder_mesh  = GenMeshCylinder(0.05f, 2.0f, 8);
    Model cylinder_model = LoadModelFromMesh(cylinder_mesh);

    Vector2 mouse_base_pos = GetMousePosition();
    Vector2 mouse_cur_pos  = GetMousePosition();
    Vector2 mouse_delta    = { 0.0f, 0.0f };

    Model ship_model
        = LoadModel(ASSETS_PATH "Intergalactic_Spaceship-(Wavefront).obj");
    Vector3 ship_position = { 0.0f, 0.0f, 0.0f };
    Vector2 aim           = { 0.0f, 0.0f };
    HideCursor();

    while (!WindowShouldClose())
    {
        if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
        {
            SetMousePosition(screen_center.x, screen_center.y);
            mouse_base_pos = GetMousePosition();
            aim            = (Vector2) { 0.0f, 0.0f };
        }

        if (IsKeyPressed(KEY_R))
        {
            SetMousePosition(screen_center.x, screen_center.y);
            mouse_base_pos = GetMousePosition();
            aim            = (Vector2) { 0.0f, 0.0f };
            ship_position  = (Vector3) { 0.0f, 0.0f, 0.0f };
        }

        mouse_cur_pos = GetMousePosition();
        mouse_delta   = Vector2Subtract(mouse_cur_pos, mouse_base_pos);
        mouse_delta   = Vector2Add(screen_center, mouse_delta);

        calculate_aim(
            screen_height, DEADZONE, &screen_center, &mouse_delta, &aim);
        camera.position     = (Vector3) { ship_position.x + 10.0f,
                                          ship_position.y + 10.0f,
                                          ship_position.z + 10.0f };
        camera.target       = ship_position;
        Matrix pitch_matrix = MatrixRotateX(aim.y);
        Matrix yaw_matrix   = MatrixRotateY(aim.x);

        Matrix rotation_matrix = MatrixMultiply(pitch_matrix, yaw_matrix);

        update_ship_position(&ship_position, &rotation_matrix);
        ship_model.transform = rotation_matrix;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawGrid(10000, 1.0f); // Draw grid to visualize space
        draw_axis(&cylinder_model);

        DrawModel(ship_model, ship_position, 0.5f, WHITE);

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
        EndDrawing();
    }

    UnloadModel(cylinder_model); // Unload the model
    UnloadModel(ship_model);     // Unload the model
    CloseWindow();               // Close window and OpenGL context

    return 0;
}

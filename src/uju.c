#include "uju.h"

#define SHIP_MODEL   ASSETS_PATH "lowpoly/ship.gltf"
#define SHIP_TEXTURE ASSETS_PATH "lowpoly/a16.png"

int main (int argc, char ** argv)
{
    // Initial setup
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju raylib");
    SetTargetFPS(60);

    movement_t movement_stats = {
        .max_speed      = 1.0f,
        .engine_accel   = 0.1f,
        .thruster_accel = 0.05f,
    };
    ship_t * p_ship = ship_init(
        (Vector3) { 0, 0, 0 }, SHIP_MODEL, SHIP_TEXTURE, &movement_stats);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// bool draw_reticle (const int   screen_width,
//                    const int   screen_height,
//                    const float deadzone)
// {
//     if ((0 >= screen_width) || (0 >= screen_height) || (0 >= deadzone))
//     {
//         return false;
//     }

//     float max_size
//         = screen_width > screen_height ? screen_height : screen_width;
//     int center_x = screen_width / 2;
//     int center_y = screen_height / 2;

//     DrawCircleLines(center_x, center_y, max_size / 3.0f, Fade(RED, 0.5f));
//     DrawCircleLines(center_x, center_y, deadzone, Fade(RED, 0.5f));
//     return true;
// }

// void calculate_aim (Vector2 * p_mouse_delta, Vector3 * p_rotation)
// {
//     static float previous_scale_factor = 0.0f;
//     float        distance = Vector2Distance(*p_mouse_delta, SCREEN_CENTER);

//     if ((DEADZONE > distance))
//     {
//         return; // Ignore small distances to avoid jittering
//     }

//     if (!IsCursorOnScreen())
//     {
//     }

//     float max_size
//         = SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_HEIGHT : SCREEN_WIDTH;
//     float radius = max_size / 3.0f;

//     // Clamp mouse position to circle's edge if outside the radius
//     if (distance > radius)
//     {
//         Vector2 direction = Vector2Subtract(*p_mouse_delta, SCREEN_CENTER);
//         direction         = Vector2Normalize(direction);
//         direction         = Vector2Scale(direction, radius);
//         *p_mouse_delta    = Vector2Add(SCREEN_CENTER, direction);
//     }

//     Vector2 aim = Vector2Subtract(*p_mouse_delta, SCREEN_CENTER);
//     aim         = Vector2Normalize(aim);

//     // Adjust sensitivity and apply exponential factor
//     float base_sensitivity = 0.5f; // Base sensitivity factor
//     float exponent         = 2.0f; // Exponential growth factor
//     float scale_factor = powf(distance / radius, exponent) *
//     base_sensitivity;

//     if (distance > radius)
//     {
//         scale_factor = previous_scale_factor;
//     }
//     else
//     {
//         previous_scale_factor = scale_factor;
//     }

//     aim.x = aim.x * scale_factor; // Invert x direction
//     aim.y = aim.y * scale_factor;
//     RotateLocalEuler((Vector3) { 0, 1, 0 }, -aim.x, p_rotation);
//     RotateLocalEuler((Vector3) { 1, 0, 0 }, -aim.y, p_rotation);
// }

// int main (void)
// {
//     setup();

//     Quaternion rotation       = QuaternionIdentity();
//     Vector3    position       = { 0.0f, 0.0f, 0.0f };
//     Vector3    velocity       = { 0.0f, 0.0f, 0.0f };
//     float      acceleration   = 0.1f;  // Acceleration rate
//     float      drag           = 0.98f; // Drag factor for deceleration
//     float      maxSpeed       = 1.0f;  // Maximum speed limit
//     float      speed          = 0.0f;
//     Vector2    mouse_base_pos = GetMousePosition();
//     Vector2    mouse_cur_pos  = GetMousePosition();
//     Vector2    mouse_delta    = Vector2Zero();

//     while (!WindowShouldClose())
//     {
//         if (IsKeyDown(KEY_R))
//         {
//             // reset the position and rotation
//         }

//         if (IsKeyDown(KEY_SPACE))
//         {
//             if (IsKeyDown(KEY_W))
//             {
//                 position = Vector3Add(position,
//                                       Vector3Scale(GetUp(&rotation), 0.05f));
//             }
//             if (IsKeyDown(KEY_S))
//             {
//                 position = Vector3Add(position,
//                                       Vector3Scale(GetDown(&rotation),
//                                       0.05f));
//             }
//             if (IsKeyDown(KEY_A))
//             {
//                 position = Vector3Add(position,
//                                       Vector3Scale(GetLeft(&rotation),
//                                       0.05f));
//             }
//             if (IsKeyDown(KEY_D))
//             {
//                 position = Vector3Add(position,
//                                       Vector3Scale(GetRight(&rotation),
//                                       0.05f));
//             }
//         }
//         else
//         {
//             if (IsKeyDown(KEY_W))
//             {
//                 speed += acceleration;
//             }
//             if (IsKeyDown(KEY_S))
//             {
//                 speed -= acceleration;
//             }
//             if (IsKeyDown(KEY_A))
//             {
//                 RotateLocalEuler((Vector3) { 0, 0, 1 }, -1.0f, &rotation);
//             }
//             if (IsKeyDown(KEY_D))
//             {
//                 RotateLocalEuler((Vector3) { 0, 0, 1 }, 1.0f, &rotation);
//             }

//             if (IsKeyDown(KEY_Q))
//             {
//                 speed = 0.0f;
//             }
//         }

//         if (speed > maxSpeed)
//         {
//             speed = maxSpeed;
//         }
//         else if (speed < -maxSpeed)
//         {
//             speed = -maxSpeed;
//         }

//         if (speed > 0)
//         {
//             velocity = Vector3Scale(GetForward(&rotation), speed);
//         }
//         else if (speed < 0)
//         {
//             velocity = Vector3Scale(GetBack(&rotation), -speed);
//         }
//         else
//         {
//             velocity = Vector3Zero();
//         }

//         position = Vector3Add(position, velocity);

//         mouse_cur_pos = GetMousePosition();
//         mouse_delta   = Vector2Subtract(mouse_cur_pos, mouse_base_pos);
//         mouse_delta   = Vector2Add(SCREEN_CENTER, mouse_delta);
//         calculate_aim(&mouse_delta, &rotation);

//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         BeginMode3D(camera);
//         DrawGrid(10000, 1.0f);

//         // draw the ship
//         Matrix transform = MatrixTranslate(position.x, position.y,
//         position.z); transform = MatrixMultiply(QuaternionToMatrix(rotation),
//         transform); ship.transform = transform;

//         // calculate the camera position

//         DrawModel(ship, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, RAYWHITE);
//         DrawLine3D(
//             position, Vector3Add(position, GetForward(&rotation)), GREEN);
//         DrawLine3D(position, Vector3Add(position, GetLeft(&rotation)), RED);
//         DrawLine3D(position, Vector3Add(position, GetUp(&rotation)), BLUE);

//         EndMode3D();
//         DrawFPS(10, 10);
//         DrawText(TextFormat("Rotation: %f %f %f %f",
//                             rotation.x,
//                             rotation.y,
//                             rotation.z,
//                             rotation.w),
//                  10,
//                  30,
//                  20,
//                  BLACK);
//         DrawText(TextFormat(
//                      "Position: %f %f %f", position.x, position.y,
//                      position.z),
//                  10,
//                  50,
//                  20,
//                  BLACK);
//         DrawText(TextFormat(
//                      "Velocity: %f %f %f", velocity.x, velocity.y,
//                      velocity.z),
//                  10,
//                  70,
//                  20,
//                  BLACK);

//         draw_reticle(SCREEN_WIDTH, SCREEN_HEIGHT, DEADZONE);
//         DrawCircleV(mouse_delta, 10, RED);

//         EndDrawing();
//     }

//     UnloadModel(ship);
//     CloseWindow();
// }
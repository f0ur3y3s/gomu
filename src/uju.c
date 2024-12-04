#include "uju.h"

#define SHIP_MODEL   ASSETS_PATH "lowpoly/ship.gltf"
#define SHIP_TEXTURE ASSETS_PATH "lowpoly/a16.png"
#define HIGH_POLY_MODEL \
    ASSETS_PATH "highres/Intergalactic_Spaceship-(Wavefront).obj"

Vector2 update_aim (ship_t * p_ship, Vector2 * p_out_mouse_delta)
{
    Vector2 mouse_delta   = GetMousePosition();
    Vector2 screen_center = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    mouse_delta           = Vector2Subtract(mouse_delta, screen_center);

    if (Vector2Length(mouse_delta) < DEADZONE)
    {
        *p_out_mouse_delta = screen_center;
        return (Vector2) { 0, 0 };
    }

    float max_size
        = SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_HEIGHT : SCREEN_WIDTH;
    float radius       = max_size / 3.0f;
    float delta_length = Vector2Length(mouse_delta);

    if (delta_length > radius)
    {
        mouse_delta = Vector2Scale(Vector2Normalize(mouse_delta), radius);
    }

    *p_out_mouse_delta = Vector2Add(mouse_delta, screen_center);

    Vector2 aim    = Vector2Divide(mouse_delta, (Vector2) { radius, radius });
    aim.x          = Clamp(aim.x, -1.0f, 1.0f);
    aim.y          = Clamp(aim.y, -1.0f, 1.0f);
    float exponent = 2.0f;
    aim.x          = copysign(pow(fabs(aim.x), exponent), aim.x);
    aim.y          = copysign(pow(fabs(aim.y), exponent), aim.y);
    return aim;
}

void update_input (ship_t * p_ship, Vector2 * p_out_mouse_delta)
{
    delta_reset(&(p_ship->input_delta));

    if (IsKeyDown(KEY_R))
    {
        ship_reset(p_ship);
    }

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        p_ship->is_boosted = true;
    }
    else
    {
        p_ship->is_boosted = false;
    }

    // normal flight controls
    if (IsKeyDown(KEY_W))
    {
        p_ship->input_delta.forward += 1.0f;
    }
    if (IsKeyDown(KEY_S))
    {
        p_ship->input_delta.forward += -1.0f;
    }
    if (IsKeyDown(KEY_A))
    {
        p_ship->input_delta.left += 1.0f;
    }
    if (IsKeyDown(KEY_D))
    {
        p_ship->input_delta.left += -1.0f;
    }

    if (IsKeyDown(KEY_Q))
    {
        p_ship->input_delta.roll_right = -0.5f;
    }
    if (IsKeyDown(KEY_E))
    {
        p_ship->input_delta.roll_right = 0.5f;
    }

    if (IsKeyDown(KEY_SPACE))
    {
        p_ship->input_delta.up += 1.0f;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        p_ship->input_delta.up += -1.0f;
    }

    Vector2 aim = update_aim(p_ship, p_out_mouse_delta);

    p_ship->input_delta.yaw_left   = -aim.x;
    p_ship->input_delta.pitch_down = aim.y;
}

int main (int argc, char ** argv)
{
    // Initial setup
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju raylib");
    SetTargetFPS(60);

    camera_t * p_camera = camera_init(
        (Vector3) { 0, 1, -3 }, (Vector3) { 0, 0, 0 }, (Vector3) { 0, 1, 0 });
    // movement_t movement_stats = {
    //     .max_speed      = 40.0f,
    //     .engine_accel   = 20.0f,
    //     .thruster_accel = 10.0f,
    // };
    movement_t movement_stats = {
        .max_speed         = 20.0f,
        .throttle_response = 10.0f,
        .turn_rate         = 180.0f,
        .turn_response     = 10.0f,
    };
    ship_t * p_ship = ship_init(
        (Vector3) { 0, 0, 0 }, SHIP_MODEL, SHIP_TEXTURE, &movement_stats);

    if (NULL == p_ship)
    {
        printf("Failed to initialize ship\n");
    }

    // add rings thoughout the level
    Model   ring_model = LoadModelFromMesh(GenMeshTorus(0.1f, 50.0f, 16, 32));
    Vector3 ring_locations[100] = { 0 };

    for (int i = 0; i < 100; i++)
    {
        ring_locations[i] = (Vector3) { GetRandomValue(-500, 500),
                                        GetRandomValue(-500, 500),
                                        GetRandomValue(-500, 500) };
    }

    // Main game loop
    Vector2      mouse_delta = { 0, 0 };
    static float timer       = 0.0f;
    bool         health_up   = false;
    while (!WindowShouldClose())
    {
        float frame_time = GetFrameTime();

        timer += frame_time;
        if (timer >= 1.0f)
        {
            if (health_up)
            {
                p_ship->health += 10.0f;

                if (p_ship->health >= 100.0f)
                {
                    health_up = false;
                }
            }
            else
            {
                p_ship->health -= 10.0f;

                if (p_ship->health <= 0.0f)
                {
                    health_up = true;
                }
            }
            timer = 0.0f;
        }

        // Update
        update_input(p_ship, &mouse_delta);
        ship_update(p_ship, frame_time);

        // Draw calls
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3D drawing
        BeginMode3D(p_camera->camera);

        for (int i = 0; i < 100; i++)
        {
            DrawModel(ring_model, ring_locations[i], 1.0f, GRAY);
        }

        DrawGrid(10000, 10.0f);
        ship_draw(p_ship);
        camera_follow(p_camera, p_ship, frame_time);
        EndMode3D();

        // 2D drawing
        draw_reticle(SCREEN_WIDTH, SCREEN_HEIGHT, DEADZONE);
        draw_mouse(mouse_delta);
        draw_health(SCREEN_WIDTH, SCREEN_HEIGHT, p_ship->health);
        draw_energy(SCREEN_WIDTH, SCREEN_HEIGHT, p_ship->energy);
        DrawFPS(10, 10);
        DrawText(TextFormat("Rotation: %f %f %f %f",
                            p_ship->actor.rotation.x,
                            p_ship->actor.rotation.y,
                            p_ship->actor.rotation.z,
                            p_ship->actor.rotation.w),
                 10,
                 30,
                 20,
                 BLACK);
        DrawText(TextFormat("Position: %f %f %f",
                            p_ship->actor.position.x,
                            p_ship->actor.position.y,
                            p_ship->actor.position.z),
                 10,
                 50,
                 20,
                 BLACK);
        DrawText(TextFormat("Velocity: %f %f %f",
                            p_ship->actor.velocity.x,
                            p_ship->actor.velocity.y,
                            p_ship->actor.velocity.z),
                 10,
                 70,
                 20,
                 BLACK);
        DrawText(TextFormat("Input: %f %f %f",
                            p_ship->input_delta.forward,
                            p_ship->input_delta.left,
                            p_ship->input_delta.up),
                 10,
                 90,
                 20,
                 BLACK);
        EndDrawing();
    }

    ship_teardown(p_ship);
    camera_teardown(p_camera);
    UnloadModel(ring_model);
    CloseWindow();

    return 0;
}

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
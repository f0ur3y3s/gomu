// #include "uju.h"

// int main (int argc, char ** argv)
// {
//     InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju raylib");

//     SetTargetFPS(60);

//     while (!WindowShouldClose())
//     {
//         BeginDrawing();
//         ClearBackground(RAYWHITE);

//         DrawText("uju", 10, 10, 20, DARKGRAY);

//         EndDrawing();
//     }

//     CloseWindow();

//     return 0;
// }

#include "raylib.h"
#include "raymath.h"
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH  1920
#define DEADZONE      20.0f
#define SCREEN_CENTER (Vector2) { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }
static const Vector3 camera_offset = { 0.0f, 1.0f, -3.0f };
Camera               camera        = { .position   = { 0.0f, 1.0f, -3.0f },
                                       .target     = { 0.0f, 0.0f, 0.0f },
                                       .up         = { 0.0f, 1.0f, 0.0f },
                                       .fovy       = 45.0f,
                                       .projection = CAMERA_PERSPECTIVE };

bool draw_axis (const Vector3 * p_position)
{
    Vector3 origin = *p_position;

    float axis_length = 2.0f;

    Vector3 x_end = { origin.x + axis_length, origin.y, origin.z };
    DrawLine3D(origin, x_end, RED);

    Vector3 y_end = { origin.x, origin.y + axis_length, origin.z };
    DrawLine3D(origin, y_end, GREEN);

    Vector3 z_end = { origin.x, origin.y, origin.z + axis_length };
    DrawLine3D(origin, z_end, BLUE);

    return true;
}

bool draw_reticle (const int   screen_width,
                   const int   screen_height,
                   const float deadzone)
{
    if ((0 >= screen_width) || (0 >= screen_height) || (0 >= deadzone))
    {
        return false;
    }

    float max_size
        = screen_width > screen_height ? screen_height : screen_width;
    int center_x = screen_width / 2;
    int center_y = screen_height / 2;

    DrawCircleLines(center_x, center_y, max_size / 3.0f, Fade(RED, 0.5f));
    DrawCircleLines(center_x, center_y, deadzone, Fade(RED, 0.5f));
    return true;
}

void setup (void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju");

    SetTargetFPS(60); // Set FPS
}

// objects follow the right hand rule
Vector3 GetForward (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, 1 }, *p_rotation);
}

Vector3 GetBack (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 0, -1 }, *p_rotation);
}

Vector3 GetLeft (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { 1, 0, 0 }, *p_rotation);
}

Vector3 GetRight (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { -1, 0, 0 }, *p_rotation);
}

Vector3 GetUp (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { 0, 1, 0 }, *p_rotation);
}

Vector3 GetDown (Quaternion * p_rotation)
{
    return Vector3RotateByQuaternion((Vector3) { 0, -1, 0 }, *p_rotation);
}

void RotateLocalEuler (Vector3 axis, float degrees, Quaternion * p_rotation)
{
    float converted_degrees = degrees * DEG2RAD;
    *p_rotation             = QuaternionMultiply(
        *p_rotation, QuaternionFromAxisAngle(axis, converted_degrees));
}

Vector3 TransformPoint (Vector3 point, Quaternion * p_rotation, Vector3 * p_pos)
{
    Matrix mPos = MatrixTranslate(p_pos->x, p_pos->y, p_pos->z);
    Matrix mRot = QuaternionToMatrix(*p_rotation);
    Matrix m    = MatrixMultiply(mRot, mPos);
    return Vector3Transform(point, m);
}

void calculate_aim (Vector2 * p_mouse_delta, Vector3 * p_rotation)
{
    static float previous_scale_factor = 0.0f;
    float        distance = Vector2Distance(*p_mouse_delta, SCREEN_CENTER);

    if ((DEADZONE > distance))
    {
        return; // Ignore small distances to avoid jittering
    }

    if (!IsCursorOnScreen())
    {
    }

    float max_size
        = SCREEN_WIDTH > SCREEN_HEIGHT ? SCREEN_HEIGHT : SCREEN_WIDTH;
    float radius = max_size / 3.0f;

    // Clamp mouse position to circle's edge if outside the radius
    if (distance > radius)
    {
        Vector2 direction = Vector2Subtract(*p_mouse_delta, SCREEN_CENTER);
        direction         = Vector2Normalize(direction);
        direction         = Vector2Scale(direction, radius);
        *p_mouse_delta    = Vector2Add(SCREEN_CENTER, direction);
    }

    Vector2 aim = Vector2Subtract(*p_mouse_delta, SCREEN_CENTER);
    aim         = Vector2Normalize(aim);

    // Adjust sensitivity and apply exponential factor
    float base_sensitivity = 0.5f; // Base sensitivity factor
    float exponent         = 2.0f; // Exponential growth factor
    float scale_factor = powf(distance / radius, exponent) * base_sensitivity;

    if (distance > radius)
    {
        scale_factor = previous_scale_factor;
    }
    else
    {
        previous_scale_factor = scale_factor;
    }

    aim.x = aim.x * scale_factor; // Invert x direction
    aim.y = aim.y * scale_factor;
    RotateLocalEuler((Vector3) { 0, 1, 0 }, -aim.x, p_rotation);
    RotateLocalEuler((Vector3) { 1, 0, 0 }, -aim.y, p_rotation);
}

int main (void)
{
    setup();

    Texture2D texture = LoadTexture(ASSETS_PATH "lowpoly/a16.png");
    texture.mipmaps   = 0;
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    Model ship = LoadModel(ASSETS_PATH "lowpoly/ship.gltf");
    ship.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    if (!IsModelValid(ship))
    {
        TraceLog(LOG_ERROR, "Model is not valid");
        return 1;
    }

    HideCursor();
    Quaternion rotation       = QuaternionIdentity();
    Vector3    position       = { 0.0f, 0.0f, 0.0f };
    Vector3    velocity       = { 0.0f, 0.0f, 0.0f };
    float      acceleration   = 0.1f;  // Acceleration rate
    float      drag           = 0.98f; // Drag factor for deceleration
    float      maxSpeed       = 1.0f;  // Maximum speed limit
    float      speed          = 0.0f;
    Vector2    mouse_base_pos = GetMousePosition();
    Vector2    mouse_cur_pos  = GetMousePosition();
    Vector2    mouse_delta    = Vector2Zero();

    // array of buildings
    Vector3 buildings[100];
    Vector3 building_sizes[100];
    for (int i = 0; i < 100; i++)
    {
        buildings[i]      = (Vector3) { GetRandomValue(-1000, 1000),
                                        0.0f,
                                        GetRandomValue(-1000, 1000) };
        building_sizes[i] = (Vector3) { GetRandomValue(5, 20),
                                        GetRandomValue(5, 20),
                                        GetRandomValue(5, 20) };
    }

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_R))
        {
            // reset the position and rotation
            position       = Vector3Zero();
            velocity       = Vector3Zero();
            rotation       = QuaternionIdentity();
            ship.transform = MatrixIdentity();
            speed          = 0.0f;
            SetMousePosition(SCREEN_CENTER.x, SCREEN_CENTER.y);
            mouse_base_pos = GetMousePosition();
        }

        if (IsKeyDown(KEY_SPACE))
        {
            if (IsKeyDown(KEY_W))
            {
                position = Vector3Add(position,
                                      Vector3Scale(GetUp(&rotation), 0.05f));
            }
            if (IsKeyDown(KEY_S))
            {
                position = Vector3Add(position,
                                      Vector3Scale(GetDown(&rotation), 0.05f));
            }
            if (IsKeyDown(KEY_A))
            {
                position = Vector3Add(position,
                                      Vector3Scale(GetLeft(&rotation), 0.05f));
            }
            if (IsKeyDown(KEY_D))
            {
                position = Vector3Add(position,
                                      Vector3Scale(GetRight(&rotation), 0.05f));
            }
        }
        else
        {
            if (IsKeyDown(KEY_W))
            {
                speed += acceleration;
                // velocity = Vector3Add(
                //     velocity,
                //     Vector3Scale(GetForward(&rotation), acceleration));

                // position = Vector3Add(
                //     position, Vector3Scale(GetForward(&rotation), 0.05f));
            }
            if (IsKeyDown(KEY_S))
            {
                speed -= acceleration;
                // velocity = Vector3Add(
                //     velocity, Vector3Scale(GetBack(&rotation),
                //     acceleration));

                // position = Vector3Add(position,
                //                       Vector3Scale(GetBack(&rotation),
                //                       0.05f));
            }
            if (IsKeyDown(KEY_A))
            {
                RotateLocalEuler((Vector3) { 0, 0, 1 }, -1.0f, &rotation);
            }
            if (IsKeyDown(KEY_D))
            {
                RotateLocalEuler((Vector3) { 0, 0, 1 }, 1.0f, &rotation);
            }

            if (IsKeyDown(KEY_Q))
            {
                // decelerate
                speed = 0.0f;
            }
        }
        // position should be updated by rotation
        // velocity = Vector3Scale(velocity, drag);

        // Limit speed to maxSpeed
        // float speed = Vector3Length(velocity);

        // if (speed > maxSpeed)
        // {
        //     velocity = Vector3Scale(velocity, maxSpeed / speed);
        // }

        if (speed > maxSpeed)
        {
            speed = maxSpeed;
        }
        else if (speed < -maxSpeed)
        {
            speed = -maxSpeed;
        }

        if (speed > 0)
        {
            velocity = Vector3Scale(GetForward(&rotation), speed);
        }
        else if (speed < 0)
        {
            velocity = Vector3Scale(GetBack(&rotation), -speed);
        }
        else
        {
            velocity = Vector3Zero();
        }

        position = Vector3Add(position, velocity);

        // if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        // {
        mouse_cur_pos = GetMousePosition();
        mouse_delta   = Vector2Subtract(mouse_cur_pos, mouse_base_pos);
        mouse_delta   = Vector2Add(SCREEN_CENTER, mouse_delta);
        calculate_aim(&mouse_delta, &rotation);

        // SetMousePosition(screen_center.x, screen_center.y);

        // RotateLocalEuler(
        //     (Vector3) { 0, 1, 0 }, -delta.x * 0.01f, &rotation);
        // RotateLocalEuler(
        //     (Vector3) { 1, 0, 0 }, -delta.y * 0.01f, &rotation);
        // }

        // camera.target   = position;
        // camera.position = Vector3Add(position, camera_offset);

        // Vector3 new_camera_position
        //     = TransformPoint(position, &rotation, &camera_offset);
        // Vector3 ship_forwards =
        // Vector3Scale(GetForward(&rotation), 5.0f); Vector3 target =
        // Vector3Add(position, ship_forwards); Vector3 up            =
        // GetUp(&rotation);

        // camera.position = new_camera_position;
        // camera.target   = target;
        // camera.up       = up;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(10000, 1.0f);

        // draw the ship
        Matrix transform = MatrixTranslate(position.x, position.y, position.z);
        transform = MatrixMultiply(QuaternionToMatrix(rotation), transform);
        ship.transform = transform;

        // calculate the camera position

        DrawModel(ship, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, RAYWHITE);
        // DrawModel(ship, (Vector3) { 0.0f, 0.0f, 0.0f }, 5.0f, RAYWHITE);
        DrawLine3D(
            position, Vector3Add(position, GetForward(&rotation)), GREEN);
        DrawLine3D(position, Vector3Add(position, GetLeft(&rotation)), RED);
        DrawLine3D(position, Vector3Add(position, GetUp(&rotation)), BLUE);

        for (int i = 0; i < sizeof(buildings) / sizeof(Vector3); i++)
        {
            DrawCubeV(buildings[i], building_sizes[i], GRAY);
        }

        EndMode3D();
        DrawFPS(10, 10);
        DrawText(TextFormat("Rotation: %f %f %f %f",
                            rotation.x,
                            rotation.y,
                            rotation.z,
                            rotation.w),
                 10,
                 30,
                 20,
                 BLACK);
        DrawText(TextFormat(
                     "Position: %f %f %f", position.x, position.y, position.z),
                 10,
                 50,
                 20,
                 BLACK);
        DrawText(TextFormat(
                     "Velocity: %f %f %f", velocity.x, velocity.y, velocity.z),
                 10,
                 70,
                 20,
                 BLACK);

        draw_reticle(SCREEN_WIDTH, SCREEN_HEIGHT, DEADZONE);
        DrawCircleV(mouse_delta, 10, RED);

        EndDrawing();
        Vector3 camera_position
            = TransformPoint(camera_offset, &rotation, &position);
        camera.position = camera_position;
        camera.target   = position;
        camera.up       = GetUp(&rotation);
    }

    UnloadModel(ship);
    CloseWindow();
}
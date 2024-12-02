#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

int main (void)
{
    const int screenWidth  = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "3D Rectangle Control");

    // Camera setup
    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = (Vector3) { 10.0f, 10.0f, 10.0f }; // Camera position
    camera.target   = (Vector3) { 0.0f, 0.0f, 0.0f }; // Camera looking at point
    camera.up       = (Vector3) {
        0.0f, 1.0f, 0.0f
    }; // Camera up vector (rotation towards target)
    camera.fovy       = 45.0f;              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    Vector3 model_size = { 1.0f, 0.5f, 2.0f };
    Mesh    mesh       = GenMeshCube(model_size.x, model_size.y, model_size.z);
    Model   model      = LoadModelFromMesh(mesh);
    Vector3 model_position = { 0.0f, 0.0f, 0.0f };
    Vector3 model_rotation = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60); // Set FPS

    while (!WindowShouldClose())
    {
        float wheelMove = GetMouseWheelMove();

        if (0 != wheelMove)
        {
            Vector3 direction = Vector3Subtract(camera.target, camera.position);
            direction         = Vector3Normalize(direction);
            camera.position   = Vector3Add(camera.position,
                                         Vector3Scale(direction, wheelMove));
        }

        if (IsKeyDown(KEY_R))
        {
            model_position = (Vector3) { 0.0f, 0.0f, 0.0f };
            model_rotation = (Vector3) { 0.0f, 0.0f, 0.0f };
        }

        // Update rotation
        if (IsKeyDown(KEY_LEFT))
            model_rotation.y += 1.0f;
        if (IsKeyDown(KEY_RIGHT))
            model_rotation.y -= 1.0f;
        if (IsKeyDown(KEY_UP))
            model_rotation.x -= 1.0f;
        if (IsKeyDown(KEY_DOWN))
            model_rotation.x += 1.0f;

        Matrix transform
            = MatrixRotateXYZ((Vector3) { DEG2RAD * model_rotation.x,
                                          DEG2RAD * model_rotation.y,
                                          DEG2RAD * model_rotation.z });
        model.transform = transform;

        Vector3 forward = { transform.m8,
                            transform.m9,
                            transform.m10 }; // Forward vector (Z axis)
        Vector3 right   = { transform.m0,
                            transform.m1,
                            transform.m2 }; // Right vector (X axis)
        Vector3 up      = { transform.m4,
                            transform.m5,
                            transform.m6 }; // Up vector (Y axis)

        // Movement controls
        if (IsKeyDown(KEY_W))
            model_position
                = Vector3Subtract(model_position, Vector3Scale(forward, 0.1f));
        if (IsKeyDown(KEY_S))
            model_position
                = Vector3Add(model_position, Vector3Scale(forward, 0.1f));
        if (IsKeyDown(KEY_A))
            model_position
                = Vector3Subtract(model_position, Vector3Scale(right, 0.1f));
        if (IsKeyDown(KEY_D))
            model_position
                = Vector3Add(model_position, Vector3Scale(right, 0.1f));
        if (IsKeyDown(KEY_Q))
            model_position
                = Vector3Subtract(model_position, Vector3Scale(up, 0.1f));
        if (IsKeyDown(KEY_E))
            model_position = Vector3Add(model_position, Vector3Scale(up, 0.1f));

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(camera);
        DrawGrid(20, 1.0f); // Draw grid to visualize space

        DrawModel(model, model_position, 1.0f, RED);
        // draw the forward vector on top
        DrawLine3D(model_position,
                   Vector3Add(model_position, Vector3Scale(forward, 2.0f)),
                   GREEN);

        EndMode3D();

        DrawText("W/S/A/D: Move, Arrow Keys: Rotate", 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Position: %.2f, %.2f, %.2f",
                            model_position.x,
                            model_position.y,
                            model_position.z),
                 10,
                 40,
                 20,
                 DARKGRAY);
        DrawText(TextFormat("Rotation: %.2f, %.2f, %.2f",
                            model_rotation.x,
                            model_rotation.y,
                            model_rotation.z),
                 10,
                 60,
                 20,
                 DARKGRAY);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context

    return 0;
}
#include "uju.h"

static Vector2 update_aim (Ship & p_ship, Vector2 * p_out_mouse_delta);
static void    update_input (Ship & p_ship, Vector2 * p_out_mouse_delta);

int main (void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "uju raylib");
    InitAudioDevice();

    SetTargetFPS(60);

    movement_t movement_stats = {
        20.0f,  // max_speed
        20.0f,  // throttle_response
        180.0f, // turn_rate
        10.0f   // turn_response
    };

    Ship ship = Ship(Vector3 { 0.0f, 0.0f, 0.0f },
                     SHIP_MODEL,
                     SHIP_TEXTURE,
                     ENGINE_AUDIO,
                     &movement_stats);

    UJUCamera camera = UJUCamera(Vector3 { 0.0f, 1.0f, -3.0f },
                                 Vector3Zero(),
                                 Vector3 { 0.0f, 1.0f, 0.0f });
    HUD       hud    = HUD(SCREEN_WIDTH, SCREEN_HEIGHT, DEADZONE);

    Vector2      mouse_delta = { 0, 0 };
    static float timer       = 0.0f;
    bool         health_up   = false;

    while (!WindowShouldClose())
    {
        update_input(ship, &mouse_delta);

        float frame_time = GetFrameTime();

        ship.update(frame_time);

        camera.follow(ship, frame_time);
        hud.update(ship);

        BeginDrawing();
        ClearBackground(BACKGROUND);

        camera.draw_begin();
        DrawGrid(1000, 1.0f);
        ship.draw();

        camera.draw_end();

        hud.draw(mouse_delta);
        // draw a line to the npc ship
        DrawFPS(10, 10);
        DrawText(TextFormat("Rotation: %f %f %f %f",
                            ship.rotation.x,
                            ship.rotation.y,
                            ship.rotation.z,
                            ship.rotation.w),
                 10,
                 40,
                 20,
                 FOREGROUND);
        DrawText(TextFormat("Position: %f %f %f",
                            ship.position.x,
                            ship.position.y,
                            ship.position.z),
                 10,
                 60,
                 20,
                 FOREGROUND);

        DrawText(TextFormat("Velocity: %f %f %f",
                            ship.velocity.x,
                            ship.velocity.y,
                            ship.velocity.z),
                 10,
                 80,
                 20,
                 FOREGROUND);
        DrawText(TextFormat("Input: %f %f %f %f %f %f",
                            ship.input_delta.forward,
                            ship.input_delta.left,
                            ship.input_delta.up,
                            ship.input_delta.pitch_down,
                            ship.input_delta.roll_right,
                            ship.input_delta.yaw_left),
                 10,
                 100,
                 20,
                 FOREGROUND);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

static Vector2 update_aim (Ship & p_ship, Vector2 * p_out_mouse_delta)
{
    Vector2 mouse_delta   = GetMousePosition();
    Vector2 screen_center = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    mouse_delta           = Vector2Subtract(mouse_delta, screen_center);

    if (Vector2Length(mouse_delta) < DEADZONE)
    {
        *p_out_mouse_delta = screen_center;
        return Vector2 { 0, 0 };
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

    Vector2 aim    = Vector2Divide(mouse_delta, Vector2 { radius, radius });
    aim.x          = Clamp(aim.x, -1.0f, 1.0f);
    aim.y          = Clamp(aim.y, -1.0f, 1.0f);
    float exponent = 2.0f;
    aim.x          = copysign(pow(fabs(aim.x), exponent), aim.x);
    aim.y          = copysign(pow(fabs(aim.y), exponent), aim.y);
    return aim;
}

static void update_input (Ship & p_ship, Vector2 * p_out_mouse_delta)
{
    p_ship.input_delta.reset();

    if (IsKeyDown(KEY_R))
    {
        SetMousePosition(SCREEN_CENTER.x, SCREEN_CENTER.y);
        p_ship.reset();
    }

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        p_ship.is_boosted = true;
    }
    else
    {
        p_ship.is_boosted = false;
    }

    // normal flight controls
    if (IsKeyDown(KEY_W))
    {
        ResumeMusicStream(p_ship.engine_audio);
        p_ship.input_delta.forward += 1.0f;
    }
    else if (IsKeyDown(KEY_S))
    {
        ResumeMusicStream(p_ship.engine_audio);
        p_ship.input_delta.forward += -1.0f;
    }
    else
    {
        PauseMusicStream(p_ship.engine_audio);
    }
    if (IsKeyDown(KEY_A))
    {
        p_ship.input_delta.left += 1.0f;
    }
    if (IsKeyDown(KEY_D))
    {
        p_ship.input_delta.left += -1.0f;
    }

    if (IsKeyDown(KEY_Q))
    {
        p_ship.input_delta.roll_right = -0.5f;
    }
    if (IsKeyDown(KEY_E))
    {
        p_ship.input_delta.roll_right = 0.5f;
    }

    if (IsKeyDown(KEY_SPACE))
    {
        p_ship.input_delta.up += 1.0f;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL))
    {
        p_ship.input_delta.up += -1.0f;
    }

    Vector2 aim = update_aim(p_ship, p_out_mouse_delta);

    p_ship.input_delta.yaw_left   = -aim.x;
    p_ship.input_delta.pitch_down = aim.y;
}
#include "ship.h"

Ship::Ship (Vector3      initial_position,
            const char * p_model_path,
            const char * p_texture_path,
            const char * p_audio_path,
            movement_t * p_movement_stats)
{
    model                = LoadModel(p_model_path);
    texture              = LoadTexture(p_texture_path);
    engine_audio         = LoadMusicStream(p_audio_path);
    engine_audio.looping = true;
    PlayMusicStream(engine_audio);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    position = initial_position;
    velocity = Vector3Zero();
    rotation = QuaternionIdentity();

    movement_stat = *p_movement_stats;

    is_boosted = false;
}

Ship::~Ship ()
{
    UnloadMusicStream(engine_audio);
    UnloadTexture(texture);
    UnloadModel(model);
}

void Ship::draw ()
{
    DrawModel(model, Vector3Zero(), 1.0f, WHITE);
    // DrawLine3D(
    //     position, Vector3Add(position, Vector3Scale(get_forward(), 2.0f)),
    //     RED);
    // DrawLine3D(
    //     position, Vector3Add(position, Vector3Scale(get_left(), 2.0f)),
    //     GREEN);
    // DrawLine3D(
    //     position, Vector3Add(position, Vector3Scale(get_up(), 2.0f)), BLUE);
}

void Ship::reset ()
{
    position     = Vector3Zero();
    velocity     = Vector3Zero();
    rotation     = QuaternionIdentity();
    input_delta  = Delta();
    smooth_delta = Delta();
    visual_bank  = 0.0f;
    health       = 100.0f;
    energy       = 100.0f;
    is_boosted   = false;
}

void Ship::update (float delta_time)
{
    UpdateMusicStream(engine_audio);

    float boost = 1.0f;

    if (is_boosted && energy > 0.0f)
    {
        boost = 3.0f;
        energy -= 10.0f * delta_time;
        energy               = Clamp(energy, 0.0f, 100.0f);
        boost_recharge_timer = 0.0f;
    }
    else
    {
        boost_recharge_timer += delta_time;

        if (boost_recharge_timer >= 3.0f)
        {
            energy += 20.0f * delta_time;
            energy = Clamp(energy, 0.0f, 100.0f);
        }
    }

    smooth_delta.forward = float_damp(smooth_delta.forward,
                                      input_delta.forward * boost,
                                      movement_stat.throttle_response,
                                      delta_time);

    smooth_delta.left = float_damp(smooth_delta.left,
                                   input_delta.left,
                                   movement_stat.throttle_response,
                                   delta_time);

    smooth_delta.up = float_damp(smooth_delta.up,
                                 input_delta.up,
                                 movement_stat.throttle_response,
                                 delta_time);

    float speed_multiplier = smooth_delta.forward > 0.0f ? 1.0f : 0.5f;

    Vector3 velocity = Vector3Zero();
    velocity
        = Vector3Add(velocity,
                     Vector3Scale(get_forward(),
                                  movement_stat.max_speed * smooth_delta.forward
                                      * speed_multiplier));
    velocity = Vector3Add(
        velocity,
        Vector3Scale(get_up(),
                     movement_stat.max_speed * 0.5f * smooth_delta.up));
    velocity = Vector3Add(
        velocity,
        Vector3Scale(get_left(),
                     movement_stat.max_speed * 0.5f * smooth_delta.left));

    velocity = vector3_damp(velocity, velocity, 2.5f, delta_time);
    position = Vector3Add(position, Vector3Scale(velocity, delta_time));

    smooth_delta.roll_right = float_damp(smooth_delta.roll_right,
                                         input_delta.roll_right,
                                         movement_stat.turn_response,
                                         delta_time);

    smooth_delta.yaw_left = float_damp(smooth_delta.yaw_left,
                                       input_delta.yaw_left,
                                       movement_stat.turn_response,
                                       delta_time);

    smooth_delta.pitch_down = float_damp(smooth_delta.pitch_down,
                                         input_delta.pitch_down,
                                         movement_stat.turn_response,
                                         delta_time);

    rotate_local_euler(Vector3 { 0, 0, 1 },
                       smooth_delta.roll_right * delta_time
                           * movement_stat.turn_rate);
    rotate_local_euler(Vector3 { 0, 1, 0 },
                       smooth_delta.yaw_left * delta_time
                           * movement_stat.turn_rate);
    rotate_local_euler(Vector3 { 1, 0, 0 },
                       smooth_delta.pitch_down * delta_time
                           * movement_stat.turn_rate);

    rotate_local_euler(Vector3 { 0, 0, -1 },
                       smooth_delta.yaw_left * delta_time * 0.5f
                           * movement_stat.turn_rate);

    float new_visual_bank = (-30 * DEG2RAD * smooth_delta.yaw_left)
                            + (-15 * DEG2RAD * smooth_delta.left);
    visual_bank = float_damp(visual_bank, new_visual_bank, 10.0f, delta_time);
    Quaternion visual_rotation
        = QuaternionFromAxisAngle(Vector3 { 0, 0, 1 }, visual_bank);

    Matrix transform = MatrixTranslate(position.x, position.y, position.z);
    transform = MatrixMultiply(QuaternionToMatrix(visual_rotation), transform);
    transform = MatrixMultiply(QuaternionToMatrix(rotation), transform);
    model.transform = transform;
}
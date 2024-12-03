#include "ui.h"

bool draw_axis (Model * p_model, const Vector3 * p_position)
{
    if ((NULL == p_model) || (NULL == p_position))
    {
        return false;
    }

    Matrix transforms[] = {
        MatrixMultiply(MatrixRotateZ(PI / 2.0f),
                       MatrixTranslate(0.0f, 0.0f, 0.0f)), // X-axis
        MatrixTranslate(0.0f, 0.0f, 0.0f),                 // Y-axis
        MatrixMultiply(MatrixRotateX(PI / 2.0f),
                       MatrixTranslate(0.0f, 0.0f, 0.0f)) // Z-axis
    };

    Color colors[] = { RED, GREEN, BLUE };

    for (int i = 0; i < sizeof(transforms) / sizeof(Matrix); i++)
    {
        p_model->transform = transforms[i];
        DrawModel(*p_model, *p_position, 1.0f, colors[i]);
    }

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
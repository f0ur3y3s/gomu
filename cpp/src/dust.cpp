#include "dust.hpp"

#include <cmath>
#include <array>
#include "raymath.h"
#include "rlgl.h"

inline float GetPrettyBadRandomFloat (float min, float max)
{
    auto value
        = static_cast<float>(GetRandomValue((int)min * 1000, (int)max * 1000));
    value /= 1000.f;
    return value;
}

Dust::Dust (float size, int count)
{
    points = std::vector<Vector3>();
    points.reserve(count);
    extent = size * .5f;

    for (int i = 0; i < count; ++i)
    {
        auto point = Vector3 { GetPrettyBadRandomFloat(-extent, extent),
                               GetPrettyBadRandomFloat(-extent, extent),
                               GetPrettyBadRandomFloat(-extent, extent) };
        points.push_back(point);

        auto color = Color { (unsigned char)GetRandomValue(192, 255),
                             (unsigned char)GetRandomValue(192, 255),
                             (unsigned char)GetRandomValue(192, 255),
                             255 };
        colors.push_back(color);
    }
}

Dust::~Dust ()
{
    points.clear();
}

void Dust::update_view (Vector3 viewPosition)
{
    float size = extent * 2;
    for (auto & p : points)
    {
        while (p.x > viewPosition.x + extent)
            p.x -= size;
        while (p.x < viewPosition.x - extent)
            p.x += size;

        while (p.y > viewPosition.y + extent)
            p.y -= size;
        while (p.y < viewPosition.y - extent)
            p.y += size;

        while (p.z > viewPosition.z + extent)
            p.z -= size;
        while (p.z < viewPosition.z - extent)
            p.z += size;
    }
}

void Dust::draw (Vector3 viewPosition, Vector3 velocity) const
{
    BeginBlendMode(BlendMode::BLEND_ADDITIVE);

    for (int i = 0; i < points.size(); ++i)
    {
        float distance = Vector3Distance(viewPosition, points[i]);

        float farLerp = Clamp(Normalize(distance, extent * .9f, extent), 0, 1);
        unsigned char farAlpha = (unsigned char)Lerp(255, 0, farLerp);

        const float cubeSize = 0.01f;

        DrawLine3D(Vector3Add(points[i], Vector3Scale(velocity, 0.02f)),
                   points[i],
                   { colors[i].r, colors[i].g, colors[i].b, farAlpha });
    }

    rlDrawRenderBatchActive();
    EndBlendMode();
}
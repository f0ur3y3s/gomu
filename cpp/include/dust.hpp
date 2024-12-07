#pragma once

#include <raylib.h>
#include <vector>

class Dust
{
public:
    Dust (float size, int count);
    ~Dust ();

    void update_view (Vector3 view_position);
    void draw (Vector3 view_position, Vector3 velocity) const;

private:
    std::vector<Vector3> points;
    std::vector<Color>   colors;
    float                extent;
};
#ifndef UJU_H
#define UJU_H

#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"

#include "ship.h"
#include "uju_camera.h"
#include "hud.h"

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define SCREEN_CENTER (Vector2 { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f })
#define DEADZONE      30.0f
#define BACKGROUND    Color { 14, 19, 45, 200 }

#define SHIP_MODEL   ASSETS_PATH "lowpoly/ship.gltf"
#define SHIP_TEXTURE ASSETS_PATH "lowpoly/a16.png"

#endif
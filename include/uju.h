#ifndef UJU_H
#define UJU_H

#include "raylib.h"
#include "raymath.h"
#include "ship.h"
#include "camera.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef DDEBUG
#include <stdio.h>
#endif

#define SCREEN_WIDTH  1920
#define SCREEN_HEIGHT 1080
#define SCREEN_CENTER ((Vector2) { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f })
#define DEADZONE      20.0f

#endif
#ifndef DRAWING_HELPER_H
#define DRAWING_HELPER_H

#include <stdbool.h>
#include "game_state.h"

void drawArenaGrid();
void drawRobot(const Robot *robot);
void drawObstacle(Coord coord);
void drawMarker(Coord coord, bool erase);

#endif
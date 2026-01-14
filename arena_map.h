#ifndef ARENA_MAP_H
#define ARENA_MAP_H

#include "game_state.h"

Coord generateArena();
TileContent getTileContent(Coord coord);
void placeMarker(Coord coord);
void removeMarker(Coord coord);

#endif
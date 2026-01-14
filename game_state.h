#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

#define WINDOW_WIDTH      1000
#define WINDOW_HEIGHT     600
#define ARENA_SIZE        15
#define GRID_SIZE         30
#define BORDER_THICKNESS  6
#define OBSTACLE_PADDING  1
#define MARKER_PADDING    6
#define SLEEP_TIME        10

#define MARKER_NO 3
#define OBSTACLE_NO 4

typedef struct {
    int x;
    int y;
} Coord;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef enum {
    UNKNOWN,
    EMPTY,
    OBSTACLE,
    MARKER
} TileContent;

typedef struct {
    TileContent content;
    int marker_count;
    bool visited;
} Tile;

typedef struct {
    Coord pos;
    Direction dir;
    int marker_count;
    Tile arena_memory[15][15];
    int known_arena_size;
} Robot;

void sleep(int miliseconds);

#endif
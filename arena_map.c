#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "drawing_helper.h"

static Tile **arena = NULL;

Coord generateRandomEmptyCoord() {

    Coord coord;
    do {
        coord.x = rand() % ARENA_SIZE;
        coord.y = rand() % ARENA_SIZE;
    } while (arena[coord.y][coord.x].content != EMPTY);
    return coord;
}

void placeObstacle(Coord coord) {

    arena[coord.y][coord.x].content = OBSTACLE;
    drawObstacle(coord);
}

void placeMarker(Coord coord) {
    
    arena[coord.y][coord.x].content = MARKER;
    arena[coord.y][coord.x].marker_count++;
    drawMarker(coord, false);
}

void removeMarker(Coord coord) {
    if (--arena[coord.y][coord.x].marker_count == 0) {
        drawMarker(coord, true);
        arena[coord.y][coord.x].content = EMPTY;
    }
}

void generateDiamond() {

    int center_index = (ARENA_SIZE - 1) / 2;
    int radius = center_index + 1;
    
    Coord coord;

    for (int y = 0; y < ARENA_SIZE; y++) {
        for (int x = 0; x < ARENA_SIZE; x++) {
            int dist_x = abs(x - center_index);
            int dist_y = abs(y - center_index);
            if ((dist_x + dist_y) > radius) {
                coord.x = x;
                coord.y = y;
                placeObstacle(coord);
            }
        }
    }
}

Coord generateArena() {

    drawArenaGrid();

    arena = malloc(ARENA_SIZE * sizeof(Tile *));
    for (int i = 0; i < ARENA_SIZE; i++) {
        arena[i] = malloc(ARENA_SIZE * sizeof(Tile));
        for (int j = 0; j < ARENA_SIZE; j++) {
            arena[i][j].content = EMPTY;
            arena[i][j].marker_count = 0;
            arena[i][j].visited = false;
        }
    }

    generateDiamond();

    srand(time(NULL));
    for (int i = 0; i < OBSTACLE_NO; i++) {
        placeObstacle(generateRandomEmptyCoord());
    }
    for (int i = 0; i < MARKER_NO; i++) {
        placeMarker(generateRandomEmptyCoord());
    }

    return generateRandomEmptyCoord();
}

TileContent getTileContent(Coord coord) {
    return arena[coord.y][coord.x].content;
}
#include <time.h>
#include "robot_logic.h"
#include "queue.h"
#include "drawing_helper.h"
#include "arena_map.h"

static const int X_DELTA[4] = {0, 1, 0, -1};
static const int Y_DELTA[4] = {-1, 0, 1, 0};

Coord getTileAheadCoords(Robot *robot) {
    int x = robot->pos.x + X_DELTA[(int)robot->dir];
    int y = robot->pos.y + Y_DELTA[(int)robot->dir];
    Coord coord = {x, y};
    return coord;
}

bool canMoveForward(Robot *robot) {

    Coord target = getTileAheadCoords(robot);

    if (target.x < 0 || target.x >= ARENA_SIZE ||
        target.y < 0 || target.y >= ARENA_SIZE ||
        getTileContent(target) == OBSTACLE) {
            return false;
    }
    return true;
}

bool atMarker(Robot *robot) {
    return getTileContent(robot->pos) == MARKER;
}

void pickUpMarker(Robot *robot) {
    removeMarker(robot->pos);
    robot->marker_count++;
    sleep(SLEEP_TIME);
}

void dropMarker(Robot *robot) {
    placeMarker(robot->pos);
    robot->marker_count--;
    sleep(SLEEP_TIME);
}

void dropAllMarkers(Robot *robot) {
    while (robot->marker_count--) {
        dropMarker(robot);
    }
}

void forward(Robot *robot) {

    if (!canMoveForward(robot)) return;

    robot->pos = getTileAheadCoords(robot);
    robot->arena_memory[robot->pos.y][robot->pos.x].visited = true;
    drawRobot(robot);
    sleep(SLEEP_TIME);
}

void left(Robot *robot) {

    robot->dir = (robot->dir - 1 + 4) % 4;
    drawRobot(robot);
    sleep(SLEEP_TIME);
}

void right(Robot *robot) {

    robot->dir = (robot->dir + 1) % 4;
    drawRobot(robot);
    sleep(SLEEP_TIME);
}

void scanTileAhead(Robot *robot) {

    Coord coord = getTileAheadCoords(robot);

    if (coord.x < 0 || coord.x >= robot->known_arena_size ||
        coord.y < 0 || coord.y >= robot->known_arena_size) {
        return;
    }

    robot->arena_memory[coord.y][coord.x].content =
        (canMoveForward(robot)) ? EMPTY : OBSTACLE;
}

void scanSurroundings(Robot *robot) {

    scanTileAhead(robot);
    right(robot);
    scanTileAhead(robot);
    right(robot);
    right(robot);
    scanTileAhead(robot);
    right(robot);
}

bool isKnownAndUnvisited(Robot *robot, Coord pos) {
    return pos.x >= 0 && pos.x < robot->known_arena_size &&
           pos.y >= 0 && pos.y < robot->known_arena_size &&
           robot->arena_memory[pos.y][pos.x].content == EMPTY &&
           !robot->arena_memory[pos.y][pos.x].visited;
}

Coord findClosestUnvisited(Robot *robot) {

    Coord current;
    for (int R = 1; R < robot->known_arena_size; R++) { 
        for (int dx = -R; dx <= R; dx++) {
            int dy = R - abs(dx);
            current.x = robot->pos.x + dx;
            current.y = robot->pos.y + dy;
            if (isKnownAndUnvisited(robot, current)) {
                return current;
            }
            current.y = robot->pos.y - dy;
            if (dy != 0 && isKnownAndUnvisited(robot, current)) { 
                return current;
            }
        }
    }
    current.x = -1;
    current.y = -1;
    return current;
}

static int getIndexFromCoord(Coord pos, int arena_size) {
    return pos.y * arena_size + pos.x;
}

static bool isValidAndPassable(const Robot *robot, Coord pos, const bool *visited) {
    int index;

    if (pos.x < 0 || pos.x >= robot->known_arena_size ||
        pos.y < 0 || pos.y >= robot->known_arena_size) {
        return false;
    }

    index = getIndexFromCoord(pos, robot->known_arena_size);

    if (visited[index]) {
        return false;
    }

    if (robot->arena_memory[pos.y][pos.x].content == OBSTACLE ||
        robot->arena_memory[pos.y][pos.x].content == UNKNOWN) {
        return false;
    }

    return true;
}

static Direction get_direction_to_neighbor(Coord start, Coord end) {
    if (end.x > start.x) return EAST;
    if (end.x < start.x) return WEST;
    if (end.y > start.y) return SOUTH;
    if (end.y < start.y) return NORTH;
    return NORTH;
}

static Direction* reconstructFullPath(Coord start, Coord target, 
                                        const Coord *parent_map, int size, int *path_length) {
    
    int length = 0;
    Coord current = target;
    
    while (current.x != start.x || current.y != start.y) {
        int index = getIndexFromCoord(current, size);
        current = parent_map[index];
        if (current.x == -1) return NULL;
        length++;
    }
    
    if (length == 0) {
        *path_length = 0;
        return NULL;
    }

    Direction *path = (Direction *)malloc(length * sizeof(Direction));
    if (!path) return NULL;
    
    *path_length = length;
    current = target;

    for (int i = length - 1; i >= 0; i--) {
        int index = getIndexFromCoord(current, size);
        Coord parent = parent_map[index];
        path[i] = get_direction_to_neighbor(parent, current);
        current = parent;
    }
    
    return path;
}

Direction* findFullPath(const Robot *robot, Coord target_coord, int *path_length) {

    int size = robot->known_arena_size;
    int flat_size = size * size;
    Coord start = robot->pos;

    Coord *parent_map = (Coord *)calloc(flat_size, sizeof(Coord));
    bool *search_visited = (bool *)calloc(flat_size, sizeof(bool));
    Queue q;

    init_queue(&q);
    enqueue(&q, start);
    search_visited[getIndexFromCoord(start, size)] = true;

    Direction *final_path = NULL;

    while (!is_empty(&q)) {
        Coord current = dequeue(&q);

        if (current.x == target_coord.x && current.y == target_coord.y) {
            final_path = reconstructFullPath(start, target_coord, parent_map, size, path_length);
            goto cleanup;
        }

        for (int i = 0; i < 4; i++) {
             Coord neighbor = {current.x + X_DELTA[i], current.y + Y_DELTA[i]};
             int neighbor_index = getIndexFromCoord(neighbor, size);

             if (isValidAndPassable(robot, neighbor, search_visited)) {
                 search_visited[neighbor_index] = true;
                 parent_map[neighbor_index] = current;
                 enqueue(&q, neighbor);
             }
         }
    }

cleanup:
    free_queue(&q);
    free(parent_map);
    free(search_visited);
    
    if (final_path == NULL) *path_length = 0;
    return final_path;
}

void moveToTargetCoord(Robot *robot, Coord target_coord) {

    if (target_coord.x == -1 || target_coord.y == -1) return;

    int path_length = 0;
        
        Direction *path = findFullPath(robot, target_coord, &path_length);

        if (path == NULL || path_length == 0) {
            free(path);
            return; 
        }

        for (int i = 0; i < path_length; i++) {
            Direction next_dir = path[i];
            while (robot->dir != next_dir) {
                right(robot);
            }
            forward(robot);
        }
        
        free(path);
}

void collectAllMarkers(Robot *robot) {

    Coord target;
    robot->arena_memory[robot->pos.y][robot->pos.x].visited = true;
    scanTileAhead(robot);
    right(robot);
    right(robot);
    do {
        if (atMarker(robot)) {
            pickUpMarker(robot);
        }
        scanSurroundings(robot);
        target = findClosestUnvisited(robot);
        moveToTargetCoord(robot, target);
    } while (target.x != -1 && target.y != -1);

}

void deployRobot(Robot *robot) {

    Coord start_coord = robot->pos;
    collectAllMarkers(robot);
    moveToTargetCoord(robot, start_coord);
    dropAllMarkers(robot);
}
#include "graphics.h"
#include "drawing_helper.h"

static Coord arena_offset = {0, 0};
static const int ROBOT_CIRCUMRADIUS = 10;

void initArenaOffset() {

    arena_offset.x = (WINDOW_WIDTH - ARENA_SIZE * GRID_SIZE) / 2;
    arena_offset.y = (WINDOW_HEIGHT - ARENA_SIZE * GRID_SIZE) / 2;
}

void drawArenaGrid() {

    initArenaOffset();
    int column_length = ARENA_SIZE * GRID_SIZE;
    int row_length = ARENA_SIZE * GRID_SIZE;
    background();

    setColour(red);
    fillRect(arena_offset.x - BORDER_THICKNESS, arena_offset.y - BORDER_THICKNESS, 
            row_length + 2 * BORDER_THICKNESS + 1, column_length + 2 * BORDER_THICKNESS + 1);
    setColour(white);
    fillRect(arena_offset.x, arena_offset.y, row_length, column_length);

    setColour(black);
    for (int column_no = 0; column_no <= ARENA_SIZE; column_no++) {
        int line_x = arena_offset.x + column_no * GRID_SIZE;
        drawLine(line_x, arena_offset.y, line_x, arena_offset.y + column_length);
    }
    for (int row_no = 0; row_no <= ARENA_SIZE; row_no++) {
        int line_y = arena_offset.y + row_no * GRID_SIZE;
        drawLine(arena_offset.x, line_y, arena_offset.x + row_length, line_y);
    }
}

void drawRobot(const Robot *robot) {

    Coord robot_offset = {robot->pos.x * GRID_SIZE + arena_offset.x + GRID_SIZE / 2, 
                          robot->pos.y * GRID_SIZE + arena_offset.y + GRID_SIZE / 2};
    int xs[3];
    int ys[3];
    
    switch (robot->dir) {
        case NORTH:
            xs[0] = robot_offset.x;
            xs[1] = robot_offset.x - ROBOT_CIRCUMRADIUS;
            xs[2] = robot_offset.x + ROBOT_CIRCUMRADIUS;

            ys[0] = robot_offset.y - ROBOT_CIRCUMRADIUS;
            ys[1] = robot_offset.y + ROBOT_CIRCUMRADIUS;
            ys[2] = robot_offset.y + ROBOT_CIRCUMRADIUS;
            break;
        case EAST:
            xs[0] = robot_offset.x + ROBOT_CIRCUMRADIUS;
            xs[1] = robot_offset.x - ROBOT_CIRCUMRADIUS;
            xs[2] = robot_offset.x - ROBOT_CIRCUMRADIUS;

            ys[0] = robot_offset.y;
            ys[1] = robot_offset.y + ROBOT_CIRCUMRADIUS;
            ys[2] = robot_offset.y - ROBOT_CIRCUMRADIUS;
            break;
        case SOUTH:
            xs[0] = robot_offset.x;
            xs[1] = robot_offset.x - ROBOT_CIRCUMRADIUS;
            xs[2] = robot_offset.x + ROBOT_CIRCUMRADIUS;

            ys[0] = robot_offset.y + ROBOT_CIRCUMRADIUS;
            ys[1] = robot_offset.y - ROBOT_CIRCUMRADIUS;
            ys[2] = robot_offset.y - ROBOT_CIRCUMRADIUS;
            break;
        case WEST:
            xs[0] = robot_offset.x - ROBOT_CIRCUMRADIUS;
            xs[1] = robot_offset.x + ROBOT_CIRCUMRADIUS;
            xs[2] = robot_offset.x + ROBOT_CIRCUMRADIUS;

            ys[0] = robot_offset.y;
            ys[1] = robot_offset.y + ROBOT_CIRCUMRADIUS;
            ys[2] = robot_offset.y - ROBOT_CIRCUMRADIUS;
            break;
    }

    foreground();
    clear(); 
    setColour(blue);
    fillPolygon(3, xs, ys);
}

void drawObstacle(Coord coord) {

    background();
    setColour(black);
    int x = arena_offset.x + coord.x * GRID_SIZE + 1 + OBSTACLE_PADDING;
    int y = arena_offset.y + coord.y * GRID_SIZE + 1 + OBSTACLE_PADDING;
    int side_length = GRID_SIZE - 1 - 2  * OBSTACLE_PADDING;
    fillRect(x, y, side_length, side_length);
}

void drawMarker(Coord coord, bool erase) {

    background();
    setColour(erase ? white : gray);
    int x = arena_offset.x + coord.x * GRID_SIZE + 1 + MARKER_PADDING;
    int y = arena_offset.y + coord.y * GRID_SIZE + 1 + MARKER_PADDING;
    int diameter = GRID_SIZE - 1 - 2 * MARKER_PADDING;
    fillOval(x, y, diameter, diameter);
}
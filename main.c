#include <stdlib.h>
#include <time.h>
#include "graphics.h"
#include "game_state.h"
#include "arena_map.h"
#include "drawing_helper.h"
#include "robot_logic.h"

static Robot robot;

int main(void) {

    setWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    Coord robot_start_coord = generateArena();
    srand(time(NULL));
    robot = (Robot){
        .pos = robot_start_coord,
        .dir = rand() % 4, 
        .marker_count = 0, 
        .known_arena_size = 15
    };
    deployRobot(&robot);

    return 0;
}
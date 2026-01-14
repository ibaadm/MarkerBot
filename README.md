# MarkerBot

## Program Description

This program simulates an intelligent robot tasked with finding all the markers in an arena riddled with obstacles.

It works by scanning the tiles around it, adding what it sees to its internal memory map. It then moves to the closest unvisited tile in its memory, picking up markers along the way. The robot stops after all reachable tiles have been visited. The robot then moves back to its start position and drops off all the markers it has collected.

## AI Usage Citation

Parts of the `moveToTargetCoord` and `findClosestUnvisited` functions, as well as the entire `queue.c`/`queue.h` implementation, were developed with assistance from an AI (Google Gemini Pro) to implement the Breadth-First Search (BFS) pathfinding algorithm.

## Compile and Run Commands

**To compile the program, run the following line:**
```bash
gcc -o robot_sim main.c graphics.c robot_logic.c drawing_helper.c arena_map.c queue.c
```

**To run the program, run the following line:**
```bash
./robot_sim | java -jar drawapp-4.5.jar
```

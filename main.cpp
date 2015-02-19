#include <iostream>
#include <fstream>
#include "Maze.h"
#include "Solver.h"


int main (int argc, char * const argv[]) {
    // This constructor will generate a random maze of
	// the given dimensions.
	// Optional 3rd argument: pause milliseconds
	//    how many milliseconds to pause when drawing.  allows
	//    you time to trace through the agent's actions. (1000 = 1 second)
	// Optional 4th argument: seed (default=-1, means use time)
	//    if not -1, passed to srand to seed the random
	//    number generator so that Maze will be the same
	//    each time.
	// Optional 5th argument: verbose (default=true)
	//    if true, prints current pose and actions
	//    as they are executed/undone
    Maze maze(50,21);

	// uncomment to leave trail of maze drawings (no erase)
	// on mac erase doesn't work anyway.
	// maze.setEraseBeforeDraw(false);

	// If you'd prefer to read the maze in from a file,
	// uncomment these lines instead.
    // Maze maze;
    // cin >> maze;
    
    solveMaze(maze, 2*maze.numCells());

#if defined (_WIN32)
    // Uncomment this line if you're still using DevC++
    //system("PAUSE");
#endif
    return 0;
}

/*
 *  Maze.h
 *  Maze
 *
 *  Created by sjf on 2/18/10.
 *
	The Maze is a two-dimensional grid of locations, each of which is either
	free or blocked and a starting and goal location, and a current position.  
	The current position is changed by doing and undoing actions.

	Each cell can also be marked and unmarked, which effects nothing except 
	the way the entire grid is printed, allowing for paths to be displayed for 
	debugging.
 */
#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#if defined (_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

/* Move values */
const int INVALID_MOVE = -1;
const int FORWARD = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int NUM_MOVES = 3;

enum Direction {NORTH, EAST, SOUTH, WEST};

struct Position {
    int x;
    int y;
};

struct Pose {
    Position cell;
    int orientation;
};

class Maze {
public:
    // makes an empty maze
    Maze();
    // make an Maze with given dimensions, 
	// filling in the walls (somewhat) randomly
    Maze(int xDim, int yDim, int pauseMilliseconds = 1, int randomSeed = -1, bool verbose = true);
    Maze(const Maze & other);
    ~Maze();
    Maze & operator=(const Maze & other);

	// moveBlocked: Returns true if a move is not 
	// possible from the current location (and false 
	// otherwise)
    bool moveBlocked(int move);

	// doMove: executes the move from the current location
	// if it is not blocked.  If the move is blocked, the
	// current location is unchanged.
	void doMove(int move);
	
	// undoMove: does the inverse of the move from the 
	// current location if the target location is not 
	// blocked.  If the target location is blocked, the
	// current location is unchanged.
    void undoMove(int move);

/* the rest of the functions are not functions you should need to call */

	// Set the amount of time to pause after redrawing
	// (to help with debugging)
	void setRedrawPause(double milliseconds) {pauseMilliseconds = milliseconds;};
	// Set whether or not to erase the maze before redrawing.
	// On Windows, default is to erase
	// On Mac, erasing is complicated, so it can't be done currently
	void setEraseBeforeDraw(bool eraseBeforeDraw) {this->eraseBeforeDraw = eraseBeforeDraw;};
	// atGoal: Returns true if the current pose is 
	// at the goal location, and false otherwise
    bool atGoal();
	// returns the number of potentially open cells
	// (for figuring out how long to search)
	int numCells() {return (xDim-1)*(yDim-1);};
	// undoMoveBlocked: Returns true if undoing a move
	// is blocked and false otherwise.  That is, if
	// undoing a move would cause the agent to collide
	// with the wall.
	bool undoMoveBlocked(int move);
    // some functions to help with debugging
    void markCurrentLocation();  // will print a . in a location
    void unmarkCurrentLocation();  // removes the . from the location
	// function for printing an action as an intelligable string
    string moveToStr(int move);
	// function for printing the current location
	string currentPoseToStr();
    // input/output
    void display(ostream & out);
    // reads the stream from a file stream (not from the user)
    void read(ifstream & in);
private:
	// internal functions for memory (de)allocation
    void allocateCells();
    void deallocateCells();
	// For random maze generation.  Clears cells along a
	// random path, so long as loops are not generated.
	Pose clearRandomWalk(int numSteps, const Pose & startPose);
	// return a randomly chosen cell suitable for starting a random walk
	Pose randomStartPose();
	// Returns new pose after executing move from current pose
    Pose getNewPose(Pose pose, int move, bool invert = false);
	// For going forward/backward
    Pose translate(Pose pose, int dir);
	// For turning
    Pose turn(Pose pose, int move, int dir);
	// Returns true iff pose is blocked
    bool blocked(Pose pose);
	// Returns the number of neighboring cells that are 
	// blocked (does not include diagonal)
    int neighborsBlocked(Pose pose);
	// Returns string for any pose
	string poseToStr(Pose pose);
	// dimensions of grid
    int xDim;
    int yDim;
	// poses
    Pose currPose;
    Pose startPose;
    Pose goalPose;
	// actual grid
	// first bit indicates blocked/clear
	// second bit indicates marked/unmarked (for printing only)
    int ** cells;
	int seed;
	bool eraseBeforeDraw;
	double pauseMilliseconds;
	bool verbose;
};

// input/output operators
ostream & operator<<(ostream & out, Maze & m);
istream & operator>>(istream & in, Maze & m);

// utility function because C++ mod function doesn't work 
// with negative numbers
int fixMod(int n, int mod);

// UGLINESS!
// On windows, puts the cursor back at the top of the console window
// so that maze is drawn on top of old maze (instead of drawing a
// bazillion mazes)
// This is much harder to do on the Mac (and doesn't work with the
// xCode terminal anyway), so on a Mac you're just stuck with 
// drawing a bazillion mazes.
//#if defined (_WIN32)
void putCursor(int x, int y);
//#endif
void initScr();
//void putCursor(int x, int y);
void closeScr();
#endif 


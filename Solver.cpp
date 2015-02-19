#include "Solver.h"
#include <iostream>
#include <cstdlib>
//Solving algorithm written by me, Josh. Maze written by... someone else I can't remember.
void solveMaze(Maze & maze, int timeout) {
	// uncomment to use the stack
	// Stack stack(2048);
	// Feel free to add/remove/rename local variables
    //int move, lastMove = INVALID_MOVE;
	
	/*const int INVALID_MOVE = -1;
const int FORWARD = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int NUM_MOVES = 3;*/

	Stack * moves = new Stack[100];
	int stackNumber = 0, numSteps = 0;
	bool leftFlagDump = false, rightFlagDump = false;

	while (!maze.atGoal()) {

		
		if (!maze.moveBlocked(RIGHT) && /*moves[stackNumber].top()*/moves[stackNumber].length() != moves[stackNumber].rightFlagLoc) { //If RIGHT isn't blocked. // && moves[stackNumber].rightFlag == true
			stackNumber++;
			moves[stackNumber].push(RIGHT);
			maze.doMove(RIGHT);
			if (leftFlagDump = true) {
				for (int i = 0; i < 10; i++) {
					moves[stackNumber + i].leftFlagLoc = -5;
				}
			}
			numSteps++;
		}
		else if (!maze.moveBlocked(LEFT) && /*moves[stackNumber].top()*/moves[stackNumber].length() != moves[stackNumber].leftFlagLoc) { // && moves[stackNumber].leftFlag == true
			stackNumber++;
			moves[stackNumber].push(LEFT);
			if (rightFlagDump = true) {
				for (int i = 0; i < 10; i++) {
					moves[stackNumber + i].rightFlagLoc = -5;
				}
			}
			maze.doMove(LEFT);
			numSteps++;
		}
		else if (!maze.moveBlocked(FORWARD)) {
			moves[stackNumber].push(FORWARD);
			maze.doMove(FORWARD);
			numSteps++;
		}

		else { //If we've hit this, we're blocked in. Can't go forward, left, or right. Undoing all moves in current sta
			if (stackNumber > 0) {
				if (moves[stackNumber].bottom() == LEFT) {
					//moves[stackNumber - 1].leftFlag = false;
					if (moves[stackNumber - 1].top() != -1)
						moves[stackNumber - 1].leftFlagLoc = moves[stackNumber - 1].length(); //moves[stackNumber - 1].top();
						leftFlagDump = true;

				}
				if (moves[stackNumber].bottom() == RIGHT) {
					//moves[stackNumber - 1].rightFlag = false;
					if (moves[stackNumber - 1].top() != -1) {
						moves[stackNumber - 1].rightFlagLoc = moves[stackNumber - 1].length(); //moves[stackNumber - 1].top();
						rightFlagDump = true;
					}
				}
				while (!moves[stackNumber].empty()) {
					maze.undoMove(moves[stackNumber].top());
					moves[stackNumber].pop();


				}

				stackNumber--;
			}
			else {

				while (!moves[stackNumber].empty()) {
					maze.undoMove(moves[stackNumber].top());
					moves[stackNumber].pop();


				}

			}
		}



		// [START] Between here and [END] is where you should
		// concentrate your efforts.  You will probably want
		// to remove or comment out the code that exists now.
		/*i++;
		move = i % 3;
		if (!maze.moveBlocked(move)) { //If the move you're about to do is valid...
		maze.doMove(move); //Do that move. 0 = FORWARD, 1 = LEFT, 2 = RIGHT, 3 = NUM_MOVES, -1 = INVALID_MOVE
		lastMove = move; //You just did that move, so that is now your last move.
		}
		else if (lastMove != INVALID_MOVE) { //You tried to move with that previous if, and you couldn't, so now it looks to see if your last move was valid.
		maze.undoMove(lastMove); //It undoes your last move
		lastMove = INVALID_MOVE; //It now says your last move was invalid.
		}

		if (maze.moveBlocked(move)) */



		// [END]

		// leave this part intact so that maze
		// will be displayed, and function will
		// timeout eventually

		cout << maze;
		numSteps++;
		if (timeout >= 0 && numSteps > timeout)
			break;
	}
	if (maze.atGoal()) {
        cout << maze;
		system("pause");
	    cout << "Found path";
    }
    else
        cout << "Search failed";
    cout << " after " << numSteps << " steps." << endl;
	delete[] moves;
}

#include "Maze.h"
#include <cmath>

Maze::Maze() {
    cells = 0;
    currPose.orientation = EAST;
	pauseMilliseconds = 10;
	verbose = true;
}

Maze::Maze(const Maze & other) {
    xDim = other.xDim;
    yDim = other.yDim;
    allocateCells();
    currPose.orientation = EAST;
	pauseMilliseconds = 10;
	verbose = true;
}

Maze::Maze(int xDim, int yDim, int pauseMilliseconds, int randomSeed, bool verbose) {
    this->xDim = xDim;
    this->yDim = yDim;
	this->eraseBeforeDraw = true;
	this->pauseMilliseconds = pauseMilliseconds;
	this->verbose = verbose;
    allocateCells();
	if (randomSeed >= 0) {
		seed = (unsigned)randomSeed;
		srand(seed);
	}
	else {
		seed = (unsigned)time(0);
	    srand(seed);
	}
    for (int i=0; i<yDim; i++) {
        for (int j=0; j<xDim; j++) {
            cells[i][j] = 1;
        }
    }
    startPose.cell.x = 1;
    startPose.cell.y = 1;
    startPose.orientation = EAST;

	goalPose = clearRandomWalk(2 * xDim * yDim, startPose);
	currPose = startPose;
}

Pose Maze::clearRandomWalk(int numSteps, const Pose & startPose) {
	Pose furthestPose = startPose, newPose;
	double distSq, furthestDistSq = 0.0;
	int move, numNeighborsBlocked, num;
	bool stuck;
	currPose = startPose;
	if (rand()%2 == 0)
		currPose.orientation = SOUTH;
	cells[currPose.cell.y][currPose.cell.x] = 0;
	for (int i=0; i<numSteps; i++) {
		// looks more maze-y if we are more likely to go straight
		num = rand() % 100;
		if (num < 70)
			move = FORWARD;
		else if (num < 85)
			move = LEFT;
		else
			move = RIGHT;
		newPose = getNewPose(currPose, move);
        numNeighborsBlocked = neighborsBlocked(newPose);
		// to clear a blocked cell, it must be in bounds and 
		// have three sides blocked currently (to avoid loops)
		// if it's already clear, we can go there
        if ((newPose.cell.x > 0) && (newPose.cell.y > 0) &&
            (newPose.cell.x < xDim-1) && (newPose.cell.y < yDim-1) &&
			(!blocked(newPose) || (numNeighborsBlocked == 3))) {
            currPose = newPose;
            cells[currPose.cell.y][currPose.cell.x] = 0;
            distSq = pow(startPose.cell.y-currPose.cell.y, 2.0) + pow(startPose.cell.x-currPose.cell.x, 2.0);
			if (distSq > furthestDistSq) {
				furthestDistSq = distSq;
				furthestPose = currPose;
			}
        }
		else {
			stuck = true;
			for (int m=0; m<NUM_MOVES; m++) {
				newPose = getNewPose(currPose, m);
				numNeighborsBlocked = neighborsBlocked(newPose);
				if ((newPose.cell.x > 0) && (newPose.cell.y > 0) &&
					(newPose.cell.x < xDim-1) && (newPose.cell.y < yDim-1) &&
					(!blocked(newPose) || (numNeighborsBlocked == 3)))  {
					stuck = false;
					break;
				}
			}
			if (stuck) {
				currPose = randomStartPose();
			}
		}
	}
	return furthestPose;
}

Pose Maze::randomStartPose() {
	Pose rPose;
	rPose.cell.x = rand() % (xDim-2) + 1;
	rPose.cell.y = rand() % (yDim-2) + 1;
	int numBlockedNeighbors = neighborsBlocked(rPose);
	while (!blocked(rPose) && numBlockedNeighbors < 3) {
		rPose.cell.x = rand() % (xDim-2) + 1;
		rPose.cell.y = rand() % (yDim-2) + 1;
		numBlockedNeighbors = neighborsBlocked(rPose);
	}
	rPose.orientation = rand() % 4;
	return rPose;
}

Maze::~Maze() {
    deallocateCells();
}

void Maze::allocateCells() {
    cells = new int*[yDim];
    for (int i=0; i<yDim; i++)
        cells[i] = new int[xDim];
}

void Maze::deallocateCells() {
	// protect against accidentally
	// double-deallocation
	if (cells) {
	    for (int i=0; i<yDim; i++)
		    delete[] cells[i];
		delete[] cells;
		cells = 0;
	}
}

Maze & Maze::operator=(const Maze & other) {
    if (this != &other) {
        if ((xDim != other.xDim) || (yDim != other.yDim)) {
            deallocateCells();
            xDim = other.xDim;
            yDim = other.yDim;
            allocateCells();
        }
        for (int i=0; i<yDim; i++) {
            for (int j=0; j<xDim; j++) {
                cells[i][j] = other.cells[i][j];
            }
        }
    }
    return *this;
}

void Maze::markCurrentLocation() {
	// set the second bit, without changing the first
    cells[currPose.cell.y][currPose.cell.x] |= 2;
}

void Maze::unmarkCurrentLocation() {
	// unset the second bit, without changing the first
    cells[currPose.cell.y][currPose.cell.x] &= 1;
}

string Maze::moveToStr(int move) {
    if (move == FORWARD)
        return "FORWARD     ";
    else if (move == RIGHT)
        return "RIGHT       ";
    else if (move == LEFT)
        return "LEFT        ";
    return "INVALID_MOVE";
}

string Maze::currentPoseToStr() {
	return poseToStr(currPose);
}

string Maze::poseToStr(Pose pose) {
	ostringstream ostr;
	ostr << "in cell " << pose.cell.x << " " << pose.cell.y;
	ostr << ", facing ";
	if (pose.orientation == NORTH)
		ostr << "NORTH";
	else if (pose.orientation == EAST)
		ostr << "EAST ";
	else if (pose.orientation == SOUTH)
		ostr << "SOUTH";
	else if (pose.orientation == WEST)
		ostr << "WEST ";
	else
		ostr << "INVALID";
	return ostr.str();
}

void Maze::display(ostream & out) {
#if defined (_WIN32)
	if (eraseBeforeDraw)
		putCursor(0,0);
#endif
    for (int i=0; i<yDim; i++) {
        for (int j=0; j<xDim; j++) {
            if (startPose.cell.x == j && startPose.cell.y == i)
                out << 'S';
            else if (goalPose.cell.x == j && goalPose.cell.y == i)
                out << 'G';
			else if (currPose.cell.x == j && currPose.cell.y == i) {
				if (currPose.orientation == NORTH)
					out << '^';
				else if (currPose.orientation == EAST)
					out << '>';
				else if (currPose.orientation == SOUTH)
					out << 'v';
				else if (currPose.orientation == WEST)
					out << '<';

			}
            else if (cells[i][j] & 2)
                out << '.'; 
            else if (cells[i][j] & 1)
                out << 'X';
            else
                out << ' ';
        }
        out << endl;
    }
	out << "random seed: " << seed << endl;
	if (verbose)
		cout << "Current pose: " << currentPoseToStr() << endl;
#if defined (_WIN32)
	Sleep((DWORD)pauseMilliseconds);
#else
    usleep(pauseMilliseconds*1000);
#endif
}

void Maze::read(ifstream & in) {
	deallocateCells();
    string str;
    // first, read in all the lines to find out how big the array needs to be
    getline(in, str, '\n');
    xDim = (int)str.length();
    in.seekg(0, ios::end);
    yDim = in.tellg()/xDim;
    in.seekg(0, ios::beg);
    allocateCells();
    // return the get pointer to the beginning of the file
    getline(in, str);
    for (int i=0; i<yDim; i++) {
        for (int j=0; j<xDim; j++) {
            switch(str[j]) {
                case 'S': 
                {
                    currPose.cell.x = j;
                    currPose.cell.y = i;
                    startPose.cell.x = j;
                    startPose.cell.y = i;
                    cells[i][j] = 0;
                    break;
                }
                case 'G':
                {
                    goalPose.cell.x = j;
                    goalPose.cell.y = i;
                    cells[i][j] = 0;
                    break;
                }
            }
            cells[i][j] = (str[j] == 'X');
        }
        getline(in, str);
    }
}

bool Maze::atGoal() {
    return (currPose.cell.x == goalPose.cell.x && currPose.cell.y == goalPose.cell.y);
}

bool Maze::moveBlocked(int move) {
	// consider a moved blocked if it isn't a valid move
	if (move < FORWARD || move > RIGHT)
		return true;
    Pose newPose = getNewPose(currPose, move);
    return (cells[newPose.cell.y][newPose.cell.x] == 1);
}

bool Maze::undoMoveBlocked(int move) {
	// consider a moved blocked if it isn't a valid move
	if (move < FORWARD || move > RIGHT)
		return true;
	Pose newPose = getNewPose(currPose, move, true);
	return (cells[newPose.cell.y][newPose.cell.x] == 1);
}

bool Maze::blocked(Pose pose) {
    return (cells[pose.cell.y][pose.cell.x] & 1);
}

int Maze::neighborsBlocked(Pose pose) {
    return ((cells[fixMod(pose.cell.y-1,yDim)][pose.cell.x] & 1) +
            (cells[fixMod(pose.cell.y+1,yDim)][pose.cell.x] & 1) +
            (cells[pose.cell.y][fixMod(pose.cell.x+1,xDim)] & 1) +
            (cells[pose.cell.y][fixMod(pose.cell.x-1,xDim)] & 1));
}

void Maze::doMove(int move) {
    Pose newPose = getNewPose(currPose, move);
	if (!blocked(newPose)) {
		if (verbose)
			cout << "Doing move: " << moveToStr(move);
		currPose = newPose;
        markCurrentLocation();
    }
	else if (verbose)
		cout << "*** Trying to do blocked move! ***" << endl;
}

void Maze::undoMove(int move) {
    Pose newPose = getNewPose(currPose, move, true);
	if (!blocked(newPose)) {
		if (verbose)
			cout << "Undoing move: " << moveToStr(move);
        unmarkCurrentLocation();
		currPose = newPose;
    }
	else if (verbose)
		cout << "*** Trying to undo blocked move! ***" << endl;
}

Pose Maze::getNewPose(Pose pose, int move, bool invert) {
    Pose newPose = pose;
    int dir = (invert ? -1 : 1);
    // if we're going backwards, move, then turn
    if (invert)
        newPose = translate(newPose, dir);
    newPose = turn(newPose, move, dir);
    // if we're going foward, turn, then move
    if (!invert)
        newPose = translate(newPose, dir);
    return newPose;
}

Pose Maze::turn(Pose pose, int move, int directionSign) {
    Pose newPose = pose;
    if (move == LEFT)
        newPose.orientation = fixMod(newPose.orientation-directionSign, 4);
    if (move == RIGHT)
        newPose.orientation = fixMod(newPose.orientation+directionSign, 4);
    return newPose;
}

Pose Maze::translate(Pose pose, int directionSign) {
    Pose newPose = pose;
    switch(newPose.orientation) {
        case NORTH:
            newPose.cell.y = fixMod(newPose.cell.y-directionSign, yDim);
            break;
        case EAST:
            newPose.cell.x = fixMod(newPose.cell.x+directionSign, xDim);
            break;
        case SOUTH:
            newPose.cell.y = fixMod(newPose.cell.y+directionSign, yDim);
            break;
        case WEST:
            newPose.cell.x = fixMod(newPose.cell.x-directionSign, xDim);
            break;
    }
    return newPose;
}

/* non-member functions */

ostream & operator<<(ostream & out, Maze & m) {
    m.display(out);
    return out;
}


istream & operator>>(istream & in, Maze & m) {
    string filename;
    cout << "Enter maze filename: ";
    in >> filename;
    ifstream filein(filename.data());
    if (filein.is_open()) {
        m.read(filein);
        return in;
    }
    else {
        cerr << "Couldn't open maze file.  Aborting." << endl;
        exit(1);
    }
}

int fixMod(int n, int mod) {
    return (n >= 0 ? n % mod : mod + (n - (n/mod)*mod));
}


#if defined (_WIN32) 
void putCursor(int x, int y) {
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
	csbiInfo.dwCursorPosition.X=x;
	csbiInfo.dwCursorPosition.Y=y;
	SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}
#endif
#ifndef SRC_BOT_H_
#define SRC_BOT_H_
#include "Move.h"



// This structure is for the graphMap[10][10]
struct node {
	int value;  // The value in the table
	bool visited; // This is for depth first search to marke a node as visited, so that it is only visited once.
};

class Bot
{
	private:
		int ownMap[10][10];
		int mapCopy[10][10];
		vector<Move> possibleMoves;
		bool greedy = false;

		node graphMap[10][10];
	public:
		Bot();
		int* getNextMove(int map[], int nextMove[]);
		bool isLegalMove(int x1, int y1, int x2, int y2);
		bool isLegalH(int x1, int y1, int x2, int y2);
		bool isLegalV(int x1, int y1, int x2, int y2);
		void fillMap(int map[]);
		void findPossibleMoves(int bestMove[]);
		int calculateScore(int x1, int y1, int x2, int y2);
		int calculateType();
		int findSets();
		void findTileTypes(int types[]);
		void findBestMove(int bestMove[]);
		void findBestMove2(int bestMove[]);
		void printMap();
		void printMapCopy();
		bool moveExists(int type);



		void findBestMove3(int bestMove[]);
		int traverseUp(int x, int y);
		int traverseDown(int x, int y);
		int traverseLeft(int x, int y);
		int traverseRight(int x, int y);
		void findBestMove4(int bestMove[]);
		void resetGraphMap();
		void DepthFirstSearch(int value, int x, int y, int& pathLength);
		void swapMap(node& node1, node& node2);
};

Bot::Bot() //initialise Bot class - constructor
{

}

void Bot::fillMap(int map[]){
	for(int i=0; i<100; i++){
		ownMap[i/10][i%10] = map[i];
	}
}

void Bot::findPossibleMoves(int bestMove[]){
	possibleMoves.clear();
	for(int x =0; x<10; x++){
		for(int y=0; y<10; y++){
			if(x!=9 && isLegalMove(x,y,x+1,y)){
				int score = calculateScore(x,y,x+1,y);
				int type = calculateType();
				Move m(x,y,x+1,y,score, type);
				possibleMoves.push_back(m);
			}
			if(y!=9 && isLegalMove(x,y,x,y+1)){
				int score = calculateScore(x,y,x,y+1);
				int type = calculateType();
				Move m(x,y,x,y+1,score,type);
				possibleMoves.push_back(m);
			}
		}
	}
	if(this->greedy){
		findBestMove(bestMove);
	} else {
		findBestMove2(bestMove);
	}

}

int Bot::calculateType(){
	int type = 0;
	for(int x=0; x<10; x++){
		for(int y=0; y<8; y++){
			if(mapCopy[x][y] == mapCopy[x][y+1] && mapCopy[x][y] == mapCopy[x][y+2]){
				type = mapCopy[x][y];
				return type;
			}
		}
	}
	for(int y=0; y<10; y++){
		for(int x=0; x<8; x++){
			if(mapCopy[x][y] == mapCopy[x+1][y] && mapCopy[x][y] == mapCopy[x+2][y]){
				type = mapCopy[x][y];
				return type;
			}
		}
	}

	return type;
}

void Bot::findBestMove(int bestMove[]){
	int max = 0;
	for(int i=0; i<possibleMoves.size(); i++){
		if(possibleMoves[i].getScore()>max){
			possibleMoves[i].getCoordinates(bestMove);
			max = possibleMoves[i].getScore();
		}
	}
	cout<<"best move is: "<<bestMove[0]<<bestMove[1]<<bestMove[2]<<bestMove[3]<<" with a score of "<<max<<endl;
}

void Bot::findTileTypes(int types[]){
	for(int x=0; x<10; x++){
		for(int y=0; y<10; y++){
			int tile = mapCopy[x][y];
			if (tile == 0)
				continue;
			types[tile-1]++;
		}
	}
}



void Bot::findBestMove2(int bestMove[]){
	int types[] = {0,0,0,0};
	findTileTypes(types);

	int max = 0;
	int maxType = 0;

	for(int t=0 ; t<4; t++){
		if(types[t]>max){
			max = types[t];
			maxType = t+1;
		}
	}

	if(moveExists(maxType)){
		for(int i=0; i<possibleMoves.size(); i++){
			if(possibleMoves[i].getType() == maxType){
				possibleMoves[i].getCoordinates(bestMove);
			}
		}
	} else {
		possibleMoves[0].getCoordinates(bestMove);
	}
}



bool Bot::moveExists(int type){
	bool found = false;
	for(int i=0; i<possibleMoves.size(); i++){
		if(possibleMoves[i].getType() == type){
			found = true;
		}
	}
	return found;
}


int Bot::calculateScore(int x1, int y1, int x2, int y2){
	for(int x=0; x<10; x++){
		for(int y=0; y<10; y++){
			mapCopy[x][y] = ownMap[x][y];
		}
	}
	swap(mapCopy[x1][y1], mapCopy[x2][y2]);
	int score = findSets();

	return score;
}


int Bot::findSets(){
	int totalScore = 0;
	for(int x=0; x<10; x++){
		int tempScore = 1;
		for(int y=0; y<10; y++){
			if(y>0 && mapCopy[x][y] == mapCopy[x][y-1] && mapCopy[x][y]!=0){
				tempScore++;

			} else if(y>0 && mapCopy[x][y] != mapCopy[x][y-1] && tempScore>2){
				totalScore += tempScore;
				tempScore = 1;
			} else {
				tempScore = 1;
			}
		}
		if(tempScore>2){
			totalScore += tempScore;
		}
	}

	for(int y=0; y<10; y++){
			int tempScore = 1;
			for(int x=0; x<10; x++){
				if(x>0 && mapCopy[x][y] == mapCopy[x-1][y] && mapCopy[x][y]!=0){
					tempScore++;
				} else if(y>0 && mapCopy[x][y] != mapCopy[x-1][y] && tempScore>2){
					totalScore += tempScore;
					tempScore = 1;
				} else {
					tempScore = 1;
				}
			}
			if(tempScore>2){
				totalScore += tempScore;
			}
		}

	return totalScore;
}

bool Bot::isLegalMove(int x1, int y1, int x2, int y2){
	if((isLegalH(x1,y1,x2,y2) || isLegalV(x1,y1,x2,y2)) && ownMap[x1][y1]!=0 && ownMap[x2][y2]!=0){
		return true;
	}
	return false;
}

bool Bot::isLegalH(int x1, int y1, int x2, int y2){
	if((y2>=y1 && y1>1 && ownMap[x2][y2]==ownMap[x1][y1-1] && ownMap[x2][y2]==ownMap[x1][y1-2])
				|| (y1>=y2 && y2>1 && ownMap[x1][y1]==ownMap[x2][y2-1] && ownMap[x1][y1]==ownMap[x2][y2-2])
				|| (y2>=y1 && y2<8 && ownMap[x1][y1]==ownMap[x2][y2+1] && ownMap[x1][y1]==ownMap[x2][y2+2])
				|| (y1>=y2 && y1<8 && ownMap[x2][y2]==ownMap[x1][y1+1] && ownMap[x2][y2]==ownMap[x1][y1+2])
				|| (x1!=x2 && y1>0 && y1<9 && ownMap[x2][y2]==ownMap[x1][y1+1] && ownMap[x2][y2]==ownMap[x1][y1-1])
				|| (x1!=x2 && y2>0 && y2<9 && ownMap[x1][y1]==ownMap[x2][y2+1] && ownMap[x1][y1]==ownMap[x2][y2-1])){
			return true;
		} else {
			return false;
		}
}

bool Bot::isLegalV(int x1, int y1, int x2, int y2){
	if((x1>=x2 && x1>1 && ownMap[x2][y2]==ownMap[x1-2][y1] && ownMap[x2][y2]==ownMap[x1-1][y1])
			|| (x2>=x1 && x2>1 && ownMap[x1][y1]==ownMap[x2-2][y2] && ownMap[x1][y1]==ownMap[x2-1][y2])
			|| (x1>=x2 && x1<8 && ownMap[x2][y2]==ownMap[x1+2][y1] && ownMap[x2][y2]==ownMap[x1+1][y1])
			|| (x2>=x1 && x2<8 && ownMap[x1][y1]==ownMap[x2+2][y2] && ownMap[x1][y1]==ownMap[x2+1][y2])
			|| (y1!=y2 && x1>0 && x1<9 && ownMap[x2][y2]==ownMap[x1-1][y1] && ownMap[x2][y2]==ownMap[x1+1][y1])
			|| (y1!=y2 && x2>0 && x2<9 && ownMap[x1][y1]==ownMap[x2-1][y2] && ownMap[x1][y1]==ownMap[x2+1][y2])){
		return true;
	} else {
		return false;
	}
}

int* Bot::getNextMove(int map[], int nextMove[]) //decide and return next move, given the current state of the map.
{
	int bestMove[4];
	fillMap(map);
	//findPossibleMoves(bestMove);

	//findBestMove3(bestMove);  //the greedy algorithm
	findBestMove4(bestMove); //the depth first serach algorithm
	cout << "hello" <<  endl; // I had problem with bot.h so I was testing sometihng
	int tempx1 = bestMove[0];
	int tempy1 = bestMove[1];
	int tempx2 = bestMove[2];
	int tempy2 = bestMove[3];
	nextMove[0] = tempx1;
	nextMove[1] = tempy1;
	nextMove[2] = tempx2;
	nextMove[3] = tempy2;
	return nextMove;
}



// move up the map from the point x, y, so long as
// the first value is matched.  Provide a count of the
// length of moved up and return that length
int Bot::traverseUp(int x, int y)
{
	int x1 = x;
	// The value to match
	int value = ownMap[x][y];
	int count=0;
	while (--x1 >= 0) {
		if (ownMap[x1][y] == value) {
			count++;
		}
		else {
			break;
		}
	}
	return count;
}


// move down the map from the point x, y, so long as
// the first value is matched.  Provide a count of the
// length of moved down and return that length
int Bot::traverseDown(int x, int y)
{
	int x1 = x;
	// The value to match
	int value = ownMap[x][y];
	int count = 0;
	while (++x1 < 10) {
		if (ownMap[x1][y] == value) {
			count++;
		}
		else {
			break;
		}
	}
	return count;
}



// move left in the map from the point x, y, so long as
// the first value is matched.  Provide a count of the
// length of moved left and return that length
int Bot::traverseLeft(int x, int y)
{
	int y1 = y;
	// The value to match
	int value = ownMap[x][y];
	int count = 0;
	while (--y1 >= 0) {
		if (ownMap[x][y1] == value) {
			count++;
		}
		else {
			break;
		}
	}
	return count;
}


// move right in the map from the point x, y, so long as
// the first value is matched.  Provide a count of the
// length of moved right and return that length
int Bot::traverseRight(int x, int y)
{
	int y1 = y;
	// The value to match
	int value = ownMap[x][y];
	int count = 0;
	while (++y1 < 10) {
		if (ownMap[x][y1] == value) {
			count++;
		}
		else {
			break;
		}
	}
	return count;
}



// This is the greedy algorithm.  What it does is move through every
// cell of the map and check every vertical path.  It then does the
// goes thorugh every cell and checks every horizontal path.
//  When checking for a horizontal of vertial path it swaps 4 elements
// (elements above, below, left and right) and checks to see how long the
// paths are.  It retains the longest path in the "bestScore" variable
// and it retains the x1, y1, x2, and y2 values in the bestMove[] array,
// which is passed back to the calling function.
void Bot::findBestMove3(int bestMove[])
{
	int bestScore = 2;
	int count;
	//ownMap[][]
	//go through the map vertically, then horizontally

	// Go through all vertical lines
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			// Skip zero elements
			if (ownMap[x][y] == 0)
				continue;
			// Swap above, below, left and right

			// swap the element above
			if (x > 0 && ownMap[x-1][y] != 0) {
				swap(ownMap[x][y], ownMap[x - 1][y]);
				count = 1;
				count += traverseUp(x, y);
				count += traverseDown(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x - 1;
					bestMove[3] = y;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x - 1][y]);
			}

			// swap the element below
			if (x < 9 && ownMap[x + 1][y] != 0) {
				swap(ownMap[x][y], ownMap[x + 1][y]);
				count = 1;
				count += traverseUp(x, y);
				count += traverseDown(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x + 1;
					bestMove[3] = y;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x + 1][y]);
			}

			// swap the element to the right
			if (y < 9 && ownMap[x][y+1] != 0) {
				swap(ownMap[x][y], ownMap[x][y+1]);
				count = 1;
				count += traverseUp(x, y);
				count += traverseDown(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x;
					bestMove[3] = y + 1;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x][y + 1]);
			}

			// swap the element to the left
			if (y > 0 && ownMap[x][y - 1] != 0) {
				swap(ownMap[x][y], ownMap[x][y - 1]);
				count = 1;
				count += traverseUp(x, y);
				count += traverseDown(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x;
					bestMove[3] = y - 1;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x][y - 1]);
			}
		}
	}

	// Go through all horizontal lines
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			// Skip zero elements
			if (ownMap[x][y] == 0)
				continue;
			// Swap above, below, left and right

			// swap the element above
			if (x > 0 && ownMap[x - 1][y] != 0) {
				swap(ownMap[x][y], ownMap[x - 1][y]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x - 1;
					bestMove[3] = y;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x - 1][y]);
			}

			// swap the element below
			if (x < 9 && ownMap[x + 1][y] != 0) {
				swap(ownMap[x][y], ownMap[x + 1][y]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x + 1;
					bestMove[3] = y;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x + 1][y]);
			}

			// swap the element to the right
			if (y < 9 && ownMap[x][y + 1] != 0) {
				swap(ownMap[x][y], ownMap[x][y + 1]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x;
					bestMove[3] = y + 1;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x][y + 1]);
			}

			// swap the element to the left
			if (y > 0 && ownMap[x][y - 1] != 0) {
				swap(ownMap[x][y], ownMap[x][y - 1]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count > bestScore) {
					bestScore = count;
					bestMove[0] = x;
					bestMove[1] = y;
					bestMove[2] = x;
					bestMove[3] = y - 1;
				}
				// swap elements back
				swap(ownMap[x][y], ownMap[x][y - 1]);
			}
		}
	}
}


// Refresh graph map with the contents of the current map
// and set visited value to false so that it can be used
// for the next path search
void Bot::resetGraphMap()
{
	int x, y;
	for (x = 0; x < 10; x++) {
		for (y = 0; y < 10; y++) {
			graphMap[x][y].value = ownMap[x][y];
			graphMap[x][y].visited = false;
		}
	}
}


// swap these node values
void Bot::swapMap(node& node1, node& node2)
{
	node nd;

	nd = node1;
	node1 = node2;
	node2 = nd;
}


// This does a depth first search.  It's a recursive function, so
// it calls itself as it climbs through a path.  The path is determined
// by the value parameter passed in.  So long as there are contigous locations
// with that parameter, it will consider each part of the path.  It will
// pass back the total length of this path in "pathLength",which should
// start out as zero on when the function is first called.
void Bot::DepthFirstSearch(int value, int x, int y, int& pathLength)
{
	graphMap[x][y].visited = true;
	pathLength++;
	//Move left
	//if(x )
	if (y > 0 && graphMap[x][y - 1].value == value && graphMap[x][y - 1].visited == false) {
		DepthFirstSearch(value, x, y - 1, pathLength);
	}

	// Move right
	if (y < 9 && graphMap[x][y + 1].value == value && graphMap[x][y + 1].visited == false) {
		DepthFirstSearch(value, x, y + 1, pathLength);
	}

	// Move up
	if (x > 0 && graphMap[x-1][y].value == value && graphMap[x-1][y].visited == false) {
		DepthFirstSearch(value, x - 1, y, pathLength);
	}

	// Move down
	if (x < 9 && graphMap[x + 1][y].value == value && graphMap[x + 1][y].visited == false) {
		DepthFirstSearch(value, x + 1, y, pathLength);
	}
}


// This is the function that uses the depth first search for every single node of
// the map.  Before calling a depthfirst search, it makes sure that a vertical or
// horizontal path exists of length three or more, because the depthfirst won't
// be able to determine that. It just determines how long a path is all together.
void Bot::findBestMove4(int bestMove[])
{
	int bestScore = 2;
	int count;

	resetGraphMap();
	// Go through all map cells
	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			// Skip zero elements
			if (ownMap[x][y] == 0)
				continue;
			// Swap above, below, left and right

			// swap the element above
			if (x > 0 && ownMap[x - 1][y] != 0) {
				//Validate that there are at least three before using the depth search of map
				swap(ownMap[x][y], ownMap[x - 1][y]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count < 3) {
					count = 1;
					count += traverseUp(x, y);
				}
				swap(ownMap[x][y], ownMap[x - 1][y]);
				if (count >= 3) { // Set up for depth first search
					//Swap the elements
					swapMap(graphMap[x][y], graphMap[x - 1][y]);
					count = 0;
					DepthFirstSearch(graphMap[x][y].value, x, y, count);
					if (count > bestScore) {
						bestScore = count;
						bestMove[0] = x;
						bestMove[1] = y;
						bestMove[2] = x - 1;
						bestMove[3] = y;
					}
					// swap elements back
					swapMap(graphMap[x][y], graphMap[x - 1][y]);
					resetGraphMap();
				}
			}

			// swap the element below
			if (x < 9 && ownMap[x + 1][y] != 0) {
				swap(ownMap[x][y], ownMap[x + 1][y]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count < 3) {  // Set up for depth first search
					count = 1;
					count += traverseUp(x, y);
					count += traverseDown(x, y);
				}
				swap(ownMap[x][y], ownMap[x + 1][y]);
				if (count >= 3) { // Set up for depth first search
					//Swap the elements
					swapMap(graphMap[x][y], graphMap[x + 1][y]);
					count = 0;
					DepthFirstSearch(graphMap[x][y].value, x, y, count);
					if (count > bestScore) {
						bestScore = count;
						bestMove[0] = x;
						bestMove[1] = y;
						bestMove[2] = x + 1;
						bestMove[3] = y;
					}
					// swap elements back
					swapMap(graphMap[x][y], graphMap[x + 1][y]);
					resetGraphMap();
				}
			}

			// swap the element to the right
			if (y < 9 && ownMap[x][y + 1] != 0) {
				swap(ownMap[x][y], ownMap[x][y+1]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count < 3) {
					count = 1;
					count += traverseUp(x, y);
				}
				swap(ownMap[x][y], ownMap[x][y+1]);
				if (count >= 3) { // Set up for depth first search
					//Swap the elements
					swapMap(graphMap[x][y], graphMap[x][y + 1]);
					count = 0;
					DepthFirstSearch(graphMap[x][y].value, x, y, count);
					if (count > bestScore) {
						bestScore = count;
						bestMove[0] = x;
						bestMove[1] = y;
						bestMove[2] = x;
						bestMove[3] = y + 1;
					}
					// swap elements back
					swapMap(graphMap[x][y], graphMap[x][y + 1]);
					resetGraphMap();
				}
			}

			// swap the element to the left
			if (y > 0 && ownMap[x][y - 1] != 0) {
				swap(ownMap[x][y], ownMap[x][y-1]);
				count = 1;
				count += traverseLeft(x, y);
				count += traverseRight(x, y);
				if (count < 3) {
					count = 1;
					count += traverseUp(x, y);
				}
				swap(ownMap[x][y], ownMap[x][y-1]);
				if (count >= 3) { // Set up for depth first search
					//Swap the elements
					swapMap(graphMap[x][y], graphMap[x][y - 1]);
					count = 0;
					DepthFirstSearch(graphMap[x][y].value, x, y, count);
					if (count > bestScore) {
						bestScore = count;
						bestMove[0] = x;
						bestMove[1] = y;
						bestMove[2] = x;
						bestMove[3] = y - 1;
					}
					// swap elements back
					swapMap(graphMap[x][y], graphMap[x][y - 1]);
					resetGraphMap();
				}
			}
		}
	}
}

void Bot::printMap(){

    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(j != 9){
                cout << ownMap[i][j] << " ";
            } else {
                cout << ownMap[i][j];
            }
        }
        cout << endl;
    }
    cout<<endl;
}

void Bot::printMapCopy(){

    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(j != 9){
                cout << mapCopy[i][j] << " ";
            } else {
                cout << mapCopy[i][j];
            }
        }
        cout << endl;
    }
    cout<<endl;
}



#endif /* SRC_BOT_H_ */

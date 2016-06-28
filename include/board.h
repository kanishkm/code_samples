#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
using namespace std;

class Board
{
private:

	unordered_map<int, char> pieceMatrix;

protected:
	 

public:
	Board();
        ~Board();
	enum Pieces{ NONE=0, BLACK , WHITE};
        vector <vector <int> > boardMatrix;
	void setDefaultPieces(Pieces piece);
	void showBoard();
        std::string getBoardStream();
        int ROWS;
        int COLUMNS;
};

#endif // BOARD

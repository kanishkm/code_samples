#ifndef MOVES_H
#define MOVES_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
using namespace std;

#include "board.h"

class Moves
{

private:
	unordered_multimap<string, string> capturingMoves;
        unordered_multimap<string, string> nonCapturingMoves;
        vector< unordered_multimap<string, string >  > validMoves;

protected:

public:
        Moves(std::shared_ptr <Board> board);
        virtual ~Moves();
        void printLegalMovesAvailable();
        void completeMove(int i, int j, int mRow, int mColumn, Board::Pieces piece, int type);
        void addMove(int i, int j, int mRow, int mColumn, int type);
        int getLegalMoves(Board::Pieces piece);
        void showLegalMovesAvailable();
        std::string getLegalMovesString();
        bool selectMove(unsigned int i);
        Board::Pieces turn;
        int moveType;
        std::shared_ptr<Board> moveBoard;
};

#endif

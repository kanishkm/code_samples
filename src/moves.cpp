#include "moves.h"

Moves::Moves(std::shared_ptr<Board> board):moveBoard(board)
{
    moveType = 1;
    turn = Board::BLACK;
}

Moves::~Moves()
{


}


void Moves::printLegalMovesAvailable()
{
	if(capturingMoves.size() > 0)
	{
		for(auto itr = capturingMoves.begin(); itr != capturingMoves.end() ; ++itr)
		{
			cout << itr->first << " --> " << itr->second <<endl;
		}
	}
	else if(nonCapturingMoves.size() > 0)
	{
		for(auto itr = nonCapturingMoves.begin(); itr != nonCapturingMoves.end() ; ++itr)
		{
			cout << itr->first << " --> " << itr->second <<endl;
		}
	}
}

std::string Moves::getLegalMovesString()
{
    std::stringstream movesStream;
    int index =1;
    if(capturingMoves.size() > 0)
    {
        for(auto itr = capturingMoves.begin(); itr != capturingMoves.end() ; ++itr)
        {
            movesStream << index << ". " <<itr->first << " --> " << itr->second <<endl;
            ++index;
        }
    }
    else if(nonCapturingMoves.size() > 0)
    {
        for(auto itr = nonCapturingMoves.begin(); itr != nonCapturingMoves.end() ; ++itr)
        {
            movesStream << index << ". " << itr->first << " --> " << itr->second <<endl;
            ++index;
        }
    }
    return movesStream.str();
}

void Moves::completeMove(int i, int j, int mRow, int mColumn, Board::Pieces piece, int type)
{
        // clear existing piece
        moveBoard->boardMatrix[i][j] = Board::NONE;
        // set new piece
        moveBoard->boardMatrix[mRow][mColumn] = piece;

        if(piece == Board::BLACK)
        {
             turn = Board::WHITE;
        }
        else
        {
            turn = Board::BLACK;
        }
        if(type == 0)
        {
                if(piece == Board::BLACK)
                {
                        if(mColumn > j)
                        moveBoard->boardMatrix[i +1][j+1] = Board::NONE;
                        else
                        moveBoard->boardMatrix[i +1][j -1] = Board::NONE;


                }
                else if(piece == Board::WHITE)
                {
                        if(mColumn > j)
                        moveBoard->boardMatrix[i -1][j+1] = Board::NONE;
                        else
                        moveBoard->boardMatrix[i -1][j -1] = Board::NONE;

                }
        }
        validMoves.clear();
        capturingMoves.clear();
        nonCapturingMoves.clear();
}

bool Moves::selectMove(unsigned int i)
{
    if(i < 1 || i > validMoves.size() || validMoves.size() == 0)
    {
        return false;
    }
    else
    {
        unordered_multimap<string,string> tempMap;
        tempMap = validMoves.at(i-1);
        auto itr = tempMap.begin();
        while(itr != tempMap.end())
        {
            std::string from = itr->first;
            std::string to =  itr->second;
            char a = from.at(0) - 65;
            char b = to.at(0) - 65;
            this->completeMove(int(a), from.at(1) - '0', int(b), to.at(1)-'0', turn, moveType);
            return true;
        }
    }
    return true;
}


void Moves::addMove(int i, int j, int mRow, int mColumn, int type)
{
        string startPosition, endPosition;
        stringstream temp ;
        char a = (i +65);
        temp << a;
        startPosition.append(temp.str());
        stringstream s;
        s << j;
        startPosition.append(s.str());
        stringstream temp1;
        char b = (mRow + 65);
        temp1 << b;
        endPosition.append(temp1.str());
        stringstream temp2;
        temp2 << mColumn;
        endPosition.append(temp2.str());

        if(type == 0)
        {
                capturingMoves.insert(std::make_pair(startPosition, endPosition));
        }
        else
        {
                nonCapturingMoves.insert(std::make_pair(startPosition, endPosition));
        }
}

int Moves::getLegalMoves(Board::Pieces piece)
{
        unsigned int capturingMoveCount = 0;
        unsigned int nonCapturingMoveCount = 0;

        for (int i = 0; i < moveBoard->ROWS; ++i)
        {
                for (int j = 0; j < moveBoard->COLUMNS; ++j)
        {
                        if(moveBoard->boardMatrix[i][j] == Board::BLACK && piece == Board::BLACK)
                    {
                                if(j != 0 && i !=7)
                                {
                                        if(moveBoard->boardMatrix[i+1][j-1] == Board::NONE)
                                        {
                                                // Non Capturing move
                                                addMove(i, j , i +1 , j -1, 1);
                                                ++nonCapturingMoveCount;
                                        }
                                        else if(moveBoard->boardMatrix[i+1][j-1] == Board::WHITE )
                                        {
                                                // check for edge
                                                if(i != 6)
                                                {
                                                        // Capturing move
                                                        if(moveBoard->boardMatrix[i+2][j-2] == Board::NONE)
                                                        {
                                                                addMove(i, j , i +2 , j -2, 0);
                                                                ++capturingMoveCount;
                                                        }
                                                }
                                        }
                                }
                                if(j != 7 && i != 7)
                                {
                                        if(moveBoard->boardMatrix[i+1][j+1] == Board::NONE)
                                        {
                                                // Non Capturing move
                                                addMove(i, j , i +1 , j +1, 1);
                                                ++nonCapturingMoveCount;
                                        }
                                        else if(moveBoard->boardMatrix[i+1][j+1] == Board::WHITE)
                                        {
                                                // check for edge
                                                if(i!=6)
                                                {
                                                        // Capturing move
                                                        if(moveBoard->boardMatrix[i+2][j+2] == Board::NONE)
                                                        {
                                                                addMove(i, j , i +2 , j +2, 0);
                                                                ++capturingMoveCount;
                                                        }
                                                }
                                        }
                                }
                        }
                        else if(moveBoard->boardMatrix[i][j] == Board::WHITE && piece  == Board::WHITE )
                        {
                                if(j != 0 && i != 0)
                                {
                                        if(moveBoard->boardMatrix[i-1][j-1] == Board::NONE)
                                        {
                                                addMove(i, j , i -1 , j -1, 1);
                                                ++nonCapturingMoveCount;
                                        }
                                        else if(moveBoard->boardMatrix[i-1][j-1] == Board::BLACK)
                                        {
                                                // check for edge
                                                if(i != 1)
                                                {
                                                        // Capturing move
                                                        if(moveBoard->boardMatrix[i-2][j-2] == Board::NONE)
                                                        {
                                                                addMove(i, j , i -2 , j -2, 0);
                                                                ++capturingMoveCount;
                                                        }
                                                }
                                        }
                                }
                                if(j != 7 && i!=0)
                                {
                                        if(moveBoard->boardMatrix[i-1][j+1] == Board::NONE)
                                        {
                                                        // Non Capturing move
                                                        addMove(i, j , i -1 , j +1, 1);
                                                        ++nonCapturingMoveCount;
                                        }
                                        else if(moveBoard->boardMatrix[i-1][j+1] == Board::BLACK)
                                        {
                                                // check for edge
                                                if(i!=1)
                                                {
                                                        // Capturing move
                                                        if(moveBoard->boardMatrix[i-2][j+2] == Board::NONE)
                                                        {
                                                                addMove(i, j , i -2 , j +2, 0);
                                                                ++capturingMoveCount;
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
        if(capturingMoveCount > 0)
        {
                // return all capturing moves
                return capturingMoveCount;
        }
        else if(nonCapturingMoveCount > 0)
        {
                // return all non capturing moves
                return nonCapturingMoveCount;
        }
        else
        {
                // No moves to display , hence game is over
                return 0;
        }
}

void Moves::showLegalMovesAvailable()
{
        int index = 1;
        if(capturingMoves.size() > 0)
        {
                moveType = 0;
                for(auto itr = capturingMoves.begin(); itr != capturingMoves.end() ; ++itr)
                {
                        unordered_multimap<string,string> tempMap;
                        tempMap.insert(std::make_pair(itr->first , itr->second));
                        validMoves.push_back(tempMap);
                        cout << index << ". " <<itr->first << " --> " << itr->second <<endl;
                        ++index;
                }
        }
        else if(nonCapturingMoves.size() > 0)
        {
                moveType = 1;
                for(auto itr = nonCapturingMoves.begin(); itr != nonCapturingMoves.end() ; ++itr)
                {
                        unordered_multimap<string,string> tempMap;
                        tempMap.insert(std::make_pair(itr->first , itr->second));
                        validMoves.push_back(tempMap);
                        cout << index << ". " << itr->first << " --> " << itr->second <<endl;
                        ++index;
                }
        }
        else
        {
                cout << "No Legal Moves Available. Game Over ! You Lose.";
        }
}


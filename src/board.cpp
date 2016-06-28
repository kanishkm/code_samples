#include "board.h"


Board::Board()
{
    ROWS = 8;
    COLUMNS= 8;
	// set pieces 
	pieceMatrix[NONE] = '.';
    pieceMatrix[BLACK] = 'X';
    pieceMatrix[WHITE] = 'O';

	// initialize board matrix size
    boardMatrix.resize(ROWS, vector<int>(COLUMNS, NONE));
}

Board::~Board()
{
}

// prints the board 
void Board::showBoard()
{
	cout <<endl <<endl <<"         ";
	for (int i = 0; i < COLUMNS; ++i)
	{
		cout << i << "      ";
	}
	cout <<endl <<endl;
	for (int i = 0; i < ROWS; ++i)
	{
		cout << "   " << char(i+65) << "     ";
		for (int j = 0; j < COLUMNS; ++j)
    	{
        	cout<< pieceMatrix[boardMatrix[i][j]];
  			cout<< "      ";
    	}
    	cout << endl << endl;
	}
}

std::string Board::getBoardStream()
{
    std::stringstream ss;
    ss <<endl <<endl <<"         ";
    for (int i = 0; i < COLUMNS; ++i)
    {
            ss << i << "      ";
    }
    ss <<endl <<endl;
    for (int i = 0; i < ROWS; ++i)
    {
            ss << "   " << char(i+65) << "     ";
            for (int j = 0; j < COLUMNS; ++j)
    {
            ss<< pieceMatrix[boardMatrix[i][j]];
                    ss<< "      ";
    }
    ss << endl << endl;
    }
    return ss.str();
}

// sets the default pieces on the board, based off color
void Board::setDefaultPieces(Pieces piece)
{
	switch(piece)
	{
		case BLACK:
		{
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < COLUMNS; ++j)
		    	{
		    		if(i == 0 || i == 2)
		    		{
		    			if(j%2 == 1)
		        		boardMatrix[i][j] = BLACK; 
					}
		        	if(i == 1)	
		        	{
		        		if(j%2 == 0)
		        		boardMatrix[i][j] = BLACK; 
		        	}	
		    	}
			}
		}
		break;

		case WHITE:
			{
				for (int i = 5; i < 8; ++i)
				{
					for (int j = 0; j < COLUMNS; ++j)
			    	{
			    		if(i == 5 || i == 7)
			    		{
			    			if(j%2 == 0)
			        		boardMatrix[i][j] = WHITE; 
						}
			        	else if(i == 6)	
			        	{
			        		if(j%2 == 1)
			        		boardMatrix[i][j] = WHITE; 
			        	}	
			    	}
				}
			}
			break;

		case NONE:

			break;
	}
}

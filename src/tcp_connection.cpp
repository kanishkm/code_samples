#include "tcp_connection.h"

unsigned int TcpConnection::playerCount = 0; 
unsigned int TcpConnection::sendCount = 0;
shared_ptr<Board> TcpConnection::board = std::make_shared<Board>();
shared_ptr<Moves> TcpConnection::moves = std::make_shared<Moves>(board);
vector<TcpConnection::pointer> TcpConnection::connectionVector;
const int bufferSize = 2048;

TcpConnection::TcpConnection(boost::asio::io_service& io_service)
    : socket_(io_service)
{

}

void TcpConnection::OnSend(const boost::system::error_code& ErrorCode, size_t transferred)
{
    if (ErrorCode == 0)
    {
        //cout<<"Send Successful "<< "bytes Sent:" << transferred << endl;
        if(sendCount == 0)
        {
            cout << "Game over" <<endl;
        }

        sendCount = 1;
        //sent data is removed from the input sequence
        streamObj.consume(transferred);
        // Send Moves info to one player

    } else {
        //cout<<"Send Unsuccessful"<<endl;
        //closeSocket();
    }
}

void TcpConnection::start(vector<TcpConnection::pointer> &connVector)
{
    connectionVector = connVector;
    if(playerCount == 0 )
    {
	board->setDefaultPieces(Board::BLACK);
        // Send message to client, waiting for client 2
        sendWelcomeMessage(1);
	     playerCount++;
    }
    else if(playerCount == 1)
    {
        board->setDefaultPieces(Board::WHITE);
        board->showBoard();
        sendWelcomeMessage(2);
        playerCount++;
    }
    else 
    {
        // more players connected than required, close connection
        sendWelcomeMessage(3);
        //closeSocket();
    }

    boost::asio::mutable_buffer bufs = incomingStreamObj.prepare(bufferSize);
    boost::asio::async_read(socket_, boost::asio::buffer(bufs, 4),
                   boost::bind(&TcpConnection::receiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void TcpConnection::sendWelcomeMessage(int playerNumber)
{
    std::string message = welcomeMessage(playerNumber);
    std::ostream outgoingStream(&streamObj);
    int32_t messageLength= message.length();
    outgoingStream.write(reinterpret_cast<const char*>(&messageLength), sizeof(messageLength));
    outgoingStream.write(message.c_str(), messageLength);

    boost::asio::async_write(socket_, boost::asio::buffer(streamObj.data(), streamObj.size()), boost::bind(&TcpConnection::handleWrite, shared_from_this(),
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

std::string TcpConnection::welcomeMessage(int playerNumber)
{
  stringstream ss1;
  ss1 << playerNumber;

  std::string welcomeMessage("Welcome to Checkers Lite !! You are Player:"+ss1.str());

  if(playerNumber == 1)
  {
      welcomeMessage += ".Waiting for Player 2......";
  }
  else if(playerNumber == 2)
  {
      welcomeMessage += ".Player 1(Black/ X) plays first..." ;
  }
  else
  {
      welcomeMessage = "Players complete. You are not allowed.";
  }
  return welcomeMessage;
}

void TcpConnection::handleWrite(const boost::system::error_code& error,
      size_t bytes_transferred)
{
    if(error == 0)
    {
      //cout << "Message sent. bytes_transferred:" <<bytes_transferred<< endl;
      streamObj.consume(bytes_transferred);

      if(playerCount == 2)
      {
          size_t bytes_transferred = 0;
          sendCount = 1;
          writeToPlayer(error, bytes_transferred, moves->turn);
      }
    }
}

void TcpConnection::writeToPlayer(const boost::system::error_code& error,
                                  size_t bytes_transferred, unsigned int player)
{
    if(error == 0)
    {
        if(bytes_transferred > 0)
        {
            streamObj.consume(bytes_transferred);
            sendCount++;
        }

        // send board info
        //cout<< "Send board info" <<endl;
        std::string message;
        message = board->getBoardStream();
        std::ostream outgoingStream(&streamObj);
        int32_t messageLength= message.length();
        outgoingStream.write(reinterpret_cast<const char*>(&messageLength), sizeof(messageLength));
        outgoingStream.write(message.c_str(), messageLength);

        if(sendCount == player)
        {
            if(moves->getLegalMoves(moves->turn) > 0)
            {
                moves->showLegalMovesAvailable();
                std::string movesString  = moves->getLegalMovesString();
                movesString += "Your turn !! Enter a number from the list to make a move";
                int32_t movesStringLength = movesString.length();
                outgoingStream.write(reinterpret_cast<const char*>(&movesStringLength), sizeof(movesStringLength));
                outgoingStream.write(movesString.c_str(), movesStringLength);
            }
            else
            {
                //Game over. Player loses
                std::string gameOver("No More moves remaining. You Lose !!!");
                int32_t gameOverStringLength = gameOver.length();
                outgoingStream.write(reinterpret_cast<const char*>(&gameOverStringLength), sizeof(gameOverStringLength));
                outgoingStream.write(gameOver.c_str(), gameOverStringLength);
            }
        }
        else
        {
            std::string movesString;
            movesString += "Waiting for the other player to make a move";
            int32_t movesStringLength = movesString.length();
            outgoingStream.write(reinterpret_cast<const char*>(&movesStringLength), sizeof(movesStringLength));
            outgoingStream.write(movesString.c_str(), movesStringLength);
        }

        if(sendCount == 1)
        {
            boost::asio::async_write(connectionVector.at(0)->socket(), boost::asio::buffer(streamObj.data(), streamObj.size()), boost::bind(&TcpConnection::writeToPlayer, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred, moves->turn));
        }
        else
        if(sendCount == 2)
        {
            boost::asio::async_write(connectionVector.at(1)->socket(), boost::asio::buffer(streamObj.data(), streamObj.size()), boost::bind(&TcpConnection::OnSend, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
        }
    }
}

void TcpConnection::receiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received)
{
    if (ErrorCode == 0)
    {
        //cout << "Bytes received:"<<bytes_received;
        incomingStreamObj.commit(bytes_received);
        std::istream incomingStream(&incomingStreamObj);
        int32_t playerMove;
        incomingStream.read((char*)&playerMove , sizeof(playerMove));
        if(moves->selectMove(playerMove))
        {
            board->showBoard();
            boost::system::error_code error;
            size_t bytes_transferred = 0;
            writeToPlayer(error, bytes_transferred, moves->turn);
        }
        else
        {
            // retry logic.
        }
        boost::asio::mutable_buffer bufs = incomingStreamObj.prepare(bufferSize);
        boost::asio::async_read(socket_, boost::asio::buffer(bufs, 4),
                       boost::bind(&TcpConnection::receiveHandler, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        //closeSocket();
        playerCount--;
    }
}

void TcpConnection::closeSocket()
{
    socket_.close();
}


#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;

#include "board.h"
#include "moves.h"

class TcpConnection
  : public boost::enable_shared_from_this<TcpConnection>
{
public:
  typedef boost::shared_ptr<TcpConnection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new TcpConnection(io_service));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start(vector<TcpConnection::pointer> &connVector);

  void receiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received);

private:
  TcpConnection(boost::asio::io_service& io_service);

  void OnSend(const boost::system::error_code& ErrorCode, size_t transferred);

  void handleWrite(const boost::system::error_code& error,
      size_t bytes_transferred);

  void writeToPlayer(const boost::system::error_code& error,
                                    size_t bytes_transferred, unsigned int player);

   std::string welcomeMessage(int playerNumber);

   void sendWelcomeMessage(int playerNumber);

   void closeSocket();

   void writeToPlayer(unsigned int player);

  tcp::socket socket_;
  std::string message_;
  static unsigned int playerCount;
  static unsigned int sendCount;
  static std::shared_ptr<Board> board;
  static vector<std::string> ipAddr; // ipaddr to send to
  boost::asio::streambuf streamObj;
  boost::asio::streambuf incomingStreamObj;
  static const size_t m_BufLen = 4;
  boost::array<char, m_BufLen> m_Buffer;
  static vector<TcpConnection::pointer> connectionVector;
  static std::shared_ptr <Moves> moves;
};

#endif //TCP_CONNECTION_H

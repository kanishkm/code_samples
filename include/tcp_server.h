#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "tcp_connection.h"
using namespace std;
using boost::asio::ip::tcp;

class TcpServer
{
public:
  TcpServer(boost::asio::io_service& io_service);

private:
  tcp::acceptor acceptor_;

  void start_accept();
  void handle_accept(TcpConnection::pointer new_connection,
      const boost::system::error_code& error);

  static vector<TcpConnection::pointer> connectionVector;

};
#endif // TCP_SERVER_H

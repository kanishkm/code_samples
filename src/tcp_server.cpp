#include "tcp_server.h"

const int SERVER_PORT = 50000;
vector<TcpConnection::pointer>TcpServer::connectionVector;

TcpServer::TcpServer(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), SERVER_PORT))
  {
    start_accept();
  }


void TcpServer::start_accept()
{
    TcpConnection::pointer new_connection =
      TcpConnection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&TcpServer::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
}

void TcpServer::handle_accept(TcpConnection::pointer new_connection,
      const boost::system::error_code& error)
{
    if (!error)
    {
       connectionVector.push_back(new_connection);
       new_connection->start( connectionVector);
    }
    start_accept();
}


int main(int argc, char* argv[])
{
        try
        {
              boost::asio::io_service io_service;
              TcpServer server(io_service);
              io_service.run();
        }
        catch (std::exception& e)
        {
              std::cerr << e.what() << std::endl;
        }
       return 0;
}


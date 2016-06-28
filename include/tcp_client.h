#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <boost/array.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <sstream>

using namespace std;

using boost::asio::ip::tcp;

class TcpClient
{

private:
    boost::asio::io_service& mIoService;
    boost::asio::streambuf incomingStreamObj;
    tcp::socket mSocket;
    static const size_t m_BufLen = 1024;
    boost::array<char, m_BufLen> m_Buffer;
    std::string message_;
    boost::asio::streambuf streamObj;

protected:


public:

    TcpClient(boost::asio::io_service& ioService, tcp::resolver::iterator endPointIter);
    void send(uint32_t move);
    void closeSocket();
    void connectHandler(const boost::system::error_code& ErrorCode, tcp::resolver::iterator EndPointIter);
    void receive(const boost::system::error_code& ErrorCode, size_t bytes_received);
    void headerReceiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received);
    void payloadReceiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received);
    void writeHandler(const boost::system::error_code& ErrorCode, size_t bytes_transferred);
    int32_t messageLen;

};

#endif // TCP_CLIENT_H

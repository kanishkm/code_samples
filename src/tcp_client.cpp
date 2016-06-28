#include "tcp_client.h"

const int messageSize = 4;
const int bufferSize = 2048;

TcpClient::TcpClient(boost::asio::io_service& ioService, tcp::resolver::iterator endPointIter): mIoService(ioService), mSocket(ioService)
{
    tcp::endpoint endPoint = *endPointIter;
    mSocket.async_connect(endPoint,
            boost::bind(&TcpClient::connectHandler, this, boost::asio::placeholders::error, ++endPointIter));
}

void TcpClient::connectHandler(const boost::system::error_code& ErrorCode,
    tcp::resolver::iterator EndPointIter)
{
    if (ErrorCode == 0)
    {
        cout <<"Player Connected Successfully!!!" <<endl;
        boost::asio::mutable_buffer mutableBuffer = incomingStreamObj.prepare(bufferSize);
        boost::asio::async_read(mSocket, boost::asio::buffer(mutableBuffer, messageSize),
                       boost::bind(&TcpClient::headerReceiveHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else if (EndPointIter != tcp::resolver::iterator())
    {
        closeSocket();
    }
}

void TcpClient::closeSocket()
{
    if(mSocket.is_open())
    {
        mSocket.close();
    }
}

void TcpClient::headerReceiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received)
{
    if (ErrorCode == 0)
    {
        //cout << "bytes:"<<bytes_received <<endl;
        incomingStreamObj.commit(bytes_received);
        std::istream incomingStream(&incomingStreamObj);
        int32_t messageLength;
        incomingStream.read((char*)&messageLength , sizeof(messageLength));
        boost::asio::mutable_buffer mutableBuffer = incomingStreamObj.prepare(bufferSize);
        boost::asio::async_read(mSocket, boost::asio::buffer(mutableBuffer, messageLength),
                       boost::bind(&TcpClient::payloadReceiveHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        closeSocket();
    }
}

void TcpClient::payloadReceiveHandler(const boost::system::error_code& ErrorCode, size_t bytes_received)
{
    if (ErrorCode == 0)
    {
        //cout << "Payload bytes:"<<bytes_received <<endl;
        incomingStreamObj.commit(bytes_received);
        std::istream incomingStream(&incomingStreamObj);
        std::istreambuf_iterator<char> eos;
        std::string message(std::istreambuf_iterator<char>(incomingStream), eos);
        cout<< message <<endl;
        boost::asio::mutable_buffer mutableBuffer = incomingStreamObj.prepare(bufferSize);
        boost::asio::async_read(mSocket, boost::asio::buffer(mutableBuffer, messageSize),
                       boost::bind(&TcpClient::headerReceiveHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        closeSocket();
    }
}


void TcpClient::send(uint32_t move)
{
    std::ostream outgoingStream(&streamObj);
    outgoingStream.write(reinterpret_cast<const char*>(&move), sizeof(move));
    boost::asio::async_write(mSocket, boost::asio::buffer(streamObj.data(), streamObj.size()), boost::bind(&TcpClient::writeHandler, this,
    boost::asio::placeholders::error,
    boost::asio::placeholders::bytes_transferred));
}

void TcpClient::writeHandler(const boost::system::error_code& ErrorCode, size_t bytes_transferred)
{
    if (ErrorCode == 0)
    {
        //cout<<"Send Successful "<< "bytes Sent:" << bytes_transferred << endl;
        //sent data is removed from the input sequence
        streamObj.consume(bytes_transferred);
    } else 
    {
        //cout<<"Send Unsuccessful"<<endl;
        closeSocket();
    }
}

int main(int argc, char* argv[])
{
    try 
    {
        boost::asio::io_service IO_Service;
        tcp::resolver Resolver(IO_Service);
        tcp::resolver::query Query("localhost", "50000");
        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);

        TcpClient Client(IO_Service, EndPointIterator);
        boost::thread ClientThread(
                boost::bind(&boost::asio::io_service::run, &IO_Service));

        std::cout << "Client started."<<endl;
        uint32_t input;
        while (input != 0)
        {
            cin >> input;
            Client.send(input);
        }
        Client.closeSocket();
        ClientThread.join();
    } 
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

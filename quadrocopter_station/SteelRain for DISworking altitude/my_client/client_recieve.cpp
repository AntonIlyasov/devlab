#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define IP "192.168.0.20"
#define PORT "11000"

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), IP, PORT);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    using namespace std; // For strlen.
    char request[max_length] = "123456789\n";
    size_t request_length = strlen(request);

    char reply[max_length];
    std::cout << "here1\n";
    size_t reply_length = boost::asio::read(s,boost::asio::buffer(reply, request_length));
    std::cout << "here2\n";
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
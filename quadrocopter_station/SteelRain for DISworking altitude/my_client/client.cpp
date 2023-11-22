#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

#define ZAPRAVKA        " 60.02905 30.259607 30"
#define SHINOMONTAZHKA  " 60.030417 30.258093 300"
#define DEFAULT         " 60.028628 30.257101 30"
#define BUSINESSCENTER  " 60.029976 30.254948 30"
#define SBER            " 60.028068 30.25821  30"
#define RIGHT_45_DEG    " 60.028726 30.262057 30000"
#define LEFT_45_DEG     " 60.028794 30.250932 30000"

int send_count = 1;

std::string make_string(boost::asio::streambuf& streambuf)
{
    return {buffers_begin(streambuf.data()), 
            buffers_end(streambuf.data())};
}

int main()
{
    int count = 0;
    boost::asio::io_service ios;
    boost::asio::ip::tcp::socket socket(ios);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.0.20"), 11000);
    socket.connect(endpoint);
    boost::array<char, 128> buf;
    boost::system::error_code error;

    std::cout << "success connect\n";

    boost::asio::streambuf read_buffer;
    boost::asio::read_until(socket, read_buffer, '\n');
    std::cout << "Read buffer contains: " << make_string(read_buffer) << std::endl;

    while(send_count > 0){
        send_count--;
        // std::cout << "введите  LAT LONG ALT" << std::endl;
        std::string message = DEFAULT;
        // std::string message = "";
        // getline(std::cin, message);
        std::cout << message << std::endl;
        std::copy(message.begin(),message.end(),buf.begin());
        socket.write_some(boost::asio::buffer(buf, message.size()), error);
        count++;
        std::cout << "send count = " << count << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    socket.close(); 
    return 0;
}
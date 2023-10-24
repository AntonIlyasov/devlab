#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

#define ZAPRAVKA        " 60.02905 30.259607 30"
#define SHINOMONTAZHKA  " 60.030417 30.258093 30"
#define DEFAULT         " 60.028669 30.257185 30"
#define BUSINESSCENTER  " 60.029976 30.254948 30"
#define SBER            " 60.028068 30.25821  30"
#define RIGHT_45_DEG    " 60.028726 30.262057 500"
#define LEFT_45_DEG     " 60.028794 30.250932 5000"

int send_count = 1000;

int main()
{
    int count = 0;
    boost::asio::io_service ios;
    boost::asio::ip::tcp::socket socket(ios);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("172.24.1.20"), 11000);
    socket.connect(endpoint);
    boost::array<char, 128> buf;
    boost::system::error_code error;

    std::cout << "success connect\n";
    while(send_count > 0){
        send_count--;
        // std::cout << "введите  LAT LONG ALT" << std::endl;
        std::string message = RIGHT_45_DEG;
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
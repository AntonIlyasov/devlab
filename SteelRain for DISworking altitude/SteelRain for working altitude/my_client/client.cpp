#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

#define ZAPRAVKA        " 60.0 154 30.259595 30"
#define SHINOMONTAZHKA  " 60.029944 30.257826 30"
#define DEFAULT         " 60.0285855 30.2571131 30"
#define BUSINESSCENTER  " 60.029818 30.255197 30"
#define SBER            " 60.026783 30.260047 30"
#define RIGHT_45_DEG    " 60.0277956 30.2658165 30"
#define LEFT_45_DEG     " 60.028886 30.255764 30"

int main()
{
    int count = 0;
    boost::asio::io_service ios;
    boost::asio::ip::tcp::socket socket(ios);
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("192.168.0.15"), 11000);
    socket.connect(endpoint);
    boost::array<char, 128> buf;
    boost::system::error_code error;

    double lat = 60.028574;
    double lon = 30.357217;
    double alt = 30;
    std::cout << "success connect\n";
    while(1){
        std::cout << "введите  LAT LONG ALT" << std::endl;
        //std::string message = SBER;
        std::string message = "";
        getline(std::cin, message);
        std::cout << message << std::endl;
        std::copy(message.begin(),message.end(),buf.begin());
        socket.write_some(boost::asio::buffer(buf, message.size()), error);
        count++;
        std::cout << "send count = " << count << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    socket.close(); 
    return 0;
}
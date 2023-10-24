#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>

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
    double lon = 30.257217;
    double alt = 30;

    int cycle_count = 0;
    bool flag = 0;

    while(1){

        if (cycle_count == 1){
            flag = 0;
        } 
        if (cycle_count == 0) flag = 1;

        if (flag){
            cycle_count++;
            lon += 0.001;
        } else {
            cycle_count--;
            lon -= 0.001;  
        }

        std::string message = " " + std::to_string(lat) + " " + std::to_string(lon) + " " + std::to_string(alt);

        std::cout << message << std::endl;
        std::copy(message.begin(),message.end(),buf.begin());
        socket.write_some(boost::asio::buffer(buf, message.size()), error);
        count++;
        std::cout << "send count = " << count << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    socket.close(); 
    return 0;
}
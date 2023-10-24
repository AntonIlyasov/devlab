#include <iostream>
#include <boost/asio.hpp>

#define PORT 11000

using namespace boost::asio;
using ip::tcp;
using boost::asio::ip::address;
using std::string;
using std::cout;
using std::endl;

class TCPServer{
private:
    boost::asio::io_context& io_context_;
    tcp::socket socket_;
public:

    TCPServer(boost::asio::io_context& io_context)
        : io_context_(io_context), socket_(io_context){
    }

    void receive_data(){
        boost::system::error_code error;
        io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), PORT));
        socket_ = acceptor.accept();
        
        while (1) {
            
            std::vector<uint8_t> msgRecBuf(1448);
            std::vector<uint8_t> imgBufToImdecode;
            size_t bytesReadToImgBufToImdecode = 0;
            size_t uncounted_bytes_remaining = 0;
            std::vector<uchar> imgSizeBuf(8);
            size_t frameSizeFromBuf = 0;

            bool is_first_read = true;

            while(is_first_read || bytesReadToImgBufToImdecode < imgBufToImdecode.size()){
                if (is_first_read){
                    std::cout << "is_first_read" << std::endl;
                    size_t length = socket_.read_some(boost::asio::buffer(msgRecBuf), error);
                    
                    if (error == boost::asio::error::eof)
                        break; // Connection closed cleanly by peer.    
                    else if (error)
                        throw boost::system::system_error(error); // Some other error.

                    std::cout << "length = " << length << std::endl;
                    
                    std::move(msgRecBuf.begin(), msgRecBuf.begin() + imgSizeBuf.size(), imgSizeBuf.begin());
                    std::string frameSizeFromBufStr;
                    frameSizeFromBufStr.resize(8);
                    for(int i = 0; i < imgSizeBuf.size(); i++){
                        frameSizeFromBufStr[i] = imgSizeBuf[i];
                    }

                    frameSizeFromBuf = std::stoi(frameSizeFromBufStr);
                    uncounted_bytes_remaining = frameSizeFromBuf;
                    std::cout << "frameSizeFromBuf = " << frameSizeFromBuf << std::endl;
                    imgBufToImdecode.resize(frameSizeFromBuf);
                    std::cout << "imgBufToImdecode.size() = " << imgBufToImdecode.size() << std::endl;
                    std::move(msgRecBuf.begin() + imgSizeBuf.size(), msgRecBuf.end(), imgBufToImdecode.begin());
                    bytesReadToImgBufToImdecode += msgRecBuf.size() - imgSizeBuf.size();
                    uncounted_bytes_remaining -= msgRecBuf.size() - imgSizeBuf.size();
                    std::cout << "bytesReadToImgBufToImdecode = " << bytesReadToImgBufToImdecode << std::endl;
                    std::cout << "uncounted_bytes_remaining = " << uncounted_bytes_remaining << std::endl; 

                    is_first_read = false;
                } else{
                    std::cout << "is_NOT_first_read" << std::endl;
                    size_t length = socket_.read_some(boost::asio::buffer(msgRecBuf), error);

                    if (error == boost::asio::error::eof)
                        break; // Connection closed cleanly by peer.    
                    else if (error)
                        throw boost::system::system_error(error); // Some other error.
                        
                    std::cout << "length = " << length << std::endl;
                    std::move(msgRecBuf.begin(), msgRecBuf.begin() + length, imgBufToImdecode.begin() + bytesReadToImgBufToImdecode);
                    bytesReadToImgBufToImdecode += length;
                    uncounted_bytes_remaining -= length;
                    std::cout << "bytesReadToImgBufToImdecode = " << bytesReadToImgBufToImdecode << std::endl;
                    std::cout << "uncounted_bytes_remaining = " << uncounted_bytes_remaining << std::endl;
                    std::cout << "imgBufToImdecode.size() = " << imgBufToImdecode.size() << std::endl;
                }
            }
            send_(socket_, "frame readed");
            if(imgBufToImdecode.size() == 0) continue;
            //if(imgBufToImdecode.size() > bytesReadToImgBufToImdecode){

            //    std::move(imgBufToImdecode.begin(), imgBufToImdecode.begin() + imgBufToImdecode.size(), imgBufToImdecode.begin() + bytesReadToImgBufToImdecode);
            //}
            std::cout << "HEY!\n";
            img = cv::imdecode(imgBufToImdecode, cv::IMREAD_UNCHANGED);
            // cv::aruco::detectMarkers(img, dictionary, corners, ids);
            // if (ids.size() > 0){
            //     cv::aruco::drawDetectedMarkers(img, corners, ids);
            //     // std::vector<cv::Vec3d> rvecs, tvecs;
            //     // cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rvecs, tvecs);
            //     // // draw axis for each marker
            //     // for(int i=0; i<ids.size(); i++){
            //     //     cv::aruco::drawAxis(img, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], 0.1);
            //     // }
            // }
            imshow("frameFromWeb", img);
            cv::waitKey(1);
        }
    }
    
    ~TCPServer(){
		socket_.close();
	}
};

int main() {
    boost::asio::io_context io_context;
    TCPServer tcpServer(io_context);
    tcpServer.receive_data();
}
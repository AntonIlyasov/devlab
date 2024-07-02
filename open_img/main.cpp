#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core.hpp>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
  cv::Mat image = cv::imread("../2.png");
  std::vector<uchar> imgBufferFromImencode, msgSendBuf;


  cv::imencode(".png", image, imgBufferFromImencode, std::vector<int> {cv::IMWRITE_JPEG_QUALITY, 100});

  const std::string frameSizeStringFromImencode = std::to_string(imgBufferFromImencode.size());
  std::ofstream out("output.txt");
  out << imgBufferFromImencode;
  out.close();

  
  cv::waitKey();
}
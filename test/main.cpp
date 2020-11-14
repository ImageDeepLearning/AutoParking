#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>

using namespace std;
int main() {
    cv::Mat src;
    string configFile = "/home/jyp/AutoParking/test/config.yaml";
    std::cout << "Hello, World!" << std::endl;
    cv::FileStorage fs(configFile, cv::FileStorage::READ);
    cout << fs.isOpened()<< endl;
    cv::Mat cam_k;
    if (fs.isOpened()) {
        fs["cam_k"] >> cam_k;
    }
    return 0;
}
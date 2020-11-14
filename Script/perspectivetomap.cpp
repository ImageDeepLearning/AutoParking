//
// Created by jyp on 2020/11/3.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/objdetect/objdetect_c.h>

using namespace std;
using namespace cv;

void perspective_to_maps(const cv::Mat &perspective_mat, const cv::Size img_size,
                         cv::Mat &map1, cv::Mat &map2)
{
    cv::Mat inv_perspective(perspective_mat.inv());
    inv_perspective.convertTo(inv_perspective, CV_32FC1);
    cv::Mat xy(img_size, CV_32FC2);
    float *pxy = (float*)xy.data;
    for (int y = 0; y < img_size.height; y++)
        for (int x = 0; x < img_size.width; x++)
        {
            *pxy++ = x;
            *pxy++ = y;
        }
    cout << perspective_mat.rows  << " " << perspective_mat.cols << endl;
    // perspective transformation of the points
    cv::Mat xy_transformed;
    cv::perspectiveTransform(xy, xy_transformed, inv_perspective);
    //Prevent errors when float32 to int16
    float *pmytest = (float*)xy_transformed.data;
    for (int y = 0; y < xy_transformed.rows; y++)
        for (int x = 0; x < xy_transformed.cols; x++)
        {
            if (abs(*pmytest) > 5000) *pmytest = 5000.00;
            pmytest++;
            if (abs(*pmytest) > 5000) *pmytest = 5000.00;
            pmytest++;
        }

    // split x/y to extra maps
    assert(xy_transformed.channels() == 2);
    cv::Mat maps[2]; // map_x, map_y
    cv::split(xy_transformed, maps);

    // remap() with integer maps is faster
    cv::convertMaps(maps[0], maps[1], map1, map2, CV_16SC2);
}

int main() {
    Mat map1, map2;
    Mat H0;
    Size size = Size(760, 900);
    FileStorage fsH0;
    fsH0.open("/home/jyp/AutoParking/Script/H0.xml", FileStorage::READ);
    fsH0["H0"] >> H0;
    fsH0.release();
    perspective_to_maps(H0, size, map1, map2);

    FileStorage fs_H0("map1_3H.xml", FileStorage::WRITE);
    fs_H0 << "map1" << map1;
    fs_H0.release();
    FileStorage fs_H1("map2_3H.xml", FileStorage::WRITE);
    fs_H1 << "map2" << map2;
    fs_H1.release();
    Mat srcimg = imread("/home/jyp/AutoParking/Script/b0.jpg");
    Mat dstimg;
    remap(srcimg, dstimg, map1, map2, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    imshow("das", dstimg);
    imwrite("tst0.jpg", dstimg);
    cv::waitKey(0);
    return 0;
}

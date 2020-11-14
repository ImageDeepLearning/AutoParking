//
// Created by jyp on 2020/11/3.
//

#ifndef AUTOPARKING_BIRDVIEW_H
#define AUTOPARKING_BIRDVIEW_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/objdetect/objdetect_c.h>


using namespace cv;
using namespace std;

// calculate correspondence point for every input
// 0: left up  1: right up  2:rigth down  3: left down


class BirdView
{
public:
    BirdView()
    {
        SourcePoint_OK=ParamSet_OK = false;
        maskHeigth = clickCount = camID = 0;
        targetPoint.resize(4);
        sourcePoint.resize(4);
        try
        {
            carPic = imread("/home/jyp/AutoParking/BirdView/car.jpg",CV_8UC4);//read car model
            //imshow("car",carPic);
        }
        catch (...)
        {
            std::cout <<"[WARNING] Car model view pic not found!\n";
        }
        for (int i = 0;i < 4; i++)
        {
            targetPoint[i].resize(4);
            sourcePoint[i].resize(4);
        }
    }

    void  setInternalShift(int W, int H);//Size distance between actual vehicle and camera blind zone

    void setShift(int W, int H); //Image size distance outside the calibration point

    void setCarSize(int W,int H); //Vehicle occupies size in the image (including blind spots)

    void setChessSize(int W, int H); //Checkerboard size

    void setMaskHeigth(int maskHeigth_);

    void getH();//Write perspective transformation matrix

    Mat transformView(Mat* v);

    Mat transformView(vector<Mat>&v);
    void saveConfig(const char* configFile = "config.yaml");

    void readConfig(const char* configFile = "/home/jyp/AutoParking/BirdView/config.yaml");

    void sourcePointClick(Mat *v);

    void sourcePointClick(cv::VideoCapture *v);

    static void on_MouseHandle(int e, int x, int y, int flag, void* param);

    Mat fusion(cv::Mat imup, cv::Mat imdown, Point2i start, Point2i end);

    Mat fusionat(cv::Mat imup, cv::Mat imdown, Point2i start, Point2i end);
    void setParam(bool tranformCheck = false);
private:
    Rect r[8],carPicPos;
    int clickCount, camID, maskHeigth;
    Mat Birdtransform[4],maskF, maskB, carPic,carPicTmp;
    vector<vector<Point2f> > targetPoint, sourcePoint;
    Size ShiftAdjust, Shift, chessBordWidth, mSize, carSize;
    bool SourcePoint_OK,ParamSet_OK;
/*left&front*/
    Mat roi1 = Mat::zeros(Size(760, 900), CV_8U);
    vector<vector<Point>> contour1;
    vector<Point> pts1;
/*right&front*/
    Mat roi2 = Mat::zeros(Size(760, 900), CV_8U);
    vector<vector<Point>> contour2;
    vector<Point> pts2;
/*right&rear*/
    Mat roi3 = Mat::zeros(Size(760, 900), CV_8U);
    vector<vector<Point>> contour3;
    vector<Point> pts3;
/*left&rear*/
    Mat roi4 = Mat::zeros(Size(760, 900), CV_8U);
    vector<vector<Point>> contour4;
    vector<Point> pts4;
};
#endif //AUTOPARKING_BIRDVIEW_H

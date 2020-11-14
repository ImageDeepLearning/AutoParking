//
// Created by jyp on 2020/11/3.
//
#include "BirdView.h"

void BirdView::setInternalShift(int W, int H)
{
    ShiftAdjust = Size(W,H);
    ParamSet_OK = false;
    setParam();
}
void BirdView::setShift(int W, int H)
{
    Shift = Size(W,H);
    ParamSet_OK = false;
    setParam();
}
void BirdView::setCarSize(int W,int H)
{
    carSize = Size(W, H);
    ParamSet_OK = false;
    setParam();
}
void BirdView::setChessSize(int W, int H)
{
    chessBordWidth.width = W;
    chessBordWidth.height = H;
    ParamSet_OK = false;
    setParam();
}
void BirdView::setMaskHeigth(int maskHeigth_)
{
    maskHeigth = maskHeigth_;
    ParamSet_OK = false;
    setParam();
}
void BirdView::getH()//Write perspective transformation matrix
{
    FileStorage fs_H0("H0.xml", FileStorage::WRITE);
    fs_H0 << "H0" << Birdtransform[0];
    fs_H0.release();
    FileStorage fs_H1("H1.xml", FileStorage::WRITE);
    fs_H1 << "H1" << Birdtransform[1];
    fs_H1.release();
    FileStorage fs_H2("H2.xml", FileStorage::WRITE);
    fs_H2 << "H2" << Birdtransform[2];
    fs_H2.release();
    FileStorage fs_H3("H3.xml", FileStorage::WRITE);
    fs_H3 << "H3" << Birdtransform[3];
    fs_H3.release();

}
Mat BirdView::transformView(Mat* v)//Image conversion function(Source->Top view->Look around bird view)
{
    char buf[10];
    char buf_H[10];
    char buf_f[10];

    if (!SourcePoint_OK)
    {
        std::cerr<<"[ERROR] Source Points have not been pointed! please Add function sourcePointClick to get Source Points!\n";
        throw "[ERROR] Source Points have not been pointed! please Add function sourcePointClick to get Source Points!\n";
    }
    if (!ParamSet_OK)
    {
        setParam();
    }
    Mat b[4];
    Mat m = Mat(mSize, CV_8UC3 );
    int seq[4] = { 0,2,1,3 };
    //int seq[4] = { 0,1,2,3 };
    for (int i = 0; i < 4; i++)
    {
        snprintf(buf, sizeof(buf),"out%d.jpg", i);//Four top view names
        //sprintf_s(buf_H, "undis%d", i);
        if(!v[seq[i]].data)
        {
            continue;
        }
        //imwrite(buf_H, v[seq[i]]);
        warpPerspective(v[seq[i]], b[seq[i]], Birdtransform[seq[i]], mSize);//Output transformed image, size:mSize   seq[0], v[0]: left view
        imshow(buf,b[seq[i]]);
        imwrite(buf,b[seq[i]]);
        switch (seq[i])
        {
            case 1:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]), maskF);
                break;
            case 3:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]), maskB);
                break;
            default:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]));
                break;
        }
    }
    imshow("b0", b[0]);
    imwrite("b0.jpg", b[0]);
    imshow("b1", b[1]);
    imwrite("b1.jpg", b[1]);
    imshow("b2", b[2]);
    imwrite("b2.jpg", b[2]);
    imshow("b3", b[3]);
    imwrite("b3.jpg", b[3]);

    //Eliminate black blind zone
    b[0].copyTo(m, roi1);//left&front
    b[2].copyTo(m, roi2);//right&front
    b[2].copyTo(m, roi3);//rignt&rear
    b[0].copyTo(m, roi4);//left&rear

    if(carPic.data) carPicTmp.copyTo(m(carPicPos));
    //drawing target points
    const Scalar color[4] = { Scalar(255,0,0),Scalar(0,255,0), Scalar(255,255,0), Scalar(0,255,255) };
    for (int i = 0; i < 16; i++) circle(m, targetPoint[i / 4][i % 4], 5, color[i / 4], 5);
    return m;
}

Mat BirdView::transformView(vector<Mat>&v)//Image conversion function(Source->Top view->Look around bird view)
{
    char buf[10];
    char buf_H[10];
    char buf_f[10];
    if (!SourcePoint_OK)
    {
        std::cerr<<"[ERROR] Source Points have not been pointed! please Add function sourcePointClick to get Source Points!\n";
        throw "[ERROR] Source Points have not been pointed! please Add function sourcePointClick to get Source Points!\n";
    }
    if (!ParamSet_OK)
    {
        setParam();
    }
    Mat b[4];
    Mat m = Mat(mSize, CV_8UC3);
    int seq[4] = { 0,2,1,3 };
    //int seq[4] = { 0,1,2,3 };
    for (int i = 0; i < 4; i++)
    {
        snprintf(buf, sizeof(buf),"out%d.jpg", i);//Four top view names
        //sprintf_s(buf_H, "undis%d", i);
        if(!v[seq[i]].data)
        {
            continue;
        }
        //imwrite(buf_H, v[seq[i]]);
        warpPerspective(v[seq[i]], b[seq[i]], Birdtransform[seq[i]], mSize);//Output transformed image, size:mSize   seq[0], v[0]: left view
        //imshow(buf,b[seq[i]]);
        //imwrite(buf,b[seq[i]]);
        switch (seq[i])
        {
            case 1:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]), maskF);
                break;
            case 3:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]), maskB);
                break;
            default:
                b[seq[i]](r[seq[i]]).copyTo(m(r[seq[i]]));
                break;
        }
    }
    /*
    imshow("b0", b[0]);
    imwrite("b0.jpg", b[0]);
    imshow("b1", b[1]);
    imwrite("b1.jpg", b[1]);
    imshow("b2", b[2]);
    imwrite("b2.jpg", b[2]);
    imshow("b3", b[3]);
    imwrite("b3.jpg", b[3]);
*/
    //Eliminate black blind zone
    b[0].copyTo(m, roi1);//left&front
    b[2].copyTo(m, roi2);//right&front
    b[2].copyTo(m, roi3);//rignt&rear
    b[0].copyTo(m, roi4);//left&rear

    if(carPic.data) carPicTmp.copyTo(m(carPicPos));
    //drawing target points
    const Scalar color[4] = { Scalar(255,0,0),Scalar(0,255,0), Scalar(255,255,0), Scalar(0,255,255) };
    for (int i = 0; i < 16; i++) circle(m, targetPoint[i / 4][i % 4], 5, color[i / 4], 5);
    return m;
}
void BirdView::saveConfig(const char* configFile)
{
    for (int i = 0;i < 4; i++)
    {
        if (sourcePoint[i].empty())
        {
            std::cout << "[ERROR] sourcePoint has not been comfired all\n"<<std::endl;
            return ;
        }
    }
    FileStorage fs(configFile, FileStorage::WRITE);
    if (fs.isOpened())
    {
        for (int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 4; k++)
            {
                char buf[20];
                snprintf(buf, sizeof(buf),"sourcePoint%d%d", i, k);
                write(fs, buf, sourcePoint[i][k]);
            }
        }
        fs.release();
        std::cout << "\n param save complete! \n\n";
    }
}
void BirdView::readConfig(const char* configFile)
{
    FileStorage fs(configFile, FileStorage::READ);
    if (fs.isOpened())
    {
        for (int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 4; k++)
            {
                char buf[20];
                snprintf(buf, sizeof(buf),"sourcePoint%d%d", i, k);
                fs[buf] >> sourcePoint[i][k];
            }
        }
        SourcePoint_OK = true;  // source point reading completed
        ParamSet_OK = false; // setting parma
        setParam();
        std::cout << "[WARNING] Config file read sucessfully!\n";
    }
    else  std::cout << "[WARNING] There is not a config file in folder\n";
}
void BirdView::sourcePointClick(Mat *v)
{
    //setParam(1);
    // click corner-points and record them
    printf("cam: %d ,pointID: %d  ", camID, clickCount);
    const char *windowsName = "Source point set";
    namedWindow(windowsName);
    setMouseCallback(windowsName,on_MouseHandle, (void*)this);
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            sourcePoint[i][j]= Point2f(0,0);
        }
    }

    for (camID = 0, clickCount = 0; camID<4;)
    {
        for (int i = 0; i < sourcePoint[camID].size(); i++)
        {
            circle(v[camID], sourcePoint[camID][i], 5, Scalar(255, 0, 0), 2);
        }
        imshow(windowsName, v[camID]);
        if (waitKey(20) == 'j')	break;
    }
    setMouseCallback(windowsName, NULL, NULL);
    destroyWindow(windowsName);
    saveConfig("config.yml");/*save source's points*/
    SourcePoint_OK = true;
}
void BirdView::sourcePointClick(cv::VideoCapture *v)
{
    setParam(1);
    Mat ans;
    // click corner-points and record them
    printf("cam: %d ,pointID: %d  ", camID, clickCount);
    const char *windowsName = "Source point set";
    namedWindow(windowsName);
    setMouseCallback(windowsName,on_MouseHandle, (void*)this);
    for(int i=0;i<4;i++)
    {
        sourcePoint[i].clear();
    }
    for (camID = 0, clickCount = 0; camID<4;)
    {
        v[camID] >> ans;
        for (int i = 0; i < sourcePoint[camID].size(); i++)
        {
            circle(ans, sourcePoint[camID][i], 5, Scalar(255, 0, 0), 2);
        }
        imshow(windowsName, ans);
        if (waitKey(20) == 'j')	break;
    }
    setMouseCallback(windowsName, NULL, NULL);
    destroyWindow(windowsName);
    saveConfig("config.yml");/*save source's points*/
    SourcePoint_OK = true;
}
void BirdView::on_MouseHandle(int e, int x, int y, int flag, void* param)
{
    BirdView & birdView = *(BirdView*)param;
    int camID = birdView.camID;
    switch (e)
    {
        case EVENT_LBUTTONUP:
        {
            birdView.sourcePoint[birdView.camID][birdView.clickCount] = Point2f(x, y)	;
            printf("x:%d y:%d\n", x, y);
            birdView.clickCount++;
            if (birdView.clickCount> 3)
            {
                birdView.clickCount = 0;
                birdView.Birdtransform[camID] = getPerspectiveTransform(birdView.sourcePoint[camID], birdView.targetPoint[camID]);
                birdView.camID++;
            }
            if (birdView.camID<3)
            {
                printf("cam: %d ,pointID: %d  ", birdView.camID, birdView.clickCount);
            }
            else printf("\n");
        }
        default: break;
    }
}



Mat BirdView::fusion(cv::Mat imup, cv::Mat imdown, Point2i start, Point2i end)
{
    double divide = 1.0 / double(end.y-start.y);
    double d = 1.0 / double(start.y - end.y);
    cv::Mat dst = cv::Mat(imdown.rows, imdown.cols, imdown.type());
    int channle = imdown.channels();
    for (int j = start.y; j < end.y; j++)
    {   // �ж�λ
        d += divide;
        uchar *data_dst = dst.ptr<uchar>(j);				// ����Ҫ�õ���ƴ��ͼ
        uchar *datau = imup.ptr<uchar>(j);		// ��ͼ
        uchar *datad = imdown.ptr<uchar>(j);		// ��Ӧ�任����ͼ��
        for (int i = start.x; i < start.x; i++)
        {
            if (datad[i*channle+0] == 0 && datad[i*channle+1] == 0 && datad[i*channle +2] ==0)
            {//ֻ����ͼ
                data_dst[i*channle + 0] = datau[i*channle + 0];
                data_dst[i*channle + 1] = datau[i*channle + 1];
                data_dst[i*channle + 2] = datau[i*channle + 2];
            }
            else if (datau[i*channle + 0] == 0 && datau[i*channle + 1] == 0 && datau[i*channle + 2] == 0)
            {
                data_dst[i*channle + 0] = datad[i*channle + 0];
                data_dst[i*channle + 1] = datad[i*channle + 1];
                data_dst[i*channle + 2] = datad[i*channle + 2];
            }
            else
            {
                data_dst[i*channle + 0] = d * datad[i*channle + 0] + (1 - d)*datau[i*channle + 0];
                data_dst[i*channle + 1] = d * datad[i*channle + 0] + (1 - d)*datau[i*channle + 1];
                data_dst[i*channle + 2] = d * datad[i*channle + 0] + (1 - d)*datau[i*channle + 2];
            }
        }
    }
    return dst;
}

Mat BirdView::fusionat(cv::Mat imup, cv::Mat imdown, Point2i start, Point2i end) {
    const double divide = 1.0 / double(end.y - start.y);
    double d = 1.0 / double(start.y - end.y);
    cv::Mat dst = cv::Mat(imdown.rows, imdown.cols, imdown.type());
    /*int rows = imdown.row;
    int cols = imdown.col;*/
    for (int j = start.y; j < end.y; ++j) {
        d += divide;
        for (int i = start.x; i < end.x; ++i) {
            /*dst.at<Vec3b>(i, j)[0] = d * imdown.at<Vec3b>(i, j)[0] + (1 - d)*imup.at<Vec3b>(i, j)[0];
            dst.at<Vec3b>(i, j)[1] = d * imdown.at<Vec3b>(i, j)[1] + (1 - d)*imup.at<Vec3b>(i, j)[1];
            dst.at<Vec3b>(i, j)[2] = d * imdown.at<Vec3b>(i, j)[2] + (1 - d)*imup.at<Vec3b>(i, j)[2];*/
            if (imdown.at<Vec3b>(i, j)[0] == 0 && imdown.at<Vec3b>(i, j)[1] == 0 && imdown.at<Vec3b>(i, j)[2] == 0) {//ֻ����ͼ
                dst.at<Vec3b>(i, j)[0] = imup.at<Vec3b>(i, j)[0];
                dst.at<Vec3b>(i, j)[1] = imup.at<Vec3b>(i, j)[1];
                dst.at<Vec3b>(i, j)[2] = imup.at<Vec3b>(i, j)[2];
            }
            else if (imup.at<Vec3b>(i, j)[0] == 0 && imup.at<Vec3b>(i, j)[1] == 0 && imup.at<Vec3b>(i, j)[2] == 0) {
                dst.at<Vec3b>(i, j)[0] = imdown.at<Vec3b>(i, j)[0];
                dst.at<Vec3b>(i, j)[1] = imdown.at<Vec3b>(i, j)[1];
                dst.at<Vec3b>(i, j)[2] = imdown.at<Vec3b>(i, j)[2];
            }
            else {
                dst.at<Vec3b>(i, j)[0] = d * imdown.at<Vec3b>(i, j)[0] + (1 - d)*imup.at<Vec3b>(i, j)[0];
                dst.at<Vec3b>(i, j)[1] = d * imdown.at<Vec3b>(i, j)[1] + (1 - d)*imup.at<Vec3b>(i, j)[1];
                dst.at<Vec3b>(i, j)[2] = d * imdown.at<Vec3b>(i, j)[2] + (1 - d)*imup.at<Vec3b>(i, j)[2];
            }

        }
    }
    return dst;
}


void BirdView::setParam(bool tranformCheck) {
    //// WARMING will show when Transform is running but not all parameters have been set
    if (Shift.area() == 0) {
        if (tranformCheck)std::cout << "[WARMING] Shift has not been set! Default value will be used" << std::endl;
        Shift.width = Shift.height = 200;
    }
    if (chessBordWidth.area() == 0) {
        if (tranformCheck)
            std::cout << "[WARMING] chessBordWidth has not been set! Default value will be used" << std::endl;
        chessBordWidth.width = chessBordWidth.height = 60;
    }
    if (ShiftAdjust.area() == 0) {
        if (tranformCheck)
            std::cout << "[WARMING] ShiftAdjust has not been set! Default value will be used" << std::endl;
        /*ShiftAdjust.width = 0;
        ShiftAdjust.height = 0;*/
        ShiftAdjust.width = 36;
        ShiftAdjust.height = 27;
    }
    if (carSize.area() == 0) {
        if (tranformCheck)std::cout << "[WARMING] carSize has not been set! Default value will be used" << std::endl;
        carSize = Size(240, 380);
    }
    if (maskHeigth >= 100 || maskHeigth <= 0) {
        if (tranformCheck)std::cout << "[WARMING] maskHeigth has not been set! Default value will be used" << std::endl;
        maskHeigth = 200;
    }
    if (!ParamSet_OK) {
        /*The size of the entire output image*/
        mSize = Size(Shift.width * 2 + carSize.width + chessBordWidth.width * 2,
                     Shift.height * 2 + carSize.height + chessBordWidth.height * 2);
        /*make targetPoint, need chessBordWidth,mSize,Shift*/
        ///*left*/
        //targetPoint[0][0] = (Point2f(Shift.width + chessBordWidth.width+11, Shift.height));
        //targetPoint[0][1] = (Point2f(Shift.width + chessBordWidth.width+11, mSize.height - Shift.height-4));
        //targetPoint[0][2] = (Point2f(Shift.width, mSize.height - Shift.height));
        //targetPoint[0][3] = (Point2f(Shift.width, Shift.height));

        ///*forward*/
        //targetPoint[1][0] = (Point2f(mSize.width - Shift.width, Shift.height + chessBordWidth.height+41));
        //targetPoint[1][1] = (Point2f(Shift.width, Shift.height + chessBordWidth.height+41));
        //targetPoint[1][2] = (Point2f(Shift.width, Shift.height));
        //targetPoint[1][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        ///*backward*/
        //targetPoint[3][0] = (Point2f(Shift.width+1, mSize.height - Shift.height - chessBordWidth.width-41));//��200��652��
        //targetPoint[3][1] = (Point2f(mSize.width - Shift.width-1, mSize.height - Shift.height - chessBordWidth.height-41));
        //targetPoint[3][2] = (Point2f(mSize.width - Shift.width, mSize.height - Shift.height));
        //targetPoint[3][3] = (Point2f(Shift.width, mSize.height - Shift.height));
        ///*right*/
        //targetPoint[2][0] = (Point2f(mSize.width - Shift.width - chessBordWidth.width-13, Shift.height));
        //targetPoint[2][1] = (Point2f(mSize.width - Shift.width - chessBordWidth.width-13, mSize.height - Shift.height-1));
        //targetPoint[2][2] = (Point2f(mSize.width - Shift.width-2, mSize.height - Shift.height));
        //targetPoint[2][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        /*left*/
        targetPoint[0][0] = (Point2f(Shift.width + chessBordWidth.width + 19, Shift.height));
        targetPoint[0][1] = (Point2f(Shift.width + chessBordWidth.width + 22, mSize.height - Shift.height - 1));
        targetPoint[0][2] = (Point2f(Shift.width, mSize.height - Shift.height));
        targetPoint[0][3] = (Point2f(Shift.width, Shift.height));

        /*forward*/
        targetPoint[1][0] = (Point2f(mSize.width - Shift.width, Shift.height + chessBordWidth.height + 47));
        targetPoint[1][1] = (Point2f(Shift.width, Shift.height + chessBordWidth.height + 48));
        targetPoint[1][2] = (Point2f(Shift.width, Shift.height));
        targetPoint[1][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        /*backward*/
        targetPoint[3][0] = (Point2f(Shift.width + 1,
                                     mSize.height - Shift.height - chessBordWidth.width - 46));//��200��652��
        targetPoint[3][1] = (Point2f(mSize.width - Shift.width - 1,
                                     mSize.height - Shift.height - chessBordWidth.height - 46));
        targetPoint[3][2] = (Point2f(mSize.width - Shift.width, mSize.height - Shift.height));
        targetPoint[3][3] = (Point2f(Shift.width, mSize.height - Shift.height));
        /*right*/
        targetPoint[2][0] = (Point2f(mSize.width - Shift.width - chessBordWidth.width - 20, Shift.height));
        targetPoint[2][1] = (Point2f(mSize.width - Shift.width - chessBordWidth.width - 21,
                                     mSize.height - Shift.height));
        targetPoint[2][2] = (Point2f(mSize.width - Shift.width, mSize.height - Shift.height));
        targetPoint[2][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        ///*left*/
        //targetPoint[0][0] = (Point2f(Shift.width + chessBordWidth.width, Shift.height));
        //targetPoint[0][1] = (Point2f(Shift.width + chessBordWidth.width , mSize.height - Shift.height ));
        //targetPoint[0][2] = (Point2f(Shift.width, mSize.height - Shift.height));
        //targetPoint[0][3] = (Point2f(Shift.width, Shift.height));

        ///*forward*/
        //targetPoint[1][0] = (Point2f(mSize.width - Shift.width , Shift.height + chessBordWidth.height));
        //targetPoint[1][1] = (Point2f(Shift.width, Shift.height + chessBordWidth.height));
        //targetPoint[1][2] = (Point2f(Shift.width, Shift.height));
        //targetPoint[1][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        ///*backward*/
        //targetPoint[3][0] = (Point2f(Shift.width, mSize.height - Shift.height - chessBordWidth.width));//��200��652��
        //targetPoint[3][1] = (Point2f(mSize.width - Shift.width , mSize.height - Shift.height - chessBordWidth.height ));
        //targetPoint[3][2] = (Point2f(mSize.width - Shift.width, mSize.height - Shift.height));
        //targetPoint[3][3] = (Point2f(Shift.width, mSize.height - Shift.height));
        ///*right*/
        //targetPoint[2][0] = (Point2f(mSize.width - Shift.width - chessBordWidth.width, Shift.height ));
        //targetPoint[2][1] = (Point2f(mSize.width - Shift.width - chessBordWidth.width, mSize.height - Shift.height));
        //targetPoint[2][2] = (Point2f(mSize.width - Shift.width, mSize.height - Shift.height));
        //targetPoint[2][3] = (Point2f(mSize.width - Shift.width, Shift.height));

        //need  Shift, chessBordWidth, ShiftAdjust, mSize
        /*roi*/
        r[0] = Rect(0, 0, Shift.width + chessBordWidth.width + ShiftAdjust.width, mSize.height);     //
        r[1] = Rect(0, 0, mSize.width, Shift.height + chessBordWidth.height + ShiftAdjust.height);
        r[2] = Rect(mSize.width - Shift.width - chessBordWidth.width - ShiftAdjust.width, 0,
                    Shift.width + chessBordWidth.width + ShiftAdjust.width, mSize.height);
        r[3] = Rect(0, mSize.height - Shift.width - chessBordWidth.width - ShiftAdjust.width, mSize.width,
                    Shift.height + chessBordWidth.height + ShiftAdjust.height);


        //triangle  clockwise

        /*left&front*/
        pts1.push_back(Point(0, 125));
        pts1.push_back(Point(280, 125));
        pts1.push_back(Point(280, 275));
        pts1.push_back(Point(0, 275));
        //pts1.push_back(Point(290, 288));
        //pts1.push_back(Point(0, 288));
        contour1.push_back(pts1);
        drawContours(roi1, contour1, 0, Scalar::all(255), -1);
        /*right&front*/
        pts2.push_back(Point(470, 290));
        pts2.push_back(Point(760, 100));
        pts2.push_back(Point(760, 290));
        //pts2.push_back(Point(760, 290));
        //pts2.push_back(Point(468, 290));
        contour2.push_back(pts2);
        drawContours(roi2, contour2, 0, Scalar::all(255), -1);
        /*right&rear*/
        pts3.push_back(Point(470, 600));
        pts3.push_back(Point(760, 600));
        pts3.push_back(Point(760, 800));
        //pts3.push_back(Point(600, 902));
        //pts3.push_back(Point(475, 627));
        contour3.push_back(pts3);
        drawContours(roi3, contour3, 0, Scalar::all(255), -1);
        /*left&rear*/
        pts4.push_back(Point(0, 600));
        pts4.push_back(Point(290, 600));
        pts4.push_back(Point(0, 800));
        //pts4.push_back(Point(160,902));
        //pts4.push_back(Point(0, 902));
        contour4.push_back(pts4);
        drawContours(roi4, contour4, 0, Scalar::all(255), -1);

        maskF = Mat(r[1].size(), CV_8UC1, Scalar(1));
        maskB = Mat(r[1].size(), CV_8UC1, Scalar(1));
        /*make mask, need r */
        vector<vector<Point> > maskVec;
        /*forward*/
        maskVec.push_back(vector<Point>());
        maskVec[0].push_back(Point(0, r[1].height));
        maskVec[0].push_back(Point(0, r[1].height - maskHeigth));
        maskVec[0].push_back(Point(r[0].width, r[1].height));
        maskVec.push_back(vector<Point>());
        maskVec[1].push_back(Point(r[1].width, r[1].height));
        maskVec[1].push_back(Point(r[1].width, r[1].height - maskHeigth));
        maskVec[1].push_back(Point(r[1].width - r[2].width, r[1].height));
        /*backward*/
        maskVec.push_back(vector<Point>());
        maskVec[2].push_back(Point(0, 0));
        maskVec[2].push_back(Point(0, maskHeigth));
        maskVec[2].push_back(Point(r[0].width, 0));
        maskVec.push_back(vector<Point>());
        maskVec[3].push_back(Point(mSize.width, 0));
        maskVec[3].push_back(Point(mSize.width, maskHeigth));
        maskVec[3].push_back(Point(mSize.width - r[2].width, 0));
        /*draw  mask*/

        /* drawContours(maskF, maskVec, 0, Scalar(0), CV_FILLED);
         drawContours(maskF, maskVec, 1, Scalar(0), CV_FILLED);
         drawContours(maskB, maskVec, 2, Scalar(0), CV_FILLED);
         drawContours(maskB, maskVec, 3, Scalar(0), CV_FILLED);*/


        for (size_t i = 0; i < 4; i++) {
            Birdtransform[i] = getPerspectiveTransform(sourcePoint[i], targetPoint[i]);
        }

        if (carPic.data) {
            Size newCarSize = Size(carSize.width - 2 * ShiftAdjust.width, carSize.height - ShiftAdjust.height - 7);
            resize(carPic, carPicTmp, newCarSize, CV_INTER_CUBIC);
            /*carPicPos = Rect(Shift.width+chessBordWidth.width+ShiftAdjust.width,
                             Shift.height +chessBordWidth.height,
                             newCarSize.width,newCarSize.height);*/
            carPicPos = Rect(296,
                             283,
                             newCarSize.width, newCarSize.height);

        }
        ParamSet_OK = true;
    }
}
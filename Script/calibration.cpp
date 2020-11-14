//
// Created by jyp on 2020/11/3.
//
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

using namespace std;
using namespace cv;

string root = "/home/jyp/AutoParking/Script/";

int main()
{

    ofstream fout("/home/jyp/AutoParking/Script/test.txt");
    /*FileStorage fs1("camera0_mapx.xml", cv::FileStorage::WRITE);
    FileStorage fs2("camera3_mapy.xml", cv::FileStorage::WRITE);*/
    cout << "start calibration ..." << endl;
    int image_count = 11; // pic count
    Size board_size = Size(11, 8); //
    //int x_expand = 256, y_expand = 144;

    vector<Point2f> corners;
    vector<vector<Point2f>>  corners_Seq;
    vector<Mat>  image_Seq;
    int successImageNum = 0;
    bool conner_flag = true;
    for (int i = 0; i != image_count; i++)
    {
        cout << "img " << i + 1  << endl;
        string imageFileName ;
        std::stringstream StrStm;
        StrStm << i + 1;
        StrStm >> imageFileName;
        imageFileName += ".jpg";
        imageFileName = root + imageFileName;
        cout << imageFileName << endl;
        Mat imageSrc = imread(imageFileName);
        imshow(imageFileName, imageSrc);
        Mat image;//
        //copyMakeBorder(imageSrc, image, (int)(y_expand / 2), (int)(y_expand / 2), (int)(x_expand / 2), (int)(x_expand / 2), BORDER_CONSTANT);
        Mat imageGray;

        cvtColor(imageSrc, imageGray, CV_RGB2GRAY);//
        bool patternfound = findChessboardCorners(imageSrc, board_size, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE +
                                                                                 CALIB_CB_FAST_CHECK);
        if (!patternfound)
        {
            cout << "img" << i + 1  << endl;
            conner_flag = false;
            break;
        }
        else
        {
            /*  SubPix */
            cornerSubPix(imageGray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
            Mat imageTemp = imageSrc.clone();//����ԭͼƬ���Ҹı�imageTempʱ��imageSrc����ı䡣
            for (int j = 0; j < corners.size(); j++)
            {
                circle(imageTemp, corners[j], 10, Scalar(0, 0, 255), 2, 8, 0);//�Խǵ�����ΪԲ�ģ�10Ϊ�뾶��Բ����ɫB(��)G(��)R(��)
            }
            string imageFileName;
            std::stringstream StrStm;
            StrStm << i + 1;
            StrStm >> imageFileName;
            imageFileName += "_corner.jpg";
            imageFileName = root+imageFileName;
            imwrite(imageFileName, imageTemp);
            cout << "img" << i + 1 << endl;

            successImageNum = successImageNum + 1;
            corners_Seq.push_back(corners);
        }
        image_Seq.push_back(imageSrc);
    }
    if (!conner_flag)
        return 0;
    cout << "......" << endl;
    Size square_size = Size(20, 20);
    vector<vector<Point3f>>  object_Points;

    vector<int>  point_counts;
    for (int t = 0; t < successImageNum; t++)
    {
        vector<Point3f> tempPointSet;
        for (int i = 0; i < board_size.height; i++)
        {
            for (int j = 0; j < board_size.width; j++)
            {
                Point3f tempPoint;
                tempPoint.x = i * square_size.width;
                tempPoint.y = j * square_size.height;
                tempPoint.z = 0;
                tempPointSet.push_back(tempPoint);
            }
        }
        object_Points.push_back(tempPointSet);
    }
    for (int i = 0; i < successImageNum; i++)
    {
        point_counts.push_back(board_size.width*board_size.height);
    }
    Size image_size = image_Seq[0].size();
    cv::Matx33d intrinsic_matrix;
    cv::Vec4d distortion_coeffs;
    std::vector<cv::Vec3d> rotation_vectors;
    std::vector<cv::Vec3d> translation_vectors;
    int flags = 0;
    flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flags |= cv::fisheye::CALIB_CHECK_COND;
    flags |= cv::fisheye::CALIB_FIX_SKEW;
    fisheye::calibrate(object_Points, corners_Seq, image_size, intrinsic_matrix, distortion_coeffs, rotation_vectors, translation_vectors, flags, cv::TermCriteria(3, 20, 1e-6));

    ////cout << "\nStoring Intrinsics.xml and Distortions.xml files\n\n";
    //FileStorage fs("intrinsics.xml", FileStorage::WRITE);
    //fs << "image_width" << image_size.width << "image_height" << image_size.height
    //	<< "camera_matrix" << intrinsic_matrix << "distortion_coefficients"
    //	<< distortion_coeffs;
    //fs.release();



    ///************************************************************************
    //*************************************************************************/
    cout << "..... 2 ......" << endl;
    double total_err = 0.0;
    double err = 0.0;
    vector<Point2f>  image_points2;

    for (int i = 0; i < image_count; i++)
    {
        vector<Point3f> tempPointSet = object_Points[i];
        fisheye::projectPoints(tempPointSet, image_points2, rotation_vectors[i], translation_vectors[i], intrinsic_matrix, distortion_coeffs);
        vector<Point2f> tempImagePoint = corners_Seq[i];
        Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
        Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
        for (size_t i = 0; i != tempImagePoint.size(); i++)
        {
            image_points2Mat.at<Vec2f>(0, i) = Vec2f(image_points2[i].x, image_points2[i].y);
            tempImagePointMat.at<Vec2f>(0, i) = Vec2f(tempImagePoint[i].x, tempImagePoint[i].y);
        }
        err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
        total_err += err /= point_counts[i];
        cout << "img+" << i + 1 << " error" << err << endl;
        fout << "img " << i + 1 << "   error: " << err  << endl;
    }
    cout << "ave_err " << total_err / image_count << endl;
    fout << "ave_err: " << total_err / image_count << endl;

    /************************************************************************
    rotation_matrix
    *************************************************************************/
    Mat rotation_matrix = Mat(3, 3, CV_32FC1, Scalar::all(0));

    fout <<"\nIntrinsic_Matrix" << endl;
    fout << intrinsic_matrix << endl;
    fout << "\nDistortion_Coeffs \n";
    fout << distortion_coeffs << endl;
    for (int i = 0; i < image_count; i++)
    {
        fout << "\nimg " << i + 1 << endl;
        fout << "rotation_vectors" << endl;
        fout << rotation_vectors[i] << endl;

        Rodrigues(rotation_vectors[i], rotation_matrix);
        fout << "\nrotation_matrix" << endl;
        fout << rotation_matrix << endl;
        fout << "\ntranslation_vectors"<< endl;
        fout << translation_vectors[i] << endl;
    }
    fout << endl;

    Mat mapx;
    Mat mapy;
    Mat R = Mat::eye(3, 3, CV_32F);
    Size m;
    m.width = 1280;
    m.height = 720;
    for (int i = 0; i != image_count; i++)
    {
        fisheye::initUndistortRectifyMap(intrinsic_matrix, distortion_coeffs, R, getOptimalNewCameraMatrix(intrinsic_matrix, distortion_coeffs, m, 3, m, 0), m, CV_32FC1, mapx, mapy);
        Mat t = image_Seq[i].clone();
        cv::remap(image_Seq[i], t, mapx, mapy, INTER_LINEAR);
        string imageFileName;
        std::stringstream StrStm;
        StrStm << i + 1;
        StrStm >> imageFileName;
        imageFileName += "_d.jpg";
        imageFileName = root+imageFileName;
        imwrite(imageFileName, t);
        //imshow(imageFileName, t);
        cout << "img" << i + 1  << endl;
    }
    return 0;
}
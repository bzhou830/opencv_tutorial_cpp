// ch4_hist.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <opencv.hpp>
using namespace cv;

void equalHist(Mat& img)
{
    Mat imageRGB[3];
    split(img, imageRGB);
    for (int i = 0; i < 3; i++)
    {
        equalizeHist(imageRGB[i], imageRGB[i]);
    }
    merge(imageRGB, 3, img);
}

void logTrans(Mat& img)
{
    Mat imageLog(img.size(), CV_32FC3);
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            imageLog.at<Vec3f>(i, j)[0] = log(1 + img.at<Vec3b>(i, j)[0]);
            imageLog.at<Vec3f>(i, j)[1] = log(1 + img.at<Vec3b>(i, j)[1]);
            imageLog.at<Vec3f>(i, j)[2] = log(1 + img.at<Vec3b>(i, j)[2]);
        }
    }
    //归一化到 0 ~ 255
    normalize(imageLog, imageLog, 0, 255, CV_MINMAX);
    //转换成 8bit 图像显示
    convertScaleAbs(imageLog, img);
}

float gamma(float val, float ga)
{
    return powf(val, ga);
}

void gammaTrans(Mat& img)
{
    Mat imageGamma(img.size(), CV_32FC3);
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            imageGamma.at<Vec3f>(i, j)[0] = gamma((img.at<Vec3b>(i, j)[0]), 0.0004);
            imageGamma.at<Vec3f>(i, j)[1] = gamma((img.at<Vec3b>(i, j)[1]), 0.0004);
            imageGamma.at<Vec3f>(i, j)[2] = gamma((img.at<Vec3b>(i, j)[2]), 0.0004);
        }
    }
    //归一化到0~255    
    normalize(imageGamma, imageGamma, 0, 255, CV_MINMAX);
    //转换成8bit图像显示    
    convertScaleAbs(imageGamma, img);
}


int main()
{
    Mat src = imread("c:/enhance.png");

    Mat en1 = src.clone();
    equalHist(en1);

    Mat en2 = src.clone();
    logTrans(en2);

    Mat en3 = src.clone();
    gammaTrans(en3);

    imshow("src", src);
    imshow("en1", en1);
    imshow("en2", en2);
    imshow("en3", en3);
    waitKey(0);
    return 0;
}
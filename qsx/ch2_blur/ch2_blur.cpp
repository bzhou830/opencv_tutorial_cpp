// ch2_blur.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace cv;

int main()
{
    Mat src01, src02, src03, src04;
    Mat dst01, dst02, dst03, dst04;
    src01 = imread("c:/timg.jpg");
    src02 = imread("c:/blur.png");
    src03 = imread("c:/GUASIS.png");
    src04 = imread("c:/papper.png");

    //blur
    blur(src01, dst01, Size(3, 3));

    medianBlur(src04, dst04, 3);

    GaussianBlur(src03, dst03, Size(3, 3), 0);

    bilateralFilter(src04, dst04, 5, 10, 5 / 2);

    waitKey(0);
    return 0;
    return 0;
}


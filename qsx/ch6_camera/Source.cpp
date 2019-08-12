#include <opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

void OpenVideo(std::string videopath)
{
    VideoCapture cap(videopath);
    if (!cap.isOpened())
        return;
    Mat frame;
    while (1)
    {
        cap.read(frame);
        Mat gray;
        cvtColor(frame, gray, COLOR_RGB2GRAY);
        Canny(gray, gray, 0, 255);
        imshow("video", gray);
        if (waitKey(30) >= 0) break;
    }
}

void OpenCamera(int CameraNum)
{
    VideoCapture cap(CameraNum);
    if (!cap.isOpened()) return;
    Mat edges;
    while (1)
    {
        Mat frame;
        cap >> frame;

        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

        imshow("edges", edges);
        if (waitKey(30) >= 0) break;
    }
}


int main()
{
    OpenCamera(0);
    return 0;
}
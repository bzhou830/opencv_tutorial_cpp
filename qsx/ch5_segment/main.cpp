#include <opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

Mat RegionGrow(Mat src, Point2i pt, int th)
{
    Point2i ptGrowing;                      //待生长点位置  
    int nGrowLable = 0;                             //标记是否生长过  
    int nSrcValue = 0;                              //生长起点灰度值  
    int nCurValue = 0;                              //当前生长点灰度值  
    Mat matDst = Mat::zeros(src.size(), CV_8UC1);   //创建一个空白区域，填充为黑色  
                                                    //生长方向顺序数据  
    int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
    vector<Point2i> vcGrowPt;                     //生长点栈  
    vcGrowPt.push_back(pt);                         //将生长点压入栈中  
    matDst.at<uchar>(pt.y, pt.x) = 255;               //标记生长点  
    nSrcValue = src.at<uchar>(pt.y, pt.x);            //记录生长点的灰度值  

    while (!vcGrowPt.empty())                       //生长栈不为空则生长  
    {
        pt = vcGrowPt.back();                       //取出一个生长点  
        vcGrowPt.pop_back();

        //分别对八个方向上的点进行生长  
        for (int i = 0; i<9; ++i)
        {
            ptGrowing.x = pt.x + DIR[i][0];
            ptGrowing.y = pt.y + DIR[i][1];
            //检查是否是边缘点  
            if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
                continue;

            nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);      //当前待生长点的灰度值  

            if (nGrowLable == 0)                    //如果标记点还没有被生长  
            {
                nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
                if (abs(nSrcValue - nCurValue) < th)                 //在阈值范围内则生长  
                {
                    matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;     //标记为白色  
                    vcGrowPt.push_back(ptGrowing);                  //将下一个生长点压入栈中  
                }
            }
        }
    }
    return matDst.clone();
}


void IsodataSeg(Mat &src, int n)
{
    int threshold = 0;                          //历史阈值
    int MeansO = 0;                             //前景灰度均值
    int nObject = 0;                            //实质像素点个数
    int MeansB = 0;                             //背景灰度均值
    int nBack = 0;                              //背景像素点个数
    int nCol = src.cols * src.channels();       //每行的像素个数
    while (abs(threshold - n) > 10)             //迭代停止条件
    {
        threshold = n;
        for (int i = 0; i<src.rows; ++i)
        {
            uchar* pData = src.ptr<uchar>(i);
            for (int j = 0; j<nCol; ++j)
            {
                if (pData[j] < threshold)   //背景
                {
                    ++nBack;
                    MeansB += pData[j];
                }
                else                        //物体
                {
                    ++nObject;
                    MeansO += pData[j];
                }
            }
        }
        if (nBack == 0 || nObject == 0)     //防止出现除以0的计算
            continue;
        n = (MeansB / nBack + MeansO / nObject) / 2;
    }
    cv::threshold(src, src, n, 255, 0);     //进行阈值分割
}

Mat OstuSeg(Mat src)
{
    int tbHist[256] = { 0 };                      //直方图数组
    double average = 0.0;                       //平均像素值
    double cov = 0.0;                           //方差
    double maxcov = 0.0;                        //方差最大值
    int index = 0;                              //分割像素值
    Mat dst;
    int nCol = src.cols * src.channels();       //每行的像素个数
    for (int i = 0; i < src.rows; i++)
    {
        uchar* pData = src.ptr<uchar>(i);
        for (int j = 0; j < nCol; ++j)
        {
            tbHist[pData[j]] += 1;
        }
    }

    int sum = 0;
    for (int i = 0; i<256; ++i)
        sum += tbHist[i];

    double w0 = 0.0, w1 = 0.0, u0 = 0.0, u1 = 0.0;
    int count0 = 0;
    for (int i = 0; i < 255; ++i)
    {
        u0 = 0;
        count0 = 0;
        for (int j = 0; j <= i; ++j)
        {
            u0 += j * tbHist[j];
            count0 += tbHist[j];
        }
        u0 = u0 / count0;
        w0 = (float)count0 / sum;

        u1 = 0;
        for (int j = i + 1; j<256; ++j)
            u1 += j*tbHist[j];

        u1 = u1 / (sum - count0);
        w1 = 1 - w0;
        cov = w0*w1*(u1 - u0)*(u1 - u0);
        if (cov > maxcov)
        {
            maxcov = cov;
            index = i;
        }
    }
    cv::threshold(src, dst, index, 255, 0);    //进行阈值分割
    return dst.clone();
}


int main()
{
    Mat src;
    src = imread("C:/papper.png");
    cvtColor(src, src, COLOR_RGB2GRAY);
    ////Mat region = RegionGrow(src, Point2i(130,130), 20);  
    //Mat dst = OstuSeg(src);
    //
    ////imshow("", region);
    Mat dst1, dst2;

    //Sobel(src, dst1, CV_8UC1, 1, 0);
    //Sobel(src, dst2, CV_8UC1, 0, 1);
    Laplacian(src, dst2, CV_8UC1, 3);
    Canny(src, dst1, 0, 255);

    waitKey(0);
    return 0;
}
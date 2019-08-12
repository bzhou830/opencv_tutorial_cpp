#include <opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;

Mat RegionGrow(Mat src, Point2i pt, int th)
{
    Point2i ptGrowing;                      //��������λ��  
    int nGrowLable = 0;                             //����Ƿ�������  
    int nSrcValue = 0;                              //�������Ҷ�ֵ  
    int nCurValue = 0;                              //��ǰ������Ҷ�ֵ  
    Mat matDst = Mat::zeros(src.size(), CV_8UC1);   //����һ���հ��������Ϊ��ɫ  
                                                    //��������˳������  
    int DIR[8][2] = { { -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },{ 1,1 },{ 0,1 },{ -1,1 },{ -1,0 } };
    vector<Point2i> vcGrowPt;                     //������ջ  
    vcGrowPt.push_back(pt);                         //��������ѹ��ջ��  
    matDst.at<uchar>(pt.y, pt.x) = 255;               //���������  
    nSrcValue = src.at<uchar>(pt.y, pt.x);            //��¼������ĻҶ�ֵ  

    while (!vcGrowPt.empty())                       //����ջ��Ϊ��������  
    {
        pt = vcGrowPt.back();                       //ȡ��һ��������  
        vcGrowPt.pop_back();

        //�ֱ�԰˸������ϵĵ��������  
        for (int i = 0; i<9; ++i)
        {
            ptGrowing.x = pt.x + DIR[i][0];
            ptGrowing.y = pt.y + DIR[i][1];
            //����Ƿ��Ǳ�Ե��  
            if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
                continue;

            nGrowLable = matDst.at<uchar>(ptGrowing.y, ptGrowing.x);      //��ǰ��������ĻҶ�ֵ  

            if (nGrowLable == 0)                    //�����ǵ㻹û�б�����  
            {
                nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
                if (abs(nSrcValue - nCurValue) < th)                 //����ֵ��Χ��������  
                {
                    matDst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;     //���Ϊ��ɫ  
                    vcGrowPt.push_back(ptGrowing);                  //����һ��������ѹ��ջ��  
                }
            }
        }
    }
    return matDst.clone();
}


void IsodataSeg(Mat &src, int n)
{
    int threshold = 0;                          //��ʷ��ֵ
    int MeansO = 0;                             //ǰ���ҶȾ�ֵ
    int nObject = 0;                            //ʵ�����ص����
    int MeansB = 0;                             //�����ҶȾ�ֵ
    int nBack = 0;                              //�������ص����
    int nCol = src.cols * src.channels();       //ÿ�е����ظ���
    while (abs(threshold - n) > 10)             //����ֹͣ����
    {
        threshold = n;
        for (int i = 0; i<src.rows; ++i)
        {
            uchar* pData = src.ptr<uchar>(i);
            for (int j = 0; j<nCol; ++j)
            {
                if (pData[j] < threshold)   //����
                {
                    ++nBack;
                    MeansB += pData[j];
                }
                else                        //����
                {
                    ++nObject;
                    MeansO += pData[j];
                }
            }
        }
        if (nBack == 0 || nObject == 0)     //��ֹ���ֳ���0�ļ���
            continue;
        n = (MeansB / nBack + MeansO / nObject) / 2;
    }
    cv::threshold(src, src, n, 255, 0);     //������ֵ�ָ�
}

Mat OstuSeg(Mat src)
{
    int tbHist[256] = { 0 };                      //ֱ��ͼ����
    double average = 0.0;                       //ƽ������ֵ
    double cov = 0.0;                           //����
    double maxcov = 0.0;                        //�������ֵ
    int index = 0;                              //�ָ�����ֵ
    Mat dst;
    int nCol = src.cols * src.channels();       //ÿ�е����ظ���
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
    cv::threshold(src, dst, index, 255, 0);    //������ֵ�ָ�
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
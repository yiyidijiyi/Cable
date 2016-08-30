/*
* �������ڣ�2016-08-16
* ����޸ģ�2016-08-29
* ��    �ߣ�syf
* ��    ����
*/
#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QString>
#include <QTextCodec>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <windows.h>

using namespace cv;

class Image : public QObject
{
	Q_OBJECT

public:
	enum format{
		Format_GRAYSCALE,
		Format_COLOR
	};
	
	Image(QObject *parent = 0);
	~Image();

	BOOL LoadImage(const QString& path, int flag = 1);
	BOOL LoadImage(Mat& image);
	void DrawLineInMask();
	void CreateMask1(Point p1, Point p2);
	void CreateMask2(Point p1, Point p2);
	Mat& GetMask(int index);
	int GetAvg();
	int GetAvgRoi();
	double GetNoZero(Mat& m);
	double GetDiff(Vec3b vec);

	Mat& DrawMask(Point p1, Point p2);
	Mat& GetImage();
	Mat& GetSrc();
	Mat& GetDst();
	Mat& GetRoi(int x, int y, int size);

	Mat& Threshold(int thresh);
	Mat& AdaptiveThreshold(int method, int size);
	Mat& ThreshFilter(int thresh, int size);
	void Preprocessing(void);
	Mat& ColorFilter(int channel, int thresh);
	Mat& MaskProcess(double s, int gap, int y, vector<int>& vec1, vector<int>& vec2);
	Mat& MaskProcess(double s, int gap, int y, vector<int>& vec);

	int FindCable(void);
	Mat& Dilate(int size);
	Mat& Erode(int size);

	void SegmentPoints(vector<Point>& seg, Point p1, Point p2);
	Mat& Calc(int y0, double s, int gap, vector<int>& vec1, vector<int>& vec2);
	Mat& Calc(int y0, double s, int gap, vector<int>& vec);
private:
	Mat m_imageSrc;
	Mat m_image;
	Mat m_imageDst;
	Mat m_imageRoi;
	Mat m_mask1;
	Mat m_mask2;

	int m_mask1Pixs;		// ��Ĥ������ֵΪ255���ص�ĸ���
	int m_roi1Height;		// ͼ���и���Ȥ������ʼy����
	int m_mask2Pixs;
	int m_roi2Height;
};

#endif // IMAGE_H

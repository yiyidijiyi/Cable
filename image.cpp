/*
* 创建日期：2016-08-16
* 最后修改：2016-09-02
* 作    者：syf
* 描    述：
*/
#include "image.h"


/*
* 参数：
* 返回：
* 功能：构造函数
*/
Image::Image(QObject *parent)
	: QObject(parent),
	m_mask1Pixs(0),
	m_mask2Pixs(0),
	m_roi1Height(0),
	m_roi2Height(0),
	m_k1(0.61),
	m_k2(-0.129),
	m_pixsOfCable(100)
{
	//DrawLineInMask();
}


/*
* 参数：
* 返回：
* 功能：析构函数
*/
Image::~Image()
{

}



/*
* 参数：path--图像文件路径
* 返回：0--图像打开失败
*       1--图像打开成功
* 功能：根据path指定的路径和文件名，打开图像
*/
BOOL Image::LoadImage(const QString& path, int flag)
{
	BOOL state = FALSE;

	// 用于支持中文路径
	QTextCodec* codec = QTextCodec::codecForName("gb18030");
	string fileName = codec->fromUnicode(path).data();

	m_imageSrc.release();
	m_image.release();

	if (Image::Format_GRAYSCALE == flag)
	{
		m_imageSrc = imread(fileName, IMREAD_GRAYSCALE);
	}
	else if (Image::Format_COLOR == flag)
	{
		
		m_imageSrc = imread(fileName, IMREAD_COLOR);
	}
	else
	{
		return state;
	}

	if (m_imageSrc.data)
	{
		m_image.release();
		m_imageSrc.copyTo(m_image);
		state = TRUE;
	}

	return state;	
}


/*
* 参数：image--Mat类型的图像
* 返回：0--图像打开失败
*       1--图像打开成功
* 功能：复制Mat类型的图像
*/
BOOL Image::LoadImage(Mat& image)
{
	BOOL state = FALSE;

	if (image.data)
	{
		m_imageSrc.release();
		m_image.release();
		image.copyTo(m_imageSrc);
		image.copyTo(m_image);

		state = TRUE;
	}

	return state;
}


/*
* 参数：
* 返回：
* 功能：设置斜率1
*/
void Image::SetK1(double k)
{
	m_k1 = k;
}


/*
* 参数：
* 返回：
* 功能：设置斜率2
*/
void Image::SetK2(double k)
{
	m_k2 = k;
}


/*
* 参数：
* 返回：
* 功能：设置光缆宽度所占像素
*/
void Image::SetPixsOfCable(int pixs)
{
	m_pixsOfCable = pixs;
}


/*
* 参数：
* 返回：
* 功能：返回光缆宽度所占像素
*/
int Image::GetPixsOfCable()
{
	return m_pixsOfCable;
}


/*
* 参数：
* 返回：
* 功能：返回已打开图像Mat的引用
*/
Mat& Image::GetImage()
{
	return m_image;
}


/*
* 参数：
* 返回：
* 功能：
*/
Mat& Image::GetSrc()
{
	return m_imageSrc;
}


/*
* 参数：
* 返回：
* 功能：
*/
Mat& Image::GetDst()
{
	return m_imageDst;
}


/*
* 参数：x-列坐标，y-行坐标，size-窗口大小
* 返回：Roi区域
* 功能：从打开的图像中获取指定位置，指定大小的Roi区域
*/
Mat& Image::GetRoi(int x, int y, int size)
{
	if (m_image.data)
	{
		if (x > m_image.cols - 1)
		{
			x = m_image.cols - 1;
		}

		if (y > m_image.rows - 1)
		{
			y = m_image.rows - 1;
		}

		if (x + size > m_image.cols - 1)
		{
			size = 1;
		}

		if (y + size > m_image.rows - 1)
		{
			size = 1;
		}
		m_imageRoi = m_image(Rect(x, y, size, size));
	}

	return m_imageRoi;
}


/*
* 参数：阈值
* 返回：
* 功能：固定阈值，图像二值化
*/
Mat& Image::Threshold(int thresh)
{
	int channel = m_image.channels();
	Mat gray;
	if (m_image.data)
	{
		if (3 == channel)
		{
			cvtColor(m_image, gray, CV_BGR2GRAY);
			threshold(gray, m_imageDst, thresh, 255, CV_THRESH_BINARY);
		}
		else
		{
			threshold(m_image, m_imageDst, thresh, 255, CV_THRESH_BINARY);
		}
		

		return m_imageDst;
	}
	else
	{
		return m_image;
	}	
}


/*
* 参数：自适应方法
* 返回：
* 功能：自适应方法图像二值化
*/
Mat& Image::AdaptiveThreshold(int method, int size)
{
	int channel = m_image.channels();
	Mat gray;
	if (m_image.data)
	{
		if (3 == channel)
		{
			cvtColor(m_image, gray, CV_BGR2GRAY);
			adaptiveThreshold(gray, m_imageDst, 255, method, THRESH_BINARY, size, -1.2);
		}
		else
		{
			adaptiveThreshold(m_image, m_imageDst, 255, method, THRESH_BINARY, size, -1.2);
		}


		return m_imageDst;
	}
	else
	{
		return m_image;
	}
}

/*
* 参数：
* 返回：
* 功能：目标识别前，图像预处理
*/
void Image::Preprocessing(void)
{
	if (m_imageSrc.data)
	{
		m_image.release();

		GaussianBlur(m_imageSrc, m_image, Size(5, 5), 0, 0);
		blur(m_image, m_image, Size(5, 5));
	}
}


/*
* 参数：颜色通道选择
*       0--R,红色通道
*       1--G,绿色通道
*       2--B,蓝色通道
*       3--W,白色部分
* 返回：颜色滤波后的图像
* 功能：选择R,G,B通道的一种，分离出颜色成分最多的一个通道，其它数值都置0
*/
Mat& Image::ColorFilter(int index, int thresh)
{
	int channel = m_image.channels();
	int value = 0;
	double diffThresh = 0.6;
	// 0--黑色，1--白色， 2--红色成本占有，3--绿色成本占优，4--蓝色成分占优
	//int color = 0;

	if ((m_image.data) && (3 == channel))
	{
		int w = m_image.cols;
		int h = m_image.rows;

		m_imageDst.release();
		m_imageDst.create(h, w, CV_8UC1);

		for (int i = 0; i < h; i++)
		{
			uchar* data = m_image.ptr<uchar>(i);
			uchar* dst = m_imageDst.ptr<uchar>(i);

			for (int j = 0; j < w; j++)
			{
				int k = j * channel;
				uchar b = data[k];
				uchar g = data[k + 1];
				uchar r = data[k + 2];
				int n = cvRound(b * 0.114 + g * 0.587 + r * 0.299);
				double dif = GetDiff(Vec3b(b, g, r));

				switch (index)
				{
				case 0:
					if (((3 * r) < thresh) || (r < g) || (r < b) || (dif < diffThresh) || (n >= 3 * thresh))
					{
						*(dst + j) = value;
					}
					else
					{
						*(dst + j) = 255;
					}
					break;
				case 1:
					if (((3 * g) < thresh) || (g < r) || (g < b) || (dif < diffThresh) || (n >= 3 * thresh))
					{
						*(dst + j) = value;
					}
					else
					{
						*(dst + j) = 255;
					}
					break;					
				case 2:
					if (((3 * b) < thresh) || (b < g) || (b < r) || (dif < diffThresh) || (n >= 3 * thresh))
					{
						*(dst + j) = value;
					}
					else
					{
						*(dst + j) = 255;
					}
					break;
				case 3:
					if ((n < cvRound(thresh * 0.5)) || (dif > 0.4))
					{
						*(dst + j) = value;
					}
					else
					{
						*(dst + j) = 255;
					}
					break;
				default:
					break;
				}
			}
		}

		//cvtColor(m_imageDst, m_imageDst, CV_BGR2GRAY);
		//Canny(m_imageDst, m_imageDst, 50, 200, 3);

		//vector<Vec2f> lines;
		//HoughLines(m_imageDst, lines, 1, CV_PI / 360, 150, 0, 0);
		//
		//for (size_t i = 0; i < lines.size(); i++)
		//{
		//	
		//	float theta = lines[i][1];

		//	if (abs(theta - (CV_PI / 3.0)) < CV_PI / 360)
		//	{
		//		float rho = lines[i][0];
		//		Point pt1, pt2;

		//		double a = cos(theta);
		//		double b = sin(theta);
		//		double x0 = a * rho;
		//		double y0 = b * rho;

		//		pt1.x = cvRound(x0 + 3000 * (-b));
		//		pt1.y = cvRound(y0 + 3000 * (a));
		//		pt2.x = cvRound(x0 - 3000 * (-b));
		//		pt2.y = cvRound(y0 - 3000 * (a));

		//		line(m_imageDst, pt1, pt2, Scalar(255), 1, CV_AA);
		//	}

		//vector<Vec4i> lines;
		//HoughLinesP(m_imageDst, lines, 1, CV_PI / 360, 50,  20, 5);

		//for (size_t i = 0; i < lines.size(); i++)
		//{
		//	Vec4i l = lines[i];
		//	double w = abs(l[0] - l[2]);
		//	double h = abs(l[1] - l[3]);
		//	double t = h / w;
		//	if (abs(t - 0.5) < 0.05)
		//	{
		//		line(m_imageDst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(128), 1, CV_AA);
		//	}			
		//}

		return m_imageDst;
	}
	else
	{
		return m_image;
	}
}


/*
* 参数：
* 返回：图像的平均灰度值
* 功能：返回原图ROI区域的平均灰度值，如果是彩色图，先转换为灰度图再计算
*/
int Image::GetAvgRoi()
{
	IplImage* img;
	Mat roi = m_image(Rect(0, 880, m_image.cols, 140));
	Mat gray;

	if (3 == m_image.channels())
	{
		cvtColor(roi, gray, CV_BGR2GRAY);
		img = &IplImage(gray);
		
	}
	else
	{
		img = &IplImage(roi);
	}

	CvScalar avg = cvAvg(img);

	return static_cast<int>(avg.val[0]);
}


/*
* 参数：
* 返回：图像的平均灰度值
* 功能：返回原图的平均灰度值，如果是彩色图，先转换为灰度图再计算
*/
int Image::GetAvg(int y0)
{
	int avg = 0;	
	
	/*if (m_imageSrc.data)
	{		
		int y0 = FindCable();
		int h0 = 100;

		Mat gray;
		Mat roi = m_imageSrc(Rect(0, y0 - h0, m_imageSrc.cols, 2 * h0));
		IplImage* img;

		if (3 == m_imageSrc.channels())
		{
			cvtColor(roi, gray, CV_BGR2GRAY);
			img = &IplImage(gray);
		}
		else
		{
			img = &IplImage(gray);
		}

		CvScalar avgScalar = cvAvg(img);

		avg = cvRound(avgScalar.val[0]);
	}*/

	if (m_image.data)
	{
		Mat gray;
		int w = m_image.cols;
		//int h = m_image.rows;
		int h0 = m_pixsOfCable;
		int pixCnt = 0;
		int pixSum = 0;
		int thresh = 0;

		if(3 == m_image.channels())
		{
			cvtColor(m_image, gray, CV_BGR2GRAY);
		}
		else
		{
			gray = m_image;
		}

		uchar* data = gray.ptr<uchar>(10);

		for (int k = 0; k < w; k++)
		{
			pixSum += data[k];
		}

		thresh = cvRound(1.2 * pixSum / w);

		pixSum = 0;

		for (int i = y0 - h0; i < y0 + h0; i++)
		{
			data = gray.ptr<uchar>(i);

			for (int j = 0; j < w; j++)
			{
				uchar val = data[j];
				if (val >= thresh)
				{
					pixCnt++;
					pixSum += val;
				}
			}
		}

		avg = pixSum / pixCnt;
	}

	return avg;
}


/*
* 参数：
* 返回：
* 功能：
*/
double Image::GetDiff(Vec3b vec)
{
	double avg = (vec[0] + vec[1] + vec[2]) / 3.0;

	double dif = abs(vec[0] - avg) + abs(vec[1] - avg) + abs(vec[2] - avg);

	return (dif / avg);
}


/*
* 参数：
* 返回：
* 功能：
*/
Mat& Image::GetMask(int index)
{
	if (1 == index)
	{
		return m_mask1;
	}
	else
	{
		return m_mask2;
	}
	
}


/*
* 参数：
* 返回：
* 功能：手动绘制直线测试
*/
void Image::DrawLineInMask()
{
	m_mask1.release();
	m_mask1.create(200, 400, CV_8UC1);
	m_mask1 = Scalar(0);

	int x0 = 0, y0 = 199, x1 = 399, y1 = 0;
	int incx = 0, incy = 0;
	int distance = 0;
	int xerr = 0, yerr = 0;
	int deltaX = 0, deltaY = 0;
	int x = x0, y = y0;

	deltaX = x1 - x0;
	deltaY = y1 - y0;

	if (deltaX > 0)
	{
		incx = 1;
	}
	else if (deltaX < 0)
	{
		incx = -1;
		deltaX = -deltaX;
	}
	else
	{
		incx = 0;
	}

	if (deltaY > 0)
	{
		incy = 1;
	}
	else if (deltaY < 0)
	{
		incy = -1;
		deltaY = -deltaY;
	}
	else
	{
		incy = 0;
	}

	if (deltaX > deltaY)
	{
		distance = deltaX;
	}
	else
	{
		distance = deltaY;
	}

	for (int k = 0; k < distance + 1; k++)
	{
		if ((x < 400) && (y < 200))
		{
			uchar* data = m_mask1.ptr<uchar>(y);
			*(data + x) = 255;
		}

		xerr += deltaX;
		yerr += deltaY;

		if (xerr > distance)
		{
			xerr -= distance;
			x += incx;
		}

		if (yerr > distance)
		{
			yerr -= distance;
			y += incy;
		}
	}

	imshow("mask", m_mask1);
}


/*
* 参数：mask--需要生成的模板的引用
*       p1--左下坐标
*       p2--右上坐标
*       index--模板索引
* 返回：
* 功能：创建模板
*/
void Image::CreateMask1(Point p1, Point p2)
{
	int cols = abs(p1.x - p2.x);
	int rows = abs(p1.y - p2.y);

	int& roiHeight = m_roi1Height;
	int& pixs = m_mask1Pixs;
	Mat& mask = m_mask1;

	mask.release();
	mask.create(rows, cols, CV_8UC1);
	mask = Scalar(0);


	cv::line(mask, Point(0, rows - 1), Point(cols - 1, 0), Scalar(255), 1, CV_AA);

	roiHeight = p1.y < p2.y ? p1.y : p2.y;
	pixs = 0;

	for (int i = 0; i < rows; i++)
	{
		uchar* data = mask.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			if (data[j])
			{
				pixs++;
			}
		}
	}

	imshow("mask", mask);
}


/*
* 参数：mask--需要生成的模板的引用
*       p1--左下坐标
*       p2--右上坐标
*       index--模板索引
* 返回：
* 功能：创建模板
*/
void Image::CreateMask2(Point p1, Point p2)
{
	int cols = abs(p1.x - p2.x);
	int rows = abs(p1.y - p2.y);

	int& roiHeight = m_roi2Height;
	int& pixs = m_mask2Pixs;
	Mat& mask = m_mask2;

	mask.release();
	mask.create(rows, cols, CV_8UC1);
	mask = Scalar(0);


	cv::line(mask, Point(0, rows - 1), Point(cols - 1, 0), Scalar(255), 1, CV_AA);

	roiHeight = p1.y < p2.y ? p1.y : p2.y;
	pixs = 0;

	for (int i = 0; i < rows; i++)
	{
		uchar* data = mask.ptr<uchar>(i);
		for (int j = 0; j < cols; j++)
		{
			if (data[j])
			{
				pixs++;
			}
		}
	}

	imshow("mask", mask);
}


/*
* 参数：s--模板与实际直线像素点关系比例因子
*       gap--直线间距
*       vector--图像中丝带的截距与宽度
* 返回：
* 功能：
*/
Mat& Image::MaskProcess(double s, int gap, int y, vector<int>& vec1, vector<int>& vec2)
{
	vec1.clear();
	vec2.clear();
	Mat image;

	if (m_imageDst.data)
	{
		if (3 == m_imageDst.channels())
		{
			cvtColor(m_imageDst, image, CV_BGR2GRAY);
		}
		else
		{
			image = m_imageDst;
		}

		int w0 = m_mask1.cols;
		int h0 = m_mask1.rows;

		int w = image.cols;
		uchar* data = image.ptr<uchar>(y);
		int i = 0;
		int cnt = 0;


		while (i < w)
		{
			cnt = 0;

			for (int n = 0; n < h0; n++)
			{
				uchar* data1 = m_mask1.ptr<uchar>(n);
				for (int m = 0; m < w0; m++)
				{
					uchar p1 = *(data + w * n + i + m);
					uchar p2 = *(data1 + m);

					if ((p1 > 0) && (p2 > 0))
					{
						cnt++;
					}
				}
			}

			if (cnt > static_cast<int>(m_mask1Pixs * s))
			{
				cv::line(m_image, Point(i, y + h0), Point(i + w0, y), Scalar(0, 0, 255), 1, CV_AA);
				vec1.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		i = w;
		while (i > 0)
		{
			cnt = 0;

			for (int n = 0; n < h0; n++)
			{
				uchar* data1 = m_mask1.ptr<uchar>(n);
				for (int m = 0; m < w0; m++)
				{
					uchar p1 = *(data + w * n + i + m);
					uchar p2 = *(data1 + m);

					if ((p1 > 0) && (p2 > 0))
					{
						cnt++;
					}
				}
			}

			if (cnt > static_cast<int>(m_mask1Pixs * s))
			{
				cv::line(m_image, Point(i, y + h0), Point(i + w0, y), Scalar(0, 0, 255), 1, CV_AA);
				vec2.push_back(i);
				i -= gap;
			}
			else
			{
				i--;
			}
		}
	}

	return m_image;
}

/*
* 参数：s--模板与实际直线像素点关系比例因子
*       gap--直线间距
*       vector--图像中丝带的截距与宽度
* 返回：
* 功能：
*/
Mat& Image::MaskProcess(double s, int gap, int y, vector<int>& vec)
{
	vec.clear();
	Mat image;

	if (m_imageDst.data)
	{
		if (3 == m_imageDst.channels())
		{
			cvtColor(m_imageDst, image, CV_BGR2GRAY);
		}
		else
		{
			image = m_imageDst;
		}

		int w0 = m_mask2.cols;
		int h0 = m_mask2.rows;

		int w = image.cols;
		uchar* data = image.ptr<uchar>(y);
		int i = 0;
		int cnt = 0;


		while (i < w)
		{
			cnt = 0;

			for (int n = 0; n < h0; n++)
			{
				uchar* data1 = m_mask2.ptr<uchar>(n);
				for (int m = 0; m < w0; m++)
				{
					uchar p1 = *(data + w * n + i + m);
					uchar p2 = *(data1 + m);

					if ((p1 > 0) && (p2 > 0))
					{
						cnt++;
					}
				}
			}

			if (cnt > static_cast<int>(m_mask2Pixs * s))
			{
				cv::line(m_image, Point(i, y + h0), Point(i + w0, y), Scalar(0, 0, 255), 2, CV_AA);
				vec.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}
	}

	return m_image;
}


/*
* 参数：
* 返回：
* 功能：返回矩阵中非零值的个数
*/
double Image::GetNoZero(Mat& m)
{
	int channel = m.channels();
	int w = m.cols * channel;
	int h = m.rows;
	double cnt = 0;

	for (int i = 0; i < h; i++)
	{
		uchar* data = m.ptr<uchar>(i);
		for (int j = 0; j < w; j += channel)
		{
			if (data[j])
			{
				cnt++;
			}
		}
	}

	return cnt;
}



/*
* 参数：thresh--阈值，size--滤波核大小
* 返回：
* 功能：自定义的滤波器，滤波框内像素值和大于阈值的保留该像素值，否则置为0
*/
Mat& Image::ThreshFilter(int thresh, int size)
{
	if (m_image.data)
	{	
		int channel = m_image.channels();
		int w = m_image.cols;
		int h = m_image.rows;
		Mat gray;

		if (3 == channel)
		{
			cvtColor(m_image, gray, CV_BGR2GRAY);
		}
		else
		{
			m_image.copyTo(gray);
		}

		thresh *= (size * size);

		for (int i = size / 2; i < h - size / 2; i++)
		{
			uchar* data = gray.ptr<uchar>(i);
			uchar* data1 = m_image.ptr<uchar>(i);

			for (int j = size / 2; j < w - size / 2; j++)
			{
				int val = *(data - w - 2 + j) + *(data - w - 1 + j) + *(data - w + j) + *(data - w + 1 + j) + *(data - w + 2 + j)
						+ *(data - 2 + j) + *(data - 1 + j) + *(data + j) + *(data + 1 + j) + *(data + 2 + j)
						+ *(data + w - 2 + j) +  *(data + w - 1 + j) + *(data + w + j) + *(data + w + 1 + j) + *(data + w + 2 + j);

				if (val < thresh)
				{
					if (3 == channel)
					{
						int k = j * channel;
						*(data1 + k)     = 0;
						*(data1 + k + 1) = 0;
						*(data1 + k + 2) = 0;
					}
					else
					{
						*(data1 + j) = 0;
					}
				}			
			}
		}
	}

	return m_image;
}


/*
* 参数：thresh--图像该兴趣部分的平均灰度值
* 返回：
* 功能：寻找光缆在图片中大致的起始位置
*/
int Image::FindCable(void)
{
	int y = 0;
	//int h0 = 105;
	int w0 = 400;

	if (m_imageSrc.data)
	{
		Mat gray;

		if (3 == m_imageSrc.channels())
		{
			cvtColor(m_imageSrc, gray, CV_BGR2GRAY);
		}
		else
		{
			gray = m_imageSrc;
		}

		int w = gray.cols;
		int h = gray.rows;
		int x = (w - w0) / 2;
		int sumMax = 0;
		int sum = 0;


		for (int i = 0; i < h; i++)
		{
			uchar* data = gray.ptr<uchar>(i);
			sum = 0;

			for (int j = x; j < x + w0; j++)
			{
				sum += *(data + j);
			}

			if (sumMax < sum)
			{
				sumMax = sum;
				y = i;
			}
		}

		//cv::line(m_imageSrc, Point(0, y + h0), Point(w - 1, y + h0), Scalar(0, 0, 255), 1, CV_AA);
		//cv::line(m_imageSrc, Point(0, y - h0), Point(w - 1, y - h0), Scalar(0, 0, 255), 1, CV_AA);
	}

	

	return y;
}


/*
* 参数：thresh--图像该兴趣部分的平均灰度值
* 返回：
* 功能：寻找光缆在图片中大致的起始位置
*/
void Image::CableRange(int y0)
{
	if (m_imageSrc.data)
	{
		int w = m_imageSrc.cols;
		//int h = m_imageSrc.rows;
		int h0 = m_pixsOfCable + 5;

		cv::line(m_imageSrc, Point(0, y0 + h0), Point(w - 1, y0 + h0), Scalar(0, 0, 255), 1, CV_AA);
		cv::line(m_imageSrc, Point(0, y0 - h0), Point(w - 1, y0 - h0), Scalar(0, 0, 255), 1, CV_AA);
	}
}



/*
* 参数：
* 返回：
* 功能：
*/
Mat& Image::Dilate(int size)
{
	if (m_imageDst.data && (1 == m_imageDst.channels()))
	{
		Mat element = getStructuringElement(MORPH_RECT, Size(size, size));
		dilate(m_imageDst, m_imageDst, element);
	}

	return m_imageDst;
}



/*
* 参数：
* 返回：
* 功能：
*/
Mat& Image::Erode(int size)
{
	if (m_imageDst.data && (1 == m_imageDst.channels()))
	{
		Mat element = getStructuringElement(MORPH_RECT, Size(size, size));
		erode(m_imageDst, m_imageDst, element);
	}

	return m_imageDst;
}


/*
* 参数：p1--起始坐标
*       p2--终点坐标
*       step--步长
* 返回：
* 功能：生成处理模板，确定图像内有效范围
*/
Mat& Image::DrawMask(Point p1, Point p2)
{
	int y1 = p1.y;
	int y2 = p2.y;

	if (m_imageSrc.data)
	{
		m_image.release();
		m_imageSrc.copyTo(m_image);;

		int w = m_image.cols;

		cv::line(m_image, Point(0, y1), Point(w - 1, y1), Scalar(0, 0, 255), 1, CV_AA);
		cv::line(m_image, Point(0, y2), Point(w - 1, y2), Scalar(0, 0, 255), 1, CV_AA);
		cv::line(m_image, p1, p2, Scalar(0, 0, 255), 1, CV_AA);
	}

	return m_image;
}


/*
* 参数：seg--绘制线段坐标点集合
*       p1--起点坐标
*       p2--终点坐标
* 返回：
* 功能:根据起点坐标p1和终点坐标p2,得到绘制的直线坐标点集合
*/
void Image::SegmentPoints(vector<Point>& seg, Point p1, Point p2)
{
	int x1 = p1.x;
	int y1 = p1.y;
	int x2 = p2.x;
	int y2 = p2.y;

	int errx = 0, erry = 0;
	int distance = 0;
	int incx = 0, incy = 0;
	int delta_x = 0, delta_y = 0;
	int x = x1, y = y1;

	seg.clear();

	delta_x = x2 - x1;
	delta_y = y2 - y1;

	if (delta_x > 0)
	{
		incx = 1;
	}
	else if (delta_x < 0)
	{
		incx = -1;
		delta_x = -delta_x;
	}
	else
	{
		incx = 0;
	}

	if (delta_y > 0)
	{
		incy = 1;
	}
	else if (delta_y < 0)
	{
		incy = -1;
		delta_y = -delta_y;
	}
	else
	{
		incy = 0;
	}

	if (delta_x > delta_y)
	{
		distance = delta_x;
	}
	else
	{
		distance = delta_y;
	}

	for (int i = 0; i < distance + 1; i++)
	{
		seg.push_back(Point(x, y));

		errx += delta_x;
		erry += delta_y;

		if (errx > distance)
		{
			x += incx;
			errx -= distance;
		}

		if (erry > distance)
		{
			y += incy;
			erry -= distance;
		}
	}
}


/*
* 参数：
* 返回：
* 功能：匹配线缆中的丝带并标注位置
*/
Mat& Image::Calc(int y0, double s, int gap, vector<int>& vec1, vector<int>& vec2)
{
	int error = 0;
	vec1.clear();
	vec2.clear();

	if (m_imageDst.data)
	{
		int h0 = m_pixsOfCable;
		double k = -0.61;
		int x1, y1, x2, y2;
		int b;
		int row = m_imageDst.rows;
		int col = m_imageDst.cols;
		int end1, end2;

		int count = 0;
		int i = 0;
		vector<Point> seg;
		size_t n = 0;

		// 从左至右匹配，第一部分
		end1 = cvRound((-h0) / k);
		i = 0;

		while (i < end1)
		{
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			x2 = 0;
			y2 = b;

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{	
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 1, CV_AA);
				vec1.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		// 从左至右匹配，第二部分
		end2 = cvRound(h0 / k + col - 1);

		while (i < end2)
		{
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 1, CV_AA);
				vec1.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		// 从左至右匹配，第三部分
		while (i < col)
		{
			b = cvRound(y0 - k * i);
			x1 = col - 1;
			y1 = cvRound(k * x1 + b);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 0, 255), 1, CV_AA);
				vec1.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		// 从右至左匹配，第一部分
		i = col - 1;

		while (i > end2)
		{
			b = cvRound(y0 - k * i);
			x1 = col - 1;
			y1 = cvRound(k * x1 + b);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 0), 1, CV_AA);
				vec2.push_back(i);
				i -= gap;
			}
			else
			{
				i--;
			}
		}

		// 从右至左匹配，第二部分
		while (i > end1)
		{
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{				
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 0), 1, CV_AA);
				vec2.push_back(i);
				i -= gap;
			}
			else
			{
				i--;
			}
		}

		// 从左至右匹配，第三部分
		while (i > 0)
		{
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			x2 = 0;
			y2 = b;

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(255, 255, 0), 1, CV_AA);
				vec2.push_back(i);
				i -= gap;
			}
			else
			{
				i--;
			}
		}
	}

	return m_imageSrc;
}


/*
* 参数：
* 返回：
* 功能：匹配特定颜色的线缆并标注位置
*/
Mat& Image::Calc(int y0, double s, int gap, vector<int>& vec)
{
	int error = 0;
	vec.clear();

	if (m_imageDst.data)
	{
		double k = -0.129;
		int h0 = m_pixsOfCable;
		int x1, y1, x2, y2;
		int b;
		int row = m_imageDst.rows;
		int col = m_imageDst.cols;
		int end1, end2;

		int count = 0;
		int i = 0;
		vector<Point> seg;
		size_t n = 0;

		// 从左至右匹配，第一部分
		end1 = cvRound((-h0) / k);
		i = 0;

		while (i < end1)
		{	
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			x2 = 0;
			y2 = b;

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 255), 1, CV_AA);
				vec.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		// 从左至右匹配，第二部分
		end2 = cvRound(h0 / k + col - 1);

		while (i < end2)
		{
			b = cvRound(y0 - k * i);
			y1 = y0 - h0;
			x1 = cvRound((y1 - b) / k);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 255), 1, CV_AA);
				vec.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}

		// 从左至右匹配，第三部分
		while (i < col)
		{
			b = cvRound(y0 - k * i);
			x1 = col - 1;
			y1 = cvRound(k * x1 + b);
			y2 = y0 + h0;
			x2 = cvRound((y2 - b) / k);

			SegmentPoints(seg, Point(x1, y1), Point(x2, y2));
			n = seg.size();
			count = 0;
			for (size_t m = 0; m < n; m++)
			{
				int x = seg[m].x;
				int y = seg[m].y;

				if ((x < col) && (y < row))
				{
					uchar* data = m_imageDst.ptr<uchar>(y);

					if (*(data + x))
					{
						count++;
					}
				}
				else
				{
					error = 1;
				}
			}

			if (count >= cvRound(s * n))
			{
				cv::line(m_imageSrc, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 255), 1, CV_AA);
				vec.push_back(i);
				i += gap;
			}
			else
			{
				i++;
			}
		}
	}

	return m_imageSrc;

}
/*
* 创建日期：2016-08-15
* 最后修改：2016-09-07
* 作    者：syf
* 描    述：
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"


/*
* 参数：
* 返回：
* 功能：构造函数
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	m_pTimer(NULL),
	m_pImage(NULL),
	m_pCamera(NULL)
{
    ui->setupUi(this);

	m_pImage = new Image();
	m_pCamera = new Camera();

	// 加载默认图片
	//m_pImage->LoadImage("./img/default.bmp", Image::Format_GRAYSCALE);
	//ShowImage(m_pImage->GetImage());

	// 窗口大小只能输入数字，（1~32）
	QValidator* windowsizeValidator = new QIntValidator(1, 32, this);
	ui->lineEdit_windowSize->setValidator(windowsizeValidator);

	// 阈值大小1~254
	QValidator* threshValidator = new QIntValidator(1, 254, this);
	ui->lineEdit_thresh->setValidator(threshValidator);

	// x坐标输入限制
	QValidator* xValidator = new QIntValidator(1, 2591, this);
	ui->lineEdit_startX->setValidator(xValidator);
	ui->lineEdit_endX->setValidator(xValidator);

	// y坐标输入限制
	QValidator* yValidator = new QIntValidator(1, 1943, this);
	ui->lineEdit_startY->setValidator(yValidator);
	ui->lineEdit_windowSize->setValidator(yValidator);

	// 步长输入限制
	ui->lineEdit_step->setValidator(windowsizeValidator);

	// 比例因子输入限制
	QValidator* scaleValidator = new QDoubleValidator(0.1, 1.0, 2, this);
	ui->lineEdit_step->setValidator(scaleValidator);

	// 间隙输入限制
	QValidator* gapValidator = new QIntValidator(10, 500, this);
	ui->lineEdit_avg->setValidator(gapValidator);

	UpdataUI();

	m_pTimer = new QTimer(this);
	m_pTimer->start(200);

	connect(ui->pushButton_openFile, &QPushButton::clicked, this, &MainWindow::OpenImage);
	connect(ui->pushButton_thresh, &QPushButton::clicked, this, &MainWindow::OnThreshClicked);
	connect(ui->pushButton_toSrc, &QPushButton::clicked, this, &MainWindow::OnToSrcClicked);
	connect(ui->pushButton_adaptive, &QPushButton::clicked, this, &MainWindow::OnAdaptiveThreshClicked);
	connect(ui->pushButton_filter, &QPushButton::clicked, this, &MainWindow::OnFilterClicked);
	connect(ui->pushButton_clear, &QPushButton::clicked, ui->textEdit, &QTextEdit::clear);
	connect(ui->pushButton_userFilter, &QPushButton::clicked, this, &MainWindow::OnUserFilterClicked);
	connect(ui->pushButton_dilate, &QPushButton::clicked, this, &MainWindow::OnDilateClicked);
	connect(ui->pushButton_erode, &QPushButton::clicked, this, &MainWindow::OnErodeClicked);
	connect(ui->pushButton_mask, &QPushButton::clicked, this, &MainWindow::OnMaskClicked);
	connect(ui->pushButton_drawMask, &QPushButton::clicked, this, &MainWindow::OnDrawMaskClicked);
	connect(ui->pushButton_up, &QPushButton::clicked, this, &MainWindow::OnUpClicked);
	connect(ui->pushButton_down, &QPushButton::clicked, this, &MainWindow::OnDownClicked);
	connect(ui->pushButton_left, &QPushButton::clicked, this, &MainWindow::OnLeftClicked);
	connect(ui->pushButton_right, &QPushButton::clicked, this, &MainWindow::OnRightClicked);
	connect(ui->pushButton_increase, &QPushButton::clicked, this, &MainWindow::OnIncreaseClicked);
	connect(ui->pushButton_cut, &QPushButton::clicked, this, &MainWindow::OnCutClicked);
	connect(ui->pushButton_increaseAngle, &QPushButton::clicked, this, &MainWindow::OnIncreaseAngleClicked);
	connect(ui->pushButton_reduceAngle, &QPushButton::clicked, this, &MainWindow::OnReduceAngleClicked);
	connect(ui->pushButton_createMask1, &QPushButton::clicked, this, &MainWindow::OnCreateMask1Clicked);
	connect(ui->pushButton_createMask2, &QPushButton::clicked, this, &MainWindow::OnCreateMask2Clicked);
	connect(ui->pushButton_setPixsOfCable, &QPushButton::clicked, this, &MainWindow::OnPixOfCableClicked);
	connect(ui->pushButton_openCamera, &QPushButton::clicked, this, &MainWindow::OnOpenCameraClicked);
	connect(ui->pushButton_closeCamera, &QPushButton::clicked, this, &MainWindow::OnCloseCameraClicked);
	connect(ui->pushButton_startSnap, &QPushButton::clicked, this, &MainWindow::OnStartSnapClicked);
	connect(ui->pushButton_stopSnap, &QPushButton::clicked, this, &MainWindow::OnStopSnapClicked);
	connect(ui->pushButton_trigger, &QPushButton::clicked, this, &MainWindow::OnSoftwareTriggerClicked);
	connect(ui->comboBox_autoExposure, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::OnExposureModeChanged);
	connect(ui->comboBox_autoWb, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::OnWhiteBalanceModeChanged);
	connect(ui->comboBox_balanceChannel, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::OnWhiteBalanceChannelChanged);
	connect(ui->lineEdit_exposure, &QLineEdit::editingFinished, this, &MainWindow::OnExporureValueChanged);
	connect(ui->lineEdit_balance, &QLineEdit::editingFinished, this, &MainWindow::OnWhiteBalanceRatioChanged);

	connect(m_pTimer, &QTimer::timeout, this, &MainWindow::OnTimer);
	connect(m_pCamera, &Camera::Signal_ImageSnaped, this, &MainWindow::OnImageSnap);
}


/*
* 参数：
* 返回：
* 功能：析构函数
*/
MainWindow::~MainWindow()
{
    delete ui;

	if (NULL != m_pTimer)
	{
		m_pTimer->stop();

		delete m_pTimer;
	}

	if (NULL != m_pImage)
	{
		delete m_pImage;
	}


	if (NULL != m_pCamera)
	{
		delete m_pCamera;
	}
}


/*
* 参数：
* 返回：
* 功能：显示选择的图片
*/
void  MainWindow::ShowImage(const Mat& image)
{
	//image = m_pCamera->GetImage();

	if (image.data)
	{
		BOOL state = TRUE;
		int channel = image.channels();
		int w = image.cols;
		int h = image.rows;
		int bytesPerLine = image.step;
		QImage qimg;

		if (1 == channel)
		{
			qimg = QImage(image.data, w, h, image.step, QImage::Format_Grayscale8);
		}
		else if (3 == channel)
		{
			qimg = QImage(image.data, w, h, bytesPerLine, QImage::Format_RGB888).rgbSwapped();
		}
		else
		{
			state = FALSE;
		}

		if (state)
		{
			ui->label_imgShow->setPixmap(QPixmap::fromImage(qimg));
			ui->label_imgShow->resize(w, h);
		}
	}
}


/*
* 参数：
* 返回：
* 功能：更新UI
*/
void MainWindow::UpdataUI()
{
	if (NULL != m_pCamera)
	{
		if (!m_pCamera->IsDeviceOpened())
		{
			ui->pushButton_openCamera->setEnabled(true);
			ui->pushButton_closeCamera->setEnabled(false);

			ui->pushButton_startSnap->setEnabled(false);
			ui->pushButton_stopSnap->setEnabled(false);
			ui->pushButton_trigger->setEnabled(false);
			ui->comboBox_snapMode->setEnabled(false);

			ui->comboBox_autoExposure->setEnabled(false);
			ui->lineEdit_exposure->setEnabled(false);

			ui->comboBox_autoWb->setEnabled(false);
			ui->lineEdit_balance->setEnabled(false);
		}
		else
		{
			ui->pushButton_openCamera->setEnabled(false);
			ui->pushButton_closeCamera->setEnabled(true);

			ui->comboBox_autoExposure->setEnabled(true);
			ui->lineEdit_exposure->setEnabled(true);

			ui->comboBox_autoWb->setEnabled(true);

			if (0 == ui->comboBox_autoWb->currentIndex())
			{
				ui->lineEdit_balance->setEnabled(true);
			}
			else
			{
				ui->lineEdit_balance->setEnabled(false);
			}

			if (!m_pCamera->IsSnap())
			{
				ui->pushButton_startSnap->setEnabled(true);
				ui->pushButton_stopSnap->setEnabled(false);
				ui->pushButton_trigger->setEnabled(false);
				ui->comboBox_snapMode->setEnabled(true);
			}
			else
			{
				if (0 == ui->comboBox_snapMode->currentIndex())
				{
					ui->pushButton_trigger->setEnabled(false);
				}
				else
				{
					ui->pushButton_trigger->setEnabled(true);
				}
				
				ui->pushButton_startSnap->setEnabled(false);
				ui->pushButton_stopSnap->setEnabled(true);
				ui->comboBox_snapMode->setEnabled(false);		
			}
		}
	}
	else
	{
		ui->pushButton_openCamera->setEnabled(true);
		ui->pushButton_closeCamera->setEnabled(false);

		ui->pushButton_startSnap->setEnabled(false);
		ui->pushButton_stopSnap->setEnabled(false);
		ui->pushButton_trigger->setEnabled(false);
		ui->comboBox_snapMode->setEnabled(false);

		ui->comboBox_autoExposure->setEnabled(false);
		ui->lineEdit_exposure->setEnabled(false);

		ui->comboBox_autoWb->setEnabled(false);
		ui->lineEdit_balance->setEnabled(false);
	}
}


/*
* 参数：
* 返回：
* 功能：
*/
void MainWindow::OnTimer()
{
	if ((TRUE == m_pCamera->IsSnap()) && (1 == ui->comboBox_autoExposure->currentIndex()))
	{
		double t = m_pCamera->GetExposure();

		ui->lineEdit_exposure->setText(QString::number(t));
	}
}


/*
* 参数：
* 返回：
* 功能：槽函数，选择图片
*/
void MainWindow::OpenImage()
{
	BOOL state = FALSE;
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择图片"), "./", "Images (*.png *.bmp *.jpg)");

	if (ui->checkBox_isGrayScale->isChecked())
	{
		state = m_pImage->LoadImage(fileName, Image::Format_GRAYSCALE);
	}
	else
	{
		state = m_pImage->LoadImage(fileName, Image::Format_COLOR);
	}
	

	if (FALSE == state)
	{
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("打开图像失败！"));
	}
	else
	{
		ShowImage(m_pImage->GetSrc());
		int y0 = m_pImage->FindCable();
		int avg = m_pImage->GetAvg(y0);
		ui->lineEdit_avg->setText(QString::number(avg));

		//int y = m_pImage->FindCable();

		//ui->textEdit->append(QString::number(y));
	}
}


/*
* 参数：鼠标事件
* 返回：
* 功能：鼠标点击事件函数重载
*/
void MainWindow::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		QPoint labelPos = ui->label_imgShow->pos();
		QPoint scrollAreaPos = ui->scrollArea->pos();
		QPoint mousePos = event->pos();

		int scrollAreaw = ui->scrollArea->width();
		int scrollAreah = ui->scrollArea->height();

		if ((mousePos.x() > labelPos.x()) && (mousePos.y() > labelPos.y()) && (mousePos.x() < labelPos.x() + scrollAreaw) && (mousePos.y() < labelPos.y() + scrollAreah))
		{
			// x方向，减去scrollArea和label的边距
			int x = mousePos.x() - labelPos.x() - scrollAreaPos.x() + ui->scrollArea->horizontalScrollBar()->value();
			// y方向，减去scrollArea和label的边距，以及toolbar和menubar的宽度
			int y = mousePos.y() - labelPos.y() - scrollAreaPos.y() + ui->scrollArea->verticalScrollBar()->value() - ui->mainToolBar->size().height() - ui->menuBar->size().height();

			if ((x >= 0) && (y >= 0))
			{
				Mat roi = m_pImage->GetRoi(x, y, ui->lineEdit_windowSize->text().toInt());
				ui->textEdit->append("x:" + QString::number(x) + "  " + "y:" + QString::number(y));

				if (roi.data)
				{
					int w = roi.cols;
					int h = roi.rows;
					int channel = roi.channels();
					QString str = "";

					for (int i = 0; i < h; i++)
					{
						uchar* data = roi.ptr<uchar>(i);
						for (int j = 0; j < w * channel; j += channel)
						{
							if (3 == channel)
							{
								str += "(" + QString::number(data[j]) + " " + QString::number(data[j + 1]) + " " + QString::number(data[j + 2]) + ")" + " ";
							}
							else
							{
								str += QString::number(data[j]) + " ";
							}
						}

						ui->textEdit->append(str);
						str = "";
					}

					ui->textEdit->append("----------");
				}
			}			
		}
	}
}



/*
* 参数：
* 返回：
* 功能：槽函数，固定阈值二值化
*/
void MainWindow::OnThreshClicked()
{
	int thresh = ui->lineEdit_thresh->text().toInt();
	
	ShowImage(m_pImage->Threshold(thresh));
}


/*
* 参数：
* 返回：
* 功能：槽函数，显示原图
*/
void MainWindow::OnToSrcClicked()
{
	ShowImage(m_pImage->GetSrc());
}


/*
* 参数：
* 返回：
* 功能：槽函数，自适应阈值法
*/
void MainWindow::OnAdaptiveThreshClicked()
{
	int index = ui->comboBox_adaptive->currentIndex();
	int size = ui->lineEdit_windowSize->text().toInt();

	if (index)
	{
		ShowImage(m_pImage->AdaptiveThreshold(ADAPTIVE_THRESH_GAUSSIAN_C, size));
	}
	else
	{
		ShowImage(m_pImage->AdaptiveThreshold(ADAPTIVE_THRESH_MEAN_C, size));
	}
}


/*
* 参数：
* 返回：
* 功能：槽函数，颜色滤波
*/
void MainWindow::OnFilterClicked()
{
	int index = ui->comboBox_filter->currentIndex();
	int thresh = ui->lineEdit_avg->text().toInt();
	m_pImage->Preprocessing();
	ShowImage(m_pImage->ColorFilter(index, thresh));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板处理
*/
void MainWindow::OnMaskClicked()
{
	double s1 = ui->lineEdit_scale1->text().toDouble();
	int gap1 = ui->lineEdit_gap1->text().toInt();
	double t = 0;
	vector<int> vec1;
	vector<int> vec2;
	vector<int> vec3;

	double s2 = ui->lineEdit_scale2->text().toDouble();
	int gap2 = ui->lineEdit_gap2->text().toInt();

	int thresh = ui->lineEdit_avg->text().toInt();
	t = static_cast<double>(getTickCount());
	int y0 = m_pImage->FindCable();
	m_pImage->Preprocessing();
	m_pImage->ColorFilter(3, thresh);
	m_pImage->Calc(y0, s1, gap1, vec1, vec2);
	m_pImage->ColorFilter(2, thresh);
	m_pImage->Calc(y0, s2, gap2, vec3);
	t = ((double)getTickCount() - t) / getTickFrequency();
	//ShowImage(m_pImage->GetSrc());
	//ShowImage(m_pImage->ColorFilter(2, thresh));

	//m_pImage->ColorFilter(3, thresh);
	//m_pImage->MaskProcess(s1, gap1, y, vec1, vec2);

	//m_pImage->ColorFilter(2, thresh);
	//m_pImage->MaskProcess(s2, gap2, y, vec);
	m_pImage->CableRange(y0);
	ShowImage(m_pImage->GetSrc());

	// 保存处理后的图像
	QString  name = QTime::currentTime().toString("h_m_s");
	QString path = "./result/" + name + ".jpg";
	vector<int> compressionParams;
	compressionParams.push_back(CV_IMWRITE_JPEG_QUALITY);
	compressionParams.push_back(95);
	imwrite(path.toStdString(), m_pImage->GetSrc(), compressionParams);

	size_t size1 = vec1.size();
	size_t size2 = vec2.size();
	size_t size3 = vec3.size();

	ui->textEdit->append(QStringLiteral("处理时间：") + QString::number(t) + "S");

	if ((size1 != size2) || (size1 < 2) || (size2 < 2))
	{
		ui->textEdit->append(QStringLiteral("识别到的丝段小于2段，无法计算截距！"));
	}
	else
	{
		int min1 = 2592; 
		int min2 = 2592;
		int max1 = 0;
		int max2 = 0;
		int avg1 = 0;
		int avg2 = 0;
		int t1, t2;

		for (size_t i = 0; i < size1; i++)
		{
			t1 = vec2[size2 - 1 - i] - vec1[i];

			if (i != size1 - 1)
			{
				t2 = vec1[i + 1] - vec1[i];
			}
			else
			{
				t2 = 0;
			}

			avg1 += t1;
			avg2 += t2;

			if (min1 > t1)
			{
				min1 = t1;
			}

			if (max1 < t1)
			{
				max1 = t1;
			}

			if ((min2 > t2) && (t2 > 0))
			{
				min2 = t2;
			}

			if (max2 < t2)
			{
				max2 = t2;
			}
		}

		avg1 = avg1 / size1;
		avg2 = avg2 / (size2 - 1);

		ui->textEdit->append(QStringLiteral("共找到：") + QString::number(size1) + QStringLiteral("段丝带。"));
		ui->textEdit->append(QStringLiteral("平均线宽：") + QString::number(avg1) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最大线宽：") + QString::number(max1) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最小线宽：") + QString::number(min1) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("平均截距：") + QString::number(avg2) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最大截距：") + QString::number(max2) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最小截距：") + QString::number(min2) + QStringLiteral("个像素点。"));
		ui->textEdit->append("    ");
	}


	ui->textEdit->append(QStringLiteral("识别到蓝色线缆") + QString::number(size3) + QStringLiteral("段。"));

	if (size3 < 2)
	{
		ui->textEdit->append(QStringLiteral("无法结算截距！"));
	}
	else
	{
		int max = vec3[1] - vec3[0];
		int min = max;
		int avg = max;
		int t;
		for (size_t j = 1; j < size3 - 1; j++)
		{
			t = vec3[j + 1] - vec3[j];

			if (t > max)
			{
				max = t;
			}

			if (t < min)
			{
				min = t;
			}

			avg += t;
		}

		avg /= size3 - 1;

		ui->textEdit->append(QStringLiteral("平均截距：") + QString::number(avg) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最大截距：") + QString::number(max) + QStringLiteral("个像素点。"));
		ui->textEdit->append(QStringLiteral("最小截距：") + QString::number(min) + QStringLiteral("个像素点。"));
	}

	ui->textEdit->append("-----------------------------------------------------------");
}


/*
* 参数：
* 返回：
* 功能：槽函数，自定义滤波
*/
void MainWindow::OnUserFilterClicked()
{
	ShowImage(m_pImage->ThreshFilter(15, 5));
}


/*
* 参数：
* 返回：
* 功能：槽函数，腐蚀
*/
void MainWindow::OnErodeClicked()
{
	int size = ui->lineEdit_windowSize->text().toInt();

	ShowImage(m_pImage->Erode(size));
}



/*
* 参数：
* 返回：
* 功能：槽函数，膨胀
*/
void MainWindow::OnDilateClicked()
{
	int size = ui->lineEdit_windowSize->text().toInt();

	ShowImage(m_pImage->Dilate(size));
}


/*
* 参数：
* 返回：
* 功能：槽函数，绘制模板
*/
void MainWindow::OnDrawMaskClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板上移
*/
void MainWindow::OnUpClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	y1 -= step;
	y2 -= step;

	if (y1 <= 0)
	{
		y1 = 1;
	}

	if (y2 <= 0)
	{
		y2 = 1;
	}

	ui->lineEdit_startY->setText(QString::number(y1));
	ui->lineEdit_endY->setText(QString::number(y2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板下移
*/
void MainWindow::OnDownClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	y1 += step;
	y2 += step;

	if (y1 >= 1944)
	{
		y1 = 1943;
	}

	if (y2 >= 1944)
	{
		y2 = 1943;
	}

	ui->lineEdit_startY->setText(QString::number(y1));
	ui->lineEdit_endY->setText(QString::number(y2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板左移
*/
void MainWindow::OnLeftClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	x1 -= step;
	x2 -= step;

	if (x1 <= 0)
	{
		x1 = 1;
	}

	if (x2 <= 0)
	{
		x2 = 1;
	}

	ui->lineEdit_startX->setText(QString::number(x1));
	ui->lineEdit_endX->setText(QString::number(x2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板右移
*/
void MainWindow::OnRightClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	x1 += step;
	x2 += step;

	if (x1 >= 2592)
	{
		x1 = 2591;
	}

	if (x2 >= 2592)
	{
		x2 = 2591;
	}

	ui->lineEdit_startX->setText(QString::number(x1));
	ui->lineEdit_endX->setText(QString::number(x2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板减小
*/
void MainWindow::OnCutClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	y1 -= step;
	y2 += step;

	if (y1 <= 0)
	{
		y1 = 1;
	}

	if (y2 >= 1944)
	{
		y2 = 1943;
	}

	ui->lineEdit_startY->setText(QString::number(y1));
	ui->lineEdit_endY->setText(QString::number(y2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，模板增大
*/
void MainWindow::OnIncreaseClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	y1 += step;
	y2 -= step;

	if (y1 >= 1944)
	{
		y1 = 1943;
	}

	if (y2 <= 0)
	{
		y2 = 1;
	}

	ui->lineEdit_startY->setText(QString::number(y1));
	ui->lineEdit_endY->setText(QString::number(y2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，角度减小
*/
void MainWindow::OnReduceAngleClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	x1 -= step;
	x2 += step;

	if (x1 <= 0)
	{
		x1 = 1;
	}

	if (x2 >= 2592)
	{
		x2 = 2591;
	}

	ui->lineEdit_startX->setText(QString::number(x1));
	ui->lineEdit_endX->setText(QString::number(x2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，角度增大
*/
void MainWindow::OnIncreaseAngleClicked()
{
	int x1 = ui->lineEdit_startX->text().toInt();
	int y1 = ui->lineEdit_startY->text().toInt();
	int x2 = ui->lineEdit_endX->text().toInt();
	int y2 = ui->lineEdit_endY->text().toInt();
	int step = ui->lineEdit_step->text().toInt();

	x1 += step;
	x2 -= step;

	if (x1 >= 2592)
	{
		x1 = 2591;
	}

	if (x2 <= 0)
	{
		x2 = 1;
	}

	ui->lineEdit_startX->setText(QString::number(x1));
	ui->lineEdit_endX->setText(QString::number(x2));

	ShowImage(m_pImage->DrawMask(Point(x1, y1), Point(x2, y2)));
}


/*
* 参数：
* 返回：
* 功能：槽函数，生成模板
*/
void MainWindow::OnCreateMask2Clicked()
{
	Point p1, p2;
	double k = 0;
	p1.x = ui->lineEdit_startX->text().toInt();
	p1.y = ui->lineEdit_startY->text().toInt();
	p2.x= ui->lineEdit_endX->text().toInt();
	p2.y= ui->lineEdit_endY->text().toInt();
	ui->lineEdit_k2->setText(QString::number(k));

	m_pImage->SetK2(k);

	//m_pImage->CreateMask2(p1, p2);
}


/*
* 参数：
* 返回：
* 功能：槽函数，设置光缆宽度所占像素
*/
void MainWindow::OnPixOfCableClicked()
{
	int pixs = ui->lineEdit_pixsOfCable->text().toInt();

	m_pImage->SetPixsOfCable(pixs);
}


/*
* 参数：
* 返回：
* 功能：槽函数，生成模板
*/
void MainWindow::OnCreateMask1Clicked()
{
	Point p1, p2;
	double k = 0;
	p1.x = ui->lineEdit_startX->text().toInt();
	p1.y = ui->lineEdit_startY->text().toInt();
	p2.x = ui->lineEdit_endX->text().toInt();
	p2.y = ui->lineEdit_endY->text().toInt();
	k = (p2.y - p1.y) / (p2.x - p1.x);

	ui->lineEdit_k1->setText(QString::number(k));

	m_pImage->SetK1(k);
	//m_pImage->CreateMask1(p1, p2);
}


/*
* 参数：
* 返回：
* 功能：槽函数，打开相机
*/
void MainWindow::OnOpenCameraClicked()
{
	BOOL state = FALSE;

	state = m_pCamera->OpenCamera();


	if (FALSE == state)
	{
		ui->textEdit->append(QStringLiteral("打开相机失败！"));
	}
	else
	{
		ui->textEdit->append(QStringLiteral("打开相机成功！"));
	}

	int index = ui->comboBox_balanceChannel->currentIndex();
	double ratio = 1.0;
	m_pCamera->GetBalanceRatio(index, &ratio);
	ui->lineEdit_balance->setText(QString::number(ratio));

	UpdataUI();
}


/*
* 参数：
* 返回：
* 功能：槽函数，关闭相机
*/
void MainWindow::OnCloseCameraClicked()
{
	m_pCamera->CloseCamera();
	ui->textEdit->append(QStringLiteral("相机关闭！"));

	UpdataUI();
}


/*
* 参数：
* 返回：
* 功能：槽函数，开始采集
*/
void MainWindow::OnStartSnapClicked()
{
	int index = ui->comboBox_snapMode->currentIndex();

	m_pCamera->StartSnap(index);

	UpdataUI();
}


/*
* 参数：
* 返回：
* 功能：槽函数，停止采集
*/
void MainWindow::OnStopSnapClicked()
{
	int index = ui->comboBox_snapMode->currentIndex();
	m_pCamera->StopSnap(index);

	UpdataUI();
}


/*
* 参数：
* 返回：
* 功能：槽函数，软件触发采集
*/
void MainWindow::OnSoftwareTriggerClicked()
{
	m_pCamera->SoftwareTrigger();
}


/*
* 参数：
* 返回：
* 功能：显示选择的图片
*/
void  MainWindow::OnImageSnap()
{
	m_pImage->LoadImage(m_pCamera->GetImage());

	int y0 = m_pImage->FindCable();
	int avg = m_pImage->GetAvg(y0);
	ui->lineEdit_level->setText(QString::number(avg));
	ui->lineEdit_avg->setText(QString::number(avg));

	if (ui->checkBox_calc->isChecked())
	{
		OnMaskClicked();
		//ShowImage(m_pImage->GetSrc());
	}
	else
	{
		ShowImage(m_pImage->GetSrc());
	}
}


/*
* 参数：
* 返回：
* 功能：曝光模式改变
*/
void MainWindow::OnExposureModeChanged()
{
	int index = ui->comboBox_autoExposure->currentIndex();
	m_pCamera->SetAutoExposure(index);

	if (1 == index)
	{
		ui->lineEdit_exposure->setEnabled(false);
	}
	else
	{
		ui->lineEdit_exposure->setEnabled(true);
	}
}


/*
* 参数：
* 返回：
* 功能：曝光值改变
*/
void MainWindow::OnExporureValueChanged()
{
	double t = ui->lineEdit_exposure->text().toDouble();

	m_pCamera->SetExposure(t);
}


/*
* 参数：
* 返回：
* 功能：白平衡模式改变
*/
void MainWindow::OnWhiteBalanceModeChanged()
{
	int index = ui->comboBox_autoWb->currentIndex();

	m_pCamera->SetAutoWhiteBalance(index);

	UpdataUI();
}


/*
* 参数：
* 返回：
* 功能：白平衡系数改变
*/
void MainWindow::OnWhiteBalanceRatioChanged()
{
	int index = ui->comboBox_balanceChannel->currentIndex();
	double ratio = ui->lineEdit_balance->text().toDouble();

	if (ratio < 0.1)
	{
		ratio = 0.1;
	}
	else if (ratio > 5.0)
	{
		ratio = 5.0;
	}

	ui->lineEdit_balance->setText(QString::number(ratio));

	m_pCamera->SetBalanceRatio(index, ratio);
}


/*
* 参数：
* 返回：
* 功能：白平衡通道改变
*/
void MainWindow::OnWhiteBalanceChannelChanged()
{
	int index = ui->comboBox_balanceChannel->currentIndex();
	double ratio = 1.0;

	m_pCamera->GetBalanceRatio(index, &ratio);

	ui->lineEdit_balance->setText(QString::number(ratio));
}
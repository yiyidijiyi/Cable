/*******************************************************************************
文件名称：camera.c
创建日期：2016-02-11
最后修改：2016-08-31
版	   本：v1.0.0
作    者：syf
功能描述：大恒相机操作类
*******************************************************************************/
#include "camera.h"


//=============================================================================
// 参数：
// 返回：
// 功能：构造函数
//=============================================================================
Camera::Camera(QWidget *parent)
	: QWidget(parent)
	, m_bIsImgEmit(false)
	, m_bDevOpened(FALSE)
	, m_bIsSnap(FALSE)
	, m_bIsImgSnaped(false)
	, m_bIsSingleSnap(false)
	, m_bIsContinuousSnap(false)
	, m_bIsColorFilter(false)
	, m_bImplementAutoGain(false)
	, m_bImplementAutoShutter(false)
	, m_bImplementLight(false)
	, m_nGray(0)
	, m_nRoiX(0)
	, m_nRoiW(0)
	, m_nRoiH(0)
	, m_nRoiY(0)
	, m_dAutoGainMin(0)
	, m_dAutoShutterMax(0)
	, m_dAutoGainMax(0)
	, m_dAutoShutterMin(0)
	, m_nImageWidth(0)
	, m_nImageHeight(0)
	, m_nPayLoadSize(0)
	, m_nPixelColorFilter(0)
	, m_pRawBuffer(NULL)
	, m_pImgBuffer(NULL)
	, m_pBmpInfo(NULL)
	, m_hDevice(NULL)
	, m_emAutoShutterMode(GX_EXPOSURE_AUTO_OFF)
	, m_emAutoGainMode(GX_GAIN_AUTO_OFF)
	, m_bEnableSaveAVI(false)
	, m_pAVIFile(NULL)
	, m_nTimeFrame(0)
	, m_psCompressed(NULL)
	, m_ps(NULL)
	, m_hic(NULL)
	, m_wPlayFPS(15)
	, m_nWrittenDataSize(0)
{
	m_dShutterValue = 0.0;
	m_strAVIFolder = "";
	m_bCompressedAVI = FALSE;
	m_bShowImage = TRUE;

	// 初始化参数
	memset(&m_stShutterFloatRange, 0, sizeof(m_stShutterFloatRange));
	memset(&m_stFrameData, 0, sizeof(GX_FRAME_DATA));
	memset(m_chBmpBuf, 0, sizeof(m_chBmpBuf));
	memset(&m_Com, 0, sizeof(m_Com));


	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}
}


//=============================================================================
// 参数：
// 返回：
// 功能：析构函数
//=============================================================================
Camera::~Camera()
{
	if (m_bDevOpened)
	{
		CloseCamera();
	}
}


//=============================================================================
// 参数：flag：false-不发送，true--发送
// 返回：
// 功能：设置发送采集到的图像标志
//=============================================================================
void Camera::SetImgEmit(bool flag)
{
	m_bIsImgEmit = flag;
}


//=============================================================================
// 参数：
// 返回：
// 功能：相机采集标志
//=============================================================================
BOOL Camera::IsSnap(void)
{
	return m_bIsSnap;
}


//=============================================================================
// 参数：
// 返回：
// 功能：相机打开状态
//=============================================================================
BOOL Camera::IsDeviceOpened(void)
{
	return m_bDevOpened;
}


//---------------------------------------------------------------------------------
/**
\brief   采集回调函数:进行图像获取和显示操作
\param   pFrame   回调参数

\return  无
*/
//----------------------------------------------------------------------------------
void __stdcall Camera::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	Camera *pCamera = (Camera*)(pFrame->pUserParam);
	VxUint32 nImageHeight = (VxUint32)pCamera->m_nImageHeight;
	VxUint32 nImageWidth = (VxUint32)pCamera->m_nImageWidth;

	if (pFrame->status == 0)
	{
		memcpy(pCamera->m_pRawBuffer, pFrame->pImgBuf, pFrame->nImgSize);

		if (!pCamera->m_bIsImgSnaped)
		{
			if (pCamera->m_bIsColorFilter)
			{
				// 彩色图像需要将Raw8图像经过RGB转换后显示
				DxRaw8toRGB24(pCamera->m_pRawBuffer, pCamera->m_pImgBuffer, nImageWidth, nImageHeight, RAW2RGB_NEIGHBOUR,
					DX_PIXEL_COLOR_FILTER(pCamera->m_nPixelColorFilter), true);
			}
			else
			{
				// 黑白相机需要翻转数据后显示
				for (VxUint32 i = 0; i < nImageHeight; i++)
				{
					memcpy(pCamera->m_pImgBuffer + i * nImageWidth, pCamera->m_pRawBuffer + (nImageHeight - i - 1) * nImageWidth, (size_t)nImageWidth);
				}
			}

			pCamera->BitmapinfoToMat(pCamera->m_pImgBuffer);

			//pCamera->m_bIsImgSnaped = true;
		}
	}
}


//----------------------------------------------------------------------------------
/**
\brief  获取设备的宽高等属性信息

\return GX_STATUS_SUCCESS:全部获取成功，其它状态码:未成功获取全部
*/
//----------------------------------------------------------------------------------
GX_STATUS Camera::GetDeviceParam()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	//	bool      bIsImplemented = false;

	// 获取图像大小
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// 获取宽度
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// 获取高度
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// 判断相机是否支持bayer格式
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);

	// 支持表示输出彩色图像
	if (m_bIsColorFilter)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
	}

	return emStatus;
}


// ---------------------------------------------------------------------------------
/**
\brief   设置相机的数据格式为8bit

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
// ----------------------------------------------------------------------------------
GX_STATUS Camera::SetPixelFormat8bit()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nPixelSize = 0;
	uint32_t  nEnmuEntry = 0;
	size_t    nBufferSize = 0;
	//	BOOL      bIs8bit = TRUE;

	GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
	//	GX_ENUM_DESCRIPTION  *pEnumTemp = NULL;

	// 获取像素点大小
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// 判断为8bit时直接返回,否则设置为8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// 获取设备支持的像素格式枚举数
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// 为获取设备支持的像素格式枚举值准备资源
		nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// 获取支持的枚举值
		emStatus = GXGetEnumDescription(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			if (pEnumDescription != NULL)
			{
				delete[]pEnumDescription;
				pEnumDescription = NULL;
			}

			return emStatus;
		}

		// 遍历设备支持的像素格式,设置像素格式为8bit,
		// 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}

		// 释放资源
		if (pEnumDescription != NULL)
		{
			delete[]pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}


//---------------------------------------------------------------------------------
/**
\brief   相机初始化

\return  无
*/
//----------------------------------------------------------------------------------
GX_STATUS Camera::InitDevice(void)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//设置采集模式连续采集
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// 不清楚当前相机的数据格式时，可以调用以下函数将图像数据格式设置为8Bit
	emStatus = SetPixelFormat8bit();
	
	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief  为彩色图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowColorImg()
{
	int type, flags;
	flags = IMREAD_COLOR;		// 彩色

	//--------------------------------------------------------------------
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 24; // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;


	//---------------------------------------------------------------------
	//------------------------为AVI存储计算相关数据-----------------------
	//计算每帧图像数据大小
	m_nFrameSize = m_nImageWidth * m_nImageHeight * 3;

	//----------------------------为图像数据分配Buffer---------------------

	// 为经过RGB转换后的图像分配Buffer
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	// 为Mat对象分配空间
	if (flags != -1)
	{
		if ((flags & IMREAD_ANYDEPTH) == 0)
		{
			type = CV_MAKETYPE(CV_8U, CV_MAT_CN(type));
		}

		if ((flags & IMREAD_COLOR) != 0 || ((flags & IMREAD_ANYCOLOR) != 0 && CV_MAT_CN(type) > 1))
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 3);	// 彩色
		}
		else
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 1);	// 灰度
		}
	}

	m_image.create(m_nImageHeight, m_nImageWidth, type);

	return true;
}


//----------------------------------------------------------------------------------
/**
\brief 为黑白图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowMonoImg()
{
	int type, flags;

	flags = IMREAD_GRAYSCALE;	// 灰度

	//--------------------------------------------------------------------
	//---------------------------初始化bitmap头---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8;  // 彩色图像为24,黑白图像为8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColorFilter)
	{
		// 黑白相机需要进行初始化调色板操作
		for (int i = 0; i < 256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//------------------------为AVI存储计算相关数据-----------------------
	//计算每帧图像数据大小
	m_nFrameSize = m_nImageWidth * m_nImageHeight;

	//---------------------------------------------------------------------
	//----------------------------为图像数据分配Buffer---------------------
	// 黑白图像Buffer分配
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	// 为Mat对象分配空间
	if (flags != -1)
	{
		if ((flags & IMREAD_ANYDEPTH) == 0)
		{
			type = CV_MAKETYPE(CV_8U, CV_MAT_CN(type));
		}

		if ((flags & IMREAD_COLOR) != 0 || ((flags & IMREAD_ANYCOLOR) != 0 && CV_MAT_CN(type) > 1))
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 3);	// 彩色
		}
		else
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 1);	// 灰度
		}
	}

	m_image.create(m_nImageHeight, m_nImageWidth, type);

	return true;
}


//----------------------------------------------------------------------------------
/**
\brief  为保存图像分配Buffer,为图像显示准备资源

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowImg()
{
	bool bRet = true;

	// 为存储原始图像分配Buffer
	m_pRawBuffer = new BYTE[(size_t)m_nPayLoadSize];
	m_stFrameData.pImgBuf = m_pRawBuffer;
	if (m_pRawBuffer == NULL)
	{
		bRet = false;
	}

	if (m_bIsColorFilter)
	{
		// 为彩色图像显示准备资源,分配Buffer
		bRet = PrepareForShowColorImg();
	}
	else
	{
		// 为黑白图像显示准备资源,分配Buffer
		bRet = PrepareForShowMonoImg();
	}

	// 若分配失败则释放已分配的资源
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}


//----------------------------------------------------------------------------------
/**
\brief  释放为图像显示准备资源

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::UnPrepareForShowImg()
{
	if (m_pRawBuffer != NULL)
	{
		delete[]m_pRawBuffer;
		m_pRawBuffer = NULL;
		m_stFrameData.pImgBuf = NULL;
	}

	if (m_pImgBuffer != NULL)
	{
		delete[]m_pImgBuffer;
		m_pImgBuffer = NULL;
	}

	if (m_image.data)
	{
		m_image.release();
	}
}


//---------------------------------------------------------------------------------
/**
\brief   初始化组合框UI界面
\param   emFeatureID    [in]    功能码ID
\param   pComboBox      [in]    控件指针
\param   bIsImplemented [in]    标识设备是否支持emFeatureID代表的功能

\return  无
*/
//----------------------------------------------------------------------------------
//void Widget::InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented)
//{
//	// 判断控件
//	if ((pComboBox == NULL) || (!bIsImplemented))
//	{
//		return;
//	}
//
//	GX_ENUM_DESCRIPTION *pEnum = NULL;
//	GX_STATUS emStatus = GX_STATUS_ERROR;
//	size_t    nbufferSize = 0;
//	uint32_t  nEntryNum = 0;
//	int64_t   nEnumValue = -1;
//	int       nCurcel = 0;
//
//	// 获取功能的枚举数
//	emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, &nEntryNum);
//	GX_VERIFY(emStatus);
//
//	// 为获取枚举型的功能描述分配空间
//	nbufferSize = nEntryNum * sizeof(GX_ENUM_DESCRIPTION);
//	pEnum = new GX_ENUM_DESCRIPTION[nEntryNum];
//	if (pEnum == NULL)
//	{
//		MessageBox(_T("分配缓冲区失败！"));
//		return;
//	}
//
//	// 获取功能的枚举描述
//	emStatus = GXGetEnumDescription(m_hDevice, emFeatureID, pEnum, &nbufferSize);
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		if (pEnum != NULL)
//		{
//			delete[]pEnum;
//			pEnum = NULL;
//		}
//		ShowErrorString(emStatus);
//	}
//
//	// 获取枚举型的当前值,并设为界面当前显示值
//	emStatus = GXGetEnum(m_hDevice, emFeatureID, &nEnumValue);
//	if (emStatus != GX_STATUS_SUCCESS)
//	{
//		if (pEnum != NULL)
//		{
//			delete[]pEnum;
//			pEnum = NULL;
//		}
//		ShowErrorString(emStatus);
//	}
//
//	// 初始化当前控件的可选项
//	pComboBox->ResetContent();
//	for (uint32_t i = 0; i < nEntryNum; i++)
//	{
//		pComboBox->SetItemData(pComboBox->AddString(pEnum[i].szSymbolic), (uint32_t)pEnum[i].nValue);
//		if (pEnum[i].nValue == nEnumValue)
//		{
//			nCurcel = i;
//		}
//	}
//
//	// 设置当前值为界面的显示值
//	pComboBox->SetCurSel(nCurcel);
//
//	// 释放函数资源
//	if (pEnum != NULL)
//	{
//		delete[]pEnum;
//		pEnum = NULL;
//	}
//}


//----------------------------------------------------------------------------------
/**
\brief  将获取到的图像数据依据黑白彩色的不同进行处理并显示到界面
\param  pFrameData  [in] 指向用户传入的图像数据的地址指针

\return 无
*/
//----------------------------------------------------------------------------------
void  Camera::ShowImage(GX_FRAME_DATA* pFrameData)
{
	// 获取到的图像是否为完整的图像
	if (pFrameData->nStatus != 0)
	{
		return;
	}

	//若支持彩色,转换为RGB图像后输出
	if (m_bIsColorFilter)
	{
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), true);
	}
	else
	{
		// 黑白相机需要翻转数据后显示
		for (int i = 0; i <m_nImageHeight; i++)
		{
			memcpy(m_pImgBuffer + i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf) + (m_nImageHeight - i - 1)*m_nImageWidth, (size_t)m_nImageWidth);
		}
	}

	//在指定窗口上一帧一帧的画图
	//DrawImg(m_pImgBuffer);
}


//---------------------------------------------------------------------------------
/**
\brief   显示图像

\return  无
*/
//----------------------------------------------------------------------------------
//void Camera::DrawImg(BYTE *pImageBuffer)
//{
//	QImage image;
//	Mat tmpImage;
//	Mat rgb;
//
//	BitmapinfoToMat(pImageBuffer);
//
//	Size dsize = Size(m_matImage.cols / 2, m_matImage.rows / 2);
//	cv::resize(m_matImage, tmpImage, dsize);
//
//	if (3 == tmpImage.channels())
//	{
//		cvtColor(tmpImage, rgb, CV_BGR2RGB);
//		//image = QImage((const uchar*)(img.data), img.cols, img.rows, (img.cols * img.channels()), QImage::Format_RGB888);
//		image = QImage((const uchar*)(rgb.data),
//			rgb.cols, rgb.rows,
//			(rgb.cols * rgb.channels()),
//			QImage::Format_RGB888);
//	}
//	else
//	{
//		image = QImage((const uchar*)(tmpImage.data),
//			tmpImage.cols, tmpImage.rows,
//			(tmpImage.cols * tmpImage.channels()),
//			QImage::Format_Indexed8);
//	}
//
//	ui->labelImageShow->setPixmap(QPixmap::fromImage(image));
//}


//----------------------------------------------------------------------------------
/**
\brief  BITMAPINFO类型转换为Mat类型，add by syf

\return
*/
//----------------------------------------------------------------------------------
void  Camera::BitmapinfoToMat(GX_FRAME_DATA* pFrameData)
{
	// 获取到的图像是否为完整的图像
	if (pFrameData->nStatus != 0)
	{
		return;
	}

	//若支持彩色,转换为RGB图像后输出
	if (m_bIsColorFilter)
	{
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), false);
	}
	else
	{
		// 黑白相机需要翻转数据后显示
		for (int i = 0; i <m_nImageHeight; i++)
		{
			memcpy(m_pImgBuffer + i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf) + i*m_nImageWidth, (size_t)m_nImageWidth);
		}
	}

	if (m_pImgBuffer)
	{
		m_image.data = (uchar*)m_pImgBuffer;
		//cv::cvtColor(m_image, m_image, CV_RGB2BGR);
		m_bIsImgSnaped = true;

		emit Signal_ImageSnaped();
	}
}


//----------------------------------------------------------------------------------
/**
\brief  BITMAPINFO类型转换为Mat类型，add by syf

\return
*/
//----------------------------------------------------------------------------------
void  Camera::BitmapinfoToMat(BYTE *pImageBuffer)
{
	// 获取到的图像是否为完整的图像
	if (pImageBuffer == NULL)
	{
		return;
	}

	if (pImageBuffer)
	{
		m_image.data = (uchar*)pImageBuffer;


		// 发送采集到好的图像
		//if (m_bIsImgEmit)
		{
			emit Signal_ImageSnaped();
			
			m_bIsImgEmit = false;
		}
	}
}



//---------------------------------------------------------------------------------
/**
\brief   刷新显示当前增益、曝光值

\return  无
*/
//---------------------------------------------------------------------------------
void Camera::RefreshCurValue()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	String   strTemp = "";
	double    dValue = 0;

	if (m_emAutoShutterMode != GX_EXPOSURE_AUTO_OFF)
	{
		//如果当前选择曝光为ONCE时，获取设备何时变为OFF，并更新界面
		if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_ONCE)
		{
			int64_t  nShutterEnumVal = 0;   //<   保存曝光的枚举值

			//获取自动曝光组合框控件的指针
			//CComboBox *pAutoShutterBox = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_SHUTTER);

			//获取自动曝光的枚举值
			emStatus = GXGetEnum(m_hDevice, GX_ENUM_EXPOSURE_AUTO, &nShutterEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}

			//类型转换
			m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nShutterEnumVal;

			// 判断设备曝光模式是否变为OFF
			//if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_OFF)
			//{
			//	for (int i = 0; i < pAutoShutterBox->GetCount(); i++)
			//	{
			//		if ((int64_t)pAutoShutterBox->GetItemData(i) == GX_EXPOSURE_AUTO_OFF)
			//		{
			//			//选中自动曝光控件中的OFF项,即由ONCE变为OFF
			//			pAutoShutterBox->SetCurSel(i);
			//			break;
			//		}
			//	}

			//	UpDateUI();
			//}
		}

		//获取当前曝光值，更新界面
		emStatus = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &dValue);
		m_dShutterValue = dValue;
		//strTemp.Format("%.4f", m_dShutterValue);
		//SetDlgItemText(IDC_EDIT_CUR_SHUTTER, strTemp);
	}

	//if (m_emAutoGainMode != GX_GAIN_AUTO_OFF)
	//{
	//	//如果当前选择增益为ONCE时，获取设备何时变为OFF，并更新界面
	//	if (m_emAutoGainMode == GX_GAIN_AUTO_ONCE)
	//	{
	//		int64_t  nGainEnumVal = 0;   //<   保存增益的枚举值

	//		//获取自动增益组合框控件的指针
	//		CComboBox *pAutoGainBox = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);

	//		//获取自动增益的枚举值
	//		emStatus = GXGetEnum(m_hDevice, GX_ENUM_GAIN_AUTO, &nGainEnumVal);
	//		if (emStatus != GX_STATUS_SUCCESS)
	//		{
	//			return;
	//		}

	//		// 类型转换
	//		m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nGainEnumVal;

	//		//判断设备增益模式是否变为OFF
	//		if (m_emAutoGainMode == GX_GAIN_AUTO_OFF)
	//		{
	//			for (int i = 0; i < pAutoGainBox->GetCount(); i++)
	//			{
	//				if ((int64_t)pAutoGainBox->GetItemData(i) == GX_GAIN_AUTO_OFF)
	//				{
	//					//选中自动曝光控件中的OFF项，即由ONCE变为OFF
	//					pAutoGainBox->SetCurSel(i);
	//					break;
	//				}
	//			}

	//			UpDateUI();
	//		}
	//	}

	//	// 获取当前增益值，更新界面
	//	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_GAIN, &dValue);
	//	m_dGainValue = dValue;
	//	strTemp.Format("%.4f", m_dGainValue);
	//	SetDlgItemText(IDC_EDIT_CUR_GAIN, strTemp);
	//}
}


//----------------------------------------------------------------------------------
/**
\brief  弹出错误信息
\param  emErrorStatus  [in] 错误码

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize = 0;
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 获取错误信息长度，并申请内存资源
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		QMessageBox::information(this,
			"ERROR",
			QStringLiteral("GXGetLastError接口调用失败！"));
	}
	else
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QMessageBox::information(this,
			"ERROR",
			codec->toUnicode((const char*)pchErrorInfo));
	}

	// 释放申请的内存资源
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  为保存avi视频文件准备资源

\return TRUE:准备成功  FALSE:准备资源失败
*/
//----------------------------------------------------------------------------------
BOOL Camera::PrepareForSaveAVI()
{
	BOOL    bRet = FALSE;
	LONG    cbFormat = 0;    //文件格式

	// 经过RGB转换的24位图像同8位灰度图像文件如下
	if (m_bIsColorFilter)
	{
		// 经过RGB转换的24图像数据格式
		cbFormat = sizeof(BITMAPINFOHEADER);
	}
	else
	{
		// 8位灰度图像文件格式
		cbFormat = sizeof(BITMAPINFOHEADER)+(256 * 4);	
	}	

	AVISTREAMINFO StreamInfo;
	HRESULT hr;

	SYSTEMTIME system;
	GetLocalTime(&system);
	string strAVIFileName = MsgComposer("/%d_%d_%d_%d_%d_%d_%d.avi", system.wYear,
		system.wMonth,
		system.wDay,
		system.wHour,
		system.wMinute,
		system.wSecond,
		system.wMilliseconds);

	wstring wstrAVIFilePath = m_strAVIFolder.toStdWString();
	wstring wstrAVIFileName = StringToWString(strAVIFileName);
	wstrAVIFilePath = wstrAVIFilePath + wstrAVIFileName;
	::DeleteFile((LPCWSTR)wstrAVIFilePath.c_str());

	AVIFileInit();
	//创建一个AVI文件	
	hr = AVIFileOpen(&m_pAVIFile, (LPCWSTR)wstrAVIFilePath.c_str(), OF_WRITE | OF_CREATE, NULL);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	// 创建一个视频流
	::ZeroMemory(&StreamInfo, sizeof(StreamInfo));
	StreamInfo.fccType = streamtypeVIDEO;
	StreamInfo.fccHandler = 0;
	StreamInfo.dwScale = 1;
	StreamInfo.dwRate = m_wPlayFPS;  //AVI视频流的播放帧率
	StreamInfo.dwSuggestedBufferSize = DWORD(m_nImageWidth * m_nImageHeight * 3);
	SetRect(&StreamInfo.rcFrame, 0, 0, (int)m_nImageWidth, (int)m_nImageHeight);
	hr = AVIFileCreateStream(m_pAVIFile, &m_ps, &StreamInfo);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	//初始化压缩视频流需要的结构体
	AVICOMPRESSOPTIONS opts;
	//AVICOMPRESSOPTIONS FAR * aopts[1] = { &opts };

	if (m_bCompressedAVI)
	{
		if (m_hic == NULL)
		{
			return FALSE;
		}

		// 压缩存储
		DWORD dwSize = ICGetStateSize(m_hic);
		LPVOID lpvod = (LPVOID)GlobalAlloc(NULL, dwSize);

		ICINFO icinfo;
		ICGetInfo(m_hic, &icinfo, sizeof(icinfo));
		opts.fccType = icinfo.fccType;
		opts.fccHandler = icinfo.fccHandler;
		opts.dwKeyFrameEvery = 0;
		opts.dwQuality = 5000;
		opts.dwBytesPerSecond = 0;
		opts.dwFlags = 8;
		opts.lpFormat = 0;
		opts.cbFormat = 0;
		opts.lpParms = lpvod;
		opts.cbParms = dwSize;
		opts.dwInterleaveEvery = 0;
	}
	else
	{
		// 非压缩存储
		opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 
	}

	//创建压缩视频流
	hr = AVIMakeCompressedStream(&m_psCompressed, m_ps, &opts, NULL);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	//设置视频流的输入文件格式。
	hr = AVIStreamSetFormat(m_psCompressed, 0, &m_pBmpInfo->bmiHeader, cbFormat);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}
	else
	{
		bRet = TRUE;
	}

	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  释放创建AVI文件及流时的资源

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::UnPrepareForSaveAVI()
{
	if (m_psCompressed != NULL)
	{
		AVIStreamClose(m_psCompressed);
		m_psCompressed = NULL;
	}
	if (m_ps != NULL)
	{
		AVIStreamClose(m_ps);

		m_ps = NULL;
	}
	if (m_pAVIFile != NULL)
	{
		AVIFileEndRecord(m_pAVIFile);
		AVIFileClose(m_pAVIFile);
		m_pAVIFile = NULL;
	}

	AVIFileExit();
	m_nTimeFrame = 0;
}

//----------------------------------------------------------------------------------
/**
\brief  释放视频编码器相关资源

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::ResetCompressor()
{
	if (m_hic != NULL)
	{
		ICClose(m_hic);
		m_hic = NULL;
	}

	ICCompressorFree(&m_Com);
	memset(&m_Com, 0, sizeof(m_Com));
}

//----------------------------------------------------------------------------------
/**
\brief  保存avi视频:该函数带有检测磁盘空间,限制AVI文件大小等扩展功能
\param  pImageBuf  [in]  指向图像缓冲区的指针

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::ExSaveAVI(BYTE * pImageBuf)
{
	if (!m_bEnableSaveAVI)
	{
		return;
	}

	LONG nBufferSize = 0; // 实际写入到AVI文件的图像数据大小,若是压缩则为压缩后的大小
	BOOL bRet = TRUE;

	// ---------------------------------------------
	// 检测当前目录磁盘空间,若剩余空间不允许继续存储时则停止保存
	if (GetDiskFreeSpace() < m_nFrameSize + AVI_FRAME_OFFSET)
	{
		m_bEnableSaveAVI = FALSE;
		m_SaverCriSec.Enter();
		UnPrepareForSaveAVI();
		m_SaverCriSec.Leave();
		return;
	}

	// 判断继续存储时AVI文件大小是否会达到允许的最大值
	// 若达到则创建新的AVI文件继续录制
	if ((m_nFrameSize + m_nWrittenDataSize + AVI_FRAME_OFFSET) > AVI_MAX_SIZE - AVI_HEADERSIZE)
	{
		bRet = ReCreatVedioFile();
		if (!bRet)
		{
			return;
		}

		m_nWrittenDataSize = 0;
	}

	// AVI视频保存实现
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		&nBufferSize);

	// 判断是否写成功
	if (hr == AVIERR_OK)
	{
		m_nTimeFrame++;

		// 统计AVI文件数据大小:每帧图像占用空间为数据大小加上偏移值8
		m_nWrittenDataSize = m_nWrittenDataSize + nBufferSize + AVI_FRAME_OFFSET;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  保存avi视频:该函数只实现写AVI文件,不带有其他扩展功能
\param  pImageBuf  [in]  指向图像缓冲区的指针

\return 无
*/
//----------------------------------------------------------------------------------
void Camera::SaveAVI(BYTE * pImageBuf)
{
	if (!m_bEnableSaveAVI)
	{
		return;
	}

	// AVI视频保存实现
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		NULL);

	// 判断是否写成功
	if (hr == AVIERR_OK)
	{
		m_nTimeFrame++;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  停止当前录制,重新创建AVI视频文件,然后继续录制

\return TRUE:重新创建成功 FALSE:创建失败
*/
//----------------------------------------------------------------------------------
BOOL Camera::ReCreatVedioFile()
{
	BOOL bRet = TRUE;

	// 先停止正在存储的文件
	m_SaverCriSec.Enter();
	UnPrepareForSaveAVI();
	m_SaverCriSec.Leave();

	// 重新创建
	bRet = PrepareForSaveAVI();

	return bRet;
}



//----------------------------------------------------------------------------------
/**
\brief  获取存储目录所在磁盘的剩余空间

\return nFreeBytes   剩余空间,单位为Byte
*/
//----------------------------------------------------------------------------------
int64_t Camera::GetDiskFreeSpace()
{
	int64_t nFreeBytes = 0;
	BOOL    bResult;

	//使用GetDiskFreeSpaceEx获取存储目录所在磁盘的剩余空间
	wstring AVIFolder = m_strAVIFolder.toStdWString();
	bResult = GetDiskFreeSpaceEx((LPCWSTR)AVIFolder.c_str(), 0, 0,
		(PULARGE_INTEGER)&nFreeBytes);

	return nFreeBytes;
}


//=============================================================================
// 参数：
// 返回：0--成功
//       1--失败
// 功能：打开相机
//=============================================================================
BOOL Camera::OpenCamera(void)
{
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum = 0;
	GX_OPEN_PARAM stOpenParam;

	// 初始化设备打开参数
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = (char *)("1");

	// 枚举设备
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// 判断当前连接设备个数
	if (nDevNum <= 0)
	{
		QMessageBox::information(this,
			"ERROR",
			QStringLiteral("未发现相机设备！"));
		return FALSE;
	}

	// 如果设备已经打开则关闭,保证相机在初始化出错情况能再次打开
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// 打开设备
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = TRUE;

	// 设置相机的默认参数:采集模式:连续采集,软件触发模式：开或关，数据格式:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// 获取设备的宽、高等属性信息
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

	// 设置白平衡系数
	SetBalanceRatio(0, 1.18);
	SetBalanceRatio(1, 1.0);
	SetBalanceRatio(2, 1.36);


	return TRUE;
}


//=============================================================================
// 参数：
// 返回：
// 功能：“关闭相机”槽函数。
//       关闭相机，并释放相关资源。
//=============================================================================
void Camera::CloseCamera(void)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 相机正在采集时则先停止采集
	if (m_bIsSnap)
	{
		//发送停止采集命令
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// 错误处理
		}

		// 连续采集，需注销回调函数，释放为采集图像开辟的Buffer
		if (m_bIsContinuousSnap)
		{

			//注销回调
			emStatus = GXUnregisterCaptureCallback(m_hDevice);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// 错误处理
			}

			m_bIsContinuousSnap = FALSE;
		}

		// 释放为采集图像开辟的图像Buffer
		UnPrepareForShowImg();

		m_bIsSnap = FALSE;
		m_bIsSingleSnap = FALSE;
	}

	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// 错误处理
	}

	m_bDevOpened = FALSE;
	m_hDevice = NULL;
}

/* 
* 参数：mode--0,连续采集，1，软件触发采集
* 返回：
* 功能：连续采集
*/
void Camera::StartSnap(int mode)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 初始化BMP头信息、分配Buffer为图像采集做准备
	if (!PrepareForShowImg())
	{
		QMessageBox::warning(this,
			"ERROR",
			QStringLiteral("为图像显示准备资源失败!")
			);
		return;
	}

	if (0 == mode)
	{
		//注册回调
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			UnPrepareForShowImg();
			ShowErrorString(emStatus);
			return;
		}
	}

	//发开始采集命令
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		UnPrepareForShowImg();
		ShowErrorString(emStatus);
		return;
	}
	
	m_bIsSnap = TRUE;
}


/*
* 参数：mode--0,连续采集，1，软件触发采集
* 返回：
* 功能：停止采集
*/
BOOL Camera::StopSnap(int mode)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//发送停止采集命令
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
	GX_VERIFY(emStatus);

	if (0 == mode)
	{
		//注销回调
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		GX_VERIFY(emStatus);
	}
	
	// 释放为采集图像开辟的图像Buffer
	UnPrepareForShowImg();

	m_bIsSnap = FALSE;

	return TRUE;
}


//=============================================================================
// 参数：
// 返回：
// 功能：软件触发图像采集
//=============================================================================
BOOL Camera::SoftwareTrigger(void)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int timeout = 1000;


	//每次发送触发命令之前清空采集输出队列
	//防止库内部缓存帧，造成本次GXGetImage得到的图像是上次发送触发得到的图
	emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);

	//发送软触发命令
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_TRIGGER_SOFTWARE);
	GX_VERIFY(emStatus);


	//获取图像
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, timeout);

	//判断GXGetImage返回值
	GX_VERIFY(emStatus);

	// 用highgui显示Mat类型打开的图片，add by syf
	BitmapinfoToMat(&m_stFrameData);

	return TRUE;
}


/*
* 参数：flag--false，关闭自动白平衡，true，开启自动白平衡
* 返回：
* 功能：打开或关闭自动白平衡(continuous模式)
*/
BOOL Camera::SetAutoWhiteBalance(int index)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 0-off, 1-contiuous, 2-once
	int64_t nAwbMode = index;

	// 0-adaptive, 1-D65, 2-Fluoresence, Incandescent
	int64_t nAwbLightMode = 0;


	// 设置白平衡
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, nAwbMode);
	GX_VERIFY(emStatus);

	if (index > 0)
	{
		// 设置白平衡光源
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_AWB_LAMP_HOUSE, nAwbLightMode);
		GX_VERIFY(emStatus);
	}

	return TRUE;
}


/*
* 参数：index--白平衡通道
        ratio--系数
* 返回：
* 功能：设置白平衡系数
*/
BOOL Camera::SetBalanceRatio(int index, double ratio)
{
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return FALSE;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 关闭自动白平衡
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, 0);
	GX_VERIFY(emStatus);


	// 选择白平衡通道
	switch (index)
	{
	case 0:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 0);
		break;
	case 1:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 1);
		break;
	case 2:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 2);
		break;
	}

	GX_VERIFY(emStatus);

	emStatus = GXSetFloat(m_hDevice, GX_FLOAT_BALANCE_RATIO, ratio);
	GX_VERIFY(emStatus);

	return TRUE;
}


/*
* 参数：index--白平衡通道
		ratio--系数
* 返回：
* 功能：设置白平衡系数
*/
BOOL Camera::GetBalanceRatio(int index, double* ratio)
{
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return FALSE;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 选择白平衡通道
	switch (index)
	{
	case 0:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 0);
		break;
	case 1:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 1);
		break;
	case 2:
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_RATIO_SELECTOR, 2);
		break;
	}

	GX_VERIFY(emStatus);

	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_BALANCE_RATIO, ratio);
	GX_VERIFY(emStatus);

	return TRUE;
}

/*
* 参数：flag--false，关闭自动曝光，true，开启自动曝光
* 返回：
* 功能：打开或关闭自动曝光
*/
BOOL Camera::SetAutoExposure(int index)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 0--off，1--continuous, 2-once
	int64_t  nExposureMode = index;


	emStatus = GXSetEnum(m_hDevice, GX_ENUM_EXPOSURE_AUTO, nExposureMode);
	GX_VERIFY(emStatus);

	return TRUE;
}


/*
* 参数：t--曝光时间
* 返回：
* 功能：设置曝光时间
*/
BOOL Camera::SetExposure(double t)
{
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return FALSE; 
	}

	GX_STATUS status = GX_STATUS_SUCCESS;

	status = GXSetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, t);
	GX_VERIFY(status);

	return TRUE;
}

/*
* 参数：
* 返回：曝光时间
* 功能：获取相机当前曝光时间
*/
double Camera::GetExposure()
{
	//判断句柄是否有效，避免设备掉线后关闭程序出现的异常
	if (m_hDevice == NULL)
	{
		return FALSE;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	double t = 0.0;

	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &t);;

	return t;
}



//=============================================================================
// 参数：
// 返回：ture--图像已采集好，false--图像未采集好
// 功能：返回当前图像采集状态
//=============================================================================
bool Camera::IsImgSnaped(void)
{
	return m_bIsImgSnaped;
}


//=============================================================================
// 参数：
// 返回：
// 功能：复位图像采集状态
//=============================================================================
void Camera::ResetImgSnaped(void)
{
	m_bIsImgSnaped = false;
}


//=============================================================================
// 参数：
// 返回：Mat类型的图像数据指针
// 功能：返回Mat类型的图像数据指针
//=============================================================================
Mat& Camera::GetImage(void)
{
	return m_image;
}
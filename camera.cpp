/*******************************************************************************
�ļ����ƣ�camera.c
�������ڣ�2016-02-11
����޸ģ�2016-08-31
��	   ����v1.0.0
��    �ߣ�syf
����������������������
*******************************************************************************/
#include "camera.h"


//=============================================================================
// ������
// ���أ�
// ���ܣ����캯��
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

	// ��ʼ������
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
// ������
// ���أ�
// ���ܣ���������
//=============================================================================
Camera::~Camera()
{
	if (m_bDevOpened)
	{
		CloseCamera();
	}
}


//=============================================================================
// ������flag��false-�����ͣ�true--����
// ���أ�
// ���ܣ����÷��Ͳɼ�����ͼ���־
//=============================================================================
void Camera::SetImgEmit(bool flag)
{
	m_bIsImgEmit = flag;
}


//=============================================================================
// ������
// ���أ�
// ���ܣ�����ɼ���־
//=============================================================================
BOOL Camera::IsSnap(void)
{
	return m_bIsSnap;
}


//=============================================================================
// ������
// ���أ�
// ���ܣ������״̬
//=============================================================================
BOOL Camera::IsDeviceOpened(void)
{
	return m_bDevOpened;
}


//---------------------------------------------------------------------------------
/**
\brief   �ɼ��ص�����:����ͼ���ȡ����ʾ����
\param   pFrame   �ص�����

\return  ��
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
				// ��ɫͼ����Ҫ��Raw8ͼ�񾭹�RGBת������ʾ
				DxRaw8toRGB24(pCamera->m_pRawBuffer, pCamera->m_pImgBuffer, nImageWidth, nImageHeight, RAW2RGB_NEIGHBOUR,
					DX_PIXEL_COLOR_FILTER(pCamera->m_nPixelColorFilter), true);
			}
			else
			{
				// �ڰ������Ҫ��ת���ݺ���ʾ
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
\brief  ��ȡ�豸�Ŀ�ߵ�������Ϣ

\return GX_STATUS_SUCCESS:ȫ����ȡ�ɹ�������״̬��:δ�ɹ���ȡȫ��
*/
//----------------------------------------------------------------------------------
GX_STATUS Camera::GetDeviceParam()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	//	bool      bIsImplemented = false;

	// ��ȡͼ���С
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ���
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	VERIFY_STATUS_RET(emStatus);

	// ��ȡ�߶�
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	VERIFY_STATUS_RET(emStatus);

	// �ж�����Ƿ�֧��bayer��ʽ
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	VERIFY_STATUS_RET(emStatus);

	// ֧�ֱ�ʾ�����ɫͼ��
	if (m_bIsColorFilter)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
	}

	return emStatus;
}


// ---------------------------------------------------------------------------------
/**
\brief   ������������ݸ�ʽΪ8bit

\return  emStatus GX_STATUS_SUCCESS:���óɹ�������:����ʧ��
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

	// ��ȡ���ص��С
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	VERIFY_STATUS_RET(emStatus);

	// �ж�Ϊ8bitʱֱ�ӷ���,��������Ϊ8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// ��ȡ�豸֧�ֵ����ظ�ʽö����
		emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		VERIFY_STATUS_RET(emStatus);

		// Ϊ��ȡ�豸֧�ֵ����ظ�ʽö��ֵ׼����Դ
		nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// ��ȡ֧�ֵ�ö��ֵ
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

		// �����豸֧�ֵ����ظ�ʽ,�������ظ�ʽΪ8bit,
		// ���豸֧�ֵ����ظ�ʽΪMono10��Mono8��������ΪMono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}

		// �ͷ���Դ
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
\brief   �����ʼ��

\return  ��
*/
//----------------------------------------------------------------------------------
GX_STATUS Camera::InitDevice(void)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//���òɼ�ģʽ�����ɼ�
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	VERIFY_STATUS_RET(emStatus);

	// ���ô�������Ϊ��
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	VERIFY_STATUS_RET(emStatus);

	// ��֪��ǰ���֧���ĸ�8λͼ�����ݸ�ʽ����ֱ������
	// �����豸֧�����ݸ�ʽGX_PIXEL_FORMAT_BAYER_GR8,��ɰ������´���ʵ��
	// emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// �������ǰ��������ݸ�ʽʱ�����Ե������º�����ͼ�����ݸ�ʽ����Ϊ8Bit
	emStatus = SetPixelFormat8bit();
	
	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief  Ϊ��ɫͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowColorImg()
{
	int type, flags;
	flags = IMREAD_COLOR;		// ��ɫ

	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 24; // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;


	//---------------------------------------------------------------------
	//------------------------ΪAVI�洢�����������-----------------------
	//����ÿ֡ͼ�����ݴ�С
	m_nFrameSize = m_nImageWidth * m_nImageHeight * 3;

	//----------------------------Ϊͼ�����ݷ���Buffer---------------------

	// Ϊ����RGBת�����ͼ�����Buffer
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	// ΪMat�������ռ�
	if (flags != -1)
	{
		if ((flags & IMREAD_ANYDEPTH) == 0)
		{
			type = CV_MAKETYPE(CV_8U, CV_MAT_CN(type));
		}

		if ((flags & IMREAD_COLOR) != 0 || ((flags & IMREAD_ANYCOLOR) != 0 && CV_MAT_CN(type) > 1))
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 3);	// ��ɫ
		}
		else
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 1);	// �Ҷ�
		}
	}

	m_image.create(m_nImageHeight, m_nImageWidth, type);

	return true;
}


//----------------------------------------------------------------------------------
/**
\brief Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowMonoImg()
{
	int type, flags;

	flags = IMREAD_GRAYSCALE;	// �Ҷ�

	//--------------------------------------------------------------------
	//---------------------------��ʼ��bitmapͷ---------------------------
	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
	m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8;  // ��ɫͼ��Ϊ24,�ڰ�ͼ��Ϊ8
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	if (!m_bIsColorFilter)
	{
		// �ڰ������Ҫ���г�ʼ����ɫ�����
		for (int i = 0; i < 256; i++)
		{
			m_pBmpInfo->bmiColors[i].rgbBlue = i;
			m_pBmpInfo->bmiColors[i].rgbGreen = i;
			m_pBmpInfo->bmiColors[i].rgbRed = i;
			m_pBmpInfo->bmiColors[i].rgbReserved = i;
		}
	}

	//------------------------ΪAVI�洢�����������-----------------------
	//����ÿ֡ͼ�����ݴ�С
	m_nFrameSize = m_nImageWidth * m_nImageHeight;

	//---------------------------------------------------------------------
	//----------------------------Ϊͼ�����ݷ���Buffer---------------------
	// �ڰ�ͼ��Buffer����
	m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
	if (m_pImgBuffer == NULL)
	{
		return false;
	}

	// ΪMat�������ռ�
	if (flags != -1)
	{
		if ((flags & IMREAD_ANYDEPTH) == 0)
		{
			type = CV_MAKETYPE(CV_8U, CV_MAT_CN(type));
		}

		if ((flags & IMREAD_COLOR) != 0 || ((flags & IMREAD_ANYCOLOR) != 0 && CV_MAT_CN(type) > 1))
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 3);	// ��ɫ
		}
		else
		{
			type = CV_MAKETYPE(CV_MAT_DEPTH(type), 1);	// �Ҷ�
		}
	}

	m_image.create(m_nImageHeight, m_nImageWidth, type);

	return true;
}


//----------------------------------------------------------------------------------
/**
\brief  Ϊ����ͼ�����Buffer,Ϊͼ����ʾ׼����Դ

\return true:Ϊͼ����ʾ׼����Դ�ɹ�  false:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
bool  Camera::PrepareForShowImg()
{
	bool bRet = true;

	// Ϊ�洢ԭʼͼ�����Buffer
	m_pRawBuffer = new BYTE[(size_t)m_nPayLoadSize];
	m_stFrameData.pImgBuf = m_pRawBuffer;
	if (m_pRawBuffer == NULL)
	{
		bRet = false;
	}

	if (m_bIsColorFilter)
	{
		// Ϊ��ɫͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowColorImg();
	}
	else
	{
		// Ϊ�ڰ�ͼ����ʾ׼����Դ,����Buffer
		bRet = PrepareForShowMonoImg();
	}

	// ������ʧ�����ͷ��ѷ������Դ
	if (!bRet)
	{
		UnPrepareForShowImg();
	}

	return bRet;
}


//----------------------------------------------------------------------------------
/**
\brief  �ͷ�Ϊͼ����ʾ׼����Դ

\return ��
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
\brief   ��ʼ����Ͽ�UI����
\param   emFeatureID    [in]    ������ID
\param   pComboBox      [in]    �ؼ�ָ��
\param   bIsImplemented [in]    ��ʶ�豸�Ƿ�֧��emFeatureID����Ĺ���

\return  ��
*/
//----------------------------------------------------------------------------------
//void Widget::InitEnumUI(GX_FEATURE_ID_CMD emFeatureID, CComboBox *pComboBox, bool bIsImplemented)
//{
//	// �жϿؼ�
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
//	// ��ȡ���ܵ�ö����
//	emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, &nEntryNum);
//	GX_VERIFY(emStatus);
//
//	// Ϊ��ȡö���͵Ĺ�����������ռ�
//	nbufferSize = nEntryNum * sizeof(GX_ENUM_DESCRIPTION);
//	pEnum = new GX_ENUM_DESCRIPTION[nEntryNum];
//	if (pEnum == NULL)
//	{
//		MessageBox(_T("���仺����ʧ�ܣ�"));
//		return;
//	}
//
//	// ��ȡ���ܵ�ö������
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
//	// ��ȡö���͵ĵ�ǰֵ,����Ϊ���浱ǰ��ʾֵ
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
//	// ��ʼ����ǰ�ؼ��Ŀ�ѡ��
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
//	// ���õ�ǰֵΪ�������ʾֵ
//	pComboBox->SetCurSel(nCurcel);
//
//	// �ͷź�����Դ
//	if (pEnum != NULL)
//	{
//		delete[]pEnum;
//		pEnum = NULL;
//	}
//}


//----------------------------------------------------------------------------------
/**
\brief  ����ȡ����ͼ���������ݺڰײ�ɫ�Ĳ�ͬ���д�����ʾ������
\param  pFrameData  [in] ָ���û������ͼ�����ݵĵ�ַָ��

\return ��
*/
//----------------------------------------------------------------------------------
void  Camera::ShowImage(GX_FRAME_DATA* pFrameData)
{
	// ��ȡ����ͼ���Ƿ�Ϊ������ͼ��
	if (pFrameData->nStatus != 0)
	{
		return;
	}

	//��֧�ֲ�ɫ,ת��ΪRGBͼ������
	if (m_bIsColorFilter)
	{
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), true);
	}
	else
	{
		// �ڰ������Ҫ��ת���ݺ���ʾ
		for (int i = 0; i <m_nImageHeight; i++)
		{
			memcpy(m_pImgBuffer + i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf) + (m_nImageHeight - i - 1)*m_nImageWidth, (size_t)m_nImageWidth);
		}
	}

	//��ָ��������һ֡һ֡�Ļ�ͼ
	//DrawImg(m_pImgBuffer);
}


//---------------------------------------------------------------------------------
/**
\brief   ��ʾͼ��

\return  ��
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
\brief  BITMAPINFO����ת��ΪMat���ͣ�add by syf

\return
*/
//----------------------------------------------------------------------------------
void  Camera::BitmapinfoToMat(GX_FRAME_DATA* pFrameData)
{
	// ��ȡ����ͼ���Ƿ�Ϊ������ͼ��
	if (pFrameData->nStatus != 0)
	{
		return;
	}

	//��֧�ֲ�ɫ,ת��ΪRGBͼ������
	if (m_bIsColorFilter)
	{
		//��Raw8ͼ��ת��ΪRGBͼ���Թ���ʾ
		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), false);
	}
	else
	{
		// �ڰ������Ҫ��ת���ݺ���ʾ
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
\brief  BITMAPINFO����ת��ΪMat���ͣ�add by syf

\return
*/
//----------------------------------------------------------------------------------
void  Camera::BitmapinfoToMat(BYTE *pImageBuffer)
{
	// ��ȡ����ͼ���Ƿ�Ϊ������ͼ��
	if (pImageBuffer == NULL)
	{
		return;
	}

	if (pImageBuffer)
	{
		m_image.data = (uchar*)pImageBuffer;


		// ���Ͳɼ����õ�ͼ��
		//if (m_bIsImgEmit)
		{
			emit Signal_ImageSnaped();
			
			m_bIsImgEmit = false;
		}
	}
}



//---------------------------------------------------------------------------------
/**
\brief   ˢ����ʾ��ǰ���桢�ع�ֵ

\return  ��
*/
//---------------------------------------------------------------------------------
void Camera::RefreshCurValue()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	String   strTemp = "";
	double    dValue = 0;

	if (m_emAutoShutterMode != GX_EXPOSURE_AUTO_OFF)
	{
		//�����ǰѡ���ع�ΪONCEʱ����ȡ�豸��ʱ��ΪOFF�������½���
		if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_ONCE)
		{
			int64_t  nShutterEnumVal = 0;   //<   �����ع��ö��ֵ

			//��ȡ�Զ��ع���Ͽ�ؼ���ָ��
			//CComboBox *pAutoShutterBox = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_SHUTTER);

			//��ȡ�Զ��ع��ö��ֵ
			emStatus = GXGetEnum(m_hDevice, GX_ENUM_EXPOSURE_AUTO, &nShutterEnumVal);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return;
			}

			//����ת��
			m_emAutoShutterMode = (GX_EXPOSURE_AUTO_ENTRY)nShutterEnumVal;

			// �ж��豸�ع�ģʽ�Ƿ��ΪOFF
			//if (m_emAutoShutterMode == GX_EXPOSURE_AUTO_OFF)
			//{
			//	for (int i = 0; i < pAutoShutterBox->GetCount(); i++)
			//	{
			//		if ((int64_t)pAutoShutterBox->GetItemData(i) == GX_EXPOSURE_AUTO_OFF)
			//		{
			//			//ѡ���Զ��ع�ؼ��е�OFF��,����ONCE��ΪOFF
			//			pAutoShutterBox->SetCurSel(i);
			//			break;
			//		}
			//	}

			//	UpDateUI();
			//}
		}

		//��ȡ��ǰ�ع�ֵ�����½���
		emStatus = GXGetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, &dValue);
		m_dShutterValue = dValue;
		//strTemp.Format("%.4f", m_dShutterValue);
		//SetDlgItemText(IDC_EDIT_CUR_SHUTTER, strTemp);
	}

	//if (m_emAutoGainMode != GX_GAIN_AUTO_OFF)
	//{
	//	//�����ǰѡ������ΪONCEʱ����ȡ�豸��ʱ��ΪOFF�������½���
	//	if (m_emAutoGainMode == GX_GAIN_AUTO_ONCE)
	//	{
	//		int64_t  nGainEnumVal = 0;   //<   ���������ö��ֵ

	//		//��ȡ�Զ�������Ͽ�ؼ���ָ��
	//		CComboBox *pAutoGainBox = (CComboBox *)GetDlgItem(IDC_COMBO_AUTO_GAIN);

	//		//��ȡ�Զ������ö��ֵ
	//		emStatus = GXGetEnum(m_hDevice, GX_ENUM_GAIN_AUTO, &nGainEnumVal);
	//		if (emStatus != GX_STATUS_SUCCESS)
	//		{
	//			return;
	//		}

	//		// ����ת��
	//		m_emAutoGainMode = (GX_GAIN_AUTO_ENTRY)nGainEnumVal;

	//		//�ж��豸����ģʽ�Ƿ��ΪOFF
	//		if (m_emAutoGainMode == GX_GAIN_AUTO_OFF)
	//		{
	//			for (int i = 0; i < pAutoGainBox->GetCount(); i++)
	//			{
	//				if ((int64_t)pAutoGainBox->GetItemData(i) == GX_GAIN_AUTO_OFF)
	//				{
	//					//ѡ���Զ��ع�ؼ��е�OFF�����ONCE��ΪOFF
	//					pAutoGainBox->SetCurSel(i);
	//					break;
	//				}
	//			}

	//			UpDateUI();
	//		}
	//	}

	//	// ��ȡ��ǰ����ֵ�����½���
	//	emStatus = GXGetFloat(m_hDevice, GX_FLOAT_GAIN, &dValue);
	//	m_dGainValue = dValue;
	//	strTemp.Format("%.4f", m_dGainValue);
	//	SetDlgItemText(IDC_EDIT_CUR_GAIN, strTemp);
	//}
}


//----------------------------------------------------------------------------------
/**
\brief  ����������Ϣ
\param  emErrorStatus  [in] ������

\return ��
*/
//----------------------------------------------------------------------------------
void Camera::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize = 0;
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ��ȡ������Ϣ���ȣ��������ڴ���Դ
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// ��ȡ������Ϣ������ʾ
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		QMessageBox::information(this,
			"ERROR",
			QStringLiteral("GXGetLastError�ӿڵ���ʧ�ܣ�"));
	}
	else
	{
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QMessageBox::information(this,
			"ERROR",
			codec->toUnicode((const char*)pchErrorInfo));
	}

	// �ͷ�������ڴ���Դ
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  Ϊ����avi��Ƶ�ļ�׼����Դ

\return TRUE:׼���ɹ�  FALSE:׼����Դʧ��
*/
//----------------------------------------------------------------------------------
BOOL Camera::PrepareForSaveAVI()
{
	BOOL    bRet = FALSE;
	LONG    cbFormat = 0;    //�ļ���ʽ

	// ����RGBת����24λͼ��ͬ8λ�Ҷ�ͼ���ļ�����
	if (m_bIsColorFilter)
	{
		// ����RGBת����24ͼ�����ݸ�ʽ
		cbFormat = sizeof(BITMAPINFOHEADER);
	}
	else
	{
		// 8λ�Ҷ�ͼ���ļ���ʽ
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
	//����һ��AVI�ļ�	
	hr = AVIFileOpen(&m_pAVIFile, (LPCWSTR)wstrAVIFilePath.c_str(), OF_WRITE | OF_CREATE, NULL);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	// ����һ����Ƶ��
	::ZeroMemory(&StreamInfo, sizeof(StreamInfo));
	StreamInfo.fccType = streamtypeVIDEO;
	StreamInfo.fccHandler = 0;
	StreamInfo.dwScale = 1;
	StreamInfo.dwRate = m_wPlayFPS;  //AVI��Ƶ���Ĳ���֡��
	StreamInfo.dwSuggestedBufferSize = DWORD(m_nImageWidth * m_nImageHeight * 3);
	SetRect(&StreamInfo.rcFrame, 0, 0, (int)m_nImageWidth, (int)m_nImageHeight);
	hr = AVIFileCreateStream(m_pAVIFile, &m_ps, &StreamInfo);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	//��ʼ��ѹ����Ƶ����Ҫ�Ľṹ��
	AVICOMPRESSOPTIONS opts;
	//AVICOMPRESSOPTIONS FAR * aopts[1] = { &opts };

	if (m_bCompressedAVI)
	{
		if (m_hic == NULL)
		{
			return FALSE;
		}

		// ѹ���洢
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
		// ��ѹ���洢
		opts.fccHandler = mmioFOURCC('D', 'I', 'B', ' '); // 0x20424944 DIB 
	}

	//����ѹ����Ƶ��
	hr = AVIMakeCompressedStream(&m_psCompressed, m_ps, &opts, NULL);
	if (hr != AVIERR_OK)
	{
		return FALSE;
	}

	//������Ƶ���������ļ���ʽ��
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
\brief  �ͷŴ���AVI�ļ�����ʱ����Դ

\return ��
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
\brief  �ͷ���Ƶ�����������Դ

\return ��
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
\brief  ����avi��Ƶ:�ú������м����̿ռ�,����AVI�ļ���С����չ����
\param  pImageBuf  [in]  ָ��ͼ�񻺳�����ָ��

\return ��
*/
//----------------------------------------------------------------------------------
void Camera::ExSaveAVI(BYTE * pImageBuf)
{
	if (!m_bEnableSaveAVI)
	{
		return;
	}

	LONG nBufferSize = 0; // ʵ��д�뵽AVI�ļ���ͼ�����ݴ�С,����ѹ����Ϊѹ����Ĵ�С
	BOOL bRet = TRUE;

	// ---------------------------------------------
	// ��⵱ǰĿ¼���̿ռ�,��ʣ��ռ䲻��������洢ʱ��ֹͣ����
	if (GetDiskFreeSpace() < m_nFrameSize + AVI_FRAME_OFFSET)
	{
		m_bEnableSaveAVI = FALSE;
		m_SaverCriSec.Enter();
		UnPrepareForSaveAVI();
		m_SaverCriSec.Leave();
		return;
	}

	// �жϼ����洢ʱAVI�ļ���С�Ƿ��ﵽ��������ֵ
	// ���ﵽ�򴴽��µ�AVI�ļ�����¼��
	if ((m_nFrameSize + m_nWrittenDataSize + AVI_FRAME_OFFSET) > AVI_MAX_SIZE - AVI_HEADERSIZE)
	{
		bRet = ReCreatVedioFile();
		if (!bRet)
		{
			return;
		}

		m_nWrittenDataSize = 0;
	}

	// AVI��Ƶ����ʵ��
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		&nBufferSize);

	// �ж��Ƿ�д�ɹ�
	if (hr == AVIERR_OK)
	{
		m_nTimeFrame++;

		// ͳ��AVI�ļ����ݴ�С:ÿ֡ͼ��ռ�ÿռ�Ϊ���ݴ�С����ƫ��ֵ8
		m_nWrittenDataSize = m_nWrittenDataSize + nBufferSize + AVI_FRAME_OFFSET;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  ����avi��Ƶ:�ú���ֻʵ��дAVI�ļ�,������������չ����
\param  pImageBuf  [in]  ָ��ͼ�񻺳�����ָ��

\return ��
*/
//----------------------------------------------------------------------------------
void Camera::SaveAVI(BYTE * pImageBuf)
{
	if (!m_bEnableSaveAVI)
	{
		return;
	}

	// AVI��Ƶ����ʵ��
	HRESULT hr = AVIStreamWrite(m_psCompressed,
		m_nTimeFrame,
		1,
		pImageBuf,
		(LONG)m_nFrameSize,
		AVIIF_KEYFRAME,
		NULL,
		NULL);

	// �ж��Ƿ�д�ɹ�
	if (hr == AVIERR_OK)
	{
		m_nTimeFrame++;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  ֹͣ��ǰ¼��,���´���AVI��Ƶ�ļ�,Ȼ�����¼��

\return TRUE:���´����ɹ� FALSE:����ʧ��
*/
//----------------------------------------------------------------------------------
BOOL Camera::ReCreatVedioFile()
{
	BOOL bRet = TRUE;

	// ��ֹͣ���ڴ洢���ļ�
	m_SaverCriSec.Enter();
	UnPrepareForSaveAVI();
	m_SaverCriSec.Leave();

	// ���´���
	bRet = PrepareForSaveAVI();

	return bRet;
}



//----------------------------------------------------------------------------------
/**
\brief  ��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�

\return nFreeBytes   ʣ��ռ�,��λΪByte
*/
//----------------------------------------------------------------------------------
int64_t Camera::GetDiskFreeSpace()
{
	int64_t nFreeBytes = 0;
	BOOL    bResult;

	//ʹ��GetDiskFreeSpaceEx��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�
	wstring AVIFolder = m_strAVIFolder.toStdWString();
	bResult = GetDiskFreeSpaceEx((LPCWSTR)AVIFolder.c_str(), 0, 0,
		(PULARGE_INTEGER)&nFreeBytes);

	return nFreeBytes;
}


//=============================================================================
// ������
// ���أ�0--�ɹ�
//       1--ʧ��
// ���ܣ������
//=============================================================================
BOOL Camera::OpenCamera(void)
{
	GX_STATUS     emStatus = GX_STATUS_SUCCESS;
	uint32_t      nDevNum = 0;
	GX_OPEN_PARAM stOpenParam;

	// ��ʼ���豸�򿪲���
	stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
	stOpenParam.openMode = GX_OPEN_INDEX;
	stOpenParam.pszContent = (char *)("1");

	// ö���豸
	emStatus = GXUpdateDeviceList(&nDevNum, 1000);
	GX_VERIFY(emStatus);

	// �жϵ�ǰ�����豸����
	if (nDevNum <= 0)
	{
		QMessageBox::information(this,
			"ERROR",
			QStringLiteral("δ��������豸��"));
		return FALSE;
	}

	// ����豸�Ѿ�����ر�,��֤����ڳ�ʼ������������ٴδ�
	if (m_hDevice != NULL)
	{
		emStatus = GXCloseDevice(m_hDevice);
		GX_VERIFY(emStatus);
		m_hDevice = NULL;
	}

	// ���豸
	emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
	GX_VERIFY(emStatus);
	m_bDevOpened = TRUE;

	// ���������Ĭ�ϲ���:�ɼ�ģʽ:�����ɼ�,�������ģʽ������أ����ݸ�ʽ:8-bit
	emStatus = InitDevice();
	GX_VERIFY(emStatus);

	// ��ȡ�豸�Ŀ��ߵ�������Ϣ
	emStatus = GetDeviceParam();
	GX_VERIFY(emStatus);

	// ���ð�ƽ��ϵ��
	SetBalanceRatio(0, 1.18);
	SetBalanceRatio(1, 1.0);
	SetBalanceRatio(2, 1.36);


	return TRUE;
}


//=============================================================================
// ������
// ���أ�
// ���ܣ����ر�������ۺ�����
//       �ر���������ͷ������Դ��
//=============================================================================
void Camera::CloseCamera(void)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ������ڲɼ�ʱ����ֹͣ�ɼ�
	if (m_bIsSnap)
	{
		//����ֹͣ�ɼ�����
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			// ������
		}

		// �����ɼ�����ע���ص��������ͷ�Ϊ�ɼ�ͼ�񿪱ٵ�Buffer
		if (m_bIsContinuousSnap)
		{

			//ע���ص�
			emStatus = GXUnregisterCaptureCallback(m_hDevice);
			if (emStatus != GX_STATUS_SUCCESS)
			{
				// ������
			}

			m_bIsContinuousSnap = FALSE;
		}

		// �ͷ�Ϊ�ɼ�ͼ�񿪱ٵ�ͼ��Buffer
		UnPrepareForShowImg();

		m_bIsSnap = FALSE;
		m_bIsSingleSnap = FALSE;
	}

	emStatus = GXCloseDevice(m_hDevice);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		// ������
	}

	m_bDevOpened = FALSE;
	m_hDevice = NULL;
}

/* 
* ������mode--0,�����ɼ���1����������ɼ�
* ���أ�
* ���ܣ������ɼ�
*/
void Camera::StartSnap(int mode)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// ��ʼ��BMPͷ��Ϣ������BufferΪͼ��ɼ���׼��
	if (!PrepareForShowImg())
	{
		QMessageBox::warning(this,
			"ERROR",
			QStringLiteral("Ϊͼ����ʾ׼����Դʧ��!")
			);
		return;
	}

	if (0 == mode)
	{
		//ע��ص�
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			UnPrepareForShowImg();
			ShowErrorString(emStatus);
			return;
		}
	}

	//����ʼ�ɼ�����
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
* ������mode--0,�����ɼ���1����������ɼ�
* ���أ�
* ���ܣ�ֹͣ�ɼ�
*/
BOOL Camera::StopSnap(int mode)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	//����ֹͣ�ɼ�����
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
	GX_VERIFY(emStatus);

	if (0 == mode)
	{
		//ע���ص�
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		GX_VERIFY(emStatus);
	}
	
	// �ͷ�Ϊ�ɼ�ͼ�񿪱ٵ�ͼ��Buffer
	UnPrepareForShowImg();

	m_bIsSnap = FALSE;

	return TRUE;
}


//=============================================================================
// ������
// ���أ�
// ���ܣ��������ͼ��ɼ�
//=============================================================================
BOOL Camera::SoftwareTrigger(void)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	int timeout = 1000;


	//ÿ�η��ʹ�������֮ǰ��ղɼ��������
	//��ֹ���ڲ�����֡����ɱ���GXGetImage�õ���ͼ�����ϴη��ʹ����õ���ͼ
	emStatus = GXFlushQueue(m_hDevice);
	GX_VERIFY(emStatus);

	//������������
	emStatus = GXSendCommand(m_hDevice, GX_COMMAND_TRIGGER_SOFTWARE);
	GX_VERIFY(emStatus);


	//��ȡͼ��
	emStatus = GXGetImage(m_hDevice, &m_stFrameData, timeout);

	//�ж�GXGetImage����ֵ
	GX_VERIFY(emStatus);

	// ��highgui��ʾMat���ʹ򿪵�ͼƬ��add by syf
	BitmapinfoToMat(&m_stFrameData);

	return TRUE;
}


/*
* ������flag--false���ر��Զ���ƽ�⣬true�������Զ���ƽ��
* ���أ�
* ���ܣ��򿪻�ر��Զ���ƽ��(continuousģʽ)
*/
BOOL Camera::SetAutoWhiteBalance(int index)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// 0-off, 1-contiuous, 2-once
	int64_t nAwbMode = index;

	// 0-adaptive, 1-D65, 2-Fluoresence, Incandescent
	int64_t nAwbLightMode = 0;


	// ���ð�ƽ��
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, nAwbMode);
	GX_VERIFY(emStatus);

	if (index > 0)
	{
		// ���ð�ƽ���Դ
		emStatus = GXSetEnum(m_hDevice, GX_ENUM_AWB_LAMP_HOUSE, nAwbLightMode);
		GX_VERIFY(emStatus);
	}

	return TRUE;
}


/*
* ������index--��ƽ��ͨ��
        ratio--ϵ��
* ���أ�
* ���ܣ����ð�ƽ��ϵ��
*/
BOOL Camera::SetBalanceRatio(int index, double ratio)
{
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return FALSE;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// �ر��Զ���ƽ��
	emStatus = GXSetEnum(m_hDevice, GX_ENUM_BALANCE_WHITE_AUTO, 0);
	GX_VERIFY(emStatus);


	// ѡ���ƽ��ͨ��
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
* ������index--��ƽ��ͨ��
		ratio--ϵ��
* ���أ�
* ���ܣ����ð�ƽ��ϵ��
*/
BOOL Camera::GetBalanceRatio(int index, double* ratio)
{
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
	if (m_hDevice == NULL)
	{
		return FALSE;
	}

	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	// ѡ���ƽ��ͨ��
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
* ������flag--false���ر��Զ��ع⣬true�������Զ��ع�
* ���أ�
* ���ܣ��򿪻�ر��Զ��ع�
*/
BOOL Camera::SetAutoExposure(int index)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	
	// 0--off��1--continuous, 2-once
	int64_t  nExposureMode = index;


	emStatus = GXSetEnum(m_hDevice, GX_ENUM_EXPOSURE_AUTO, nExposureMode);
	GX_VERIFY(emStatus);

	return TRUE;
}


/*
* ������t--�ع�ʱ��
* ���أ�
* ���ܣ������ع�ʱ��
*/
BOOL Camera::SetExposure(double t)
{
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
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
* ������
* ���أ��ع�ʱ��
* ���ܣ���ȡ�����ǰ�ع�ʱ��
*/
double Camera::GetExposure()
{
	//�жϾ���Ƿ���Ч�������豸���ߺ�رճ�����ֵ��쳣
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
// ������
// ���أ�ture--ͼ���Ѳɼ��ã�false--ͼ��δ�ɼ���
// ���ܣ����ص�ǰͼ��ɼ�״̬
//=============================================================================
bool Camera::IsImgSnaped(void)
{
	return m_bIsImgSnaped;
}


//=============================================================================
// ������
// ���أ�
// ���ܣ���λͼ��ɼ�״̬
//=============================================================================
void Camera::ResetImgSnaped(void)
{
	m_bIsImgSnaped = false;
}


//=============================================================================
// ������
// ���أ�Mat���͵�ͼ������ָ��
// ���ܣ�����Mat���͵�ͼ������ָ��
//=============================================================================
Mat& Camera::GetImage(void)
{
	return m_image;
}
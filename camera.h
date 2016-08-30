/*******************************************************************************
�ļ����ƣ�camera.h
�������ڣ�2016-02-11
����޸ģ�2016-08-29
��	   ����v1.0.0
��    �ߣ�syf
����������������������ͷ�ļ�
*******************************************************************************/
#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <iostream>
#include <QWidget>
#include <QMessageBox>
#include <QString>
#include <QTextCodec>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "formatConvert.h"

//����AVI��Ƶ������ͷ�ļ�
#include <wtypes.h>
#include <atlbase.h>
#include <shlobj.h>

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

#define  AVI_MAX_SIZE   0x80000000  ///< AVI�ļ����ֵ:2GB


///< AVI��Ƶÿ֡ͼ����������ڸ�֡ͷ��ƫ��
#define AVI_FRAME_OFFSET  8

///< ������ʾ�����궨��
#define  GX_VERIFY(emStatus) \
if (emStatus != GX_STATUS_SUCCESS)\
{\
	ShowErrorString(emStatus); \
	return FALSE; \
}

///< �жϷ���ֵ�궨��
#define VERIFY_STATUS_RET(emStatus) \
if (emStatus != GX_STATUS_SUCCESS) \
{\
	return emStatus; \
}

class CVxCriticalSection
{
public:
	/// ����
	CVxCriticalSection() { InitializeCriticalSection(&m_cs); }
	/// ����
	virtual ~CVxCriticalSection() { DeleteCriticalSection(&m_cs); }
public:
	/// �����ٽ���
	void Enter(){ EnterCriticalSection(&m_cs); }
	/// �뿪�ٽ���
	void Leave(){ LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs; ///< �ٽ�α���	
};


using namespace cv;
using namespace std;

inline std::string  MsgComposer(const char* pFormat, ...)
{
	char chBuffer[256] = { 0 };
	va_list vap;
	va_start(vap, pFormat);

#if _MSC_VER >= 1400 //�����VS2005�������ϰ汾
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
#else if _MSC_VER == 1200 //�����VC6
	_vsnprintf(chBuffer, 256, pFormat, vap);
#endif

	va_end(vap);
	return std::string(chBuffer);
};

class Camera : public QWidget
{
	Q_OBJECT

public:
	explicit Camera(QWidget *parent = 0);
	~Camera();

	/// �ص�����
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// ���÷��Ͳɼ���ͼ���־
	void SetImgEmit(bool flag);

	/// ��ȡ����򿪱�־

	/// ��ȡ����ɼ���־
	BOOL IsSnap(void);

	/// ��ȡ���״̬
	BOOL IsDeviceOpened(void);

	/// ��ȡ�豸�Ŀ�ߵ�������Ϣ
	GX_STATUS GetDeviceParam();

	/// ������������ݸ�ʽ,ʹ�����ͼ��λ��Ϊ8-bit
	GX_STATUS SetPixelFormat8bit();

	/// �豸��ʼ��
	GX_STATUS InitDevice(void);

	///  Ϊ��ɫͼ����ʾ׼����Դ
	bool PrepareForShowColorImg();

	/// Ϊ�ڰ�ͼ����ʾ׼����Դ
	bool PrepareForShowMonoImg();

	/// Ϊͼ����ʾ׼����Դ,����Buffer
	bool PrepareForShowImg();

	/// �ͷ�Ϊͼ����ʾ׼������Դ
	void UnPrepareForShowImg();

	/// ��ȡͼ��
	void  ShowImage(GX_FRAME_DATA* pFrameData);

	/// Bitmapinfoת����ΪMat��ʽ
	void BitmapinfoToMat(GX_FRAME_DATA* pFrameData);
	void BitmapinfoToMat(BYTE *pImageBuffer);

	/// ��ʾͼ��
	//void DrawImg(BYTE *pImageBuffer);

	/// ˢ����ʾ�ع�����浱ǰֵ
	void RefreshCurValue();

	///��ȡ������Ϣ
	void ShowErrorString(GX_STATUS emErrorStatus);

	//-------------------------------------------
	// AVI ��Ƶ������غ���
	//-------------------------------------------

	/// Ϊ����avi��Ƶ�ļ�׼����Դ
	BOOL PrepareForSaveAVI();

	/// �ͷ�Ϊ����AVI��Ƶʹ�õ���Դ
	void UnPrepareForSaveAVI();

	/// ����AVI��Ƶʵ�ֺ���:�ú������м����̿ռ�,����AVI�ļ���С����չ����
	void ExSaveAVI(BYTE * pImageBuf);

	/// ����AVI��Ƶʵ�ֺ���:�ú���ֻʵ��дAVI�ļ�,������������չ����
	void SaveAVI(BYTE *pImageBuf);

	/// ��ȡ�洢Ŀ¼���ڴ��̵�ʣ��ռ�
	int64_t GetDiskFreeSpace();

	/// �ͷ���Ƶ�����������Դ
	void ResetCompressor();

	/// ֹͣ��ǰ¼�����´���AVI��Ƶ�ļ�
	BOOL ReCreatVedioFile();

	/// �����������ʼ�ɼ�
	BOOL OpenCamera(void);

	/// �ر���������ͷ�ռ����Դ
	void CloseCamera(void);

	/// ��ʼ�ɼ�
	void StartSnap(int mode);

	/// ֹͣ�ɼ�
	BOOL StopSnap(int mode);

	/// �������ͼ��ɼ�
	BOOL SoftwareTrigger(void);

	/// �����Զ���ƽ��
	BOOL SetAutoWhiteBalance(int index);

	/// ���ð�ƽ��ϵ��
	BOOL SetBalanceRatio(void);

	/// �����Զ��ع�
	BOOL SetAutoExposure(int index);

	/// �����ع�ʱ��
	BOOL SetExposure(double t);

	/// ��ȡ�ع�ʱ��
	double GetExposure(void);

	/// ͼ�������Ƿ��ѱ���
	bool IsImgSnaped(void);

	/// ͼ����ʾ�󣬸�λͼ�����ݱ����־
	void ResetImgSnaped(void);

	/// ����ͼ��Mat����ͼ������ָ��
	Mat& GetImage(void);

signals:
	void Signal_ImageSnaped();

private:
	bool					m_bIsImgEmit;			  /// ���Ͳɼ�����ͼ���־
	//----------------������ͼ��ɼ���س�Ա--------------------------
	GX_DEV_HANDLE           m_hDevice;                ///< �豸���
	int64_t                 m_nImageWidth;            ///< ������ͼ����
	int64_t                 m_nImageHeight;           ///< ������ͼ��߶�
	int64_t                 m_nPayLoadSize;           ///< �豸���ԭʼͼ���С
	int64_t                 m_nPixelColorFilter;      ///< ��ɫ�����Bayer��ʽ

	GX_FLOAT_RANGE          m_stShutterFloatRange;    ///< �����ع�ʱ�䷶Χ
	GX_FLOAT_RANGE          m_stGainFloatRange;       ///< ��������ֵ��Χ           

	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	GX_FRAME_DATA			m_stFrameData;			  ///< GetImage��ȡ��ͼ���ַ

	BOOL                    m_bDevOpened;             ///< ��ʶ�豸�Ƿ��Ѵ�
	BOOL                    m_bIsSnap;                ///< ��ʶ�豸�Ƿ��ѿ���
	bool					m_bIsImgSnaped;			  ///< ���βɼ���ͼ�������Ƿ��ѱ���
	bool                    m_bIsColorFilter;         ///< �ж�����Ƿ�֧��Bayer��ʽ
	bool                    m_bImplementAutoGain;     ///< �Ƿ�֧���Զ�����
	bool                    m_bImplementAutoShutter;  ///< �Ƿ�֧���Զ��ع�
	bool                    m_bImplementLight;        ///< �Ƿ�֧��2A���ջ�������
	bool                    m_bIsContinuousSnap;	  ///< ��ʶ�豸�Ƿ������ɼ�
	bool					m_bIsSingleSnap;		  ///< ��ʶ�豸�Ƿ񵥴ʲɼ�	
	UINT					m_nEditTimeOut;		      ///< ���δ����ɼ���ʱʱ��		
	Mat 					m_image;				  ///< Mat����ͼ�����

	BYTE                   *m_pRawBuffer;             ///< ���ԭʼRAWͼ��Buffer
	BYTE                   *m_pImgBuffer;             ///< ָ�򾭹�������ͼ�����ݻ�����
	char                    m_chBmpBuf[2048];         ///< BIMTAPINFO�洢��������m_pBmpInfo��ָ��˻�����
	GX_EXPOSURE_AUTO_ENTRY  m_emAutoShutterMode;      ///< �Զ��ع�ģʽ
	GX_GAIN_AUTO_ENTRY      m_emAutoGainMode;         ///< �Զ�����ģʽ

	//----------------AVI ��Ƶ������س�Ա--------------------------
	bool       m_bEnableSaveAVI;              ///< �Ƿ�洢AVI��Ƶ
	int64_t    m_nFrameSize;                  ///< ����AVI��Ƶ����ʱÿ֡�Ĵ�С
	int64_t    m_nWrittenDataSize;            ///< д��AVI�ļ���ͼ�����ݴ�С��������AVI�ļ�ͷ��С��
	PAVIFILE       m_pAVIFile;                ///< AVI�ļ��ľ��
	PAVISTREAM     m_ps;                      ///< ͨ��AVI�ļ���������Ƶ��
	PAVISTREAM	   m_psCompressed;            ///< ͨ����Ƶ������Ƶѹ����������ѹ����Ƶ��
	LONG           m_nTimeFrame;              ///< д��avi�ļ���Ҫ��֡ID��
	HIC            m_hic;                     ///< ���ʹ�õ���Ƶѹ�������
	COMPVARS       m_Com;                     ///< ѡ�������ʹ�ò���
	CVxCriticalSection m_SaverCriSec;         ///< ���滥����

	QString	m_strAVIFolder;
	BOOL	m_bCompressedAVI;
	BOOL	m_bShowImage;
	DWORD	m_wPlayFPS;

	int		m_nGray;               ///< �����Ҷ�ֵ
	int		m_nRoiX;               ///< 2A����Ȥ����X����
	int		m_nRoiY;               ///< 2A����Ȥ����Y����
	int		m_nRoiH;               ///< 2A����Ȥ�����
	int		m_nRoiW;               ///< 2A����Ȥ�����
	double	m_dAutoGainMin;        ///< �Զ�������Сֵ
	double  m_dAutoGainMax;        ///< �Զ��������ֵ
	double	m_dAutoShutterMin;     ///< �Զ��ع���Сֵ
	double	m_dAutoShutterMax;     ///< �Զ��ع����ֵ
	double	m_dShutterValue;       ///< �ع�ֵ
	double	m_dGainValue;          ///< ����ֵ  
	
};

#endif // CAMERA_H

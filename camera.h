/*******************************************************************************
文件名称：camera.h
创建日期：2016-02-11
最后修改：2016-08-29
版	   本：v1.0.0
作    者：syf
功能描述：大恒相机操作类头文件
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

//保存AVI视频依赖的头文件
#include <wtypes.h>
#include <atlbase.h>
#include <shlobj.h>

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

#define  AVI_MAX_SIZE   0x80000000  ///< AVI文件最大值:2GB


///< AVI视频每帧图像数据相对于该帧头的偏移
#define AVI_FRAME_OFFSET  8

///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
if (emStatus != GX_STATUS_SUCCESS)\
{\
	ShowErrorString(emStatus); \
	return FALSE; \
}

///< 判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
if (emStatus != GX_STATUS_SUCCESS) \
{\
	return emStatus; \
}

class CVxCriticalSection
{
public:
	/// 构造
	CVxCriticalSection() { InitializeCriticalSection(&m_cs); }
	/// 析构
	virtual ~CVxCriticalSection() { DeleteCriticalSection(&m_cs); }
public:
	/// 进入临界区
	void Enter(){ EnterCriticalSection(&m_cs); }
	/// 离开临界区
	void Leave(){ LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs; ///< 临界段变量	
};


using namespace cv;
using namespace std;

inline std::string  MsgComposer(const char* pFormat, ...)
{
	char chBuffer[256] = { 0 };
	va_list vap;
	va_start(vap, pFormat);

#if _MSC_VER >= 1400 //如果是VS2005或者以上版本
	_vsnprintf_s(chBuffer, 256, pFormat, vap);
#else if _MSC_VER == 1200 //如果是VC6
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

	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);

	/// 设置发送采集到图像标志
	void SetImgEmit(bool flag);

	/// 获取相机打开标志

	/// 获取相机采集标志
	BOOL IsSnap(void);

	/// 获取相机状态
	BOOL IsDeviceOpened(void);

	/// 获取设备的宽高等属性信息
	GX_STATUS GetDeviceParam();

	/// 设置相机的数据格式,使其输出图像位深为8-bit
	GX_STATUS SetPixelFormat8bit();

	/// 设备初始化
	GX_STATUS InitDevice(void);

	///  为彩色图像显示准备资源
	bool PrepareForShowColorImg();

	/// 为黑白图像显示准备资源
	bool PrepareForShowMonoImg();

	/// 为图像显示准备资源,分配Buffer
	bool PrepareForShowImg();

	/// 释放为图像显示准备的资源
	void UnPrepareForShowImg();

	/// 获取图像
	void  ShowImage(GX_FRAME_DATA* pFrameData);

	/// Bitmapinfo转换额为Mat格式
	void BitmapinfoToMat(GX_FRAME_DATA* pFrameData);
	void BitmapinfoToMat(BYTE *pImageBuffer);

	/// 显示图像
	//void DrawImg(BYTE *pImageBuffer);

	/// 刷新显示曝光和增益当前值
	void RefreshCurValue();

	///获取错误信息
	void ShowErrorString(GX_STATUS emErrorStatus);

	//-------------------------------------------
	// AVI 视频保存相关函数
	//-------------------------------------------

	/// 为保存avi视频文件准备资源
	BOOL PrepareForSaveAVI();

	/// 释放为保存AVI视频使用的资源
	void UnPrepareForSaveAVI();

	/// 保存AVI视频实现函数:该函数带有检测磁盘空间,限制AVI文件大小等扩展功能
	void ExSaveAVI(BYTE * pImageBuf);

	/// 保存AVI视频实现函数:该函数只实现写AVI文件,不带有其他扩展功能
	void SaveAVI(BYTE *pImageBuf);

	/// 获取存储目录所在磁盘的剩余空间
	int64_t GetDiskFreeSpace();

	/// 释放视频编码器相关资源
	void ResetCompressor();

	/// 停止当前录制重新创建AVI视频文件
	BOOL ReCreatVedioFile();

	/// 打开相机，并开始采集
	BOOL OpenCamera(void);

	/// 关闭相机，并释放占用资源
	void CloseCamera(void);

	/// 开始采集
	void StartSnap(int mode);

	/// 停止采集
	BOOL StopSnap(int mode);

	/// 软件触发图像采集
	BOOL SoftwareTrigger(void);

	/// 设置自动白平衡
	BOOL SetAutoWhiteBalance(int index);

	/// 设置白平衡系数
	BOOL SetBalanceRatio(void);

	/// 设置自动曝光
	BOOL SetAutoExposure(int index);

	/// 设置曝光时间
	BOOL SetExposure(double t);

	/// 获取曝光时间
	double GetExposure(void);

	/// 图像数据是否已保存
	bool IsImgSnaped(void);

	/// 图像显示后，复位图像数据保存标志
	void ResetImgSnaped(void);

	/// 返回图像Mat类型图像数据指针
	Mat& GetImage(void);

signals:
	void Signal_ImageSnaped();

private:
	bool					m_bIsImgEmit;			  /// 发送采集到的图像标志
	//----------------大恒相机图像采集相关成员--------------------------
	GX_DEV_HANDLE           m_hDevice;                ///< 设备句柄
	int64_t                 m_nImageWidth;            ///< 相机输出图像宽度
	int64_t                 m_nImageHeight;           ///< 相机输出图像高度
	int64_t                 m_nPayLoadSize;           ///< 设备输出原始图像大小
	int64_t                 m_nPixelColorFilter;      ///< 彩色相机的Bayer格式

	GX_FLOAT_RANGE          m_stShutterFloatRange;    ///< 保存曝光时间范围
	GX_FLOAT_RANGE          m_stGainFloatRange;       ///< 保存增益值范围           

	BITMAPINFO             *m_pBmpInfo;	              ///< BITMAPINFO 结构指针，显示图像时使用
	GX_FRAME_DATA			m_stFrameData;			  ///< GetImage获取的图像地址

	BOOL                    m_bDevOpened;             ///< 标识设备是否已打开
	BOOL                    m_bIsSnap;                ///< 标识设备是否已开采
	bool					m_bIsImgSnaped;			  ///< 单次采集，图像数据是否已保存
	bool                    m_bIsColorFilter;         ///< 判断相机是否支持Bayer格式
	bool                    m_bImplementAutoGain;     ///< 是否支持自动增益
	bool                    m_bImplementAutoShutter;  ///< 是否支持自动曝光
	bool                    m_bImplementLight;        ///< 是否支持2A光照环境功能
	bool                    m_bIsContinuousSnap;	  ///< 标识设备是否连续采集
	bool					m_bIsSingleSnap;		  ///< 标识设备是否单词采集	
	UINT					m_nEditTimeOut;		      ///< 单次触发采集超时时间		
	Mat 					m_image;				  ///< Mat类型图像矩阵

	BYTE                   *m_pRawBuffer;             ///< 存放原始RAW图的Buffer
	BYTE                   *m_pImgBuffer;             ///< 指向经过处理后的图像数据缓冲区
	char                    m_chBmpBuf[2048];         ///< BIMTAPINFO存储缓冲区，m_pBmpInfo即指向此缓冲区
	GX_EXPOSURE_AUTO_ENTRY  m_emAutoShutterMode;      ///< 自动曝光模式
	GX_GAIN_AUTO_ENTRY      m_emAutoGainMode;         ///< 自动增益模式

	//----------------AVI 视频保存相关成员--------------------------
	bool       m_bEnableSaveAVI;              ///< 是否存储AVI视频
	int64_t    m_nFrameSize;                  ///< 计算AVI视频保存时每帧的大小
	int64_t    m_nWrittenDataSize;            ///< 写到AVI文件的图像数据大小（不包含AVI文件头大小）
	PAVIFILE       m_pAVIFile;                ///< AVI文件的句柄
	PAVISTREAM     m_ps;                      ///< 通过AVI文件创建的视频流
	PAVISTREAM	   m_psCompressed;            ///< 通过视频流和视频压缩器创建的压缩视频流
	LONG           m_nTimeFrame;              ///< 写入avi文件需要的帧ID号
	HIC            m_hic;                     ///< 标记使用的视频压缩器句柄
	COMPVARS       m_Com;                     ///< 选择编码器使用参数
	CVxCriticalSection m_SaverCriSec;         ///< 保存互斥锁

	QString	m_strAVIFolder;
	BOOL	m_bCompressedAVI;
	BOOL	m_bShowImage;
	DWORD	m_wPlayFPS;

	int		m_nGray;               ///< 期望灰度值
	int		m_nRoiX;               ///< 2A感兴趣区域X坐标
	int		m_nRoiY;               ///< 2A感兴趣区域Y坐标
	int		m_nRoiH;               ///< 2A感兴趣区域高
	int		m_nRoiW;               ///< 2A感兴趣区域宽
	double	m_dAutoGainMin;        ///< 自动增益最小值
	double  m_dAutoGainMax;        ///< 自动增益最大值
	double	m_dAutoShutterMin;     ///< 自动曝光最小值
	double	m_dAutoShutterMax;     ///< 自动曝光最大值
	double	m_dShutterValue;       ///< 曝光值
	double	m_dGainValue;          ///< 增益值  
	
};

#endif // CAMERA_H

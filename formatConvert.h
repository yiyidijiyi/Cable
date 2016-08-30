/*******************************************************************************
文件名称：formatConvert.h
创建日期：2016-01-11
最后修改：2016-03-04
版	   本：v1.0.0
作    者：syf
功能描述：格式状态全局函数
*******************************************************************************/
#ifndef __FORMAT_CONVERT__
#define __FORMAT_CONVERT__
#include <string>
#include "windows.h"
#include <stdint.h>
#include <stringapiset.h>
#include <QString>



BOOL StringToWString(const std::string &str, std::wstring &wstr);
//wstring高字节不为0，返回FALSE
BOOL WStringToString(const std::wstring &wstr, std::string &str);
std::wstring StringToWString(const std::string &str);
//只拷贝低字节至string中
std::string WStringToString(const std::wstring &wstr);

int OneUnicodeToUTF8(unsigned long unic, unsigned char *pOutput);
int GetUTF8Size(unsigned char ch);
int OneUTFToUnicode(const unsigned char* pInput, unsigned long *pUnic);

void QStringToHtmlFilter(QString &str);
void QStringToHtml(QString &str);
void ImgPathToHtml(QString &path);

#endif	// __FORMAT_CONVERT__
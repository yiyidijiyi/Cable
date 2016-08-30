/*******************************************************************************
�ļ����ƣ�formatConvert.h
�������ڣ�2016-01-11
����޸ģ�2016-03-04
��	   ����v1.0.0
��    �ߣ�syf
������������ʽ״̬ȫ�ֺ���
*******************************************************************************/
#ifndef __FORMAT_CONVERT__
#define __FORMAT_CONVERT__
#include <string>
#include "windows.h"
#include <stdint.h>
#include <stringapiset.h>
#include <QString>



BOOL StringToWString(const std::string &str, std::wstring &wstr);
//wstring���ֽڲ�Ϊ0������FALSE
BOOL WStringToString(const std::wstring &wstr, std::string &str);
std::wstring StringToWString(const std::string &str);
//ֻ�������ֽ���string��
std::string WStringToString(const std::wstring &wstr);

int OneUnicodeToUTF8(unsigned long unic, unsigned char *pOutput);
int GetUTF8Size(unsigned char ch);
int OneUTFToUnicode(const unsigned char* pInput, unsigned long *pUnic);

void QStringToHtmlFilter(QString &str);
void QStringToHtml(QString &str);
void ImgPathToHtml(QString &path);

#endif	// __FORMAT_CONVERT__
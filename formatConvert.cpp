/*******************************************************************************
文件名称：formatConvert.c
创建日期：2016-01-11
最后修改：2016-03-04
版	   本：v1.0.0
作    者：syf
功能描述：格式状态全局函数
*******************************************************************************/
#include "formatConvert.h"


//=============================================================================
// 参数：
// 返回：
// 功能：string转换为wstring
//=============================================================================
BOOL StringToWString(const std::string &str, std::wstring &wstr)
{
	int nLen = (int)str.length();
	wstr.resize(nLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}


//=============================================================================
// 参数：
// 返回：
// 功能：wstring转换为sstring，wstring高字节不为0，返回FALSE
//=============================================================================
BOOL WStringToString(const std::wstring &wstr, std::string &str)
{
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');

	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}


//=============================================================================
// 参数：
// 返回：
// 功能：string转换为wstring
//=============================================================================
std::wstring StringToWString(const std::string &str)
{
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}


//=============================================================================
// 参数：
// 返回：
// 功能：wstring转换为string，只拷贝低字节至string中
//=============================================================================
std::string WStringToString(const std::wstring &wstr)
{
	std::string str(wstr.length(), ' ');
	std::copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}



/*****************************************************************************
* 将一个字符的Unicode(UCS-2和UCS-4)编码转换成UTF-8编码.
*
* 参数:
*    unic     字符的Unicode编码值
*    pOutput  指向输出的用于存储UTF8编码值的缓冲区的指针
*
* 返回值:
*    返回转换后的字符的UTF8编码所占的字节数, 如果出错则返回 0 .
*
* 注意:
*     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
*        字节序分为大端(Big Endian)和小端(Little Endian)两种;
*        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
*     2. 请保证 pOutput 缓冲区有最少有 6 字节的空间大小!
****************************************************************************/
int OneUnicodeToUTF8(unsigned long unic, unsigned char *pOutput)
{
	if (NULL == pOutput)
	{
		return  0;
	}

	if (unic <= 0x0000007F)
	{
		// * U-00000000 - U-0000007F:  0xxxxxxx  
		*pOutput = (unic & 0x7F);
		return 1;
	}
	else if (unic >= 0x00000080 && unic <= 0x000007FF)
	{
		// * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
		*(pOutput + 1) = (unic & 0x3F) | 0x80;
		*pOutput = ((unic >> 6) & 0x1F) | 0xC0;
		return 2;
	}
	else if (unic >= 0x00000800 && unic <= 0x0000FFFF)
	{
		// * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
		*(pOutput + 2) = (unic & 0x3F) | 0x80;
		*(pOutput + 1) = ((unic >> 6) & 0x3F) | 0x80;
		*pOutput = ((unic >> 12) & 0x0F) | 0xE0;
		return 3;
	}
	else if (unic >= 0x00010000 && unic <= 0x001FFFFF)
	{
		// * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
		*(pOutput + 3) = (unic & 0x3F) | 0x80;
		*(pOutput + 2) = ((unic >> 6) & 0x3F) | 0x80;
		*(pOutput + 1) = ((unic >> 12) & 0x3F) | 0x80;
		*pOutput = ((unic >> 18) & 0x07) | 0xF0;
		return 4;
	}
	else if (unic >= 0x00200000 && unic <= 0x03FFFFFF)
	{
		// * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
		*(pOutput + 4) = (unic & 0x3F) | 0x80;
		*(pOutput + 3) = ((unic >> 6) & 0x3F) | 0x80;
		*(pOutput + 2) = ((unic >> 12) & 0x3F) | 0x80;
		*(pOutput + 1) = ((unic >> 18) & 0x3F) | 0x80;
		*pOutput = ((unic >> 24) & 0x03) | 0xF8;
		return 5;
	}
	else if (unic >= 0x04000000 && unic <= 0x7FFFFFFF)
	{
		// * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
		*(pOutput + 5) = (unic & 0x3F) | 0x80;
		*(pOutput + 4) = ((unic >> 6) & 0x3F) | 0x80;
		*(pOutput + 3) = ((unic >> 12) & 0x3F) | 0x80;
		*(pOutput + 2) = ((unic >> 18) & 0x3F) | 0x80;
		*(pOutput + 1) = ((unic >> 24) & 0x3F) | 0x80;
		*pOutput = ((unic >> 30) & 0x01) | 0xFC;
		return 6;
	}

	return 0;
}


/*****************************************************************************
* 获得的UTF8编码表示Unicode时使用的字节长.
*
* 参数:
*    pInput      指向输入缓冲区, 以UTF-8编码 .
*
* 返回值:
*    成功则返回该字符的UTF8编码所占用的字节数，失败返回0
****************************************************************************/
int GetUTF8Size(unsigned char ch)
{
	int n = 0;

	for (int i = 0; i < 8; i++)
	{
		if (ch & 0x80)
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if (!n)
	{
		// 0xxxxxxx
		return 1;
	}
	else if ((n > 6) || (1 == n))
	{
		// 11111111
		// 11111110
		// 10xxxxxx
		return 0;
	}
	else
	{
		// 110xxxxx
		// 1110xxxx
		// 11110xxx
		// 111110xx
		// 1111110x
		return n;
	}
}


/*****************************************************************************
* 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
*
* 参数:
*    pInput      指向输入缓冲区, 以UTF-8编码
*    Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
*                类型为unsigned long .
*
* 返回值:
*    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
*
* 注意:
*     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
*        字节序分为大端(Big Endian)和小端(Little Endian)两种;
*        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
****************************************************************************/
int OneUTFToUnicode(const unsigned char* pInput, unsigned long *pUnic)
{

	// b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...  
	char b1, b2, b3, b4, b5, b6;

	*pUnic = 0x0; // 把 *Unic 初始化为全零  
	int utfbytes = GetUTF8Size(*pInput);
	if (!utfbytes)
	{
		return 0;
	}

	unsigned char *pOutput = (unsigned char *)pUnic;

	switch (utfbytes)
	{
	case 0:
		*pOutput = *pInput;
		utfbytes += 1;
		break;
	case 2:
		b1 = *pInput;
		b2 = *(pInput + 1);
		if ((b2 & 0xE0) != 0x80)
			return 0;
		*pOutput = (b1 << 6) + (b2 & 0x3F);
		*(pOutput + 1) = (b1 >> 2) & 0x07;
		break;
	case 3:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b2 << 6) + (b3 & 0x3F);
		*(pOutput + 1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
		break;
	case 4:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b3 << 6) + (b4 & 0x3F);
		*(pOutput + 1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
		*(pOutput + 2) = ((b1 << 2) & 0x1C) + ((b2 >> 4) & 0x03);
		break;
	case 5:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		b5 = *(pInput + 4);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b4 << 6) + (b5 & 0x3F);
		*(pOutput + 1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
		*(pOutput + 2) = (b2 << 2) + ((b3 >> 4) & 0x03);
		*(pOutput + 3) = (b1 << 6);
		break;
	case 6:
		b1 = *pInput;
		b2 = *(pInput + 1);
		b3 = *(pInput + 2);
		b4 = *(pInput + 3);
		b5 = *(pInput + 4);
		b6 = *(pInput + 5);
		if (((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
			|| ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
			|| ((b6 & 0xC0) != 0x80))
			return 0;
		*pOutput = (b5 << 6) + (b6 & 0x3F);
		*(pOutput + 1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
		*(pOutput + 2) = (b3 << 2) + ((b4 >> 4) & 0x03);
		*(pOutput + 3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
		break;
	default:
		return 0;
		break;
	}

	return utfbytes;
}


//=============================================================================
// 参数：
// 返回：
// 功能：过滤QString中个特殊字符，使得装换成html时可以正常显示
//=============================================================================
void QStringToHtmlFilter(QString &str)
{
	// 一下代码的顺序不能更改，否则会造成多次替换
	str.replace("&", "&amp;");
	str.replace(">", "&gt;");
	str.replace("<", "&lt;");
	str.replace("\"", "&quot;");
	str.replace("\'", "&#39;");
	str.replace(" ", "&nbsp;");
	str.replace("\n", "<br>");
	str.replace("\r", "<br>");
}


//=============================================================================
// 参数：
// 返回：
// 功能：QString，转换为html
//=============================================================================
void QStringToHtml(QString &str)
{
	QStringToHtmlFilter(str);

	str = QString("<span>%1</span>").arg(str);
}


//=============================================================================
// 参数：
// 返回：
// 功能：QString的图片路径，转换为html
//=============================================================================
void ImgPathToHtml(QString &path)
{
	path = QString("<img src=\"%1\"/>").arg(path);
}
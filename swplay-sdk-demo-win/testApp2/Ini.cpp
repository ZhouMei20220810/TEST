#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define SW_LOG_TAG "Ini"
#include "common/log.h"

#include "Ini.h"

/******************************************************************************
* 功  能：构造函数
* 参  数：无
* 返回值：无
* 备  注：
******************************************************************************/
CIni::CIni()
{
	m_fp = NULL;
}

/******************************************************************************
* 功  能：析构函数
* 参  数：无
* 返回值：无
* 备  注：
******************************************************************************/

CIni::~CIni()
{
	m_Map.clear();
}

/******************************************************************************
* 功  能：打开文件函数
* 参  数：无
* 返回值：
* 备  注：
******************************************************************************/
INI_RES CIni::OpenFile(const char* pathName, const char* type)
{
	std::string szLine, szMainKey, szLastMainKey("default"), szSubKey;
	char strLine[CONFIGLEN] = { 0 };
	KEYMAP mLastMap;
	size_t  nIndexPos = 0;
	size_t  nLeftPos = 0;
	size_t  nRightPos = 0;

	m_fp = fopen(pathName, type);
	if (m_fp == NULL)
	{
		ALOGE("open inifile %s error!", pathName);
		return INI_OPENFILE_ERROR;
	}

	m_Map.clear();

	while (fgets(strLine, CONFIGLEN, m_fp))
	{
		szLine.assign(strLine);
		//删除字符串中的非必要字符
        //去除前缀空格
        nLeftPos = szLine.find_first_not_of(' ') ;
        if (std::string::npos != nLeftPos && nLeftPos >= 0)
        {
            szLine.erase(0, nLeftPos) ;
        }
        // 注释行，跳过
        if ('#' == szLine[0])
            continue ;
        // 去除后缀换行符
		nLeftPos = szLine.find("\n");
		if (std::string::npos != nLeftPos && nLeftPos >= 0)
		{
			szLine.erase(nLeftPos, 1);
		}
        // 去除后缀换行符
		nLeftPos = szLine.find("\r");
		if (std::string::npos != nLeftPos && nLeftPos >= 0)
		{
			szLine.erase(nLeftPos, 1);
		}
        // 去除注释（从第一个#开始往后的都是注释）
        while (std::string::npos != szLine.find('#'))
        {
            nLeftPos = szLine.find_last_of('#') ;
            szLine.erase(nLeftPos, szLine.size()) ;
        }
        // 去除后缀空格
        nLeftPos = szLine.find_last_not_of(' ') ;
        if (std::string::npos != nLeftPos && nLeftPos >= 0)
        {
            szLine.erase(nLeftPos+1, szLine.size()) ;
        }
		//判断是否是主键
		nLeftPos = szLine.find("[");
		nRightPos = szLine.find("]");
		if (nLeftPos != std::string::npos && nLeftPos >= 0
			&& nRightPos != std::string::npos && nRightPos >= 0)
		{
			szLine.erase(nLeftPos, 1);
			nRightPos--;
			szLine.erase(nRightPos, 1);
			m_Map[szLastMainKey] = mLastMap;
			mLastMap.clear();
			szLastMainKey = szLine;
		}
		else
		{
			//是否是子键
			nIndexPos = szLine.find("=");
			if (std::string::npos != nIndexPos && nIndexPos >= 0)
			{
				std::string szSubKey, szSubValue;
				szSubKey = szLine.substr(0, nIndexPos);
				szSubValue = szLine.substr(nIndexPos + 1, szLine.length() - nIndexPos - 1);
				mLastMap[szSubKey] = szSubValue;
			}
			else
			{
				//TODO:不符合ini键值模板的内容 如注释等
			}
		}
	}
	//插入最后一次主键
	m_Map[szLastMainKey] = mLastMap;

	return INI_SUCCESS;
}

/******************************************************************************
* 功  能：关闭文件函数
* 参  数：无
* 返回值：
* 备  注：
******************************************************************************/
INI_RES CIni::CloseFile()
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}

	return INI_SUCCESS;
}

/******************************************************************************
* 功  能：获取[SECTION]下的某一个键值的字符串
* 参  数：
* label  输入参数 主键
* attr   输入参数 子键
* char* value  输出参数 子键键值
* 返回值：
* 备  注：
******************************************************************************/
INI_RES CIni::GetKey(const std::string& label, const std::string&attr, char* value, int length)
{
	if (m_Map.find(label) != m_Map.end()) {
		KEYMAP& keyMap = m_Map[label];

		if (keyMap.find(attr) != keyMap.end()) {
			std::string& temp = keyMap[attr];
			snprintf(value, length, "%s", temp.c_str());
			return INI_SUCCESS;
		}
	}
	return INI_NO_ATTR;
}

/******************************************************************************
* 功  能：获取整形的键值
* 参  数：
* label                     主键
* attr                     子键
* 返回值：正常则返回对应的数值 未读取成功则返回0(键值本身为0不冲突)
* 备  注：
******************************************************************************/
int CIni::GetInt(const char* label, const char* attr, int defaultValue)
{
	memset(m_szKey, 0, sizeof(m_szKey));

	if (INI_SUCCESS == GetKey(std::string(label), std::string(attr), m_szKey, sizeof(m_szKey)))
	{
		int nRes = atoi(m_szKey);
		return nRes;
	}
	else {
		return defaultValue;
	}
}

float CIni::GetFloat(const char* label, const char* attr, float defaultValue) {
	memset(m_szKey, 0, sizeof(m_szKey));

	if (INI_SUCCESS == GetKey(std::string(label), std::string(attr), m_szKey, sizeof(m_szKey)))
	{
		float nRes = (float)atof(m_szKey);
		return nRes;
	}
	else {
		return defaultValue;
	}
}

/******************************************************************************
* 功  能：获取键值的字符串
* 参  数：
* label                     主键
* attr                     子键
* 返回值：正常则返回读取到的子键字符串 未读取成功则返回"NULL"
* 备  注：
******************************************************************************/
const char *CIni::GetStr(const char* label, const char* attr, const char* defaultValue)
{
	memset(m_szKey, 0, sizeof(m_szKey));

	if (INI_SUCCESS != GetKey(std::string(label), std::string(attr), m_szKey, sizeof(m_szKey)))
	{
		return defaultValue;
	}
	return m_szKey;
}

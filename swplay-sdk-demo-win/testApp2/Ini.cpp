#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#define SW_LOG_TAG "Ini"
#include "common/log.h"

#include "Ini.h"

/******************************************************************************
* ��  �ܣ����캯��
* ��  ������
* ����ֵ����
* ��  ע��
******************************************************************************/
CIni::CIni()
{
	m_fp = NULL;
}

/******************************************************************************
* ��  �ܣ���������
* ��  ������
* ����ֵ����
* ��  ע��
******************************************************************************/

CIni::~CIni()
{
	m_Map.clear();
}

/******************************************************************************
* ��  �ܣ����ļ�����
* ��  ������
* ����ֵ��
* ��  ע��
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
		//ɾ���ַ����еķǱ�Ҫ�ַ�
        //ȥ��ǰ׺�ո�
        nLeftPos = szLine.find_first_not_of(' ') ;
        if (std::string::npos != nLeftPos && nLeftPos >= 0)
        {
            szLine.erase(0, nLeftPos) ;
        }
        // ע���У�����
        if ('#' == szLine[0])
            continue ;
        // ȥ����׺���з�
		nLeftPos = szLine.find("\n");
		if (std::string::npos != nLeftPos && nLeftPos >= 0)
		{
			szLine.erase(nLeftPos, 1);
		}
        // ȥ����׺���з�
		nLeftPos = szLine.find("\r");
		if (std::string::npos != nLeftPos && nLeftPos >= 0)
		{
			szLine.erase(nLeftPos, 1);
		}
        // ȥ��ע�ͣ��ӵ�һ��#��ʼ����Ķ���ע�ͣ�
        while (std::string::npos != szLine.find('#'))
        {
            nLeftPos = szLine.find_last_of('#') ;
            szLine.erase(nLeftPos, szLine.size()) ;
        }
        // ȥ����׺�ո�
        nLeftPos = szLine.find_last_not_of(' ') ;
        if (std::string::npos != nLeftPos && nLeftPos >= 0)
        {
            szLine.erase(nLeftPos+1, szLine.size()) ;
        }
		//�ж��Ƿ�������
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
			//�Ƿ����Ӽ�
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
				//TODO:������ini��ֵģ������� ��ע�͵�
			}
		}
	}
	//�������һ������
	m_Map[szLastMainKey] = mLastMap;

	return INI_SUCCESS;
}

/******************************************************************************
* ��  �ܣ��ر��ļ�����
* ��  ������
* ����ֵ��
* ��  ע��
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
* ��  �ܣ���ȡ[SECTION]�µ�ĳһ����ֵ���ַ���
* ��  ����
* label  ������� ����
* attr   ������� �Ӽ�
* char* value  ������� �Ӽ���ֵ
* ����ֵ��
* ��  ע��
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
* ��  �ܣ���ȡ���εļ�ֵ
* ��  ����
* label                     ����
* attr                     �Ӽ�
* ����ֵ�������򷵻ض�Ӧ����ֵ δ��ȡ�ɹ��򷵻�0(��ֵ����Ϊ0����ͻ)
* ��  ע��
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
* ��  �ܣ���ȡ��ֵ���ַ���
* ��  ����
* label                     ����
* attr                     �Ӽ�
* ����ֵ�������򷵻ض�ȡ�����Ӽ��ַ��� δ��ȡ�ɹ��򷵻�"NULL"
* ��  ע��
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

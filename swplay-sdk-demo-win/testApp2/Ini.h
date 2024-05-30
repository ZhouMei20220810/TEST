#ifndef INI_H__
#define INI_H__

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <string.h>

#define CONFIGLEN           256 

enum INI_RES
{
	INI_SUCCESS,			//�ɹ�
	INI_ERROR,				//��ͨ����
	INI_OPENFILE_ERROR,		//���ļ�ʧ��
	INI_NO_ATTR				//�޶�Ӧ�ļ�ֵ
};

// �Ӽ�����    �Ӽ�ֵ 
typedef std::map<std::string, std::string> KEYMAP;
// �������� ����ֵ  
typedef std::map<std::string, KEYMAP> MAINKEYMAP;

class CIni
{
public:
	// ���캯��
	CIni();

	// ��������
	~CIni();
	//��ȡ���εļ�ֵ
	int GetInt(const char* label, const char* attr, int defaultValue);
	//��ȡfloat�ļ�ֵ
	float GetFloat(const char* label, const char* attr, float defaultValue);
	//��ȡ��ֵ���ַ���
	const char *GetStr(const char* label, const char* attr, const char* defaultValue);
	// ��config �ļ�
	INI_RES OpenFile(const char* pathName, const char* type);
	// �ر�config �ļ�
	INI_RES CloseFile();
protected:
	// ��ȡconfig�ļ�
	INI_RES GetKey(const std::string& label, const std::string& attr, char* value, int length);
protected:
	// ���򿪵��ļ��ֱ�
	FILE* m_fp;
	char  m_szKey[CONFIGLEN];
	MAINKEYMAP m_Map;
};

#endif // INI_H__

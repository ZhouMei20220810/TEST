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
	INI_SUCCESS,			//成功
	INI_ERROR,				//普通错误
	INI_OPENFILE_ERROR,		//打开文件失败
	INI_NO_ATTR				//无对应的键值
};

// 子键索引    子键值 
typedef std::map<std::string, std::string> KEYMAP;
// 主键索引 主键值  
typedef std::map<std::string, KEYMAP> MAINKEYMAP;

class CIni
{
public:
	// 构造函数
	CIni();

	// 析够函数
	~CIni();
	//获取整形的键值
	int GetInt(const char* label, const char* attr, int defaultValue);
	//获取float的键值
	float GetFloat(const char* label, const char* attr, float defaultValue);
	//获取键值的字符串
	const char *GetStr(const char* label, const char* attr, const char* defaultValue);
	// 打开config 文件
	INI_RES OpenFile(const char* pathName, const char* type);
	// 关闭config 文件
	INI_RES CloseFile();
protected:
	// 读取config文件
	INI_RES GetKey(const std::string& label, const std::string& attr, char* value, int length);
protected:
	// 被打开的文件局柄
	FILE* m_fp;
	char  m_szKey[CONFIGLEN];
	MAINKEYMAP m_Map;
};

#endif // INI_H__

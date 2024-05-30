// PadInfo.h 
//
#ifndef PADINFO_H
#define PADINFO_H

#include <string>
#include <memory>

class CPadInfo{
public:
	static std::shared_ptr<CPadInfo> InitFromConfigFile(const char* config_file);

	CPadInfo();
	~CPadInfo();

	inline const std::string& GetControlAddr() { return m_strControlAddr; }
	inline const std::string& GetSessionID() { return m_strSessionID; }
	inline int	GetControlPort() { return m_ControlPort; }
	inline int GetUserID() { return m_UserID; }
	inline bool haveHWaccel() { return m_HWaccel != 0; }
private:
	std::string m_strControlAddr;
	int m_ControlPort;
	int m_UserID;
	std::string m_strSessionID;
	int m_HWaccel = 0;

};
#endif //PADINFO_H

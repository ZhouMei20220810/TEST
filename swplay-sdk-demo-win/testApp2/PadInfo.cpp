#include <stdlib.h>
#include <string.h>

#define SW_LOG_TAG "PadInfo"
#include "common/log.h"

#include "Ini.h"
#include "PadInfo.h"

std::shared_ptr<CPadInfo> CPadInfo::InitFromConfigFile(const char* config_file) {
	std::shared_ptr<CPadInfo> padInfo = std::make_shared<CPadInfo>();
	CIni ini;
	if (ini.OpenFile(config_file, "r") == INI_SUCCESS) {
		padInfo->m_strControlAddr = std::string(ini.GetStr("default", "ControlAddr", "10.100.0.253"));

		padInfo->m_ControlPort = ini.GetInt("default", "ControlPort", 19955);
		padInfo->m_UserID = ini.GetInt("default", "UserID", 100000);
		padInfo->m_strSessionID = std::string(ini.GetStr("default", "SessionID", "session"));

		padInfo->m_HWaccel = ini.GetInt("default", "HWaccel", 0);
	}
	ini.CloseFile();

	return padInfo;
}

CPadInfo::CPadInfo(){
}

CPadInfo::~CPadInfo(){
}




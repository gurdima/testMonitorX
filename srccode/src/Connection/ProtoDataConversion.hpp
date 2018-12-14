#pragma once
/*---------------------------------------------------------------------------*/
#include "Statistics/ChannelStatistics.hpp"
//#include "Statistics/DecoderStatistics.hpp"

// #include "Settings/MiscSettings.hpp"
#include "Settings/TunerSettings.hpp"
#include "Settings/AGCSettings.hpp"
//#include "Settings/ADCSettings.hpp"
#include "Settings/ChannelSettings.hpp"
#include "Settings/CommonSettings.hpp"
#include "Common/Defines.hpp"
#include "Common/ByteOperations.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/DateTime.h"
#include "Poco/Logger.h"
#include "Poco/Timestamp.h"
/*---------------------------------------------------------------------------*/
#include "proto/GUI.pb.h"

namespace SNMP
{
//void convertDeviceSettingsMon2Proto(Settings::CommonSettings& inConf, GUI::DeviceSettings& outConf);
//void convertDeviceSettingsProto2Mon(const GUI::DeviceSettings& inConf, Settings::CommonSettings& outConf);
//void convertConnectionsInfoMon2Proto(Statistics::ChannelStatistics& inConf, GUI::ChannelConnectionsInfo& outConf);
//void convertConnectionsAddInfoMon2Proto(Statistics::ChannelStatistics& inConf, GUI::ChannelConnectionsInfo* outConf);

void convertChannelStatMon2Proto(Statistics::ChannelStatistics& inStat, GUI::ChannelStat& outConf); //, unsigned int maxNumberOfSubchannel
// void convertLinkAddStatMon2Proto(Statistics::ChannelStatistics& inStat, GUI::ChannelStat& outConf, bool _status);
void convertChannelConfigMon2Proto(Settings::ChannelSettings& inConf, GUI::ChannelConf& outConf);
void convertChannelConfigProto2Mon(const GUI::ChannelConf& inConf, Settings::ChannelSettings& outConf); 

void printChannelConfigProto(Poco::Logger& logger,const GUI::ChannelConf& conf);

} // namespace SNMP

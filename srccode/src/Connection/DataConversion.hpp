#pragma once
/*---------------------------------------------------------------------------*/
#include "monitord_io.h"
/*---------------------------------------------------------------------------*/
#include "Statistics/ChannelStatistics.hpp"
// #include "Statistics/DecoderStatistics.hpp"

//#include "Settings/MiscSettings.hpp"
#include "Settings/TunerSettings.hpp"
#include "Settings/AGCSettings.hpp"
//#include "Settings/ADCSettings.hpp"
#include "Settings/ChannelSettings.hpp"
#include "Settings/CommonSettings.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/DateTime.h"
#include "Poco/Logger.h"
/*---------------------------------------------------------------------------*/
#include <string>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{
/*
void convertDeviceSettingsMon2SNMP(Settings::CommonSettings& inConf, DeviceSettings& outConf);
void convertDeviceSettingsSNMP2Mon(DeviceSettings& inConf, Settings::CommonSettings& outConf);

void convertChannelStatMon2SNMP(Statistics::ChannelStatistics& inStat, ChannelStat& outStat);

void convertChannelConfigMon2SNMP(Settings::ChannelSettings& inConf, ChannelConf& outConf, unsigned char SelectStruct);
void convertChannelConfigSNMP2Mon(ChannelConf& inConf, Settings::ChannelSettings& outConf, unsigned char SelectStruct);

void convertDateTimeSNMP2Mon(DeviceSettings& inConf, Poco::DateTime& dateTime);
void convertDateTimeMon2SNMP(const Poco::DateTime& dateTime, DeviceSettings& outConf);


void convertConnectionsInfoMon2SNMP(Statistics::ChannelStatistics& inConf, ChannelConnectionsInfo& outConf);
void convertDecoderInfoMon2SNMP(const Statistics::DecoderStatistics& inConf, const Settings::TPCCodefileManager& codeFileMgr, DecoderInfo& outConf);

void printChannelConfig(Poco::Logger& logger, const ChannelConf& conf);

void convertDateTimeMon2ZMQ(const Poco::DateTime& dateTime, DeviceStatus& outConf);

unsigned int convertChannelConfigMon2ZMQ(Settings::ChannelSettings& inConf, ChannelConf& outConf, unsigned char SelectStruct, unsigned char Subchannel, unsigned char Field, void *outVoidConf);
unsigned int copyChannelConfigZMQ2MonH(ChannelConf& Conf, void *inStructure, unsigned int length, unsigned char SelectStruct, unsigned char Subchannel, unsigned char Field);
*/
} // namespace SNMP


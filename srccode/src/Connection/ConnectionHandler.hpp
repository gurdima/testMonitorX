#pragma once
/*---------------------------------------------------------------------------*/

#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Common/Noncopyable.hpp"

#include "Poco/Runnable.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Logger.h"
#include "Poco/SharedPtr.h"

#include "monitord_io.h"
#include "monitord_def.h"
#include "zmq.hpp"
#include "proto/GUI.pb.h"
/*---------------------------------------------------------------------------*/

namespace SNMP
{

class ConnectionHandler : public Poco::Runnable, public Noncopyable
{
public:
    ConnectionHandler(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~ConnectionHandler();

    virtual void run();
    void stop();

private:

    void processHeader(const MessageHeader& header);
/*
    void sendChannelStatistics(ChannelID channel);

    void sendChannelConfiguration(ChannelID channel);
    void receiveChannelConfiguration(ChannelID channel);

    void sendDeviceInfo();

    void sendDeviceSettings();
    void receiveDeviceSettings();
*/
    void sendChannelConnectionInfo(ChannelID channel);

    void sendChannelStatisticsToProto(ChannelID channel, unsigned int idLeading);    

    void sendChannelConfigurationToProto(ChannelID channel);

    void sendDeviceInfoToProto();
    void DeviceInfoToProto(GUI::DeviceInfo& data);
//    void sendDeviceSettingsToProto();    

    void sendChannelConnectionInfoToProto(ChannelID channel);

    void receiveChannelConfigurationFromProto(ChannelID channel);
    void receiveDeviceSettingsFromProto(ChannelID channel);    

    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    // Settings::ChannelSettings&      _channelSettings;
    Poco::NotificationCenter&       _nc;
    bool                            _stop;
    Poco::Logger&                   _logger;
    unsigned char                    status_arm;
    Poco::SharedPtr<zmq::context_t> _zContext;
    Poco::SharedPtr<zmq::socket_t>  _zSocket;
    unsigned char                   old_pending[2];
    unsigned char                   refresh;
};

} // namespace SNMP

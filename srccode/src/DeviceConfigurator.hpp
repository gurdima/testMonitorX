#pragma once
/*---------------------------------------------------------------------------*/

#include "Notifications.hpp"
#include "Channel.hpp"

#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"

// #include "FPGA/LoadManager.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/SharedPtr.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
/*---------------------------------------------------------------------------*/

class DeviceConfigurator
{
public:
    DeviceConfigurator(Settings::SettingsManager& settings, Statistics::StatisticsManager& statistics, Poco::NotificationCenter& nc);
    ~DeviceConfigurator();

    void initDevice();

private:
    void handleChannelReconfigurationNotification(const Poco::AutoPtr<ChannelReconfigurationNotification>& pNf);
    void handleApplyChannelConfigurationNotification(const Poco::AutoPtr<ApplyChannelConfigurationNotification>& pNf);
    void handleApplyFadingChannelConfigurationNotification(const Poco::AutoPtr<ApplyFadingChannelConfigurationNotification>& pNf);
    void handleApplyDeviceSettingsNotification(const Poco::AutoPtr<ApplyDeviceSettingsNotification>& pNf);
    void handleApplyTunerConfigurationNotification(const Poco::AutoPtr<ApplyTunerConfigurationNotification>& pNf);
    

//    void onReconfigureOutMux_for_BYPASS_2GE(ChannelID MasterChannel);
    void onReconfigureChannel(const void* pSender, ChannelID& channel);
    void onApplyChannelConfiguration(const void* pSender, ChannelID& channel);
    void onApplyDeviceSettings(const void* pSender, int& arg);
    void onApplyTunerConfiguration(ChannelID& channel);
    void onApplyFadingChannelConfiguration(ChannelID& channel);

    bool WaitingPending(unsigned int SleepTime, unsigned int Max_count, bool _avarout);
    typedef Poco::SharedPtr<Channel>    ChannelPtr;
    typedef std::vector<ChannelPtr>     ChannelsContainer;

    Settings::SettingsManager&      _settings;
    Statistics::StatisticsManager&  _statistics;
    Poco::NotificationCenter&       _nc;

    Poco::BasicEvent<ChannelID>     _reconfigureChannelEvent;
    Poco::BasicEvent<ChannelID>     _applyChannelConfigurationEvent;
//    Poco::BasicEvent<ChannelID>     _applyFadingChannelConfigurationEvent;
    Poco::BasicEvent<int>           _deviceSettingsEvent;
//    Poco::BasicEvent<ChannelID>     _applyTunerConfigurationEvent;
    ChannelsContainer               _channels;
//    FPGA::LoadManager               _loader;

    Poco::Logger&                   _logger;
};


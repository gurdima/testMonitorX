#include "Channel.hpp"

// using Decoder::convertModulationType2Coefficient;
/*---------------------------------------------------------------------------*/
#include "Poco/NumberFormatter.h"
#include "Poco/ScopedLock.h"

using Poco::ScopedLock;
using Poco::Mutex;
using Poco::Logger;
using Poco::Util::TimerTask;
using Poco::Util::TimerTaskAdapter;
/*---------------------------------------------------------------------------*/
#include <string>
#include <cmath>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Channel::Channel(const ChannelID&                      channel,
                 Settings::SettingsManager&            settingsManager,
                 Statistics::StatisticsManager&        statisticsManager,
                 Settings::ChannelSettings&            channelSettings,
                 Statistics::ChannelStatistics&        channelStatistics,
                 Poco::NotificationCenter&             nc)
: _channel(channel)
, _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _channelSettings(channelSettings)
, _channelStatistics(channelStatistics)
, _tuner(_channel, _channelSettings, _channelStatistics)
, _timer()
, _nc(nc)
, _logger(Logger::get("Monitor.Channel." + Poco::NumberFormatter::format(channel)))
{
    TRACE();
    //_status_DEADBEEF = true;
    _tuner.addObserver(*this);
    _pTask = new TimerTaskAdapter<Channel>(*this, &Channel::onTimer);
    if(_pTask.isNull()) {
        poco_error(_logger, "Failed to create task for util timer");
        return;
    }
    _statisticsManager.getCommonStatistics().setFirstTunerConfig(_channel, true);
    _timer.schedule(_pTask, 10, 1000);
    poco_debug(_logger, "Util timer started");
}

Channel::~Channel()
{
    //TRACE();
    _tuner.removeObserver(*this);
}

void Channel::configureConstValues()
{
//    TRACE();
    _tuner.configureConstValues();
//    _tuner.collectStatisticsAfterStart();
}

void Channel::to_configure(){
//      if(_tuner.isTunerPresentAndOK()) {
//        poco_debug(_logger, "Tuner is present and ready");
    _tuner.configure();
}

void Channel::configure()
{
    // TRACE();
    ScopedLock<Mutex> lock(_configureMutex);
    // _nc.postNotification(new TeminateChannelDataConnectionsNotification(TeminateChannelDataConnectionsNotificationPayload(_channel)));
    
    // loadBitFile; 
    to_configure();
    if(_statisticsManager.getCommonStatistics().getFirstTunerConfig(_channel)) {
        _statisticsManager.getCommonStatistics().setFirstTunerConfig(_channel, false);
    }
    // ------------------------- FOR BYPASS 2GE  ------------------------------
    unsigned int _status = 0;
    _status = CHANNEL_STATUS_MSG_MASTER;
    _statisticsManager.getChannelStatistics(_channel).lessChannelStatus(_status);  // set 0
    // -------------------------------------------------------
    _nc.postNotification(new ApplyMetersSettingsNotification(ApplyMetersSettingsNotificationPayload(_channel)));
}

void Channel::TunerConfigure()
{
    // TRACE();
    _statisticsManager.getCommonStatistics().setTunerLock(_channel, false);
    //bool tunerLock = _tuner.configure();
     _tuner.configure();
    _statisticsManager.getCommonStatistics().setTunerLock(_channel, true);
}
/*
void Channel::onTunerStateChanged(bool inserted)
{
    //TRACE();
    poco_debug_f1(_logger, "Tuner %s", std::string(inserted ? "inserted" : "removed"));

    if(inserted) {
        _nc.postNotification(new ChannelReconfigurationNotification(ChannelReconfigurationNotificationPayload(_channel)));
    }
}
*/
void Channel::onTimer(Poco::Util::TimerTask& task)
{
    // TRACE();
    ScopedLock<Mutex> lock(_timerMutex);
    unsigned int _pending = _statisticsManager.getCommonStatistics().getConfigurationPending();
    if(_pending&(_channel+1)){
        return;
    }
    if(_statisticsManager.getCommonStatistics().getFirstTunerConfig(_channel)){
        return;
    }

    // _tuner.checkTunerState();
    _tuner.collectStatistics();
}


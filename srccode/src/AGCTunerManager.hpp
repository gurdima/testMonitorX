#pragma once
/*---------------------------------------------------------------------------*/
#include "TunerObserver.hpp"
#include "Common/Typedefs.hpp"
#include "Settings/ChannelSettings.hpp"
#include "Statistics/ChannelStatistics.hpp"
#include "AGCTunerControl.hpp"

// #include "AGCDevice.hpp"
// #include "MemoryDevice.hpp"
/*---------------------------------------------------------------------------*/

#include "Poco/Mutex.h"
/*---------------------------------------------------------------------------*/
class TunerManager : public TunerObservable
{
public:
    TunerManager(const ChannelID& channel, Settings::ChannelSettings& channelSettings, Statistics::ChannelStatistics& channelStatistics);
    ~TunerManager();
    void configureConstValues();
    void collectStatisticsAfterStart();
    void collectStatistics();
    void configure();
//    bool configureFreq();
//    void checkTunerState();
//    void setTunerVersion();
//    bool isTunerPresent() const;
//    bool isTunerPresentAndOK() const;

private:
    ChannelID                       _channel;
    Settings::ChannelSettings&      _channelSettings;
    Statistics::ChannelStatistics&  _channelStatistics;
    AGCTunerControl                 _device;
    bool                            _knownTunerState;
    Poco::Logger&                   _logger;
    Poco::Mutex                     _tunerMutex;
    int                             fd_UART;
    bool                            state_config;
};


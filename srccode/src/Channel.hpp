#pragma once

#include "monitord_def.h"
/*---------------------------------------------------------------------------*/
#include "Notifications.hpp"
#include "TunerObserver.hpp"
#include "AGCTunerManager.hpp"

#include "Common/Tracer.hpp"
#include "Common/Typedefs.hpp"

#include "Settings/SettingsManager.hpp"
#include "Settings/ChannelSettings.hpp"

#include "Statistics/ChannelStatistics.hpp"
#include "Statistics/StatisticsManager.hpp"

// #include "ADCManager.hpp"
// #include "Settings/DVBS2Settings.hpp"
// #include "Demodulator/DemodulatorManager.hpp"
// #include "Decoder/DecoderManager.hpp"
// #include "Decoder/LDPCBase.hpp"
// #include "PostProcessing/Configurator.hpp"
// #include "OutMux/Configurator.hpp"

// #include "FPGA/ChannelLoader.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"
#include "Poco/Util/TimerTask.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Mutex.h"
/*---------------------------------------------------------------------------*/
class Channel : public TunerObserver
{
public:
    Channel(const ChannelID&                      channel,
            Settings::SettingsManager&            settingsManager,
            Statistics::StatisticsManager&        statisticsManager,
            Settings::ChannelSettings&            channelSettings,
            Statistics::ChannelStatistics&        channelStatistics,
//            Decoder::DecoderConfigParser&         decoderConfigParser,
//            Demodulator::DemodulatorConfigParser& demodulatorConfigParser,
//            FPGA::ChannelLoader&                  channelLoader,
            Poco::NotificationCenter&             nc);

    ~Channel();
    void configureConstValues();
    void configure();
    void to_configure();
    void TunerConfigure();
//    void configureOutMux_for_BYPASS_2GE();
//    void configureOutMuxCtrl_after_BYPASS_2GE();
//    virtual void onTunerStateChanged(bool inserted);

private:
//    void collectEbN0() const;
    void onTimer(Poco::Util::TimerTask& task);
    ChannelID                       _channel;
    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    Settings::ChannelSettings&      _channelSettings;
    Statistics::ChannelStatistics&  _channelStatistics;

    TunerManager                    _tuner;
/*
    ADCManager                      _adc;
    Demodulator::DemodulatorManager _demodulator;
    Decoder::DecoderManager         _decoder;
    PostProcessing::Configurator    _postProcessing;
    OutMux::Configurator            _outMux;
*/
    Poco::Util::Timer               _timer;
    Poco::Util::TimerTask::Ptr      _pTask;

    Poco::NotificationCenter&       _nc;

    Poco::Mutex                     _configureMutex;
    Poco::Mutex                     _timerMutex;

    Poco::Logger&                   _logger;
//    bool                            _status_DEADBEEF;
};



#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Common/Noncopyable.hpp"
// #include "FFTDevice.hpp"
// #include "ConstellationDevice.hpp"
// #include "MemoryDevice.hpp"
#include "Channel.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Runnable.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Logger.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
/*---------------------------------------------------------------------------*/
#include "monitord_io.h"
#include "zmq.hpp"
#include "proto/GUI.pb.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{

class ZMQHandler : public Poco::Runnable, public Noncopyable
{
public:
    ZMQHandler(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc, unsigned int Channel);
    ~ZMQHandler();

    virtual void run();
    void stop();

private:
    
    void processHeader(ZMQManagerHeader& header); // const 
    void receiveDeviceSettings(DeviceSettings& settings);

    unsigned int checkPending();
    unsigned int checkQuery(ZMQManagerHeader& header);
    unsigned int sendZMQAnswer(ZMQManagerHeader& header);
    unsigned int checkStructureDevice(void *Structure, unsigned int _length, unsigned int _field);
    unsigned int checkStructureConf(void *Structure, unsigned int _length, unsigned int _structure, unsigned int _subchannel, unsigned int _field);
    unsigned int checkDataInStructureConf(ChannelConf& conf, unsigned int _structure, unsigned int _subchannel, unsigned int _field);
    unsigned int checkStructureFFT(void *Structure, unsigned int _length);
//    unsigned int checkStructureConst(void *Structure, unsigned int _length);

    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;

    Poco::NotificationCenter&       _nc;
    bool                            _stop;
    bool                            _close;
    Poco::Logger&                   _logger;
    unsigned  int                   _channel;
    void*                           ZMQContext;
    void*                           ZMQSocket;

//    typedef Poco::SharedPtr<DeviceIO::MemoryDevice>     MemoryDevicePtr;
//    typedef Poco::SharedPtr<DeviceIO::FFTDevice>        FFTDevicePtr;
//    typedef Poco::SharedPtr<DeviceIO::ConstellationDevice>  ConstellationDevicePtr;
//    MemoryDevicePtr         _memDev;
//    ConstellationDevicePtr  _conDev;
//    FFTDevicePtr            _fftDev;
    Poco::Mutex                     _timerMutex;

    unsigned int StructInfoMemberList[MAX_COUNT_OF_STRUCTURE_CONF];
};

} // namespace SNMP

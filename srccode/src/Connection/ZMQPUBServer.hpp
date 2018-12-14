#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Common/Noncopyable.hpp"
// #include "MemoryDevice.hpp"
//#include "AGCDevice.hpp"
//#include "FFTDevice.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Mutex.h"
#include "Poco/Runnable.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Logger.h"
#include "Poco/SharedPtr.h"
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"
/*---------------------------------------------------------------------------*/
#include "monitord_io.h"
#include "zmq.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{
class ZMQPUBServer : public Poco::Runnable, public Noncopyable
{
public:
    ZMQPUBServer(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~ZMQPUBServer();
    virtual void run();
    void stop();

private:
    void onTimer(Poco::Util::TimerTask& task);
    void handleGetData(int channel);
    unsigned int handleMakeFFTData(int channel, FFTDataZMQ& outFFT);

    Poco::Mutex                 _timerMutex;
/*
    typedef Poco::SharedPtr<DeviceIO::MemoryDevice>     MemoryDevicePtr;
    typedef Poco::SharedPtr<DeviceIO::AGCDevice>        AGCDevicePtr;
    typedef Poco::SharedPtr<DeviceIO::FFTDevice>        FFTDevicePtr;

    typedef std::vector<MemoryDevicePtr>    MemoryDeviceContainer;
    typedef std::vector<AGCDevicePtr>       AGCDeviceContainer;
    typedef std::vector<FFTDevicePtr>       FFTDeviceontainer;

    AGCDevicePtr                _agcDev;
    MemoryDevicePtr             _memDev;
    FFTDevicePtr                _fftDev;

    MemoryDeviceContainer   _memDevices;
    AGCDeviceContainer      _agcDevices;
    FFTDeviceontainer       _fftDevices;
*/
    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    Poco::NotificationCenter&       _nc;
    bool                            _stop;
    Poco::Logger&                   _logger;
    Poco::SharedPtr<zmq::context_t> _zContext;
    Poco::SharedPtr<zmq::socket_t>  _zSocket;
    Poco::Util::TimerTask::Ptr      _pTask;
    Poco::Util::Timer               _timer;
    void* guipublisher;
    void* guicontext;
    void* fftpublisher;
    void* fftcontext;
};

} // namespace SNMP

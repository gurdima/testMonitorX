#pragma once
/*---------------------------------------------------------------------------*/
#include "proto/FFT.pb.h"
/*---------------------------------------------------------------------------*/
#include "BasicDataProvider.hpp"
#include "Common/Tracer.hpp"
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Notifications.hpp"
// #include "AGCDevice.hpp"
// #include "FFTDevice.hpp"
// #include "MemoryDevice.hpp"
#include "Device/devmem.h"
/*---------------------------------------------------------------------------*/
#include "Poco/SharedPtr.h"
#include "Poco/Logger.h"
#include "Poco/Mutex.h"
#include "Poco/NotificationCenter.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace MetersSpace{
class FFTDataProvider: public BasicDataProvider
{
public:
    FFTDataProvider(Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~FFTDataProvider();

    static Poco::Mutex mutexData;
    static Poco::Mutex mutexSend;

protected:

    virtual void handleMakeProtoStream(std::ostream& ost);
    virtual int  handleGetDataSize() const;
    virtual void handleGetData(int channel);
    virtual void handleMakeProto(int channel, const HTTPHeader& header);
    virtual void handleClearData();
    virtual void handleSetConfig(int channel,const char* config, int size, const HTTPHeader& header);
private:
    // void dev_mem_write(off_t target, unsigned int data);
    // unsigned int dev_mem_read(off_t target);

    void apllySavedConfig(const Poco::AutoPtr<ApplyMetersSettingsNotification>& pNf);
    unsigned int CalculateNewFilterBand(unsigned int band);
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
    FFTConfig                       _config;
    FFTData                         _data;
    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    Poco::Logger&                   _logger;
    Poco::NotificationCenter&       _externalNC;
    //DeviceIO::FFTDevice::FFTDataContainer samples;
    unsigned char                   _first_start;
    unsigned int                    _framenumber;
    unsigned int                    _measuredSymbolRate_previous; 
    unsigned int                    _measuredScale_previous;
    DeviceIO::MemoryDevice          devmem;
};
}//namespace MetersSpace


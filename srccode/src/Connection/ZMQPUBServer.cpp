#include "Connection/ZMQPUBServer.hpp"
/*---------------------------------------------------------------------------*/
#include "Connection/DataConversion.hpp"
#include "Notifications.hpp"
#include "Common/Tracer.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/ScopedLock.h"
using Poco::Logger;
using Poco::Mutex;
using Poco::ScopedLock;

using Poco::Util::Timer;
using Poco::Util::TimerTask;
using Poco::Util::TimerTaskAdapter;
/*---------------------------------------------------------------------------*/
#include <string>
#include <vector>

using std::string;
using std::vector;
/*---------------------------------------------------------------------------*/
// #define REG_FRAME_NUM   0x68
/*---------------------------------------------------------------------------*/

/*
static unsigned int convertAGCDeviceChannelToBaseAddress(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return MEMORY_BASE_ADDRESS + MEMORY_CHANNEL_SHIFT * channel;
}

static unsigned int convertFFTDeviceChannelToBaseAddress(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return MEMORY_BASE_ADDRESS + MEMORY_CHANNEL_SHIFT * channel;
}
*/
namespace SNMP
{
ZMQPUBServer::ZMQPUBServer(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _nc(nc)
, _stop(false)
, _logger(Logger::get("Monitor.SNMP.ZMQPUBServer"))
{
}

ZMQPUBServer::~ZMQPUBServer()
{
//    TRACE();
}

void ZMQPUBServer::run()
{
     // TRACE();
/*     Poco::Thread::sleep(70000);

     poco_debug(_logger, "Startings PUB ...");
     guicontext = zmq_ctx_new();
     guipublisher = zmq_socket(guicontext, ZMQ_PUB);
     zmq_bind(guipublisher, "tcp://*:5040");

     fftcontext = zmq_ctx_new();
     fftpublisher = zmq_socket(fftcontext, ZMQ_PUB);
     zmq_bind(guipublisher, "tcp://*:5041");
     // conn = zmq_bind(publisher, "ipc://stock.ipc");

     handleGetData(0);
     handleGetData(1);
    _pTask = new TimerTaskAdapter<ZMQPUBServer>(*this,&ZMQPUBServer::onTimer);
    _timer.schedule(_pTask, 2000, 2000);
 */
}

void ZMQPUBServer::stop()
{
    // TRACE();
/*    zmq_close(guipublisher);
    zmq_ctx_destroy(guicontext);
    zmq_close(fftpublisher);
    zmq_ctx_destroy(fftcontext);
*/
}

void  ZMQPUBServer::handleGetData(int channel)
{
//    TRACE();
/*
    _memDev = new DeviceIO::MemoryDevice(200, 30 + channel);
    _memDevices.push_back(_memDev);
    _agcDev = new DeviceIO::AGCDevice(convertAGCDeviceChannelToBaseAddress(channel), *_memDev);
    _agcDevices.push_back(_agcDev);
    _fftDev = new DeviceIO::FFTDevice(convertFFTDeviceChannelToBaseAddress(channel), *_memDev);
    _fftDevices.push_back(_fftDev);
*/
}

unsigned int ZMQPUBServer::handleMakeFFTData(int channel, FFTDataZMQ& pFFTDataZMQ)
{
    // TRACE();
/*
    unsigned int length =0;
    // FFTDataZMQ pFFTDataZMQ; 

//    ScopedLock<Mutex> lock(mutexData);
    
    Settings::ChannelSettings& channelSettings = _settingsManager.getChannelSettings(channel);

    int pin = static_cast<int>(_statisticsManager.getChannelStatistics(channel).getTunerStatistics().getAGCInputSignalLevel() * 1000);
    unsigned int symbolRate =0;

    if(channelSettings.getDEMSettings().getMode() == DEM_MODE_TDMA) {
        symbolRate = channelSettings.getTDMASubChannelSettings(_fftDevices[channel]->getSubChannel()).getSymbolRate();
    } else { 
        symbolRate = channelSettings.getDemodulatorSettings(0).getSymbolRate();;
    }

    if(channelSettings.getResamplerSettings().getStatusEnable()) {

        float scale = static_cast<float>(_fftDevices[channel]->getScalling()) * 7.0f / 3.0f;

        if(_fftDevices[channel]->getMeasureSymbolRate()) {
            symbolRate = _fftDevices[channel]->getNumSr();
          //  symbolRate = static_cast<unsigned int>((static_cast<float>(125u * _fftDevices[channel]->getNumSr()) / ( 1024 * scale )) * 1000000);
        } else {
            symbolRate *= 6 / 7;
        }
        // _data.set_measuredsymbolrate(symbolRate);
        pFFTDataZMQ.measuredSymbolRate = symbolRate;
        // _data.set_measuredscale(static_cast<unsigned int>(scale * 1000000));
        pFFTDataZMQ.measuredScale = static_cast<unsigned int>(scale * 1000000);

    } else {

        unsigned int scale = (_fftDevices[channel]->getScale() & 0x7ffu);

        if(_fftDevices[channel]->getMeasureSymbolRate()){
             symbolRate = _fftDevices[channel]->getNumSr(); 
           // symbolRate = static_cast<unsigned int>((static_cast<float>(125u * _fftDevices[channel]->getNumSr()) / ( 1024 * scale )) * 1000000);
        }

        pFFTDataZMQ.measuredSymbolRate = symbolRate;
        pFFTDataZMQ.measuredScale = static_cast<unsigned int>(scale * 1000000);
    }



    pFFTDataZMQ.centerFrequency = _agcDevices[channel]->getTunerFrequency();
    pFFTDataZMQ.lpfCutoffFrequency = _agcDevices[channel]->getLPFCutoffFrequency();
    pFFTDataZMQ.Pin = pin;
    pFFTDataZMQ.coreVersion = _fftDevices[channel]->getVersion();
    pFFTDataZMQ.leaderStatus = 0;
    pFFTDataZMQ.measuredSymbolRate = _fftDevices[channel]->getMeasureSymbolRate(); 
    pFFTDataZMQ.powerOfAveraging = _fftDevices[channel]->getPowerOfAveraging();
  poco_debug_f1(_logger, "TRACE pFFTDataZMQ.powerOfAveraging =  %u",(unsigned int) pFFTDataZMQ.powerOfAveraging);
    pFFTDataZMQ.scale = _fftDevices[channel]->getScalling();
  poco_debug_f1(_logger, "TRACE pFFTDataZMQ.scale =  %u",(unsigned int) pFFTDataZMQ.scale);
    pFFTDataZMQ.subChannel = _fftDevices[channel]->getSubChannel();
    pFFTDataZMQ.subChannelsNumber = _fftDevices[channel]->getSubChannelsNumber();
//    poco_debug_f1(_logger, "TRACE _fftDevices[channel]->getSubChannel()=  %u",(unsigned int) _fftDevices[channel]->getSubChannel());
//    poco_debug_f1(_logger, "TRACE _fftDevices[channel]->getSubChannelsNumber()=  %u",(unsigned int) _fftDevices[channel]->getSubChannelsNumber());

    poco_debug_f1(_logger, "FFT 1 pFFTDataZMQ.frameNumber %u", _fftDevices[channel]->getFrameNumber());
//    unsigned int datafft;
//    _memDev[channel].read(0xC000068, datafft);
//    poco_debug_f1(_logger, "FFT 3 _memDev.read(addrRegFrameNumber(_baseAddr), datafft) - %u", datafft);
    bool result = false;    
    try {
        DeviceIO::FFTDevice::FFTDataContainer samples;

        result = _fftDevices[channel]->getDataZMQ(samples);
        if(result==true) {
            poco_debug_f1(_logger, "FFT data 2 samples.size() %u", samples.size());
            for(unsigned int i = 0; i < samples.size(); ++i){
                pFFTDataZMQ.samples[i] = samples[i]; 
            }
        }

        pFFTDataZMQ.frameNumber =  _fftDevices[channel]->getFrameNumber();
        poco_debug_f1(_logger, "FFT 4 pFFTDataZMQ.frameNumber %u", pFFTDataZMQ.frameNumber);
//        _memDev[channel].read(0xC000068, datafft);
//        poco_debug_f1(_logger, "FFT 5 _memDev.read(addrRegFrameNumber(_baseAddr), datafft) - %u", datafft);
    }
    catch (const DeviceIO::FFTDeviceException& exc){
        poco_debug_f1(_logger, "FFT data read failed %s",exc.displayText());
    }


    length = sizeof(pFFTDataZMQ);
    // memcpy(outFFT, dataZMQ, length);
    return length; 
    */
    return 0;
}


void ZMQPUBServer::onTimer(Poco::Util::TimerTask& task)
{
/*
    ScopedLock<Mutex> lock(_timerMutex);
//    poco_debug(_logger, "Publishon ...");
//     ChannelConf _conf;
//     FFTDataZMQ fftdata;
     ChannelStat data;

    unsigned int length;

    try {
        for(int i=0;i<CHANNELS_NUMBER;i++) {
            convertChannelStatMon2SNMP(_statisticsManager.getChannelStatistics(i), data);
            length = sizeof(data);
            zmq_msg_t message;
            zmq_msg_init_size(&message, length);
            memcpy(zmq_msg_data(&message), &data, length);
            zmq_msg_send(&message, guipublisher, 0);
            zmq_msg_close(&message);
        }

    } catch (const Poco::Exception& exc) {
        std::cerr << exc.displayText() << std::endl;
    }
*/    
}

} // namespace SNMP


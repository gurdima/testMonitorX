#include "FFTDataProvider.hpp"
/*---------------------------------------------------------------------------*/
//#include "Settings/DemodulatorSettings.hpp"
#include "Settings/ChannelSettings.hpp"
#include "Settings/SettingsManager.hpp"
#include "Common/Tracer.hpp"
#include "Common/Typedefs.hpp"
#include "Common/Defines.hpp"
#include "Common/Helpers.hpp"
#include "Notifications.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Thread.h"
#include "Poco/ScopedLock.h"
#include "Poco/NObserver.h"

using Poco::NObserver;
using Poco::ScopedLock;
using Poco::Mutex;
/*---------------------------------------------------------------------------*/
// #include "zmq.hpp"
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <math.h>
#include <vector>
/*---------------------------------------------------------------------------*/
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

#define MAX_MSG_SIZE    1024
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

// #define MAIN_ADDRESS    0x0b000000 //0x40000000
// #define MAIN_ADDRESS_SH8    0x40000008
#define LPF_CUTOFF_LOWER_THRESHOLD  1
#define LPF_CUTOFF_UPPER_THRESHOLD  251



#define DATA_NUMBER_16K      16384 //65536

#define DEM_MODE_FFT_64K                    10
/*---------------------------------------------------------------------------*/
namespace MetersSpace{

Mutex FFTDataProvider::mutexData;
Mutex FFTDataProvider::mutexSend;

FFTDataProvider::FFTDataProvider(Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _logger(Poco::Logger::get("Monitor.FFTDataProvider"))
, _externalNC(nc)
{     
//    _externalNC.addObserver(NObserver<FFTDataProvider, ApplyMetersSettingsNotification>(*this, &FFTDataProvider::apllySavedConfig));
    _first_start = 3; //1+2
    _framenumber = 0;
    _measuredSymbolRate_previous = 0; 
    _measuredScale_previous = 1;
    // devmem = new MemoryDevice();
}

FFTDataProvider::~FFTDataProvider()
{
}

void FFTDataProvider::handleMakeProtoStream(std::ostream& ost)
{
    // TRACE();
    ScopedLock<Mutex> lock(mutexSend);
/*    poco_error(_logger, "**********FFT************");
    poco_debug_f1(_logger, "size %d", _data.ByteSize());
    poco_debug_f1(_logger, "pin %d", _data.pin());
    poco_debug_f1(_logger, "frame %u", _data.framenumber());
    poco_error(_logger, "----------------------");
*/
    if (!_data.SerializeToOstream(&ost)){
        poco_error(_logger, "protobuf serialize error");
    }
    _data.Clear();
 
}

int FFTDataProvider::handleGetDataSize() const
{
    //TRACE();
    return _data.ByteSize();
}

void FFTDataProvider::handleGetData(int channel)
{
//    TRACE();
}

void FFTDataProvider::handleSetConfig(int channel,const char* config, int size, const HTTPHeader& header)
{
    TRACE();
    ScopedLock<Mutex> lock(mutexData);

    if(!header.leadingPart) {
        poco_error(_logger, "it is not a leader"); 
        return;
    }
 
    if(!_config.ParseFromArray(config, size)){
        poco_error(_logger, "Protobuf config parse failed");
        return;
    }
    unsigned int fft_crt = 0;
    unsigned int _getScalling = _config.scale(); 
    unsigned int _getPowerOfAveraging = _config.powerofaveraging();
    unsigned int _getmeasureSymbRate = _config.measuresymbolrate();

    fft_crt = ((_getPowerOfAveraging & 0x7)<<8) + ((_getmeasureSymbRate&1)<<16) + _getScalling;
    devmem.dev_mem_write(FFT_SCALE_ADDRESS, 0); // обнулить 
    devmem.dev_mem_write(FFT_CTRL_ADDRESS, (unsigned int) fft_crt);

    poco_information_f2(_logger, "Warning. getScalling = 0, Set data (%u) in Address 0x%X ", (fft_crt) , static_cast<unsigned int> (FFT_CTRL_ADDRESS));

    // _fftDevices[channel]->setMeasureSymbolRateMode(_config.measuresymbolrate() );
    // _fftDevices[channel]->setPowerOfAveraging(_config.powerofaveraging());
    // _fftDevices[channel]->setScale(_config.scale());
    // _fftDevices[channel]->setSubChannel(_config.subchannel()); 
    // _fftDevices[channel]->saveMeasureSymbolRate(_config.measuresymbolrate());
    // _fftDevices[channel]->savePowerOfAveraging(_config.powerofaveraging());
    //_fftDevices[channel]->saveScale(_config.scale());
     unsigned char _pending = _statisticsManager.getCommonStatistics().getConfigurationPending();
     if(_config.halfauto_flag()&1) {
         if(!(_pending&(channel+1))) {
             _statisticsManager.getCommonStatistics().setVirtualPending(static_cast<unsigned char>(channel+1+32+64)); // +32 - для handleMakeProto, +64 - для ConnectionHandler
             unsigned int frequency = _config.tuner_frequency()*1000;
             unsigned int filterband = _config.tuner_filterband();// CalculateNewFilterBand(static_cast<unsigned int> (_config.tuner_filterband()));
             _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getTunerSettings().setFrequency(frequency);
             _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getTunerSettings().setFilterBandIndex(filterband); // .setFilterBand(filterband);
             poco_trace_f2(_logger, "The start button Calc,Central frequency = %u; Filter band = %u", frequency, filterband);
             // _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getTunerSettings().setManualFilterMode(1);
             // _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getTunerSettings().setSpectrumAnalyzerEnable(0);
             _externalNC.postNotification(new ApplyTunerConfigurationNotification(ApplyTunerConfigurationNotificationPayload((ChannelID&) channel)));
         } else {
                poco_trace_f1(_logger, "ERROR! Not chanage tuner_frequency, but pending = %u", static_cast<unsigned int> (_pending));
         }
     }
     /*
        if(_config.halfauto_flag()&2) {
//            poco_debug_f1(_logger, "TRACE. GET _config.measuredsymbolrate() =  %u", _config.measuredsymbolrate());
            if(_mode>0){
                 if(!(_pending&(channel+1))) {
                     //_statisticsManager.getCommonStatistics().setVirtualPending(static_cast<unsigned char>(channel+1+32+64)); // +32 - для handleMakeProto, +64 - для ConnectionHandler
                     //unsigned int measuredsymbolrate = _config.measuredsymbolrate();
                     //_settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getDemodulatorSettings(0).setSymbolRate(measuredsymbolrate);
                     // _externalNC.postNotification(new ApplyChannelConfigurationNotification(ApplyChannelConfigurationNotificationPayload((ChannelID&) channel)));
                } else {
                    poco_trace_f1(_logger, "ERROR! Not chanage SymbolRate, but pending = %u", static_cast<unsigned int> (_pending));
                }
            } else {
                poco_trace(_logger, "ERROR! Not chanage SymbolRate, but mode = BYPASS");
            }
        }
      */
}

void FFTDataProvider::handleMakeProto(int channel, const HTTPHeader& header)
{
    // TRACE();
    
    ScopedLock<Mutex> lock(mutexData);
    if(_first_start>0) {
        _first_start = 0;
        devmem.dev_mem_write(FFT_CTRL_ADDRESS, 0x701); //default
        poco_information_f2(_logger, "Set Default data (0x%X) in Address 0x%X ", static_cast<unsigned int> (0x701), static_cast<unsigned int> (FFT_CTRL_ADDRESS));
    }
    //unsigned int _mode = 8  ; // _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getDEMSettings().getMode();
    unsigned int fft_crt = devmem.dev_mem_read(FFT_CTRL_ADDRESS);
    unsigned int _getScalling = (fft_crt & 0xff); //_fftDevices[channel]->getScalling();
    if(_getScalling==0) {
        _getScalling = 1;
        fft_crt = 0x701; //fft_crt + _getScalling
        devmem.dev_mem_write(FFT_CTRL_ADDRESS, (unsigned int) 0x701); // 
        poco_information_f2(_logger, "Warning. getScalling = 0, Set data (%u) in Address  FFT_CTRL (0x%X) ", fft_crt, static_cast<unsigned int> (FFT_CTRL_ADDRESS));
        fft_crt = devmem.dev_mem_read(FFT_CTRL_ADDRESS);
        poco_information_f1(_logger, "FFT_CTRL  = (0x%X) ", fft_crt);
    }
    unsigned int _getPowerOfAveraging = (fft_crt & 0x700)>>8;
    unsigned int _getVersion = devmem.dev_mem_read(FFT_VER_ADDRESS);
    unsigned int reg_fft_scale = devmem.dev_mem_read(FFT_SCALE_ADDRESS);

    unsigned int _decim = 1; //((devmem.dev_mem_read(ADDRESS_10GE_DATA_PACKETS) >> 25)&0x7);
    // poco_information_f1(_logger, "_decim  = %u", _decim);
    _decim = _settingsManager.getChannelSettings(0).getDecimationSettings().getDecimationRate();
    _data.set_adc_decimationrate(_decim);
    //_data.set_measuredscale(reg_fft_scale&0x3FF);
    _data.set_measuredscale(_decim*_getScalling);

    // poco_information_f3(_logger, "_decim  = %u, _getScalling = %u, (_decim*_getScalling) = %u ", _decim, _getScalling, (_decim*_getScalling));

    //  MeasureSymbolRate SR Fs*NumSR/(16384*Scale), where Fs = AGC.Fs_ADC 
    bool _getSymbRate = (fft_crt&0x10000); 
    bool _getSRValid =           (reg_fft_scale&0x80000000); 
    // unsigned int measuredScale      = 1; 
    unsigned int measuredSymbolRate = 0;
    
    if(_getSymbRate){
        if(_getSRValid) {
            measuredSymbolRate = (reg_fft_scale>>16) & 0x7FFF; 
            //measuredScale = (reg_fft_scale & 0xFFF);
            /*  Restart Measure
            if(!(fft_crt&0x80000000)) {
                devmem.dev_mem_write(FFT_CTRL_ADDRESS, fft_crt + 0x80000000);
            }
            devmem.dev_mem_write(FFT_CTRL_ADDRESS, fft_crt);
            _measuredSymbolRate_previous = measuredSymbolRate; 
            _measuredScale_previous = measuredScale;
          } else {
            measuredSymbolRate = _measuredSymbolRate_previous; 
            measuredScale = _measuredScale_previous;
            */ 
        } 
        
    }

    
//    unsigned int _LPFCutoffFrequency = _settingsManager.getChannelSettings(0).getTunerSettings().getReference() .getReference();// getLPFCutoffFrequency();
    unsigned int _TunerFrequency = _settingsManager.getChannelSettings(0).getTunerSettings().getFrequency();
    unsigned int _FFT_FS = _settingsManager.getChannelSettings(0).getAGCSettings().get_AGC_FS_ADC();
//    unsigned int _getSubChannel = 7;
//    unsigned int _getSubChannelsNumber = 1;

    unsigned char _pending = static_cast<unsigned char>(_statisticsManager.getCommonStatistics().getConfigurationPending());
    _data.set_leaderstatus(header.leadingPart);    
    
    //int pin = static_cast<int>(_statisticsManager.getChannelStatistics(channel).getTunerStatistics().getAGCInputSignalLevel() * 1000);
    _data.set_pin(static_cast<int>(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getAGCInputSignalLevel()* 1000));
//    _data.set_subchannel(_getSubChannel);
//    _data.set_subchannelsnumber(_getSubChannelsNumber);
    _data.set_lpfcutofffrequency(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getLPFCutoffFrequency()); //_LPFCutoffFrequency);

    _data.set_centerfrequency(_TunerFrequency);
    _data.set_coreversion(_getVersion);
    _data.set_powerofaveraging(_getPowerOfAveraging);
    _data.set_scale(_getScalling);

    _data.set_measuredsymbolrate(measuredSymbolRate);
    _data.set_measuresymbolrate(false);
    //_data.set_measuredscale(measuredScale * 1000000u);


    if(_pending==0){
        unsigned char _virt_pending = _statisticsManager.getCommonStatistics().getVirtualPending();
        if(_virt_pending&(channel+1)) { // ==channel
//            poco_information_f1(_logger, "Warning. virt_pending = %u ",static_cast<unsigned int> (_virt_pending));
            if(_virt_pending&32){                   // снять признак для FFTDataProvider
                _virt_pending = _virt_pending - 32  ;  
                if(_virt_pending&64){               // проверить на наличие признака для sendChannelStatisticsToProto
                    _statisticsManager.getCommonStatistics().setVirtualPending(_virt_pending);
                    _virt_pending = _virt_pending - 64;
                } else {
                    _statisticsManager.getCommonStatistics().setVirtualPending(0);
                }
                _pending = _virt_pending;
            }
        }
    }
    if(_pending&(channel+1)){
        _data.set_configurationpending(_pending);
    } else {
        _data.set_configurationpending(0);
    }
//    unsigned int _decRate = 0;// _settingsManager.getChannelSettings(static_cast<unsigned int> (channel)).getADCSettings().getDecimationRate();                           
//    _data.set_adc_decimationrate(_decRate); // ChannelConf.adc.decimationRate
//    _data.set_dem_mode(_mode);                     // ChannelConf.dem.mode
    unsigned int _fft = _FFT_FS; // 366255524;// 125000000;
    _data.set_fftfs(_fft);
    //_data.set_channelstatus(_statisticsManager.getChannelStatistics(channel).getChannelStatus());
    _data.set_channelstatus(0);
    _data.set_lpfmin(_settingsManager.getChannelSettings(0).getTunerSettings().getTunerLPFMIN());
    _data.set_lpfmax(_settingsManager.getChannelSettings(0).getTunerSettings().getTunerLPFMAX());
    // -------------------------------------------------------------------------------------- Ticket # 715 
    try {
        if(_pending&(channel+1)) { // не вычитывать
            poco_debug(_logger, "_pending&(channel+1");
            for(unsigned int i = 0; i < DATA_NUMBER_16K; ++i){
                _data.add_samples(0);
            }
        } else {
            // samples.clear();
            //_fftDevices[channel]->getData(samples);
            // unsigned int write_date =0;
            unsigned int read_result=0; // result_data = 0, 

            //testmem -w 0x4e200018   0xc0000000
            devmem.dev_mem_write(0x4e200018, 0xc0000000);
            // testmem -w 0x4e200020  0x0b000000
            devmem.dev_mem_write(0x4e200020, 0x0b000000);
            // testmem -w 0x4e200028 0x10000
            // testmem -w 0x4e200028 0x10000
            devmem.dev_mem_write(0x4e200028, 0x10000);
            unsigned int ADD_POS_DATA = 0;

            // 16384
            //for(unsigned int i = 0; i < samples.size(); ++i){
            for(unsigned int i = 0; i < DATA_NUMBER_16K; ++i){
                read_result = devmem.dev_mem_read(0x0b000000 + ADD_POS_DATA);
                // read_result = (read_result&0xFFFF);
                _data.add_samples((unsigned int) read_result);
                /*
                if(i==32) {
                    poco_debug_f2(_logger, "!!! FFT ADD_POS_DATA = 0x%X, data = %u",  ADD_POS_DATA, (unsigned int) read_result);
                }
                if(i==33) {
                    poco_debug_f2(_logger, "!!! FFT ADD_POS_DATA = 0x%X, data = %u",  ADD_POS_DATA, (unsigned int) read_result);
                }
                if(i==36) {
                    poco_debug_f2(_logger, "!!! FFT ADD_POS_DATA = 0x%X, data = %u",  ADD_POS_DATA, (unsigned int) read_result);
                }
                */
                ADD_POS_DATA = ADD_POS_DATA + 0x04;
            }

            //unsigned int _getFrameNumber =  _fftDevices[channel]->getFrameNumber();
            //dev_mem_write(MAIN_ADDRESS, SPI1_MAIN_ADDRESS);
            // _framenumber = dev_mem_read(FFT_FRMCNT_ADDRESS);
            _data.set_framenumber(_framenumber);
            _framenumber++;
            if(_framenumber==0x7FFFFFFF) {
                _framenumber = 0;
            }
        }
    }
    catch (...){ 
        poco_debug(_logger, "FFT data read failed %s");
    }
    if (!_data.IsInitialized()){
        poco_debug(_logger, "protobuf error");  
    }
}

void FFTDataProvider::apllySavedConfig(const Poco::AutoPtr<ApplyMetersSettingsNotification>& pNf)
{
    // poco_debug(_logger, "TRACE  - apllySavedConfig");
//    unsigned int channel = pNf->getData().channel;
//    _fftDevices[channel]->applyConfig();
}

void FFTDataProvider::handleClearData()
{
//    _memDevices.clear();
//    _agcDevices.clear();
//    _fftDevices.clear();
}
/*
void FFTDataProvider::dev_mem_write(off_t target, unsigned int data){
    int fd;
    void *map_base, *virt_addr;

    unsigned int workdata1 = 0;

    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    fflush(stdout);

    // Map one page
    map_base =  mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *)
            -1) FATAL;
    fflush(stdout);

    virt_addr = map_base + (target & MAP_MASK);
//    printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result);
    fflush(stdout);

     workdata1 = (unsigned int) (data);
    *((unsigned int *) virt_addr) = workdata1;

    fflush(stdout);

    if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
}

unsigned int FFTDataProvider::dev_mem_read(off_t target){
    int fd;
    void *map_base, *virt_addr;
    unsigned int read_result = 0;
    if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
    fflush(stdout);

    // Map one page
    map_base =  mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *)
            -1) FATAL;
    fflush(stdout);

    virt_addr = map_base + (target & MAP_MASK);
//    printf("Value at address 0x%X (%p): 0x%X\n", target, virt_addr, read_result);
    fflush(stdout);

    read_result = *((unsigned int *) virt_addr);
        fflush(stdout);

    if(munmap(map_base, MAP_SIZE) == -1) FATAL;
    close(fd);
    return read_result;
}
*/

}//namespace MetersSpace


#include "DeviceConfigurator.hpp"
/*---------------------------------------------------------------------------*/
#include "Common/Defines.hpp"
#include "Common/Tracer.hpp"
#include "Common/Helpers.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/NObserver.h"
#include "Poco/Util/Application.h"
#include "Poco/ScopedLock.h"
#include "Poco/Delegate.h"
using Poco::Logger;
using Poco::NObserver;
using Poco::Util::Application;
//using Poco::delegate;

/*---------------------------------------------------------------------------*/
DeviceConfigurator::DeviceConfigurator(Settings::SettingsManager& settings, Statistics::StatisticsManager& statistics, Poco::NotificationCenter& nc)
: _settings(settings)
, _statistics(statistics)
, _nc(nc)
// , _loader(CHANNELS_NUMBER)
, _logger(Logger::get("Monitor.DeviceConfigurator"))
{
//    TRACE();
    _statistics.getCommonStatistics().setVirtualPending(0);
    for(unsigned int i = 0; i < CHANNELS_NUMBER; ++i) {
        _statistics.getCommonStatistics().setConfigurationPending(i+1);
        ChannelPtr pChan = new Channel(i,
                                       _settings,
                                       _statistics,
                                       _settings.getChannelSettings(i),                                       
                                       _statistics.getChannelStatistics(i),
                                       _nc);

        _channels.push_back(pChan);
    }

    _nc.addObserver(NObserver<DeviceConfigurator,
                              ChannelReconfigurationNotification>(*this, &DeviceConfigurator::handleChannelReconfigurationNotification));

    _nc.addObserver(NObserver<DeviceConfigurator,
                              ApplyChannelConfigurationNotification>(*this, &DeviceConfigurator::handleApplyChannelConfigurationNotification));

//    _nc.addObserver(NObserver<DeviceConfigurator, ApplyFadingChannelConfigurationNotification>(*this, &DeviceConfigurator::handleApplyFadingChannelConfigurationNotification));

    _nc.addObserver(NObserver<DeviceConfigurator,
                              ApplyDeviceSettingsNotification>(*this, &DeviceConfigurator::handleApplyDeviceSettingsNotification));

    _nc.addObserver(NObserver<DeviceConfigurator, ApplyTunerConfigurationNotification>(*this, &DeviceConfigurator::handleApplyTunerConfigurationNotification));

    _reconfigureChannelEvent        += Poco::delegate(this, &DeviceConfigurator::onReconfigureChannel);
    _applyChannelConfigurationEvent += Poco::delegate(this, &DeviceConfigurator::onApplyChannelConfiguration);
    _deviceSettingsEvent            += Poco::delegate(this, &DeviceConfigurator::onApplyDeviceSettings);
}


DeviceConfigurator::~DeviceConfigurator()
{
    // TRACE();

    _nc.removeObserver(NObserver<DeviceConfigurator,
                                 ChannelReconfigurationNotification>(*this, &DeviceConfigurator::handleChannelReconfigurationNotification));

    _nc.removeObserver(NObserver<DeviceConfigurator,
                                 ApplyChannelConfigurationNotification>(*this, &DeviceConfigurator::handleApplyChannelConfigurationNotification));

//    _nc.removeObserver(NObserver<DeviceConfigurator,ApplyFadingChannelConfigurationNotification>(*this, &DeviceConfigurator::handleApplyFadingChannelConfigurationNotification));

    _nc.removeObserver(NObserver<DeviceConfigurator,
                                 ApplyDeviceSettingsNotification>(*this, &DeviceConfigurator::handleApplyDeviceSettingsNotification));

    _nc.removeObserver(NObserver<DeviceConfigurator, ApplyTunerConfigurationNotification>(*this, &DeviceConfigurator::handleApplyTunerConfigurationNotification));

    _reconfigureChannelEvent        -= Poco::delegate(this, &DeviceConfigurator::onReconfigureChannel);
    _applyChannelConfigurationEvent -= Poco::delegate(this, &DeviceConfigurator::onApplyChannelConfiguration);
    _deviceSettingsEvent            -= Poco::delegate(this, &DeviceConfigurator::onApplyDeviceSettings);
}

void DeviceConfigurator::initDevice()
{
//  TRACE();    
    for(unsigned char i = 0; i < _channels.size(); ++i) {
        _statistics.getCommonStatistics().setConfigurationPending(i+1);
        _channels[i]->configureConstValues();
        _channels[i]->configure();
    }   
    _statistics.getCommonStatistics().setConfigurationPending(0);
     Poco::Thread::sleep(1000);
}


void DeviceConfigurator::handleChannelReconfigurationNotification(const Poco::AutoPtr<ChannelReconfigurationNotification>& pNf)
{
//   TRACE();
    unsigned int channel = pNf->getData().channel;
    checkChannelNumber(channel);
    _reconfigureChannelEvent.notifyAsync(this, channel);
}


void DeviceConfigurator::handleApplyChannelConfigurationNotification(const Poco::AutoPtr<ApplyChannelConfigurationNotification>& pNf)
{
//    TRACE();
    unsigned int channel = pNf->getData().channel;
    checkChannelNumber(channel);
    _applyChannelConfigurationEvent.notifyAsync(this, channel);
}

void DeviceConfigurator::handleApplyFadingChannelConfigurationNotification(const Poco::AutoPtr<ApplyFadingChannelConfigurationNotification>& pNf)
{
//    TRACE();
    unsigned int channel = pNf->getData().channel;
    checkChannelNumber(channel);
    onApplyFadingChannelConfiguration(channel);
}

void DeviceConfigurator::handleApplyDeviceSettingsNotification(const Poco::AutoPtr<ApplyDeviceSettingsNotification>& pNf)
{
//    TRACE();
    bool save = pNf->getData().saveConfig;
    _deviceSettingsEvent.notifyAsync(this, static_cast<int>(save));
}

void DeviceConfigurator::handleApplyTunerConfigurationNotification(const Poco::AutoPtr<ApplyTunerConfigurationNotification>& pNf)
{
//    TRACE();
    unsigned int channel = pNf->getData().channel;
    checkChannelNumber(channel);
    onApplyTunerConfiguration(channel);
}

void DeviceConfigurator::onApplyChannelConfiguration(const void* pSender, ChannelID& channel)
{
//    TRACE();
   bool _controlUDP =0;
   if(WaitingPending(500, 10, false)) {
        _statistics.getCommonStatistics().setConfigurationPending(static_cast<unsigned char>(channel+1));
        _controlUDP = _statistics.getCommonStatistics().statusChannelControlConnection(channel);
        if(_controlUDP) { // канал ещё занят процессом предедущего отсоединения 
            poco_information_f2(_logger, "Erorr applying configuration for channel #%u, but status Channel Control Connection = %u ", static_cast<unsigned int> (channel+1),  static_cast<unsigned int> (_controlUDP));
        }
        poco_information_f1(_logger, "Applying configuration for channel #%u", static_cast<unsigned int> (channel+1));
        _channels[channel]->configure();
        _settings.update();
        _statistics.getCommonStatistics().setConfigurationPending(0);
   }
}


void DeviceConfigurator::onApplyFadingChannelConfiguration(ChannelID& channel)
{
    TRACE();
   bool _controlUDP =0;
   unsigned char _pending = _statistics.getCommonStatistics().getConfigurationPending();
   if(_pending==0) {
        _statistics.getCommonStatistics().setConfigurationPending(static_cast<unsigned char>(channel+1));
        _controlUDP = _statistics.getCommonStatistics().statusChannelControlConnection(channel);
        if(_controlUDP) { // канал ещё занят процессом предедущего отсоединения 
            poco_information_f2(_logger, "Erorr applying configuration for channel #%u, but status ControlUDP = %u ", static_cast<unsigned int> (channel+1),  static_cast<unsigned int> (_controlUDP));
        }
        poco_information_f1(_logger, "Applying configuration for channel #%u", static_cast<unsigned int> (channel+1));
        _channels[channel]->configure();
        _settings.update();
        _statistics.getCommonStatistics().setConfigurationPending(0);
    } else {
        poco_information_f2(_logger, "Erorr applying configuration for channel #%u, but status Pending = %u", static_cast<unsigned int> (channel+1), static_cast<unsigned int> (_pending));
    }

}

void DeviceConfigurator::onApplyTunerConfiguration(ChannelID& channel)
{
   TRACE();
   unsigned char _count = 0;
   bool _controlUDP =0;
   unsigned char _pending = _statistics.getCommonStatistics().getConfigurationPending();
   if(_pending==0) {
        _statistics.getCommonStatistics().setConfigurationPending(static_cast<unsigned char>(channel+1+32)); // +32 - всё устройство не задействуется 
        _controlUDP = _statistics.getCommonStatistics().statusChannelControlConnection(channel);
        if(_controlUDP) { // канал ещё занят процессом предедущего отсоединения 
            // poco_information(_logger, "!! _controlUDP >0");
            _count = 0;
            do{
                _controlUDP = _statistics.getCommonStatistics().statusChannelControlConnection(channel);
                if(_controlUDP==true) {
                    if(_count==0) {
                        poco_information(_logger, "Wait until you teminate all connections ...");
                    }
                    Poco::Thread::sleep(20);
                    _count++;
                }
            }while(_controlUDP==true && _count<100); 
        }
        
        if(_controlUDP==true) { // аварийное выключение 
            poco_information_f2(_logger, "Warning! Applying configuration for channel #%u, but status ControlUDP = %u ", static_cast<unsigned int> (channel+1),  static_cast<unsigned int> (_controlUDP));
            // _nc.postNotification(new TeminateChannelDataConnectionsNotification(TeminateChannelDataConnectionsNotificationPayload(channel)));
        } else {
            poco_information_f1(_logger, "Applying TUNER configuration for channel #%u", static_cast<unsigned int> (channel+1));
        }
           _channels[channel]->TunerConfigure(); 
           _settings.update();   
           _statistics.getCommonStatistics().setConfigurationPending(0);
    } else {
        poco_information_f2(_logger, "Erorr applying configuration for channel #%u, but status Pending = %u", static_cast<unsigned int> (channel+1), static_cast<unsigned int> (_pending));
    }
} 

void DeviceConfigurator::onReconfigureChannel(const void* pSender, ChannelID& channel)
{
//      TRACE();
    if(WaitingPending(500, 200, true)) {
        _statistics.getCommonStatistics().setConfigurationPending(static_cast<unsigned char>(channel+1));        
        poco_information_f1(_logger, "Reconfigure. Applying configuration for channel #%u", channel);
        _channels[channel]->configure();       
        _statistics.getCommonStatistics().setConfigurationPending(0);         
    }
}

void DeviceConfigurator::onApplyDeviceSettings(const void* pSender, int& arg)
{
 //   TRACE();
    poco_information(_logger, "Saving temporary device configuration");
    _settings.update();
    if(arg) {
         poco_information(_logger, "Saving mutable configuration");
         _settings.saveMutableConfig();
    } 
}

bool DeviceConfigurator::WaitingPending(unsigned int SleepTime, unsigned int Max_count, bool _avarout)
{
   unsigned char _pending = 0;
   unsigned int _count_attempt = 0;
   do{
       _pending = _statistics.getCommonStatistics().getConfigurationPending();
       if(_pending>0) {
           Poco::Thread::sleep(SleepTime);
           _count_attempt++;
           poco_information_f2(_logger, "%u. Configuration not applying for (pending = %u)", _count_attempt, static_cast<unsigned int> (_pending));
           if(_count_attempt==Max_count) {
               break;
           }
       }
   } while(_pending!=0);
   if(_pending>0) {
        poco_information_f1(_logger, "Erorr applying settings but Pending  = %u ", static_cast<unsigned int> (_statistics.getCommonStatistics().getConfigurationPending()));
        if(_avarout) {  // аварийное выключение через 60 секунд
            _statistics.getCommonStatistics().setConfigurationPending(0); 
            return true;
        }  else {
            return false;
        }
   }
   return true;
}

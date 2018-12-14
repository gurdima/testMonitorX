#include "Connection/ConnectionHandler.hpp"
/*-----------------------------------------*/
#include "Connection/DataConversion.hpp"
#include "Connection/ProtoDataConversion.hpp"
#include "Common/Tracer.hpp"
#include "Common/Helpers.hpp"
#include "Common/Defines.hpp"
#include "Common/BitFileParser.hpp"
#include "Common/ByteOperations.hpp"
#include "UART/tty.h"
// #include "FPGA/LoadHelpers.hpp"
// #include "TDMA/TDMAConfig.hpp"
#include "MonitorApplication.hpp"
#include "Notifications.hpp"


//#include "Decoder/DecoderManager.hpp"
// #include "BitFileParser.hpp"
// using Settings::DecoderSettings;
// using Settings::HDRMSettings;

/*---------------------------------------------------------------------------*/
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"
#include "Poco/Process.h"
#include "Poco/Format.h"

using Poco::Net::StreamSocket;
using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;

using Poco::Timespan;
using Poco::Logger;

using Poco::DateTime;

using Poco::Process;
using Poco::ProcessHandle;

using Poco::format;

/*---------------------------------------------------------------------------*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// For time setting
#include <time.h>
#include <sys/time.h>

// For system reboot
#include <unistd.h>
#include <sys/reboot.h>

#include <string>
#include <vector>

using std::string;
using std::vector;
/*---------------------------------------------------------------------------*/
#define POLL_TIMEOUT    200 // old 200

// #define PAYLOAD_SIZE    8198
/*---------------------------------------------------------------------------*/

namespace
{

                                                            
void readFromProto(zmq::socket_t& zSocket, zmq::message_t& reply)
{                                                            
    bool resp = true;                                        
                                                                              
    zSocket.recv(&reply);                                    
                                                             
    zSocket.send(&resp, sizeof(resp));                       
}                                                            
                                        
template<class T>                                        
void writeToProto(zmq::socket_t& zSocket, const T& data) 
{                                                            
    zmq::message_t msg(data.ByteSize());                     
    data.SerializeToArray(msg.data(), data.ByteSize());      
    zSocket.send(msg);                                       
}                                                            

void rebootSystem()
{
/*    int fd_UART =0;
    fd_UART = openUART1();
    char ini_Uminodiode[MAX_LEN_TTY];
    strcpy(ini_Uminodiode, "#w0000000000\r\n");
    writeUART(fd_UART, ini_Uminodiode);
    closeUART(fd_UART);
*/
    reboot(RB_AUTOBOOT);
}

} // namespace

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{
ConnectionHandler::ConnectionHandler(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _nc(nc)
, _stop(false)
, _logger(Logger::get("Monitor.SNMP.ConnectionHandler"))
{
    status_arm = 0;
    old_pending[0]  = 3;
    old_pending[1]  = 3;
    refresh = 0;
    // Statistics::ChannelStatistics& inStat  =  _statisticsManager.getChannelStatistics(0);    
    // inStat.setLeaderStatus(0);
    // TRACE();
}

ConnectionHandler::~ConnectionHandler()
{
//    TRACE();
}

void ConnectionHandler::run()
{
    TRACE();
    try{
        _zContext = new zmq::context_t(1);
        _zSocket  = new zmq::socket_t(*_zContext, ZMQ_REP);

        _zSocket->bind(SNMP_IO_UNIX_SOCKET_ADDRESS);

        zmq::pollitem_t pollitems[] = {
            {*_zSocket, 0, ZMQ_POLLIN, 0}
        };
        poco_information(_logger, "ConnectionHandler::run()");
         while(!_stop) {
            zmq::poll(pollitems, sizeOfArray(pollitems), POLL_TIMEOUT);
            if(pollitems[0].revents & ZMQ_POLLIN) {
                zmq::message_t request;
                _zSocket->recv(&request);
                processHeader(*reinterpret_cast<MessageHeader*>(request.data()));
            }
         }
    }
    catch (const zmq::error_t& exc){
        poco_debug_f1(_logger, "Exception: %s", std::string(exc.what()));
    }
}

void ConnectionHandler::stop()
{
    // TRACE();
    _stop = true;
}


void ConnectionHandler::processHeader(const MessageHeader& header)
{
    // TRACE();
    // poco_information_f1(_logger, "ProcessHeader, type - %u", header.messageType);
    switch(header.messageType) {
    case MSG_TYPE_GET_PROTO_CHANNEL_CONFIG:         // GET http://192.168.20.XX/wsgui_1_config _TDMA_
        sendChannelConfigurationToProto(header.channel);
        break;
    case MSG_TYPE_GET_PROTO_CHANNEL_STAT:           // GET http://192.168.20.XX/wsgui_1_stat ++ !!! отправляется по таймеру TDMA_
        sendChannelStatisticsToProto(header.channel, header.leadingPart);
        break;
    case MSG_TYPE_GET_PROTO_DEVICE_INFO:            // GET http://192.168.20.XX/wsgui_1_deviceInfo
        sendDeviceInfoToProto();
        break;
//    case MSG_TYPE_GET_PROTO_DEVICE_SETTINGS:        // GET http://192.168.20.XX/wsgui_1_settings
//        sendDeviceSettingsToProto();
//        break;
//case MSG_TYPE_GET_PROTO_CONNECTIONS_INFO:       // GET http://192.168.20.XX/wsgui_1_connectionInfo
//        sendChannelConnectionInfoToProto(header.channel);
//        break;
    case MSG_TYPE_SET_PROTO_DEVICE_SETTINGS:        // GET http://192.168.20.XX/wsgui_1_setDeviveSettings
        receiveDeviceSettingsFromProto(header.channel);
        break;
    case MSG_TYPE_SET_PROTO_CHANNEL_CONFIG:
        receiveChannelConfigurationFromProto(header.channel);  // _TDMA_
        break;
    default:
        poco_error_f1(_logger, "Unknown message type: %u", header.messageType);
        break;
    }
}


void ConnectionHandler::sendChannelConfigurationToProto(ChannelID channel)
{
    TRACE();
    GUI::ChannelConf data; 
    convertChannelConfigMon2Proto(_settingsManager.getChannelSettings(channel), data); // , _settingsManager.getTDMASettings().getNumberOfChannelsAllowed()
    writeToProto(*_zSocket, data);
}

#define ChannelSpeed    10

void ConnectionHandler::sendChannelStatisticsToProto(ChannelID channel, unsigned int idLeading)
{
    //TRACE();
    Statistics::ChannelStatistics& inStat  =  _statisticsManager.getChannelStatistics(channel);    
    /*
    if(inStat.getChannelStatus() & CHANNEL_STATUS_MSG_LEADER_DLL){
        inStat.setLeaderStatus(0);
    } else {
        inStat.setLeaderStatus(idLeading);
    }
    */
    // inStat.setLeaderStatus(0);
    inStat.setLeaderStatus(idLeading);
   // poco_information_f1(_logger, "TRACE. Channel status. LeaderStatus = %u", inStat.getLeaderStatus());
 
    GUI::ChannelStat data;
    convertChannelStatMon2Proto(_statisticsManager.getChannelStatistics(channel), data); 
    // poco_information(_logger, "1 TRACE. convertChannelStatMon2Proto(_statisticsManager.getChannelStatistics(channel), data)");

    // convertLinkAddStatMon2Proto         (_statisticsManager.getChannelStatistics(channel), data, false);
    // data.set_leaderstatus(_statisticsManager.getChannelStatistics(channel).getLeaderStatus());
    data.add_ethernetlinkstatus(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetLinkStatus());
// poco_information_f1(_logger, "getEthernetLinkStatus() = %u", _statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetLinkStatus());
        // data.add_ethernetlinkstatus(_statisticsManager.getChannelStatistics(0).getConnectionStatistics(0).getEthernetLinkStatus());
        // data.add_totalchannelspeed(ChannelSpeed);    


    unsigned char _pending = static_cast<unsigned char>(_statisticsManager.getCommonStatistics().getConfigurationPending());
    if(_pending>0)
    {
        status_arm=0;
    }
    if(status_arm&(static_cast<unsigned char> (channel+1))) {
            poco_information_f2(_logger, "Warning. Pending (%u) != status_arm(%u)", static_cast<unsigned int> (_pending), static_cast<unsigned int> (status_arm));
            _pending = status_arm;
            status_arm = 0;
    };  
    unsigned char _virt_pending = _statisticsManager.getCommonStatistics().getVirtualPending();
    if(_virt_pending&(channel+1)) { 
        poco_information_f2(_logger, "Warning. virt_pending = %u, channel = %u ",static_cast<unsigned int> (_virt_pending), static_cast<unsigned int> (channel));
        if(_virt_pending&64){
            _virt_pending = _virt_pending - 64;
            if(_virt_pending&32){
                _statisticsManager.getCommonStatistics().setVirtualPending(_virt_pending);
                _virt_pending = _virt_pending - 32;
            } else {
                _statisticsManager.getCommonStatistics().setVirtualPending(0);
            }
            if(_pending==0) {
                _pending = _virt_pending;
            }
        }
    }
    if(_pending&32){
        _pending = _pending - 32;
        if(_pending&(channel+1)){
            data.set_configurationpending(static_cast<unsigned int> (_pending));
        } else {
            data.set_configurationpending(0);
        }
    } else {
        data.set_configurationpending(static_cast<unsigned int> (_pending));
    }
    unsigned char _channelPlus =  (channel+1);

    if((_pending==0 && (old_pending[channel]&_channelPlus)) || refresh&(_channelPlus)) {
        DeviceInfoToProto(*data.mutable_devinfo());
        convertChannelConfigMon2Proto(_settingsManager.getChannelSettings(channel), (*data.mutable_chanconf())); 
        poco_information_f3(_logger, "Channel = %u, data.chanconf.tuner.frequency = %u, refresh= %u", static_cast<unsigned int> (channel), data.chanconf().tuner().frequency(), static_cast<unsigned int> (refresh));
        if(refresh>0) {
            refresh = refresh - _channelPlus;
        }
//        printChannelConfigProto(_logger, (*data.mutable_chanconf())); // , maxOfChannelsInTDMA
    }
    old_pending[channel] = _pending;

    MonitorApplication& monApp = dynamic_cast<MonitorApplication&>(Poco::Util::ServerApplication::instance());    
    data.set_cpuidle(static_cast<int>(monApp.getSysMonitor()->getCPUUsage()));
    // poco_information_f1(_logger, "cpuidle = %u", static_cast<unsigned int>(data.cpuidle()));
//    Poco::Timestamp currentTime;   
//    data.set_dateandtime(currentTime.epochTime()); 
    data.set_leaderstatus(_statisticsManager.getChannelStatistics(0).getLeaderStatus());
    //data.set_channelstatus(_statisticsManager.getChannelStatistics(channel).getChannelStatus());
    // data.set_leaderstatus(0);
    data.set_channelstatus(0);
    //data.set_destinationipaddr(ntohl(reinterpret_cast<const in_addr*>(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetIPDestination())));
    // outConf->mutable_channelinfo(subchannel)->mutable_subchannel(conn)->set_remoteip(ntohl(reinterpret_cast<const in_addr*>(udp[conn].getClientAddress().host().addr())->s_addr));

    data.set_destinationipaddr(     _statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetIPDestination());
    data.set_destinationport(       _statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetDestinationPort());
    data.add_totalchannelspeed(_settingsManager.getChannelSettings(0).getAGCSettings().get_AGC_FS_ADC() / _settingsManager.getChannelSettings(0).getDecimationSettings().getDecimationRate());

    data.set_numberdatarequested(   _statisticsManager.getChannelStatistics(0).getTunerStatistics().getEthernetDataPackets());
    data.set_fftfs(                 _settingsManager.getChannelSettings(0).getAGCSettings().get_AGC_FS_ADC());   

    writeToProto(*_zSocket, data);
 
}

void ConnectionHandler::receiveChannelConfigurationFromProto(ChannelID channel)
{
    TRACE();
    GUI::ChannelConf dataConf;
    zmq::message_t   reply;
    readFromProto(*_zSocket, reply);

    unsigned int _pending = 0;
    if(dataConf.ParseFromArray(reply.data(), reply.size())) {
        _pending = _statisticsManager.getCommonStatistics().getConfigurationPending();
        if(_pending==0) {
            if(_statisticsManager.getChannelStatistics(channel).getChannelStatus()==CHANNEL_STATUS_MSG_SLAVE) { // NOT SLAVE BYPASS 2GE
                poco_information_f1(_logger, "Erorr update configuration for channel #%u, but Channel is in t   he status - SLAVE BYPASS 2GE", static_cast<unsigned int> (channel+1));
            } 
            else {
                //MonitorApplication& monApp = dynamic_cast<MonitorApplication&>(Poco::Util::ServerApplication::instance());
                //bool _setThisMode = monApp.StatusSetThisMode(dataConf.dem().mode());
                //if(_setThisMode) {
                     status_arm = channel+1;
                     // unsigned int maxOfChannelsInTDMA =  _settingsManager.getTDMASettings().getNumberOfChannelsAllowed();
                     poco_information_f1(_logger, "Channel #%u configuration:", static_cast<unsigned int>(channel+1));
                     printChannelConfigProto(_logger, dataConf); // , maxOfChannelsInTDMA
                     convertChannelConfigProto2Mon(dataConf, _settingsManager.getChannelSettings(channel));
                     _nc.postNotification(new ApplyChannelConfigurationNotification(ApplyChannelConfigurationNotificationPayload(channel)));
                //} 
            }
        } else {
            poco_information_f2(_logger, "Erorr update configuration for channel #%u, but Pending = %u", static_cast<unsigned int> (channel+1) , static_cast<unsigned int> (_pending));
        }

     } else{
         poco_information(_logger, "Parse channel configuration from proto failed");
     }
 
}

void ConnectionHandler::DeviceInfoToProto(GUI::DeviceInfo& data)
{
    // TRACE();
    MonitorApplication& monApp = dynamic_cast<MonitorApplication&>(Poco::Util::ServerApplication::instance());
    data.set_cpuidle(static_cast<int>(monApp.getSysMonitor()->getCPUUsage()));
    // data.set_cpuidle(1);
    //poco_information_f1(_logger, "TRACE. _monApp.getSysMonitor()->getCPUUsage() = %u :", (static_cast<int>(monApp.getSysMonitor()->getCPUUsage())));         
    data.set_configurationpending(static_cast<unsigned char>(_statisticsManager.getCommonStatistics().getConfigurationPending()));
    // poco_information_f1(_logger, "TRACE. set_configurationpending = %u :",static_cast<unsigned int>( _statisticsManager.getCommonStatistics().getConfigurationPending()));         

    try {
        std::string swVersion = getSoftwareVersion();
        data.set_installversion(swVersion);

        std::string file = "/system/bit/interfaces_top.bit"; // FPGA::selectFileName(FPGA::composeBitFileBaseName(0, FPGA::LoadTargetM2K, 0));
        std::string version = getBitFileVersion(file);
        data.set_fpgaversion(version);
    }
    catch (const Poco::Exception& exc) {
        _logger.log(exc);
    }

    string swVersion = getSoftwareVersion();
    data.set_installversion(swVersion);

    // new field  :
    data.set_softversion(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getSoftwareVersion()); // TSFP Soft Version
    data.set_referencefrequency(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getReferenceFrequency());
    data.set_serialnumber(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getSerialNumber());
    data.set_hwrevision(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getHWRevisionType());
    data.set_calibrationdate(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getTunerSoftwareRelease());// "24.04.1991");

    Settings::TunerSettings& tunerSettings = _settingsManager.getChannelSettings(0).getTunerSettings();
    Settings::AGCSettings& agcSettings = _settingsManager.getChannelSettings(0).getAGCSettings();
    data.set_lpfmin(tunerSettings.getTunerLPFMIN());
    data.set_lpfmax(tunerSettings.getTunerLPFMAX());
    data.set_fmin(tunerSettings.getTunerFMIN());
    data.set_fmax(tunerSettings.getTunerFMAX());
    data.set_mangc1min(agcSettings.get_MANUAL_LBAND_GAIN_MIN());
    data.set_mangc1max(agcSettings.get_MANUAL_LBAND_GAIN_MAX());
    // string deviceName = _settingsManager.getCommonSettings().getDeviceName();
    // data.set_devicename(deviceName);
}

void ConnectionHandler::sendDeviceInfoToProto()
{
    TRACE();
    GUI::DeviceInfo data;
    DeviceInfoToProto(data);
    writeToProto(*_zSocket, data);
}
/*
void ConnectionHandler::sendDeviceSettingsToProto()
{
    TRACE();
    GUI::DeviceSettings settings;
    convertDeviceSettingsMon2Proto(_settingsManager.getCommonSettings(), settings);
    writeToProto(*_zSocket, settings);
}
*/
void ConnectionHandler::receiveDeviceSettingsFromProto(ChannelID channel)
{
    // TRACE();
    GUI::DeviceSettings dataSettings;
    zmq::message_t reply;

    readFromProto(*_zSocket, reply);
    
    if(dataSettings.ParseFromArray(reply.data(), reply.size())) {
       if(dataSettings.saveconfiguration() & (0x1 << 1)) {
               poco_information(_logger, "dataSettings.saveconfiguration() & (0x1 << 1)");
            // convertDeviceSettingsProto2Mon(dataSettings, _settingsManager.getCommonSettings());
            _nc.postNotification(new ApplyDeviceSettingsNotification(ApplyDeviceSettingsNotificationPayload(dataSettings.saveconfiguration() & 0x1)));
       } else if(dataSettings.saveconfiguration() & 0x1) {
               poco_information(_logger, "dataSettings.saveconfiguration() & 0x1");
              _nc.postNotification(new ApplyDeviceSettingsNotification(ApplyDeviceSettingsNotificationPayload(dataSettings.saveconfiguration() & 0x1)));
       }

       if(dataSettings.reset() & (0x4)){
            refresh = (unsigned char) (channel + 1);
            // poco_information(_logger, "Refresh");
        } 
        if(dataSettings.reset() & (0x1 << 1)){
            poco_information(_logger, "Error Setting time on device. This function deactivate in this model!!!");
            // setTime(Poco::Timestamp::fromEpochTime(dataSettings.dateandtime()));
            //setHWClock();
        }

        if(dataSettings.reset() & 0x1) {
            poco_information(_logger, "Rebooting system by users request");
            rebootSystem();
        }
     } else {
         poco_information(_logger, "Parse device settings from proto failed");
     }
}


} // namespace SNMP


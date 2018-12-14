#include "Connection/ZMQServer.hpp"
/*---------------------------------------------------------------------------*/
#include "Connection/DataConversion.hpp"
#include "Common/Tracer.hpp"
#include "Common/Helpers.hpp"
#include "Common/Defines.hpp"
#include "MonitorApplication.hpp"
#include "Notifications.hpp"
//#include "BitFileParser.hpp"
//#include "FPGA/LoadHelpers.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Timestamp.h"
#include "Poco/Format.h"
#include "Poco/ScopedLock.h"

using Poco::ScopedLock;
using Poco::Mutex;
using Poco::Timestamp;
using Poco::DateTime;
using Poco::format;
using Poco::Logger;
/*---------------------------------------------------------------------------*/
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
#include "monitord_def.h"
/*---------------------------------------------------------------------------*/
#define POLL_TIMEOUT    200 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{
ZMQServer::ZMQServer(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _nc(nc)
, _logger(Logger::get("Monitor.SNMP.ZMQServer")) 
, _connZMQHandler1(_settingsManager, _statisticsManager, _nc, 1)
, _connZMQHandler2(_settingsManager, _statisticsManager, _nc, 2)
{
// , _logger(Poco::Logger::get("Monitor.SNMP.ZMQServer"))    
    pdescrDev.DescriptorCh1 = 0;
    pdescrDev.DescriptorCh2 = 0;
    pdescrDev.ClientIPAddrCh1 = 0;
    pdescrDev.ClientIPAddrCh2 = 0;
}

ZMQServer::~ZMQServer()
{
//    TRACE();
}

void ZMQServer::run()
{
    
    Poco::Thread::sleep(70000);
/*
    try{
        poco_debug(_logger, "Received ZMQServer...");
        _stop = false;
        ZMQContext = zmq_ctx_new(); 
        ZMQSocket  = zmq_socket(ZMQContext, ZMQ_REP); 
        // zmq_bind(ZMQSocket, "tcp://*:4040");
        zmq_pollitem_t pollitems[] = {
            {ZMQSocket, 0, ZMQ_POLLIN, 0}
        };
//        _pTask = new TimerTaskAdapter<ZMQPUBServer>(*this,&ZMQPUBServer::onTimer);
//        _timer.schedule(_pTask, 2000, 1000);
        _threadStart[0] = false;
        _threadStart[1] = false; 
        char msg[256];
        while(!_stop) {
            zmq_poll(pollitems, sizeOfArray(pollitems), POLL_TIMEOUT);
            if(pollitems[0].revents & ZMQ_POLLIN) {
                zmq_msg_t request;
                zmq_msg_init(&request);
                zmq_msg_recv(&request, ZMQSocket, 0);
                int length = zmq_msg_size(&request);
                memcpy((char *)msg, zmq_msg_data(&request), length);
                processHeader(*reinterpret_cast<ZMQServerManager*>((char *)msg));

            }
        }
    }
    catch (const zmq::error_t& exc){
        poco_debug_f1(_logger, "Exception: %s", std::string(exc.what()));
    }

    zmq_close(ZMQSocket);
    zmq_ctx_destroy(ZMQContext); 
    */ 
    poco_debug(_logger, "Exit ZMQ Server ...");
}

void ZMQServer::stop()
{
    // TRACE();
    /*
    _stop = true;
    _connZMQHandler1.stop();
    _connZMQHandler2.stop();
    _threadPoll1.join();
    _threadPoll2.join(); 
    */ 
}

template <typename T>
void writeToZMQ(void* zSocket, const T& data)
{
    zmq_msg_t answer;
    zmq_msg_init_size(&answer, sizeof(data));
    memcpy(zmq_msg_data(&answer), (char *) &data, sizeof(data));
    zmq_msg_send(&answer, zSocket, 0);
    zmq_msg_close(&answer);
}

void ZMQServer::sendDeviceInfo()
{
    // TRACE();
    /* 
       DeviceInfo data;
    string version;
    string file;

    memset(&data, 0, sizeof(data));

    try {
        // M2K
        file = FPGA::selectFileName(FPGA::composeBitFileBaseName(0, FPGA::LoadTargetM2K, 0));
        version = getBitFileVersion(file);
        memcpy(data.m2kFPGAVersion, version.c_str(), version.length());

        for(unsigned int i = 0; i < CHANNELS_NUMBER; ++i) {
            // DEM[i]
            version =  _settingsManager.getChannelSettings(i).getDemBitFileVersion();
            memcpy(data.demFPGAVersion[i], version.c_str(), version.length());
            // DEC[i]
            version =  _settingsManager.getChannelSettings(i).getDecBitFileVersion();
            memcpy(data.decFPGAVersion[i], version.c_str(), version.length());
        }
    }
    catch (const Poco::Exception& exc) {
        _logger.log(exc);
    }

    string swVersion = getSoftwareVersion();
    memcpy(data.installVersion, swVersion.c_str(), swVersion.length());

    MonitorApplication& monApp = dynamic_cast<MonitorApplication&>(Poco::Util::ServerApplication::instance());
    data.cpuIdle = static_cast<int>(monApp.getSysMonitor()->getCPUUsage());
    data.configurationPending = static_cast<unsigned char>(_statisticsManager.getCommonStatistics().getConfigurationPending());

    string blockSizeAndInfBits = _statisticsManager.getDecoderConfigParser().makeLDPCCodeString();
    strncpy(data.ldpcInfo.blockSizeAndInfBits, blockSizeAndInfBits.c_str(), blockSizeAndInfBits.size());

    string ullBlockSizeAndInfBits = _statisticsManager.getDecoderConfigParser().makeULLCodeString();
    strncpy(data.ullInfo.blockSizeAndInfBits, ullBlockSizeAndInfBits.c_str(), ullBlockSizeAndInfBits.size());

    convertDecoderInfoMon2SNMP(_statisticsManager.getDecoderStatistics(), *_settingsManager.getTPCCodefileManager(0), data.decoderInfo);

    string framersTypes = _statisticsManager.getDecoderConfigParser().makeFramerTypesString();
    strncpy(data.postProcessingInfo.framers, framersTypes.c_str(), framersTypes.size());

    Settings::DVBS2Settings& dvbSettings = _settingsManager.getChannelSettings(0).getDVBS2Settings();
    string dvbSymbolRateRange = format("%u,%u", dvbSettings.getMinSymbolRate(), dvbSettings.getMaxSymbolRate());
    strncpy(data.dvbs2Info.symbolRateRange, dvbSymbolRateRange.c_str(), dvbSymbolRateRange.size());
 
    Settings::TDMAConfig* _configTDMA = _settingsManager.getChannelSettings(0).getTDMAConfig();
    std::string networks = _configTDMA->getNetworksListString();
    strncpy(data.decoderInfo.statAllNetworklist, networks.c_str(), networks.size());                               

    std::string terminals = _configTDMA->getTerminalsListString();
    strncpy(data.decoderInfo.statAllTerminalList, terminals.c_str(), terminals.size());                               

    data.decoderInfo.maxOfChannelsInTDMA = _settingsManager.getTDMASettings().getNumberOfChannelsAllowed();

 //   writeToSNMP(*_zSocket, data);
    writeToZMQ(ZMQSocket, data); 
    */ 
}

void ZMQServer::sendDeviceStatus()
{
//    TRACE();
    /*
    DeviceStatus data;

    memset(&data, 0, sizeof(data));
    strncpy(data.deviceName, _settingsManager.getCommonSettings().getDeviceName().c_str(), _settingsManager.getCommonSettings().getDeviceName().size());
    DateTime currentTime;
    convertDateTimeMon2ZMQ(currentTime, data);
    data.TunerState = static_cast<unsigned char>(_statisticsManager.getChannelStatistics(0).getTunerStatistics().getPresence());
    data.TunerState = data.TunerState  + (static_cast<unsigned char>(_statisticsManager.getChannelStatistics(1).getTunerStatistics().getPresence())*2);
    data.PendingState = static_cast<unsigned char>(_statisticsManager.getCommonStatistics().getConfigurationPending());

    data.ManagementStatus =0;
    if(pdescrDev.DescriptorCh1>0)  data.ManagementStatus = 1;
    if(pdescrDev.DescriptorCh2>0)  data.ManagementStatus = data.ManagementStatus+2;
 
    writeToZMQ(ZMQSocket, data); 
    */ 
}



void ZMQServer::processHeader(const ZMQServerManager& header)
{
  // TRACE();

   ScopedLock<Mutex> lock(_timerMutex);
   /*
   //poco_debug_f3(_logger, "TRACE. The server received: Message = %u, channel = %u, Nom_structure = %u", static_cast<unsigned int>(header.MessageType), static_cast<unsigned int>(header.Channel), static_cast<unsigned int>(header.Nom_structure));
   
   bool init_work = 0;
   pinitDev.Channel = header.Channel;
   pinitDev.MessageType = header.MessageType;
   // pinitDev.ClientIPAddr = header.ProcessStatus;
   unsigned int _IDTime = header.ProcessStatus;
   unsigned int channel;
   if(header.Channel<2) channel = 0;
   else                 channel = 1;

   void *outbuf;
//   int _count = 0;
//   bool _error = 0; 

   switch(header.MessageType) {
   case 1 : // IWontCon      -> structure initDev  
       if(header.Channel==1) {
           if(pdescrDev.DescriptorCh1 > 0) {
               init_work = 0; // Занято
               pinitDev.IDtime = 0;
               pinitDev.ClientIPAddr = pdescrDev.ClientIPAddrCh1;
               poco_debug(_logger, "Warning (Channel 1)! The Process cannot access the control because it is being used by another process.");
           } else {
               init_work = 1; // OK !!!
               _threadPoll1.start(_connZMQHandler1);
           }
       }
       if(header.Channel==2) {
           if(pdescrDev.DescriptorCh2 > 0) {
               init_work = 0; // Занято
               pinitDev.IDtime = 0;
               pinitDev.ClientIPAddr = pdescrDev.ClientIPAddrCh2;
               poco_debug(_logger, "Warning (Channel 2)! The Process cannot access the control because it is being used by another process.");
           } else {
               init_work = 1; // OK !!!
               _threadPoll2.start(_connZMQHandler2);
           }
       }
       break;
   case 2 : // IMustCon       -> structure initDev  
           // poco_debug_f3(_logger, "TRACE! header.Channel - %u, pdescrDev.DescriptorCh1 %u, pdescrDev.DescriptorCh2", (unsigned int) header.Channel, pdescrDev.DescriptorCh1, pdescrDev.DescriptorCh2); 
          if(header.Channel==1) { 
              if(!_threadStart[0]) {
                 _threadPoll1.start(_connZMQHandler1);
                 _threadStart[0] = true;
               }
               init_work = 1;
          }
          if(header.Channel==2) { 
               if(!_threadStart[1]) {
                   _threadPoll2.start(_connZMQHandler2);
                   _threadStart[1] = true;
                }
                init_work = 1; 
          }
          break;
       break;
   case 3 : //IWontDiscon      -> structure initDev 
           if(header.Channel==1) {
               if(pdescrDev.DescriptorCh1>0) {
                   if(_IDTime>0 && _IDTime!=pdescrDev.DescriptorCh1) {
                       poco_debug_f1(_logger, "Error! header.IDtime!=pdescrDev.DescriptorCh1 (%u)", pdescrDev.DescriptorCh1); 
                       break;
                   }
                   pdescrDev.DescriptorCh1 = 0;
                   pdescrDev.ClientIPAddrCh1 = 0;
                   _statisticsManager.getCommonStatistics().setConfigurationLeader(0, 0);
                   // Is controlled via RCV20Socket.dll
                   _statisticsManager.getChannelStatistics(channel).lessChannelStatus(CHANNEL_STATUS_MSG_LEADER_DLL);
                   poco_debug(_logger, "Stoped ZMQHandler (Channel 1)!"); 
               } else {
                  poco_debug_f1(_logger, "Error! pdescrDev.DescriptorCh1 = %u", pdescrDev.DescriptorCh1); 
               }
           }
           if(header.Channel==2) {
               if(pdescrDev.DescriptorCh2> 0) {
                   if(_IDTime>0 && _IDTime!=pdescrDev.DescriptorCh2) {
                       poco_debug_f1(_logger, "Error! header.IDtime!=pdescrDev.DescriptorCh2 (%u)", pdescrDev.DescriptorCh2); 
                       break;
                   }
                   // _connZMQHandler2.stop();
                    pdescrDev.DescriptorCh2 = 0;
                    pdescrDev.ClientIPAddrCh2 = 0;
                    _statisticsManager.getCommonStatistics().setConfigurationLeader(1, 0);
                    // Is controlled via RCV20Socket.dll
                    _statisticsManager.getChannelStatistics(channel).lessChannelStatus(CHANNEL_STATUS_MSG_LEADER_DLL);
                    poco_debug(_logger, "Stoped ZMQHandler (Channel 2) !"); 
               } else {
                  poco_debug_f1(_logger, "Error! pdescrDev.DescriptorCh2 =  %u", pdescrDev.DescriptorCh2); 
               }
           }
       break;
   case 10 : //Status Device   -> structure descrDevTwoCh
        // poco_debug(_logger, "TRACE. GetStatusDevice()");
         sendDeviceStatus();
        break;
   case 11 : //GetDeviceInfo   ->structure  X
        // poco_debug(_logger, "TRACE. GetDeviceInfo()");
        sendDeviceInfo();
       break;
   case 12 : //GetChannelConfig ->structure Y
     
       ChannelConf conf;
       // poco_debug_f4(_logger, "TRACE. GetChannelConfig(ip, .., channel = %u, nom_structure = %u, subchannel = %u, field = %u, structure )", static_cast<unsigned int>(header.Channel), static_cast<unsigned int>(header.Nom_structure), static_cast<unsigned int>(header.Nom_subchannel), static_cast<unsigned int>(header.Nom_field));
       unsigned int _structure = header.Nom_structure;
       unsigned int _subchannel = header.Nom_subchannel;
       unsigned int _field = header.Nom_field;
       unsigned int _size = header.ProcessStatus;
       unsigned int out_size;

       // outbuf = (char *) malloc(_size);

       outbuf = (char *) malloc(sizeof(conf));

       if(_structure==0) { // Передать всю структуру
           // _size = convertChannelConfigMon2ZMQ(_settingsManager.getChannelSettings(static_cast<unsigned int> (channel)), conf, 0, 0, 0, outbuf); 
           if(sizeof(conf)!=_size) {
               poco_debug_f2(_logger, "Error! MSG_ERROR_MASMATCH_I_O_SIZE. convertSize Conf(%u) != header.ProcessStatus(%u) ", sizeof(conf), header.ProcessStatus); 
               pinitDev.MessageType = MSG_ERROR_MASMATCH_I_O_SIZE;
               writeToZMQ(ZMQSocket, pdescrDev);
               break;
           } else {
               convertChannelConfigMon2SNMP(_settingsManager.getChannelSettings(static_cast<unsigned int> (channel)), conf, 0); 
               // poco_debug_f2(_logger, "convertSize(%u) == header.ProcessStatus(%u) ", sizeof(conf), header.ProcessStatus);
           }
           // if - OK
             out_size = sizeof(conf);
             memcpy((char *) outbuf, &conf, out_size);    
             
//           writeToZMQ(ZMQSocket, conf);
       } else {
           out_size = convertChannelConfigMon2ZMQ(_settingsManager.getChannelSettings(static_cast<unsigned int> (channel)), conf, _structure, _subchannel, _field, outbuf); 
           // DO LATER  !!! - colculate _size
           if(_size!=out_size) { //
               poco_debug_f2(_logger, "Error! MSG_ERROR_MASMATCH_I_O_SIZE. convertSize(%u) != header.ProcessStatus(%u) ", out_size, _size); 
               pinitDev.MessageType = MSG_ERROR_MASMATCH_I_O_SIZE;
               writeToZMQ(ZMQSocket, pdescrDev);
               break;
           } 
         } 
         zmq_msg_t answer;
         zmq_msg_init_size(&answer, out_size);
         memcpy(zmq_msg_data(&answer), outbuf, out_size);
         zmq_msg_send(&answer, ZMQSocket, 0);
         zmq_msg_close(&answer);

       // convertChannelConfigMon2ZMQ(_settingsManager.getChannelSettings(static_cast<unsigned int> (channel)), conf, _structure, _subchannel); 

       free(outbuf);
       break;
   default:
        pinitDev.MessageType =0;    
        break;
   }

   if(init_work) {
       poco_debug_f1(_logger, "Started ZMQHandler (Channel %u).", channel);
       Timestamp now_time;
       Timestamp start(now_time); 
       pinitDev.IDtime = static_cast<unsigned int> (now_time.epochMicroseconds()); 
       pinitDev.ClientIPAddr = header.ProcessStatus;
       // poco_debug_f1(_logger, "TRACE. Create a connection handle = %u", pinitDev.IDtime);
       if(header.Channel==1){
           pdescrDev.DescriptorCh1 = pinitDev.IDtime;
           pdescrDev.ClientIPAddrCh1 = pinitDev.ClientIPAddr;
           _statisticsManager.getCommonStatistics().setConfigurationLeader(channel, pdescrDev.DescriptorCh1);    
       }
       if(header.Channel==2){
           pdescrDev.DescriptorCh2 = pinitDev.IDtime;
           pdescrDev.ClientIPAddrCh2 = pinitDev.ClientIPAddr;
           _statisticsManager.getCommonStatistics().setConfigurationLeader(channel, pdescrDev.DescriptorCh2);
       }
       _statisticsManager.getChannelStatistics(channel).addChannelStatus(CHANNEL_STATUS_MSG_LEADER_DLL);
   }
   if(header.MessageType<10) {
         writeToZMQ(ZMQSocket, pinitDev);
   } 
   */ 
}

} // namespace SNMP


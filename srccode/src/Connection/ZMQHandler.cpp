#include "Connection/ZMQHandler.hpp"
/*---------------------------------------------------------------------------*/
#include "Notifications.hpp"
// #include "BitFileParser.hpp"

#include "Common/Tracer.hpp"
#include "Common/Helpers.hpp"
#include "Common/Defines.hpp"
// #include "FPGA/LoadHelpers.hpp"
#include "Connection/DataConversion.hpp"
#include "MonitorApplication.hpp"
//#include "Decoder/DecoderManager.hpp"

// using Settings::DecoderSettings;
// using Settings::HDRMSettings;
/*---------------------------------------------------------------------------*/
#include "Poco/Timespan.h"
#include "Poco/Process.h"
#include "Poco/Format.h"
#include "Poco/ScopedLock.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h" 

using Poco::Timespan;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::format;
using Poco::Logger;
using Poco::DateTime;
using Poco::ScopedLock;
using Poco::Mutex;

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
/*
static unsigned int convertFFTDeviceChannelToBaseAddress(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return MEMORY_BASE_ADDRESS + MEMORY_CHANNEL_SHIFT * channel;
}

static unsigned int convertConstellationDeviceChannelToBaseAddressTDMA(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return MEMORY_BASE_ADDRESS_FOR_TDMA + MEMORY_CHANNEL_SHIFT * channel;
}
*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace SNMP
{
ZMQHandler::ZMQHandler(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc, unsigned int Channel)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _nc(nc)
//, _logger(Logger::get("Monitor.SNMP.ZMQHandler" + Poco::NumberFormatter::format(Channel)))
, _logger(Logger::get("Monitor.SNMP.ZMQHandler"))
, _channel(Channel-1)
{
    poco_debug(_logger, "Create ZMQHandler...");
   StructInfoMemberList[0] = 20;   // ChannelConf
   StructInfoMemberList[1] = 8;    // TunerConf
   StructInfoMemberList[2] = 4;    // AGCConf = 4
/*   StructInfoMemberList[3] = 4;    // ADCConf = 4
   StructInfoMemberList[4] = 11;   // DemodulatorSubchannelConf = 11
   StructInfoMemberList[5] = 2;    // HDRMSubchannelConf = 2
   StructInfoMemberList[6] = 25;   // DVBS2DemodulatorConf = 25
   StructInfoMemberList[7] = 11;   // DVBS2DecoderConf = 11
   StructInfoMemberList[8] = 3;    // DEMConf = 3 n
   StructInfoMemberList[9] = 13;   // DecoderConf = 13
   StructInfoMemberList[10] = 2;   // TPCDecoderConf
   StructInfoMemberList[11] = 4;   // MultiTPCDecoderConf = 4 n 
   StructInfoMemberList[12] = 3;   // IntelsatViterbiK7Conf = 3 n
   StructInfoMemberList[13] = 1;   // SequentialConf - 1 n
   StructInfoMemberList[14] = 2;   // DVBSConf - 2 n
   StructInfoMemberList[15] = 8;   // LDPCConf - 8
   StructInfoMemberList[16] = 16;  // PostProcessingConf - 16
   StructInfoMemberList[17] = 7;   // OutMuxSubchannelConf - 7
   StructInfoMemberList[18] = 1;   // MiscChannelConf 1 n char label
   StructInfoMemberList[19] = 6;   // TDMAConf - 6
   StructInfoMemberList[20] = 6;   // TDMADecoderConf - 6
 */
}

ZMQHandler::~ZMQHandler()
{
}

void ZMQHandler::run()
{
    // TRACE();
/*    
    try{
        // context_t::context_t

        _memDev = new DeviceIO::MemoryDevice(200, 30 + _channel);
        _fftDev = new DeviceIO::FFTDevice(convertFFTDeviceChannelToBaseAddress(_channel), *_memDev);
        _conDev = new DeviceIO::ConstellationDevice(convertConstellationDeviceChannelToBaseAddressTDMA(_channel), *_memDev);

        // poco_debug(_logger, "Received ZMQHandler...");
        _stop = false;
        ZMQContext = zmq_ctx_new(); //new zmq::context_t(1);
        ZMQSocket  = zmq_socket(ZMQContext, ZMQ_REP); //new zmq::socket_t(*_zContext, ZMQ_REP);
        if(_channel==0) {
            zmq_bind(ZMQSocket, "tcp://*:4041");
        }
        if(_channel==1) {
            zmq_bind(ZMQSocket, "tcp://*:4042");
        }

        zmq_pollitem_t pollitems[] = {
            {ZMQSocket, 0, ZMQ_POLLIN, 0}
        };
        char msg[256];
         while(!_stop) {
            zmq_poll(pollitems, sizeOfArray(pollitems), POLL_TIMEOUT);
            if(pollitems[0].revents & ZMQ_POLLIN) {
                zmq_msg_t request;
                zmq_msg_init(&request);
                zmq_msg_recv(&request, ZMQSocket, 0);
                int length = zmq_msg_size(&request);
                memcpy((char *)msg, zmq_msg_data(&request), length);
                processHeader(*reinterpret_cast<ZMQManagerHeader*>((char *)msg));
            }
         }
    }
    catch (const zmq::error_t& exc){
        poco_debug_f1(_logger, "Exception: %s", std::string(exc.what()));
    }

    zmq_close(ZMQSocket);
    zmq_ctx_destroy(ZMQContext);
    poco_debug(_logger, "Exit ZMQ Handled...");
 */
}

void ZMQHandler::stop()
{
//     TRACE();
    _stop = true;
}

unsigned int ZMQHandler::checkPending()
{
    // TRACE();
    unsigned int status_error=0;
    unsigned char _pending; 
    _pending=_statisticsManager.getCommonStatistics().getConfigurationPending();
    if(_pending>0){
        //status_error = MSG_ERROR_BUSY_PENDING + _pending;
        // ƒальше проверить будет ли мешать
        if(_pending&1){
            status_error = MSG_ERROR_BUSY_PENDING_CHANNEL1; //Pending
        } else {
            if(_pending&2){
                status_error = MSG_ERROR_BUSY_PENDING_CHANNEL2; //Pending
            }
        }
    }
    return status_error;
}

// проверка на соответствие є и размера переданнной структуры 
unsigned int ZMQHandler::checkQuery(ZMQManagerHeader& header)
{
    unsigned int status_error = 0;
    unsigned int status_lider = _statisticsManager.getCommonStatistics().getConfigurationLeader(_channel);

    if(header.IDtime != status_lider){
         status_error = MSG_ERROR_EXCEEDED_DESCRIPTOR; // ERROR  - Nom_field > существующего
         return status_error; 
    }

    status_error = checkPending();
    if(status_error){
        return status_error; 
    }
    if(header.Channel>2){
        status_error = MSG_ERROR_EXCEEDED_NUM_CHANNEL; // ERROR  - Nom Channel > существующего
        return status_error;
    }

    if(header.Nom_structure<100) {
        if(header.Nom_subchannel>7){
            status_error = MSG_ERROR_EXCEEDED_NUM_SUBCHAN; // ERROR  - Nom_subchannel > существующего
            return status_error;
        }
        if(header.Nom_field>25){
            status_error = MSG_ERROR_EXCEEDED_NUM_FIELD; // ERROR  - Nom_field > существующего
            return status_error;
        }
        if(header.Nom_structure>20){
             status_error = MSG_ERROR_EXCEEDED_NUM_STRUCT; // ERROR  - Nom_structure > существующего
             return status_error;
        }
    };

    return status_error;
}

//----------------------------------------------------------------------------------------------------------
// ќтправить ответ ZMQManagerHeader (answer)
unsigned int ZMQHandler::sendZMQAnswer(ZMQManagerHeader& header)
{
   // TRACE();
   unsigned int status_error=0;
   zmq_msg_t answer;
   unsigned int length = sizeof(header);
   zmq_msg_init_size(&answer, length);
   memcpy(zmq_msg_data(&answer),  &header, length);

   // poco_debug_f1(_logger, "Answer. Sending header.ProcessStatus = %u", static_cast<unsigned int> (header.ProcessStatus));

   int len = zmq_msg_send(&answer, ZMQSocket, 0);
   //int len = zmq_msg_send(&answer, ZMQSocket, ZMQ_DONTWAIT);
   /*
    - когда ваш код принимает сообщение с ZMQ_NOWAIT ("асинхронно"), но данных не ожидаетс€. ZMQ вернет -1 и установит код ошибка равным EAGAIN;
    - когда один поток вызывает zmq_ctx_destroy(), а второй все еще выполн€ет работу в блокирующем режиме, то вызов zmq_ctx_destroy()
        вызывает закрытие контекста и всех блокирующих вызовов с кодом завершени€ -1, а код ошибки устанавливаетс€ равным ETERM.
   */
   if (len == -1) { // Error !!!
       status_error = errno;
        if (status_error != ETERM) {
            poco_information_f1(_logger, "ZmqTransportMultiplexer::pass %s from zmq_msg_recv",  zmq_strerror(status_error));
            // no need to log ordinary closure.
        }
    // throw ZmqMultiplexerShutdown();
   }
   zmq_msg_close(&answer); 
//   poco_debug(_logger, "Answer closer.");
   return status_error;
}
//----------------------------------------------------------------------------------------------------------
unsigned int ZMQHandler::checkStructureConf(void *Structure, unsigned int _length, unsigned int _structure, unsigned int _subchannel, unsigned int _field)
{
    unsigned int status_error=0;
    ChannelConf conf;
/*
    TunerConf               pTunerConf;
    AGCConf                 pAGCConf;
    ADCConf                 pADCConf;
    DemodulatorSubchannelConf pDemodulatorSubchannelConf;
    HDRMSubchannelConf      pHDRMSubchannelConf;
    DVBS2DemodulatorConf    pDVBS2DemodulatorConf;
    DVBS2DecoderConf        pDVBS2DecoderConf;
    DEMConf                 pDEMConf;
    DecoderConf             pDecoderConf;
    TPCDecoderConf          pTPCDecoderConf;
    MultiTPCDecoderConf     pMultiTPCDecoderConf;
    IntelsatViterbiK7Conf   pIntelsatViterbiK7Conf;
    SequentialConf          pSequentialConf;
    DVBSConf                pDVBSConf;
    LDPCConf                pLDPCConf;
    PostProcessingConf      pPostProcessingConf;
    OutMuxSubchannelConf    pOutMuxSubchannelConf;
    MiscChannelConf         pMiscChannelConf;
    TDMAConf                pTDMAConf;
    TDMADecoderConf         pTDMADecoderConf;
*/
    
    unsigned int MemberSizeInStructList[MAX_COUNT_OF_STRUCTURE_CONF][MAX_COUNT_OF_FIELDS_IN_CONF];
    
    for(int i=0; i<MAX_COUNT_OF_STRUCTURE_CONF;i++) {
        for(int j=0; j<MAX_COUNT_OF_FIELDS_IN_CONF;j++) {
            MemberSizeInStructList[i][j] = 0;
        }
    }
    MemberSizeInStructList[0][0] = sizeof(conf);              // TunerConf
                                                                    //
    MemberSizeInStructList[1][0] = sizeof(conf.tuner);              // TunerConf
    MemberSizeInStructList[2][0] = sizeof(conf.agc);                // AGCConf
    MemberSizeInStructList[3][0] = sizeof(conf.adc);                // ADCConf
    if(_subchannel==0) {
        MemberSizeInStructList[4][0] = sizeof(conf.demodulator); // DemodulatorSubchannelConf
    } else {
        MemberSizeInStructList[4][0] = sizeof(conf.demodulator[1]); // DemodulatorSubchannelConf
    }
    if(_subchannel==0) {
        MemberSizeInStructList[5][0] = sizeof(conf.hdrm);     // HDRMSubchannelConf = 5
    } else {
        MemberSizeInStructList[5][0] = sizeof(conf.hdrm[1]);     // HDRMSubchannelConf = 5
    }
    MemberSizeInStructList[6][0] = sizeof(conf.dvb);   // DVBS2DemodulatorConf
    MemberSizeInStructList[7][0] = sizeof(conf.dvbDecoder);       // DVBS2DecoderConf
    MemberSizeInStructList[8][0] = sizeof(conf.dem);                // DEMConf
    MemberSizeInStructList[9][0] = sizeof(conf.decoder);            // DecoderConf 
    MemberSizeInStructList[10][0] = sizeof(conf.tpcDecoder);        // TPCDecoderConf
    MemberSizeInStructList[11][0] = sizeof(conf.multiTPCDecoder);   // MultiTPCDecoderConf
    MemberSizeInStructList[12][0] = sizeof(conf.viterbiK7); // IntelsatViterbiK7Conf
    MemberSizeInStructList[13][0] = sizeof(conf.sequential);        // SequentialConf
    MemberSizeInStructList[14][0] = sizeof(conf.dvbs);              // DVBSConf
    MemberSizeInStructList[15][0] = sizeof(conf.ldpc);              // LDPCConf
    MemberSizeInStructList[16][0] = sizeof(conf.postProcessing);    // PostProcessingConf
    if(_subchannel==0) {
        MemberSizeInStructList[17][0] = sizeof(conf.outMux);     // HDRMSubchannelConf = 5
    } else {
        MemberSizeInStructList[17][0] = sizeof(conf.outMux[1]);     // HDRMSubchannelConf = 5
    }

    MemberSizeInStructList[18][0] = sizeof(conf.misc);       // MiscChannelConf
    if(_subchannel==0) {
        MemberSizeInStructList[19][0] = sizeof(conf.tdma);     // HDRMSubchannelConf = 5
    } else {
        MemberSizeInStructList[19][0] = sizeof(conf.tdma[1]);     // HDRMSubchannelConf = 5
    }

    MemberSizeInStructList[20][0] = sizeof(conf.tdmaDecoder);       // TDMADecoderConf
    
    // TunerConf
    MemberSizeInStructList[1][1] = 4;     // unsigned int  frequency
    MemberSizeInStructList[1][2] = 4;     // unsigned int  filterBand;
    MemberSizeInStructList[1][3] = 4;     // unsigned char manualFilterMode
    MemberSizeInStructList[1][4] = 4;     // unsigned int  spectrumAnalyzerShift
    MemberSizeInStructList[1][5] = 1;     // unsigned char spectrumAnalyzerEnable
    MemberSizeInStructList[1][6] = 4;     // float         supplyVoltage
    MemberSizeInStructList[1][7] = 4;     // unsigned int  referenceVoltage
    MemberSizeInStructList[1][8] = 4;     // unsigned int  maxSpectrumAnalyzerSymbolRate
    // AGCConf
    MemberSizeInStructList[2][1] = 4;   // unsigned int initialGain;														
    MemberSizeInStructList[2][2] = 4;   // unsigned int mode;																
    MemberSizeInStructList[2][3] = 4;   // unsigned int manualLBandGain;                                                
    MemberSizeInStructList[2][4] = 4;   // unsigned int manualBaseBandGain;       
    // struct ADCConf                                                                      
    MemberSizeInStructList[3][1] = 4;   // unsigned char coarseGainEnabled;                                     
    MemberSizeInStructList[3][2] = 4;   // unsigned int  decimationRate;                                              
    MemberSizeInStructList[3][3] = 4;   // unsigned char lowLatencyEnabled;                                      
    MemberSizeInStructList[3][4] = 4;   // unsigned int outputband;                                      
    // struct DemodulatorSubchannelConf
    MemberSizeInStructList[4][1] = 4;   // unsigned int  symbolRate;
    MemberSizeInStructList[4][2] = 4;   // unsigned int  spectralInversion;
    MemberSizeInStructList[4][3] = 4;   // unsigned int  rrcAlpha;			
    MemberSizeInStructList[4][4] = 4;   // unsigned int   APSKRROI;       
	MemberSizeInStructList[4][5] = 4;   // unsigned int 	APSKRRMI;      
    MemberSizeInStructList[4][6] = 4;   // unsigned int  aeqMode;         
    MemberSizeInStructList[4][7] = 4;   // unsigned int  searchRange;     
    MemberSizeInStructList[4][8] = 4;   // unsigned int  mode;            
    MemberSizeInStructList[4][9] = 4;   // unsigned int  ddcEnable;       
    MemberSizeInStructList[4][10] = 4;  // unsigned int  dcic;            
    MemberSizeInStructList[4][11] = 4;  // unsigned int  nonCoherent;     
                                        //
    MemberSizeInStructList[5][1] = 4;   // unsigned int  modulationType;													
    MemberSizeInStructList[5][2] = 4;   // int  carrierOffset;        
    // add ticket #734
    MemberSizeInStructList[5][3] = 4;   // unsigned int carrierLBand
                                        
    MemberSizeInStructList[6][1] = 4;   // unsigned int  sampleRate;														 	
    MemberSizeInStructList[6][2] = 4;   // unsigned char ragcEnable;														 	
    MemberSizeInStructList[6][3] = 4;   // unsigned char ragcInvert;                                                         	 
    MemberSizeInStructList[6][4] = 4;   // unsigned int  outputAmplitudeVCM;                                          	
    MemberSizeInStructList[6][5] = 4;   // unsigned int  outputAmplitudeQPSK;                                        	  
    MemberSizeInStructList[6][6] = 4;   // unsigned int  outputAmplitude8PSK;                                         	
    MemberSizeInStructList[6][7] = 4;   // unsigned int  outputAmplitude16APSK;                                    	
    MemberSizeInStructList[6][8] = 4;   // unsigned int  outputAmplitude32APSK;                                     	
    MemberSizeInStructList[6][9] = 4;   // unsigned char reacqHoldoff;                                                      	
	MemberSizeInStructList[6][10] = 8;  // float        reacqActivationDelay;                                                  	
    MemberSizeInStructList[6][11] = 8;  // float        reacqRestartDelay;                                                      	
    MemberSizeInStructList[6][12] = 4;  // unsigned char falseDetEnable;                                                  	 
    MemberSizeInStructList[6][13] = 4;  // unsigned char falseDetOneshot;                                                 	 
    MemberSizeInStructList[6][14] = 8;  // float        falseDetThresh;                                                            	
    MemberSizeInStructList[6][15] = 8;  // float        falseDetPeriod;														         	
    MemberSizeInStructList[6][16] = 4;  // unsigned int  aeqAutoSpeed;                                                       	 
    MemberSizeInStructList[6][17] = 4;  // unsigned int  aeqAutoModMask;                                                  	
    MemberSizeInStructList[6][18] = 4;  // unsigned char aeq2xRate;                                                           	
    MemberSizeInStructList[6][19] = 4;  // unsigned int  coarseSteps;                                                           	 
    MemberSizeInStructList[6][20] = 4;  // unsigned int  mode;															
    MemberSizeInStructList[6][21] = 4;  // unsigned int  dvbs2ModCod;												
    MemberSizeInStructList[6][22] = 4;  // unsigned int  dvbs2FrameType;								
    MemberSizeInStructList[6][23] = 4;  // unsigned char dvbs2Pilots;									
    MemberSizeInStructList[6][24] = 8;  // float          dvbs2LoopBw;										
	MemberSizeInStructList[6][25] = 4;  // unsigned char dvbs2PLFRAMEDescrambler;              

//poco_debug_f2(_logger, "TRACE 2 ! Size conf.dvbDecoder = %u or %u", sizeof(conf.dvbDecoder), MemberSizeInStructList[7][0]);                                        

    MemberSizeInStructList[7][1] = 1;   // unsigned char autoIter;
    MemberSizeInStructList[7][2] = 4;   // unsigned int  autoIterMode;
    MemberSizeInStructList[7][3] = 4;   // unsigned int  iterations;
    MemberSizeInStructList[7][4] = 1;   // unsigned char disableFooter;
    MemberSizeInStructList[7][5] = 1;   // unsigned char disableDFL;
    MemberSizeInStructList[7][6] = 1;   // unsigned char disableCRCSuppression;
    MemberSizeInStructList[7][7] = 4;   // unsigned int  outputMode;
    MemberSizeInStructList[7][8] = 1;   // unsigned char disableAS14_15;
    MemberSizeInStructList[7][9] = 4;   // unsigned int  isi;
    MemberSizeInStructList[7][10] = 4;  // unsigned int  tframeHeader;
    MemberSizeInStructList[7][11] = 4;  // unsigned int  outModCod;

    MemberSizeInStructList[8][1] = 4;   // unsigned int  mode;
    MemberSizeInStructList[8][2] = 4;   // unsigned int  multiMode;
    MemberSizeInStructList[8][3] = 4;   // unsigned int  numberOfSubchannel;
    // add ticket #734                                        
    MemberSizeInStructList[8][4] = 4;   // unsigned int  carrierFormatInMulti;
    MemberSizeInStructList[8][5] = 4;   // unsigned int  carrierFormatInTDMA;
                                       
    MemberSizeInStructList[9][1] = 4;   // unsigned int  mode;
    MemberSizeInStructList[9][2] = 1;   // unsigned char cutMPEGSync;
    MemberSizeInStructList[9][3] = 4;   // unsigned int  innerCodeRate;
    MemberSizeInStructList[9][4] = 4;   // unsigned int  outerCodeRate;
    MemberSizeInStructList[9][5] = 1;   // unsigned char dataInversion;
    MemberSizeInStructList[9][6] = 4;   // unsigned int  outerFECDeinterleaver;
    MemberSizeInStructList[9][7] = 1;   // unsigned char outerDecoderEnable;
    MemberSizeInStructList[9][8] = 1;   // unsigned char outerFECDescramblerEnable;
    MemberSizeInStructList[9][9] = 1;   // unsigned char diffDecoderEnable;
    MemberSizeInStructList[9][10] = 1;   // unsigned char innerFECDescramblerEnable;
    MemberSizeInStructList[9][11] = 4;   // unsigned int  innerFECDescramblerType;
    MemberSizeInStructList[9][12] = 1;   // unsigned char autoSpectralInversionEnable;
    MemberSizeInStructList[9][13] = 1;   // unsigned char autoAmbiguityElimination;

    MemberSizeInStructList[10][1] = 255; // char codeName[CODE_NAME_BUFSIZE];                             
    MemberSizeInStructList[10][2] = 1;   // unsigned char ahaDescramblerEnable;                                   
    
    if(_field==0 || (_field>0 && _structure<11 )) {
        if(_length!=MemberSizeInStructList[_structure][_field]){
            status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure;
            poco_debug_f4(_logger, "Error size of structure %u , field - %u. You length = %u, Need length = %u (%u)", _structure, _field, _length, MemberSizeInStructList[_structure][_field]);
            return status_error; 
        }
    } 
    
    // получить ChannelConf 
//    convertChannelConfigMon2SNMP(_settingsManager.getChannelSettings(_channel), conf, SelectStruct);
//    copyChannelConfigZMQ2MonH(conf, Structure, _length, _structure, _subchannel, _field);

//    convertChannelConfigMon2SNMP(_settingsManager.getChannelSettings(_channel), conf, _structure); 

   return 0;
}

unsigned int ZMQHandler::checkDataInStructureConf(ChannelConf& conf, unsigned int _structure, unsigned int _subchannel, unsigned int _field)
{
    unsigned int status_error=0;
/*
    unsigned i_min =0, i_max=0;
    switch(_structure){
    case 1 : // TunerConf
        switch(_field) {
        case 1 :  //  conf.tuner.frequency Range: 950000 ... 2150000
                if(conf.tuner.frequency<950000 || conf.tuner.frequency>2150000) {
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                // poco_debug_f1(_logger, "TRACE. Tuner.frequency = %u", conf.tuner.frequency);
                break;
        case 2 : // Rcv20ConfTunerFilterBand = 4000000+(x-12)*290000 √ц max 73310000
                if(conf.tuner.filterBand>73310000) {
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;    
        case 3 : // conf.TunerConf.manualFilterMode
                 if(conf.tuner.manualFilterMode>1) {
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                 break;    
        case 4 :    
            break;    
        case 5 : // conf.TunerConf.spectrumAnalyzerEnable   
                if(conf.tuner.spectrumAnalyzerEnable>1) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                // if symbol Rate, sps" меньшее или равное {ChannelStat.tuner.maxSpectrumAnalyzerSymbolRate}  spectrumAnalyzerEnable = 1: Enable
                // if если орган управлени€ Spectral Analyzer Mode на момент ввода значени€ "Symbol Rate, sps" уже был разблокирован, то он не должен переводитьс€ в положение "Enable" программно, оставл€€ право выбора "Enable"/"Disable" пользователю
                break;
        case 6 : 
                break;
        case 7 : 
                break;
        case 8 : 
                break;
          default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            };
        break;
    case 2 : // AGCConf	
            switch(_field) {
            case 1 :    // initialGain 
                status_error = 0;
                break;
            case 2 :    // mode (-1 Ц Unknown, 0 Ц Auto, 1 Ц Manual, 2 Ц One Shot)
                if(conf.agc.mode>2) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 3 :    //(L-Band Gain, dB);  Range: 1 ... 70
                if(conf.agc.manualLBandGain>70) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 4 :    //manualBaseBandGain (Baseband Gain, dB , ). Range: 1 ... 15, ??? - 1 dB 
                if(conf.agc.manualBaseBandGain>15) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;

            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 3 : // ADCConf	decimationRate  - Decimation Rate  Range: 0- 4 - 0: by 2; 1: by 4; 3: Resampler 3/7; 4: by 8. 
            switch(_field) {
            case 1 :
                status_error = 0;
                break;
            case 2 :
                if(conf.adc.decimationRate>4) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 3 :
                status_error = 0;
                break;
            case 4 :
                status_error = 0;
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 4 : // DemodulatorSubchannelConf
            if(_subchannel==0) {
                i_min =0; 
                i_max = sizeOfArray(conf.demodulator)-1;
                
            } else {
                i_min = _subchannel-1; 
                i_max = _subchannel; 
            }
            for(unsigned int i = i_min; i < i_max; i++)
            {
            
                switch(_field) {
                case 1 : // symbolRate				Ц  Range: 1200 ... 55555555
                    if(conf.demodulator[i].symbolRate<1200 || conf.demodulator[i].symbolRate>55555555) { 
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                        // poco_debug_f4(_logger, "demodulator[%u], field = %u, Error = %u -> symbolRate = %u", i, _field, status_error, conf.demodulator[i].symbolRate );
                    }
                    break;
                case 2 : // spectralInversion		Ц (0 Ц Disable, 1 - Enable); 
                    if(conf.demodulator[i].spectralInversion>1) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 3 : // rrcAlpha					Ц (20 - 40);
                    if(conf.demodulator[i].rrcAlpha>40) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 4 : // APSKRROI				Ц  Outer/Inner (code rate)  Range: 285 - 527
                    if(conf.demodulator[i].APSKRROI<284 || conf.demodulator[i].APSKRROI>528) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 5 : //  APSKRRMI				Ц Middle/Inner (code rate) Range: 253 - 284
                    if(conf.demodulator[i].APSKRRMI<253 || conf.demodulator[i].APSKRRMI>284) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 6 : // aeqMode				Ц Adaptive Equalizer ( 0 - off, 1 - on,  2 -Auto)	
                    if(conf.demodulator[i].aeqMode>2) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 7 : // searchRange			Ц (Search Range, %) Range: 0 ... 100 % 
                    if(conf.demodulator[i].searchRange>100) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 8 : // mode					Ц (1..7)
                    if(conf.demodulator[i].mode>7) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 9 : //  ddcEnable				Ц DDC (1 Ц Enable, 0 Ц Disable)ddcEnable 
                    if(conf.demodulator[i].ddcEnable>1) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 10: //  dcic					Ц CIC Output Level Range: 0 ... 255
                    if(conf.demodulator[i].dcic>255) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 11: //  nonCoherent			Ц  Non-Coherent AGC  (1 Ц Enable, 0 Ц Disable)
                    if(conf.demodulator[i].nonCoherent>1) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                default :
                    status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                    break;
                }
           }
        break;
    case 5 : // HDRMSubchannelConf 	
            if(_subchannel==0) {
                i_min =0; 
                i_max = sizeOfArray(conf.hdrm)-1;
            } else {
                i_min = _subchannel-1; 
                i_max = _subchannel; 
            }

            for(unsigned int i = i_min; i < i_max; ++i)
            {
                switch(_field) {
                case 1 : // modulationType			Ц (PSK- 0, QPSK	- 1, OQPSK -	2, 8PSK	3, 16QAM	4, 8QAM	5, 16APSK	6, 32APSK	7, 8APSK	 8,  64QAM	9, OS8QAM -	10, 32QAM	- 16, pi/4-DQPSK - 17, CQPSK-18, Alt8PSK	- 19)
                    if(conf.hdrm[i].modulationType>19) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 2 : // carrierOffset			- Range: ?? -62500000 ?? +62500000
                    if(conf.hdrm[i].carrierOffset<-62500000 || conf.hdrm[i].carrierOffset>62500000) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 3 : // carrierLBand			- Range: ?? 887500000, 2212500000
                    if(conf.hdrm[i].carrierLBand<887500000 || conf.hdrm[i].carrierLBand>2212500000LL) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                default :
                    status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                    break;
                }
            }
        break;
    case 6 : // DVBS2DemodulatorConf 
            switch(_field) {
            case 20 : // mode		 DVB-S2 (1 Ц CCM, 0 Ц VCM/ACM);							
                if(conf.dvb.mode>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 21 : // dvbs2ModCod				- ????? ???? ????????? ? ???????? ???? ? ?????? ЂCCMї  (1 Ц QPSK Љ, 2 Ц QPSK 1/3, 3 Ц QPSK 2/5, 4 Ц QPSK љ, 5 Ц QPSK 3/5, 6 Ц QPSK 2/3, 7 Ц QPSK Њ, 8 Ц QPSK 4/5, 9 Ц QPSK 5/6, 10 Ц QPSK 8/9, 11 Ц QPSK 9/10, 12 Ц 8PSK 3/5, 13 Ц 8PSK 2/3, 14 Ц 8PSK Њ, 15 Ц 8PSK 5/6, 16 Ц 8PSK 8/9, 17 Ц 8PSK 9/10, 18 Ц 16APSK 2/3, 19 Ц 16APSK Њ, 20 Ц 16APSK 4/5, 21 Ц 16APSK 5/6, 22 Ц 16APSK 8/9, 23 Ц 16APSK 9/10, 24 Ц 32APSK Њ, 25 Ц 32APSK 4/5, 26 Ц 32APSK 5/6, 27 Ц 32APSK 8/9, 28 Ц 32APSK 9/10);
                if(conf.dvb.dvbs2ModCod>28) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 22 : //  dvbs2FrameType		- ЂCCMї (0 Ц Normal 64800 ???, 1 Ц Short 16200 ???);
                if(conf.dvb.dvbs2FrameType>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 23 : //   dvbs2Pilots					- (0 Ц Off, 1 Ц On);
                if(conf.dvb.dvbs2Pilots>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 25 : //  dvbs2PLFRAMEDescrambler  -   0: enable; 1: disable.
                if(conf.dvb.dvbs2PLFRAMEDescrambler>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            default :
                if(_field>25) {
                    status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                }
                break;
            }
        break;
    case 7 : // DVBS2DecoderConf
            switch(_field) {
            case 1 : // autoIter							Ц ????????? ?????? ???????? "Auto"(1 Ц Auto, 0 - ?????) 
                if(conf.dvbDecoder.autoIter>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 2 : 
                break;
            case 3 : //  iterations							Ц ????????? ?????????? ???????? ???????? (2 - 255, 1 = 256);
                if(conf.dvbDecoder.iterations>256) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 4 : // disableFooter					Ц  TFRAME  (0 - on, 1 -off)
                if(conf.dvbDecoder.disableFooter>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 5 : //  disableDFL						Ц ????????? ????????? ???? DFL ? ?????????? ??????????? BB-??????? (DFL Processing) (0 - on, 1 - off)
                if(conf.dvbDecoder.disableDFL>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 6 : //  disableCRCSuppression Ц CRC8 Error Suppression(0 - on, 1 - off)
                if(conf.dvbDecoder.disableCRCSuppression>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 7 : //  outputMode						Ц ????????? ??????? ???????? ?????? (1 Ц TFrame, 2 - Normal);				
                if(conf.dvbDecoder.outputMode>2) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 8 : //  disableAS14_15				Ц ????????? ??????????????????????? (0 Ц AS 14,15 ??? 1 - Off)
                if(conf.dvbDecoder.disableAS14_15>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 9 : 
                break;
            case 10: //  tframeHeaderЦ TFRAME (Range: 0x00000000 ... 0xFFFFFFFF)
                break;
            case 11: 
                break;

            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 8 : // DEMConf 
            switch(_field) {
            case 1 : // mode - (1:SOFT DEC (2Тs Complement), 2:SOFT DEC (Offset Binary), 3:HARD DEC;4: SOFT DEC (2's Complement);5: SOFT DEC (Offset Binary);6: HARD DEC;7: Multichannel Demodulator;8: Packet Demodulator
                if(conf.dem.mode>9) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                } else {
                    MonitorApplication& monApp = dynamic_cast<MonitorApplication&>(Poco::Util::ServerApplication::instance());
                    bool _setThisMode = monApp.StatusSetThisMode(conf.dem.mode);
                    if(_setThisMode==false) {
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                }
                break;
            case 2 :  // multiMode					Ц   ????? ??????????? ??? ?????????? 
                 if(conf.dem.multiMode>255) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 3 : //  numberOfSubchanne  Ц   Number of Subchannels. Range: 1,2,...,max
                if(conf.dem.numberOfSubchannel<1 && conf.dem.numberOfSubchannel>6) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 4 : // carrierFormatInMulti
                if(conf.dem.carrierFormatInMulti>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 5 : // carrierFormatInTDMA
                if(conf.dem.carrierFormatInTDMA>1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 9 : // DecoderConf   
            switch(_field) {
            case 1 : // mode - (0 Ц BYPASS, 1 Ц DVB-S2, 2 Ц TPC IESS-315, 3 Ц TPC Multiblock, 4 Ц TPC Custom, 5 Ц Intelsat Viterbi (k=7), 6 Ц Viterbi (k=8), 7 Ц PTCM Intelsat, 8 Ц Sequential, 9 Ц DVB-S, 10 Ц DVB-DSNG, 11 Ц LDPC 8K, 12 Ц LDPC ULL, 13 Ц LDPC 16K, 14 - TPC Packet);
                if(conf.decoder.mode>14) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                if(conf.dem.mode==8 &&  conf.decoder.mode!=14) {
                       status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 2 :  // cutMPEGSync (0-No, 1 Ц Yes)
                 if(conf.decoder.cutMPEGSync>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;  
            case 3 : // innerCodeRate (1 - љ, 2 - 2/3, 3 - Њ, 4 - 4/5, 5 - 5/6, 6 - 7/8, 7 - 8/9);
                if(conf.decoder.innerCodeRate>7) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 4 : // outerCodeRate	(126/112/07 - 204/187/09 = 0 -10);
                if(conf.decoder.outerCodeRate > 10 ) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 5 : //dataInversion (0 - Disable, 1 Ц Enable);
                if(conf.decoder.dataInversion > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 6 : // outerFECDeinterleaver -   (0 - "04", 1- "08") 
                if(conf.decoder.outerFECDeinterleaver > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 7 : // outerDecoderEnable    -  Outer Code Rate - Off  or On (0- off, 1 - On)
                if(conf.decoder.outerDecoderEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 8 : // outerFECDescramblerEnable -  ( 0 - Disable, 1 Ц Enable);
                if(conf.decoder.outerFECDescramblerEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 9 : // diffDecoderEnable			-  Differential Decoder ( 0 - Disable, 1 Ц Enable);
                if(conf.decoder.diffDecoderEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 10: // innerFECDescramblerEnable			- Self-Sync Descramble (0 - Off, 1- Else)
                if(conf.decoder.innerFECDescramblerEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 11: // innerFECDescramblerType				- Self-Sync Descramble  (1 - Intelsat, 0 - Else)
                if(conf.decoder.innerFECDescramblerType > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 12: // autoSpectralInversionEnable  -  (Auto - 1, else -0) ; ???????  ? demodulator[n].spectralInversion (0 Ц Disable, 1 - Enable)
                if(conf.decoder.innerFECDescramblerType > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 13: //
                status_error = 0;
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 10 : // TPCDecoderConf
            switch(_field) { 
            case 1 : // codeName   - char codeName[CODE_NAME_BUFSIZE]
                break;
            case 2 :  // ahaDescramblerEnable	-  Descrambler (1 - Turbo, 0 - Else) ??????? ? multiTPCDecoder[].asdEnable
                 if(conf.tpcDecoder.ahaDescramblerEnable>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 11 : // MultiTPCDecoderConf
            switch(_field) { 
            case 1 : // asdEnable									-  Descrambler SISO (SISO - 1, else -0) ; ???????  ? tpcDecoder.ahaDescramblerEnable
                 if(conf.multiTPCDecoder.asdEnable>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 2 :  // dataInvAfterDescramblers			- ???????? ?????? ?? ?????? SISO ???????????? (1 Ц Enable, 0 Ц Disable) SISO Output Data Inversion 
                 if(conf.multiTPCDecoder.dataInvAfterDescramblers>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 3 :  // customAutoSearchSync				- ?????? ??????????? ????? ????? ??? ?????? ????????????? (Sync. Search Mode)  (1 Ц Strict, 0 Ц Up to)
                 if(conf.multiTPCDecoder.customAutoSearchSync>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 4 :  // frameLen							-  Range: 1...4096
                 if(conf.multiTPCDecoder.frameLen<1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                 if(conf.multiTPCDecoder.frameLen>4096) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;

            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 12 : // IntelsatViterbiK7Conf
            switch(_field) { 
            case 1 : // delay - : 0 Ц  1-63;Range: 0...63
                 if(conf.viterbiK7.delay>63) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 2 :  // delaySubchannel	-  ЂDelay Clockї (0 Ц P, 1 - Q)
                 if(conf.viterbiK7.delaySubchannel >1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 3 :  // stc16DeinterleaverEnable Ц ??????? ????????? ?????????????? STC-16 (1 Ц Enable, 0 - Disable);
                 if(conf.viterbiK7.stc16DeinterleaverEnable>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 13 : // SequentialConf 
            switch(_field) { 
            case 1 : // fanoPolySelect - (0 - OPEN (IESS.309), 1 - CLOSED (ALT)); (Network Type )
                 if(conf.sequential.fanoPolySelect>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 14 : // DVBSConf
            switch(_field) { 
            case 1 : // fSyncOn
                status_error = 0; // MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 2 : // fSyncOff
                status_error = 0; // MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            default :
                status_error = 0; // MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 15 : // LDPCConf
            switch(_field) { 
            case 1 : // bypassMode
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 2 : // adaptiveBalanserEnable
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 3 :  // correctionOff - ????????? ????????? ??????  (0 Ц Enable; 1 Ц Disable)
                 if(conf.ldpc.correctionOff>1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 4 : // onSync
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 5 : // offSync
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 6 : // errorInSyncBeforeLock
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 7 : // errorInSyncAfterLock
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            case 8 : // maxErrorThreshold
                status_error = 0; //MSG_ERROR_NOT_CHANGE_THIS_DATA;
                break;
            default :
                status_error = 0; //MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 16 : //  PostProcessingConf
            switch(_field) { 
            case 1 : // ssdEnable		- ????? ??????????????????????? ???????????? (0 Ц Disable, 1 - ELSE)
                 if(conf.postProcessing.ssdEnable > 1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 2 : // ssdType			 - ????? ??????????????????????? ???????????? (0 Ц Disable, 0 - 9,11,  1 - 14,15, 2 - 1,18, 3 - 3,20, 4 - 2,12, 7 - Manual)
                 if(conf.postProcessing.ssdType > 7) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 3 :  // ssdPolynomial  -  ????? ???????? ??????????????????????? ????????????; Range: 0x0 ... 0xFFFFFFFF
                 if(conf.postProcessing.ssdPolynomial > 0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 4 : // framerEnable    - Framer Type  (1 Ц Enable, 0 - Disable)
                 if(conf.postProcessing.framerEnable > 1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 5 : // framerDataInversion - ?????????/?????????? ???????? ?????? ????? ??????????????????????? ???????????? (1 Ц Enable, 0 - Disable);
                 if(conf.postProcessing.framerDataInversion > 1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 6 : // framerType - ??? ????????, (0 Ц Disabled, 1 Ц EDMAC, 2 - D&I++, 3 Ц NEXTAR, 4 - ESC++, 5 Ц IBS, 6 Ц IDR, 7 - User Sync #1, 8 - User Sync #4, 9 - User Sync #8, 10 - Sync From Decoder);
                if(conf.postProcessing.framerType > 10) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 7 : // framerFrameLength Ц ????? ?????;
                 if(conf.postProcessing.framerFrameLength > 0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 8 : // framerSyncWord Ц ????? ??????????? ??? ?????? ? ??????????? ??????; Range: 0x0 ... 0xFFFFFFFF
                 if(conf.postProcessing.framerSyncWord > 0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 9 : // framerSyncWord Ц ????? ??????????? ??? ?????? ? ??????????? ??????; Range: 0x0 ... 0xFFFFFFFF
                 if(conf.postProcessing.ssdPolynomial>0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 10 : // framerSyncMask - ????? ??? ??????????, ???????? ? ???? ЂSync. Wordї; Range: 0x0 ... 0xFFFFFFFF
                 if(conf.postProcessing.framerSyncMask > 0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 11 : // framerSyncOffset - ?????????? (? ?????) ????? ??????????????; Range: 1...4095
                 if(conf.postProcessing.framerSyncOffset > 4095) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 12 : // framerSyncGroup -  ?????? ???????? ??? User Sync (1 - User Sync #4, 2 - User Sync #8, 0 - ELSE)					
                 if(conf.postProcessing.framerSyncGroup > 2) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 13 : // adsEnable			- ?????????/?????????? ??????????? ???????????? (0 Ц Disable, 1 -  ELSE);
                 if(conf.postProcessing.adsEnable > 1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 14 : // adsType				- ????? ???????? ??????????? ????????????(1 -"14,15", 7 - Manual, 0 - ELSE);
                 if(conf.postProcessing.adsType > 7) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 15 : // adsPreset			- ????? ????????????? ??????????? ????????????; Range: 0x0 ... 0xFFFFFFFF
                 if(conf.postProcessing.adsPreset > 0xFFFFFFFF) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 16 : // outmuxDataInversion - ?????????/?????????? ????????   (1 Ц Enable, 0 - Disable)
                 if(conf.postProcessing.outmuxDataInversion >1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 17 : //  OutMuxSubchannelConf
            if(_subchannel==0) {
                i_min =0; 
                i_max = sizeOfArray(conf.outMux)-1;
            } else {
                i_min = _subchannel-1; 
                i_max = _subchannel; 
            }

            for(unsigned int i = i_min; i < i_max; ++i)
            {
                switch(_field) { 
                case 1 : // portionLength		- ????? ??????? ??????? ????????? (AA55AA55h) ? ???????? ????? ??????; Range: 16...16777215
                     if(conf.outMux[i].portionLength > 16777215) { 
                         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                     }
                    break;
                case 2 : // data - ????? ?????? ?????? ????????? ?????????????? ?????? (0 - M&C, 1 -Data, 3 - M&C+Data);
                     if(conf.outMux[i].data > 3) { 
                         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                     }
                    break;
                case 3 :  // timeStampEnable - ?????????/?????????? ??????? ????????? ????? ? ???????? ????? ?????? (1 Ц Enable, 0 - Disable);
                     if(conf.outMux[i].timeStampEnable > 1) { 
                         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                     }
                    break;
                case 4 : // headerEnable Ц ?????????/?????????? ??????? ????????? ? ???????? ????? ?????? (1 Ц Enable, 0 - Disable);
                     if(conf.outMux[i].footerEnable > 1) { 
                         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                     }
                    break;
                case 5 : // syncOnMultiframe - ????? ?????? ???????? ?????? ???????????? (1 - On Frame, 1 - On Multiframe).
                     if(conf.outMux[i].syncOnMultiframe > 1) { 
                         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                     }
                    break;
                case 6 : // footerEnable - ????????? ?????????? ????? ????? ?????? (1 Ц Enable,0 - Disable);
                    if(conf.outMux[i].footerEnable > 1) { 
                        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                    }
                    break;
                case 7 : 
                    break;
                default :
                    status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                    break;
                }
            }
        break;
    case 18 : //  MiscChannelConf 
            switch(_field) { 
            case 1 : //  label -  Channel Label
                break;
            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
    case 19 : //  TDMAConf 
            if(_subchannel==0) {
                i_min =0; 
                i_max = sizeOfArray(conf.outMux)-1;
            } else {
                i_min = _subchannel-1; 
                i_max = _subchannel; 
            }
            for(unsigned int i = i_min; i < i_max; ++i)
            {
            switch(_field) { 
            case 1 : // modulationType						-   ??? ????????? ? ????????? (0x00: BPSK, 0x01: QPSK, 0x03: 8PSK, 0x19: Alt8PSK)
                 if(conf.tdma[i].modulationType !=0 && conf.tdma[i].modulationType !=1 && conf.tdma[i].modulationType !=3 &&  conf.tdma[i].modulationType != 19) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
//                     poco_debug_f4(_logger, "tdma[%u], field = %u, Error = %u -> modulationType = %u", i, _field, status_error, conf.demodulator[i].symbolRate );
                 }
                break;
            case 2 : // carrierOffses								- Carrier Offset  Range: ?? -62500000 ?? +62500000
                 if(conf.tdma[i].carrierOffset < -62500000 || conf.tdma[i].carrierOffset > 62500000) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 3 :  // symbolRate								- Symbol Rate  Range: ?? 32000 ?? 15625000
                 if(conf.tdma[i].symbolRate<32000 || conf.tdma[i].symbolRate > 15625000) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 4 : // spectralInversion						- Spectral Inversion  (1 Ц Enable, 0 - Disable);
                 if(conf.tdma[i].spectralInversion > 1) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 5 : // terminalName							- ????? ????????? ????????? 
                 if(conf.tdma[i].terminalName > 1024) { 
                     status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                 }
                break;
            case 6 : // outputMode								- SubCh Output Mode ( 0x1: DDC; 0x2: SOTF_DEC; 0x3: SOFT_DEC(UW); 0x4: Decoder TPC)
                if(conf.tdma[i].outputMode ==0 || conf.tdma[i].outputMode > 4) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 7 : // carrierLBand			- Range: ?? 887500000, 2212500000
                if(conf.tdma[i].carrierLBand<887500000LL || conf.tdma[i].carrierLBand>2212500000LL) {
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }

            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
           }
        break;

    case 20 : //  TDMADecoderConf 
            switch(_field) { 
            case 1 : //   char codeName[CODE_NAME_BUFSIZE]	 - ?????? ????????? ? ?????? ?????? Packet Demodulator
                status_error = 0;
                break;
            case 2 : //  ahaDescramblerEnable			-  Descrambler (1 - Turbo, 0 - Else) ??????? ? multiTPCDecoder[].asdEnable
                if(conf.tdmaDecoder.ahaDescramblerEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 3 : //  modulationType						-   ??? ????????? ? ????????? (0x00: BPSK, 0x01: QPSK, 0x03: 8PSK, 0x19: Alt8PSK)
                if(conf.tdmaDecoder.modulationType !=0 &&  conf.tdmaDecoder.modulationType !=1  && conf.tdmaDecoder.modulationType !=3 &&  conf.tdmaDecoder.modulationType != 19) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 4 : //  headerValue							- Range: 0x0 ... 0xFFFFFFFF
                if(conf.tdmaDecoder.headerValue > 0xFFFFFFFF) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 5 : //  portionLength							-  ????? ??????? ??????? ????????? (AA55AA55h) ? ???????? ????? ??????; Range: 16...16777215
                if(conf.tdmaDecoder.portionLength < 16 || conf.tdmaDecoder.portionLength> 16777215) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;
            case 6 : //  timeStampEnable					- ?????????/?????????? ??????? ????????? ????? ? ???????? ????? ?????? (1 Ц Enable, 0 - Disable);
                if(conf.tdmaDecoder.timeStampEnable > 1) { 
                    status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + _structure*100 + _field;
                }
                break;

            default :
                status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
                break;
            }
        break;
         
    default :
        status_error = MSG_ERROR_EXCEEDED_NUM_STRUCT;
        break;
    }
    */
    return status_error;

}
//----------------------------------------------------------------------------------------------------------
unsigned int ZMQHandler::checkStructureDevice(void *Structure, unsigned int _length, unsigned int _field)
{
    unsigned int status_error=0;
/*
    DeviceSettings pDeviceSettings;
    unsigned int MemberSizeInDivice[5];
    MemberSizeInDivice[0] = sizeof(&pDeviceSettings);
    MemberSizeInDivice[1] = 4;      // int  saveConfiguration;								
    MemberSizeInDivice[2] = 4;      // int  reset;
    MemberSizeInDivice[3] = 255;    // char deviceName[LABEL_BUFSIZE];	
    MemberSizeInDivice[4] = 255;    // char dateAndTime[DATE_TIME_BUFSIZE]; 
  

    if(_length!=MemberSizeInDivice[_field]) {
         status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 255;
         poco_debug_f1(_logger, "Error size of structure DeviceSettings. You length = %u",   MemberSizeInDivice[_field]);
         return status_error; 
    }

    pDeviceSettings.reset = 0;
    pDeviceSettings.saveConfiguration = 0;
    memset(&pDeviceSettings, 0, sizeof(pDeviceSettings));
    convertDeviceSettingsMon2SNMP(_settingsManager.getCommonSettings(), pDeviceSettings);
    DateTime currentTime;
    convertDateTimeMon2SNMP(currentTime, pDeviceSettings);
    switch(_field) {
    case 0 :
            memcpy((void *) &pDeviceSettings, Structure, _length);
            // poco_debug_f1(_logger, "TRACE. deviceName = %s", std::string((char *) pDeviceSettings.deviceName));
            break;
    case 1 :  //  int  saveConfiguration
             memcpy((void *) &pDeviceSettings.saveConfiguration, Structure, _length);
            //if(conf.tuner.frequency<950000 || conf.tuner.frequency>2150000) {
            break;
    case 2 : // int  reset;
             memcpy((void *) &pDeviceSettings.reset, Structure, _length);
            break;    
    case 3 : // char deviceName[LABEL_BUFSIZE]
             memcpy((void *) &pDeviceSettings.deviceName, Structure, _length);
             break;    
    case 4 : // char dateAndTime[DATE_TIME_BUFSIZE]
             memcpy((void *) &pDeviceSettings.dateAndTime, Structure, _length);
             break;    
    default:
            status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
            break;
    }

    if(status_error==0) {
            receiveDeviceSettings(pDeviceSettings);
    }
    */
    return 0;
}

//----------------------------------------------------------------------------------------------------------
/*
unsigned int ZMQHandler::checkStructureConst(void *Structure, unsigned int _length)
{
//    TRACE();
    unsigned int status_error=0;
    ConstellationConfigZMQ pConstellationConfig;

    memcpy((void *) &pConstellationConfig, Structure, _length);
//    poco_debug_f3(_logger, "pConstellationConfig.contrast = %u, pConstellationConfig.powerOfAveraging = %u, pConstellationConfig.subChannel = %u", pConstellationConfig.contrast, pConstellationConfig.powerOfAveraging, pConstellationConfig.subChannel );
    if(pConstellationConfig.contrast>255) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 201;
    }
    if(pConstellationConfig.powerOfAveraging>7) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 202;
    }
    if(pConstellationConfig.subChannel>7) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 203;
    }

    if(status_error==0) {
         _conDev->setContrast(pConstellationConfig.contrast);
         _conDev->setPowerOfAveraging(pConstellationConfig.powerOfAveraging);
         _conDev->setSubChannel(pConstellationConfig.subChannel); 
 //        poco_debug(_logger, "checkStructureConst! SetContrast");
    }
    return status_error;
}
*/
unsigned int ZMQHandler::checkStructureFFT(void *Structure, unsigned int _length)
{
    unsigned int status_error=0;
    FFTConfigZMQ pFFTConfigZMQ;

    memcpy((void *) &pFFTConfigZMQ, Structure, _length);
    if(pFFTConfigZMQ.measureSymbolRate>1) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 201;
    }
    if(pFFTConfigZMQ.powerOfAveraging>7) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 202;
    }
    if(pFFTConfigZMQ.scale==0 || pFFTConfigZMQ.scale>255) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 203;
    }
    if(pFFTConfigZMQ.subChannel>7) {
        status_error = MSG_ERROR_EXCEEDED_SIZE_STRUCT + 204;
    }
/*
    if(status_error==0) {
        _fftDev->setMeasureSymbolRateMode(pFFTConfigZMQ.measureSymbolRate);
        _fftDev->setPowerOfAveraging(pFFTConfigZMQ.powerOfAveraging);
        _fftDev->setScale(pFFTConfigZMQ.scale);
        _fftDev->setSubChannel(pFFTConfigZMQ.subChannel); 
        _fftDev->saveMeasureSymbolRate(pFFTConfigZMQ.measureSymbolRate);
        _fftDev->savePowerOfAveraging(pFFTConfigZMQ.powerOfAveraging);
        _fftDev->saveScale(pFFTConfigZMQ.scale);
    }
*/
    return status_error;
}


void setHWClock()
{
    string hwClock("/sbin/hwclock");
    vector<string> args;
    args.push_back("-w");

    ProcessHandle handle = Process::launch(hwClock, args);
    handle.wait();
}

void setTime(const DateTime& dateTime)
{
    time_t curTime = time(NULL);
    tm* pTM = localtime(&curTime);
    if(pTM) {
        pTM->tm_mon  = dateTime.month() - 1;
        pTM->tm_mday = dateTime.day();
        pTM->tm_year = dateTime.year() - 1900;
        pTM->tm_hour = dateTime.hour();
        pTM->tm_min  = dateTime.minute();
        pTM->tm_sec  = dateTime.second();
    }
    const timeval tv = {mktime(pTM), 0};
    settimeofday(&tv, 0);
}

void rebootSystem()
{
    reboot(RB_AUTOBOOT);
}



void ZMQHandler::receiveDeviceSettings(DeviceSettings& settings)
{
     // TRACE();
    if(string(settings.deviceName) != _settingsManager.getCommonSettings().getDeviceName()) {
//        poco_information(_logger, "convertDeviceSettingsSNMP2Mon");
//        convertDeviceSettingsSNMP2Mon(settings, _settingsManager.getCommonSettings());
        _nc.postNotification(new ApplyDeviceSettingsNotification(ApplyDeviceSettingsNotificationPayload(settings.saveConfiguration)));
        return;
    }
    if(settings.saveConfiguration) {
         poco_information(_logger, "settings.saveConfiguration");
        _nc.postNotification(new ApplyDeviceSettingsNotification(ApplyDeviceSettingsNotificationPayload(settings.saveConfiguration)));
        return;
    }
    // Check reset flag
    if(settings.reset) {
        poco_information(_logger, "Rebooting system by users request");
        rebootSystem();
        return;
    }

    // Set time on device
    DateTime dt;
    // convertDateTimeSNMP2Mon(settings, dt);
    setTime(dt);
    setHWClock();
    poco_information(_logger, "Rebooting system after date/time configuration");
    rebootSystem();
}
//========================================================================================================
void ZMQHandler::processHeader(ZMQManagerHeader& header) // const 
{
   // TRACE();
/*   ScopedLock<Mutex> lock(_timerMutex);
   unsigned int length = 0;
   unsigned int status_error_send = 0;
   

   bool _Fading = false;
   if(header.ProcessStatus&1) {
       _Fading = true;
       poco_information(_logger,"TRACE! Fading - true !!");
   } 

   // ‘ункци€ проверки Pending          //   header.ProcessStatus = checkPending(header.Channel);
   header.ProcessStatus = checkQuery(header); 
   // ќтправить ответ ZMQManagerHeader (answer)

   if(header.ProcessStatus>0) {
       if(header.Nom_structure==210) {
           if(header.ProcessStatus==MSG_ERROR_BUSY_PENDING_CHANNEL1 || header.ProcessStatus==MSG_ERROR_BUSY_PENDING_CHANNEL2)
              header.ProcessStatus = 0;
       } else {
           poco_debug_f1(_logger, "Error running command! Number error: %u", static_cast<unsigned int> (header.ProcessStatus));
       }    
       usleep(100);
   }

   status_error_send = sendZMQAnswer(header);

   if(header.ProcessStatus>0) {
       return;
   }
   
   if(status_error_send>0) {
        poco_debug_f1(_logger, "Error running command! Status_error_send = %u ", status_error_send);
        return;
   }

   // ≈сли ожидаетс€ ответ(header.ProcessStatus) -> прин€ть структуру(поле) void 
   DateTime currentTime;
   void *work_structure;
   zmq_msg_t req;
   zmq_msg_init(&req);
   zmq_msg_recv(&req, ZMQSocket, 0);  // DLL >>> Structure
   length = zmq_msg_size(&req);
   if(length==0) {
       poco_debug(_logger, "Error running command! MSG_ERROR_SIZE_STRUCTURE ( Sending length is NULL");
       header.ProcessStatus = MSG_ERROR_SIZE_STRUCTURE;
       sendZMQAnswer(header);
       return;
   }
//   poco_debug_f1(_logger, "You sent me the size of the structure as follows -  %u", length);
   work_structure = (void*) malloc (length+1);
    //memcpy((void *) &_conf, zmq_msg_data(&req), length); 
   memcpy(work_structure, zmq_msg_data(&req), length); 
   zmq_msg_close(&req); 

   // ‘ункци€ проверки правильности прин€тых данных и возможности их внесени€ и применени€
   if(header.Nom_structure<100) {
           // получить ChannelConf 
        ChannelConf ChConf;
        header.ProcessStatus = checkStructureConf(work_structure, length, header.Nom_structure, header.Nom_subchannel, header.Nom_field);
        if(header.ProcessStatus==0) {
        
            unsigned int min_nstr = 1, max_nstr = 1;
            unsigned int min_nfld = 1, max_nfld = 1;

            if(header.Nom_structure==0) {
                header.Nom_field =0;
                min_nstr = 1;
                max_nstr = 21;
            } else {
                min_nstr = header.Nom_structure;
                max_nstr = min_nstr + 1;
            }

            convertChannelConfigMon2SNMP(_settingsManager.getChannelSettings(_channel), ChConf, 0); // header.Nom_structure
            copyChannelConfigZMQ2MonH(ChConf, work_structure, length, header.Nom_structure, header.Nom_subchannel, header.Nom_field);
            
            for(unsigned int nstr=min_nstr;nstr<max_nstr;nstr++) {
                if(header.Nom_field==0) {
                    min_nfld = 1;
                    max_nfld = StructInfoMemberList[nstr]+1;
                } else {
                    min_nfld = header.Nom_field;
                    max_nfld = min_nfld+1;
                }

                for(unsigned int nfld=min_nfld;nfld<max_nfld;nfld++) {
                        header.ProcessStatus = checkDataInStructureConf(ChConf, nstr, header.Nom_subchannel, nfld);
                        if(header.ProcessStatus>0) {
                            poco_debug_f3(_logger, "Error running command! header.ProcessStatus (checkDataInStructureConf) =  %u, (nstr = %u, nfld = %u)", header.ProcessStatus, nstr, nfld);
                            break;
                        }
                }
                if(header.ProcessStatus>0 ) {
                    break;
                }
                // convertChannelConfigSNMP2Mon(conf, _settingsManager.getChannelSettings(_channel), nstr);
            }
//            header.ProcessStatus = checkDataInStructureConf(ChConf,  header.Nom_structure, header.Nom_subchannel, header.Nom_field);
            if(header.ProcessStatus==0) {
                header.ProcessStatus =  checkPending();
                if(header.ProcessStatus==0) {
                    // convertChannelConfigSNMP2Mon(ChConf, _settingsManager.getChannelSettings(_channel), 0);
                    printChannelConfig(_logger, ChConf);
                    if(_Fading){
                        _nc.postNotification(new ApplyFadingChannelConfigurationNotification(ApplyFadingChannelConfigurationNotificationPayload(_channel)));
                    } else {
                        _nc.postNotification(new ApplyChannelConfigurationNotification(ApplyChannelConfigurationNotificationPayload(_channel)));
                    }
                }
           }

        } else {
            poco_debug_f1(_logger, "Error running command! Number error: %u", static_cast<unsigned int> (header.ProcessStatus));
        }
       
   } else {
       if(header.Nom_structure==200) { /// FFT
           header.ProcessStatus = checkStructureFFT(work_structure, length);
       }
       if(header.Nom_structure==201) { /// Const
//           header.ProcessStatus = checkStructureConst(work_structure, length);
       }
       if(header.Nom_structure==255) {
           header.ProcessStatus = checkStructureDevice(work_structure, length, header.Nom_field);
       }
       
       if(header.Nom_structure==210) { /// Analyzer
// poco_debug(_logger, "if(header.Nom_structure==210)");
               unsigned int val =  1500000000;
               memcpy((void *) &val, work_structure, length);
               // Range: 950000 ... 2150000
               if(val<950000000 || val>2150000000LL) {
                   header.ProcessStatus = MSG_ERROR_EXCEEDED_SIZE_RF;
                   poco_debug_f1(_logger, "Error running command!  Value RF (%u) transcends (Range: 950000 ... 2150000)", val);
               } else {
                   _settingsManager.getChannelSettings(static_cast<unsigned int> (_channel)).getTunerSettings().setFrequency(val);
                   _statisticsManager.getCommonStatistics().setTunerLock(_channel, false);
                   _nc.postNotification(new ApplyTunerConfigurationNotification(ApplyTunerConfigurationNotificationPayload(_channel)));
 
                   if(!_statisticsManager.getCommonStatistics().getTunerLock(_channel)) {
                       header.ProcessStatus = MSG_ERROR_TUNER_UNLOCK;
                   } else{
                       header.ProcessStatus = 0; // OK !!!
                   }
                }
       }
    }

   free(work_structure);

   header.ProcessStatus = sendZMQAnswer(header);
*/
}

} // namespace SNMP



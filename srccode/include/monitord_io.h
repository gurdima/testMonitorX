/** This file is licensed under GPLv2.
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU General Public License as
 ** published by the Free Software Foundation; either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful, but
 ** WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 ** USA
 **/

#pragma once

#include <time.h>

/*---------------------------------------------------------------------------*/

#define SNMP_IO_UNIX_SOCKET_ADDRESS     "ipc:///system/ramdisk/monitor.snmp"

/*---------------------------------------------------------------------------*/

#define SUBCHANNELS_PER_CHANNEL     5
#define TDMA_SUBCHANNELS_PER_CHANNEL    6
// #define SUBCHANNELS_PER_MODES       8

#define LABEL_BUFSIZE               255
#define DATE_TIME_BUFSIZE           255
#define VERSION_BUFSIZE             255
#define REQUEST_BUFSIZE             255
#define MODULATIONS_BUFSIZE         255
#define INNER_CODE_RATE_BUFSIZE     255
#define OUTER_CODE_RATE_BUFSIZE     INNER_CODE_RATE_BUFSIZE
#define CODES_BUFSIZE               2048
#define CORE_VERSIONS_BUFSIZE       1024
#define MODES_LIST_BUFSIZE          512
#define CODE_NAME_BUFSIZE           255
#define FRAMERS_BUFSIZE             255
#define BLOCK_SIZE_AND_INF_BITS     512
#define SYMBOL_RATE_RANGE_BUFSIZE   64
#define NETWORKS_BUFSIZE            1024
#define TERMINALS_BUFSIZE           1024
#define DATA_SPLITTER_MAX_SIZE      255 // 1024

#define MAX_CONNECTIONS_PER_CHANNEL 4

/*---------------------------------------------------------------------------*/

#define MSG_TYPE_SET_CHANNEL_CONFIG         10
#define MSG_TYPE_GET_CHANNEL_CONFIG         20
#define MSG_TYPE_GET_CHANNEL_STAT           30

#define MSG_TYPE_SET_NEW_MAIN_SOCKET        100

#define MSG_TYPE_GET_DEVICE_INFO            110

#define MSG_TYPE_SET_DEVICE_SETTINGS        120
#define MSG_TYPE_GET_DEVICE_SETTINGS        130

#define MSG_TYPE_GET_CONNECTIONS_INFO       150

#define MSG_TYPE_GET_PROTO_CHANNEL_CONFIG   200
#define MSG_TYPE_GET_PROTO_CHANNEL_STAT     210
#define MSG_TYPE_GET_PROTO_DEVICE_INFO      220
#define MSG_TYPE_GET_PROTO_DEVICE_SETTINGS  230
#define MSG_TYPE_GET_PROTO_CONNECTIONS_INFO 240

#define MSG_TYPE_SET_PROTO_DEVICE_SETTINGS  250
#define MSG_TYPE_SET_PROTO_CHANNEL_CONFIG   260

#define MSG_TYPE_GET_PENDING                333
#define MSG_TYPE_SET_LEADER                 300    
/*---------------------------------------------------------------------------*/

// Needed to determine type of agc error (TunerStat::agcError)
#define AGC_LEVEL_TOO_LOW   0x01
#define AGC_LEVEL_TOO_HIGH  0x02

// Needed to determine aeq status (DVBS2Stat::aeqStatus)
#define AEQ_LOCKED          0x0001
#define AEQ_ENABLED         0x0002

/*---------------------------------------------------------------------------*/

#define HTTP_MSG_TYPE_CONFIG     1
#define HTTP_MSG_TYPE_NOT_CONFIG 0
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------*/
/*       DeviceStatus  -  установки устройства                             */
/*---------------------------------------------------------------------------*/


struct DeviceStatus												
{
    char deviceName[32];										
    char dateAndTime[32];											
    unsigned char TunerState;									
    unsigned char PendingState;								
    unsigned char ManagementStatus;						
};
/*
    
    unsigned char DemodulatorState;						
    unsigned char DecoderState;                             
*/

/*---------------------------------------------------------------------------*/
/*  DeviceInfo  - 0,0 (Channel = 0, struct DeviceInfo = 0)                   */
/*---------------------------------------------------------------------------*/

struct DemodulatorInfo                              // (0, 7, 0, 0)     where 0 - device, 7 - structure DemodulatorInfo(dvbs2Info), 0 - channel(not used), 0 - all fields
{                                                   // (0, 8, 0, 0)     where 0 - device, 8 - structure DemodulatorInfo(hdrmInfo), 0 - channel(not used) , 0 - all fields
    char         symbolRateRange[SYMBOL_RATE_RANGE_BUFSIZE];    
};

struct DecoderInfo                                  // (0, 9, 0, 0)     where 0 - device, 9 - structure DecoderInfo , 0 - channel(not used) , 0 - all fields
{
    char         modesList[MODES_LIST_BUFSIZE];                 // (0, 9, 0, 1)     where 0 - device, 9 - structure DecoderInfo , 0 - channel(not used) , 0 - field modesList
    unsigned int modesMask;                                     // (0, 9, 0, 2)     where 0 - device, 9 - structure DecoderInfo , 0 - channel(not used) , 0 - field modesMask
    char         modulations[MODULATIONS_BUFSIZE];              // (0, 9, 0, 3)     ...
    char         innerCodeRate[INNER_CODE_RATE_BUFSIZE];        // (0, 9, 0, 4) 
    char         outerCodeRate[OUTER_CODE_RATE_BUFSIZE];        // (0, 9, 0, 5) 
    char         codes[CODES_BUFSIZE];                          // (0, 9, 0, 6) 
    char         coreVersions[CORE_VERSIONS_BUFSIZE];           // (0, 9, 0, 7) 
    char         statAllNetworklist[NETWORKS_BUFSIZE];          // (0, 9, 0, 8) 
    char         statAllTerminalList[TERMINALS_BUFSIZE];        // (0, 9, 0, 9) 
    unsigned int maxOfChannelsInTDMA;                           // (0, 9, 0, 10)    where 0 - device, 9 - structure DecoderInfo , 0 - channel(not used) , 10 - field  maxOfChannelsInTDMA
};


struct PostProcessingInfo                                       // (0, 10, 0, 0)     where 0 - device, 10 - structure PostProcessingInfo, 0 - channel(not used) , 0 - all fields
{
    char framers[FRAMERS_BUFSIZE];                              // - not used
};

struct LDPCInfo                                                 // (0, 11, 0, 0)     where 0 - device, 11 - structure LDPCInfo, 0 - channel(not used) , 0 - all fields
{
    char blockSizeAndInfBits[BLOCK_SIZE_AND_INF_BITS];          // - not used
};

struct ULLInfo                                                  // (0, 12, 0, 0)     where 0 - device, 12 - structure ULLInfo, 0 - channel(not used) , 0 - all fields
{
    char blockSizeAndInfBits[BLOCK_SIZE_AND_INF_BITS];          // - not used
};
//-----------------------
//-----------------------

struct DeviceInfo                                   // 0    (0, 0, 0, 0) where 0 - device, 0 - all structure DeviceInfo, 0 - channel(not used) , 0 - all field
{
    int  cpuIdle;                                   // (0, 0, 0, 1) where 0 - device, 0 - structure DeviceInfo, 0 - channel(not used) , 1 - № field (cpuIdle)
    unsigned char configurationPending;             // (0, 0, 0, 2) where 0 - device, 0 - structure DeviceInfo, 0 - channel(not used) , 2 - № field (configurationPending)
                                                    //  true if device configuration process is pending, false otherwise
    char installVersion[VERSION_BUFSIZE];           // (0, 0, 0, 3) where 0 - device, 0 - structure DeviceInfo, 0 - channel(not used) , 3 - № field (configurationPending)
                                                    //   installVersion[255]
    char m2kFPGAVersion[VERSION_BUFSIZE];           // (0, 0, 0, 4) where 0 - device, 0 - structure DeviceInfo, 0 - channel(not used) , 4 - № field (configurationPending)
                                                    //   m2kFPGAVersion[255]
    char demFPGAVersion[2][VERSION_BUFSIZE];        // (0, 0, y, 5) where 0 - device, 0 - structure DeviceInfo, 0 - channel ( 1 or 2) , 5 - № field (configurationPending)
                                                    //    demFPGAVersion[2][255]
    char decFPGAVersion[2][VERSION_BUFSIZE];        // (0, 0, y, 6) where 0 - device, 0 - structure DeviceInfo, 0 - channel ( 1 or 2) , 6 - № field (configurationPending)
                                                    //    decFPGAVersion[2][255]

    struct DemodulatorInfo      dvbs2Info;          // (0, 7, 0, 0)     where 0 - device, 7 - structure DemodulatorInfo(dvbs2Info), 0 - channel(not used) , 0 - all fields
    struct DemodulatorInfo      hdrmInfo;           // (0, 8, 0, 0)     where 0 - device, 8 - structure DemodulatorInfo(hdrmInfo), 0 - channel(not used) , 0 - all fields
    struct DecoderInfo          decoderInfo;        // (0, 9, 0, 0)     where 0 - device, 9 - structure decoderInfo, 0 - channel(not used) , 0 - all fields
    struct PostProcessingInfo   postProcessingInfo; // (0, 10, 0, 0)    where 0 - device, 10 - structure postProcessingInfo, 0 - channel(not used), 0 - all fields
    struct LDPCInfo             ldpcInfo;           // (0, 11, 0, 0)    where 0 - device, 11 - structure ldpcInfo, 0 - channel(not used), 0 - all fields   
    struct ULLInfo              ullInfo;            // (0, 12, 0, 0)    where 0 - device, 12 - structure ullInf, 0 - channel(not used) , 0 - all fields       
};

/*---------------------------------------------------------------------------*/
/*       DeviceSettings  -  установки устройства                             */
/*---------------------------------------------------------------------------*/

struct DeviceSettings                                           // (1, 255, 0) where 1 - channel(имеется ввиду № канала через который происходит управление устройством), 255 - structure ChannelConf, 0 - all field
{                                                               // (2, 255, 0) where 2 - channel, 255 - structure ChannelConf, 0 - all field
    int  saveConfiguration;                                     // (x, 255, 1) where x - № channel(1 or 2),255 - structure ChannelConf, 1 - field - saveConfiguration
    int  reset;                                                 // (x, 255, 2) where x - № channel(1 or 2),255 - structure ChannelConf, 2 - field - reset
    char deviceName[LABEL_BUFSIZE];                             // (x, 255, 3) where x - № channel(1 or 2),255 - structure ChannelConf, 3 - field - deviceName
    char dateAndTime[DATE_TIME_BUFSIZE];                        // (x, 255, 4) where x - № channel(1 or 2),255 - structure ChannelConf, 4 - field - dateAndTime
};
/*---------------------------------------------------------------------------*/
/*      ChannelConf - 1,0 for first channel, 2,0  for second channel         */
/*---------------------------------------------------------------------------*/

struct TunerConf                                                                // (x, 1, 0, 0) where x - № channel(1 or 2), 1 - № structure (TunerConf), 0 - subchannel(not used), 0 - all field
{                                                                               
    unsigned int  frequency;                                                    // (x, 1, 0, 1) where x - № channel(1 or 2), 1 - № structure (TunerConf), 0 - subchannel(not used), 1 - field frequency
    unsigned int  filterBand;                                                   // (x, 1, 0, 2) where x - № channel(1 or 2), 1 - № structure (TunerConf), 0 - subchannel(not used), 2 - field filterBand
    unsigned char manualFilterMode;                                             // (x, 1, 0, 3) ...
    unsigned int  spectrumAnalyzerShift;                                        // (x, 1, 0, 4)
    unsigned char spectrumAnalyzerEnable;                                       // (x, 1, 0, 5)
    float         supplyVoltage;                                                // (x, 1, 0, 6)
    unsigned int  referenceVoltage;                                             // (x, 1, 0, 7)
    unsigned int  maxSpectrumAnalyzerSymbolRate;                                // (x, 1, 0, 8) where x - № channel(1 or 2), 1 - № structure (TunerConf), 0 - subchannel(not used), 8 - field maxSpectrumAnalyzerSymbolRate
};
        
struct AGCConf                                                                  // (x, 2, 0, 0) where x - № channel(1 or 2), 2 - № structure (AGCConf ), 0 - subchannel(not used), 0 - all field
{                                                                               
    unsigned int initialGain;                                                   // - not used
    unsigned int mode;                                                          // - not used
    unsigned int manualLBandGain;                                               // - not used
    unsigned int manualBaseBandGain;                                            // - not used
};

struct ADCConf                                                                  // (x, 3, 0, 0) where x - № channel(1 or 2), 1 - № structure (ADCConf), 0 - subchannel(not used), 0 - all field
{   
    unsigned char coarseGainEnabled;                                            // - not used
    unsigned int  decimationRate;                                               // - not used
    unsigned char lowLatencyEnabled;                                            // - not used
    unsigned int  outputband;
};

struct DemodulatorSubchannelConf                                                // (x, 4, y, 0) where x - № channel(1 or 2), 1 - № structure (DemodulatorSubchannelConf), y - subchannel( 1 - 5 ), 0 - all field
{                                                                               
    unsigned int  symbolRate;                                                   // (x, 4, y, 1) where x - № channel(1 or 2), 1 - № structure (DemodulatorSubchannelConf), y - subchannel( 1 - 5 ), 1 - field symbolRate
    unsigned char spectralInversion;                                            // (x, 4, y, 2) ...
    unsigned int  rrcAlpha;                                                     // (x, 4, y, 3)
//    double        APSKRROI;                                                     // (x, 4, y, 4)
//    double        APSKRRMI;                                                     // (x, 4, y, 5)
    unsigned int   APSKRROI;                                                     // (x, 4, y, 4)
    unsigned int   APSKRRMI;                                                     // (x, 4, y, 5)
    unsigned int  aeqMode;                                                      // (x, 4, y, 6)
    unsigned int  searchRange;                                                  // (x, 4, y, 7)
    unsigned int  mode;                                                         // (x, 4, y, 8)
    unsigned int  ddcEnable;                                                    // (x, 4, y, 9)
    unsigned int  dcic;                                                         // (x, 4, y, 10)
    unsigned int  nonCoherent;                                                  // (x, 4, y, 11)
};

struct HDRMSubchannelConf                                                       // (x, 5, y, 0) where x - № channel(1 or 2), 1 - № structure (HDRMSubchannelConf), y - subchannel( 1 - 5 ), 0 - all field
{
    unsigned int  modulationType;                                               // - not used
    int  carrierOffset;                                                         // - not used
    unsigned int carrierLBand; 
};

struct DVBS2DemodulatorConf                                                     // (x, 6, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBS2DemodulatorConf), 0 - subchannel(not used), 0 - all field            
{
    unsigned int  sampleRate;                                                   // (x, 6, 0, 1) where x - № channel(1 or 2), 6 - № structure (DVBS2DemodulatorConf), 0 - № subchannel(not used), 1 - № field (sampleRate)   
    unsigned char ragcEnable;                                                   // (x, 6, 0, 2)      ...
    unsigned char ragcInvert;                                                   // (x, 6, 0, 3)
    unsigned int  outputAmplitudeVCM;                                           // (x, 6, 0, 4)
    unsigned int  outputAmplitudeQPSK;                                          // (x, 6, 0, 5)
    unsigned int  outputAmplitude8PSK;                                          // (x, 6, 0, 6)
    unsigned int  outputAmplitude16APSK;                                        // (x, 6, 0, 7)
    unsigned int  outputAmplitude32APSK;                                        // (x, 6, 0, 8)
    unsigned char reacqHoldoff;                                                 // (x, 6, 0, 9)
    float        reacqActivationDelay;                                          // (x, 6, 0, 10)
    float        reacqRestartDelay;                                             // (x, 6, 0, 11)
    unsigned char falseDetEnable;                                               // (x, 6, 0, 12)
    unsigned char falseDetOneshot;                                              // (x, 6, 0, 13)
    float        falseDetThresh;                                                // (x, 6, 0, 14)
    float        falseDetPeriod;                                                // (x, 6, 0, 15)
    unsigned int  aeqAutoSpeed;                                                 // (x, 6, 0, 16)
    unsigned int  aeqAutoModMask;                                               // (x, 6, 0, 17)
    unsigned char aeq2xRate;                                                    // (x, 6, 0, 18)
    unsigned int  coarseSteps;                                                  // (x, 6, 0, 19)
    unsigned int  mode;                                                         // (x, 6, 0, 20)
    unsigned int  dvbs2ModCod;                                                  // (x, 6, 0, 21)
    unsigned int  dvbs2FrameType;                                               // (x, 6, 0, 22)
    unsigned char dvbs2Pilots;                                                  // (x, 6, 0, 23)
    float        dvbs2LoopBw;                                                   // (x, 6, 0, 24)
	unsigned char dvbs2PLFRAMEDescrambler;                                      // (x, 6, 0, 25)
}; 

struct DVBS2DecoderConf                                                         // (x, 7, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBS2DecoderConf), 0 - subchannel(not used), 0 - all field    
{
    unsigned char autoIter;                                                     // (x, 7, 0, 1) where x - № channel(1 or 2), 1 - № structure (DVBS2DecoderConf), 0 - subchannel(not used), 1 - field autoIter       
    unsigned int  autoIterMode;                                                 // (x, 7, 0, 2)         ...
    unsigned int  iterations;                                                   // (x, 7, 0, 3)
    unsigned char disableFooter;                                                // (x, 7, 0, 4)
    unsigned char disableDFL;                                                   // (x, 7, 0, 5)
    unsigned char disableCRCSuppression;                                        // (x, 7, 0, 6)
    unsigned int  outputMode;                                                   // (x, 7, 0, 7)
    unsigned char disableAS14_15;                                               // (x, 7, 0, 8)
    unsigned int  isi;                                                          // (x, 7, 0, 9)
    unsigned int  tframeHeader;                                                 // (x, 7, 0, 10)
    unsigned int  outModCod;                                                    // (x, 7, 0, 11)
};

struct DEMConf                                                                  // (x, 8, 0, 0) where x - № channel(1 or 2), 1 - № structure (DEMConf), 0 - subchannel(not used), 0 - all field    
{
    unsigned int  mode;                                                         // - not used
    unsigned int  multiMode;                                                    // - not used
    unsigned int  numberOfSubchannel;                                           // - not used
    unsigned int  carrierFormatInMulti;
    unsigned int  carrierFormatInTDMA;
};


struct DecoderConf                                                              // (x, 9, 0, 0) where x - № channel(1 or 2), 1 - № structure (DecoderConf), 0 - subchannel(not used), 0 - all field
{
    unsigned int  mode;                                                         // (x, 9, 0, 1) where x - № channel(1 or 2), 1 - № structure (DecoderConf), 0 - subchannel(not used), 1 - field mode
    unsigned char cutMPEGSync;                                                  // (x, 9, 0, 2)         ... 
    unsigned int  innerCodeRate;                                                // (x, 9, 0, 3) 
    unsigned int  outerCodeRate;                                                // (x, 9, 0, 4) 
    unsigned char dataInversion;                                                // (x, 9, 0, 5) 
    unsigned int  outerFECDeinterleaver;                                        // (x, 9, 0, 6) 
    unsigned char outerDecoderEnable;                                           // (x, 9, 0, 7) 
    unsigned char outerFECDescramblerEnable;                                    // (x, 9, 0, 8) 
    unsigned char diffDecoderEnable;                                            // (x, 9, 0, 9) 
    unsigned char innerFECDescramblerEnable;                                    // (x, 9, 0, 10) 
    unsigned int  innerFECDescramblerType;                                      // (x, 9, 0, 11) 
    unsigned char autoSpectralInversionEnable;                                  // (x, 9, 0, 12) 
    unsigned char autoAmbiguityElimination;                                     // (x, 9, 0, 13) 
};

struct TPCDecoderConf                                                          // (x, 10, 0, 0) where x - № channel(1 or 2), 1 - № structure (TPCDecoderConf), 0 - subchannel(not used), 0 - all field
{
    char codeName[CODE_NAME_BUFSIZE];                                          // - not used
    unsigned char ahaDescramblerEnable;                                        // - not used
};

struct MultiTPCDecoderConf                                                      // (x, 11, 0, 0) where x - № channel(1 or 2), 1 - № structure (MultiTPCDecoderConf), 0 - subchannel(not used), 0 - all field
{
    unsigned char asdEnable;                                                    // - not used
    unsigned char dataInvAfterDescramblers;                                     // - not used
    unsigned char customAutoSearchSync;                                         // - not used
    unsigned int  frameLen;                                                     // - not used
};

struct IntelsatViterbiK7Conf                                                    // (x, 12, 0, 0) where x - № channel(1 or 2), 1 - № structure (IntelsatViterbiK7Conf), 0 - subchannel(not used), 0 - all field 
{
    unsigned int  delay;                                                        // - not used
    unsigned int  delaySubchannel;                                              // - not used
    unsigned char stc16DeinterleaverEnable;                                     // - not used
};

struct SequentialConf                                                           // (x, 13, 0, 0) where x - № channel(1 or 2), 1 - № structure (SequentialConf), 0 - subchannel(not used), 0 - all field     
{
    unsigned int  fanoPolySelect;                                               // - not used
};

struct DVBSConf                                                                 // (x, 14, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBSConf), 0 - subchannel(not used), 0 - all field 
{
    unsigned int  fSyncOn;                                                      // - not used
    unsigned int  fSyncOff;                                                     // - not used
};

struct LDPCConf                                                                 // (x, 15, 0, 0) where x - № channel(1 or 2), 1 - № structure (LDPCConf), 0 - subchannel(not used), 0 - all field          
{
    unsigned int  bypassMode;                                                   // (x, 15, 0, 1) where x - № channel(1 or 2), 1 - № structure (LDPCConf), 0 - subchannel(not used), 1 - field  bypassMode        
    unsigned char adaptiveBalanserEnable;                                       // (x, 15, 0, 2)    ...
    unsigned char correctionOff;                                                // (x, 15, 0, 3)
    unsigned int  onSync;                                                       // (x, 15, 0, 4)
    unsigned int  offSync;                                                      // (x, 15, 0, 5)
    unsigned int  errorInSyncBeforeLock;                                        // (x, 15, 0, 6)
    unsigned int  errorInSyncAfterLock;                                         // (x, 15, 0, 7)
    unsigned int  maxErrorThreshold;                                            // (x, 15, 0, 8)
};

struct PostProcessingConf                                                       // (x, 16, 0, 0) where x - № channel(1 or 2), 1 - № structure (PostProcessingConf), 0 - subchannel(not used), 0 - all field               
{
    unsigned char ssdEnable;                                                    // (x, 16, 0, 1) where x - № channel(1 or 2), 1 - № structure (PostProcessingConf), 0 - subchannel(not used), 1 - field ssdEnable
    unsigned int  ssdType;                                                      // (x, 16, 0, 2)       ...
    unsigned int  ssdPolynomial;                                                // (x, 16, 0, 3)
    unsigned char framerEnable;                                                 // (x, 16, 0, 4)
    unsigned int  framerType;                                                   // (x, 16, 0, 5)
    unsigned int  framerSyncWord;                                               // (x, 16, 0, 6)
    unsigned int  framerSyncMask;                                               // (x, 16, 0, 7)
    unsigned int  framerFrameLength;                                            // (x, 16, 0, 8)
    unsigned int  framerSyncOffset;                                             // (x, 16, 0, 9)
    unsigned int  framerSyncGroup;                                              // (x, 16, 0, 10)
    unsigned char outmuxDataInversion;                                          // (x, 16, 0, 11)
    unsigned char framerDataInversion;                                          // (x, 16, 0, 12)
    unsigned char adsEnable;                                                    // (x, 16, 0, 13)
    unsigned int  adsType;                                                      // (x, 16, 0, 14)
    unsigned int  adsPreset;                                                    // (x, 16, 0, 15)
    unsigned int  adsPolynomial;                                                // (x, 16, 0, 16)
    char outputDataMask[DATA_SPLITTER_MAX_SIZE];                                // #814    
};

struct OutMuxSubchannelConf                                                     // (x, 17, y, 0) where x - № channel(1 or 2), 1 - № structure (OutMuxSubchannelConf), 0 - subchannel(1 - 8), 0 - all field
{
    unsigned int  portionLength;                                                // (x, 17, y, 1) where x - № channel(1 or 2), 1 - № structure (OutMuxSubchannelConf), 0 - subchannel(1 - 8), 1 - field portionLength
    unsigned int  data;                                                         // (x, 17, y, 2)        ...
    unsigned char timeStampEnable;                                              // (x, 17, y, 3)
    unsigned char headerEnable;                                                 // (x, 17, y, 4)
    unsigned char syncOnMultiframe;                                             // (x, 17, y, 5)
    unsigned char footerEnable;                                                 // (x, 17, y, 6)
    unsigned int  header;                                                       // (x, 17, y, 7)
};

struct MiscChannelConf                                                          // (x, 18, 0, 0) where x - № channel(1 or 2), 1 - № structure (MiscChannelConf), 0 - subchannel(not used), 0 - all field
{
    char label[LABEL_BUFSIZE];                                                  // - not used
};


struct TDMAConf                                                                 // (x, 19, y, 0) where x - № channel(1 or 2), 1 - № structure (TDMAConf), 0 - subchannel (1 - 8), 0 - all field
{
    unsigned int    modulationType;                                             // (x, 19, y, 1) where x - № channel(1 or 2), 1 - № structure (TDMAConf), 0 - subchannel (1 - 8), 1 - field modulationType
                                                                                // =0x00-BPSK, =0x01-QPSK, =0x03-8PSK, =0x83-alt-8PSK              
    int             carrierOffset;                                              // (x, 19, y, 2)        ...
    unsigned int    symbolRate;                                                 // (x, 19, y, 3)
    unsigned int    spectralInversion;                                          // (x, 19, y, 4)
    unsigned int    terminalName;                                               // (x, 19, y, 5)
    unsigned int    outputMode;                                                 // (x, 19, y, 6)
    unsigned int    carrierLBand;                                               // (x, 19, y, 7)
};

struct TDMADecoderConf                                                          // (x, 20, 0, 0) where x - № channel(1 or 2), 1 - № structure (TDMADecoderConf), 0 - subchannel(not used), 0 - all field
{
    char codeName[CODE_NAME_BUFSIZE];                                           // (x, 20, 0, 1) where x - № channel(1 or 2), 1 - № structure (TDMADecoderConf), 0 - subchannel(not used), 1 - field codeName
    unsigned char ahaDescramblerEnable;                                         // (x, 20, 0, 2)        ...
    unsigned int  modulationType;                                               // (x, 20, 0, 3)
                                                                                // =0x00-BPSK, =0x01-QPSK, =0x03-8PSK, =0x83-alt-8PSK              
    unsigned int  headerValue;                                                  // (x, 20, 0, 4)
    unsigned int  portionLength;                                                // (x, 20, 0, 5)
    unsigned char timeStampEnable;                                              // (x, 20, 0, 6)
};

/*---------------------------------------------------------------------------*/
struct ChannelConf                                                              // (1, 0, 0, 0) where 1 - channel, 0 - all structure ChannelConf, 0 - subchannel(not used), 0 - all field
{                                                                               // (2, 0, 0, 0) where 2 - channel, 0 - all structure ChannelConf, 0 - subchannel(not used), 0 - all field
    struct TunerConf                    tuner;                                  // (x, 1, 0, 0) where x - № channel(1 or 2), 1 - № structure (TunerConf), 0 - subchannel(not used), 0 - all field
    struct AGCConf                      agc;                                    // (x, 2, 0, 0) where x - № channel(1 or 2), 1 - № structure (AGCConf), 0 - subchannel(not used), 0 - all field
    struct ADCConf                      adc;                                    // (x, 3, 0, 0) where x - № channel(1 or 2), 1 - № structure (ADCConf), 0 - subchannel(not used), 0 - all field
    struct DemodulatorSubchannelConf    demodulator[SUBCHANNELS_PER_CHANNEL];   // (x, 4, y, 0) where x - № channel(1 or 2), 1 - № structure (DemodulatorSubchannelConf), 0 - subchannel( 1 - 5 ), 0 - all field
                                                                                //      demodulator[5]
    struct HDRMSubchannelConf           hdrm[SUBCHANNELS_PER_CHANNEL];          // (x, 5, y, 0) where x - № channel(1 or 2), 1 - № structure (HDRMSubchannelConf), 0 - subchannel( 1 - 5 ), 0 - all field
                                                                                //      hdrm[5]
    struct DVBS2DemodulatorConf         dvb;                                    // (x, 6, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBS2DemodulatorConf ), 0 - subchannel(not used), 0 - all field            
    struct DVBS2DecoderConf             dvbDecoder;                             // (x, 7, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBS2DecoderConf), 0 - subchannel(not used), 0 - all field
    struct DEMConf                      dem;                                    // (x, 8, 0, 0) where x - № channel(1 or 2), 1 - № structure (DEMConf), 0 - subchannel(not used), 0 - all field    
    struct DecoderConf                  decoder;                                // (x, 9, 0, 0) where x - № channel(1 or 2), 1 - № structure (DecoderConf), 0 - subchannel(not used), 0 - all field
    struct TPCDecoderConf               tpcDecoder;                             // (x, 10, 0, 0) where x - № channel(1 or 2), 1 - № structure (TPCDecoderConf), 0 - subchannel(not used), 0 - all field
    struct MultiTPCDecoderConf          multiTPCDecoder;                        // (x, 11, 0, 0) where x - № channel(1 or 2), 1 - № structure (MultiTPCDecoderConf), 0 - subchannel(not used), 0 - all field
    struct IntelsatViterbiK7Conf        viterbiK7;                              // (x, 12, 0, 0) where x - № channel(1 or 2), 1 - № structure (IntelsatViterbiK7Conf), 0 - subchannel(not used), 0 - all field 
    struct SequentialConf               sequential;                             // (x, 13, 0, 0) where x - № channel(1 or 2), 1 - № structure (SequentialConf), 0 - subchannel(not used), 0 - all field
    struct DVBSConf                     dvbs;                                   // (x, 14, 0, 0) where x - № channel(1 or 2), 1 - № structure (DVBSConf), 0 - subchannel(not used), 0 - all field
    struct LDPCConf                     ldpc;                                   // (x, 15, 0, 0) where x - № channel(1 or 2), 1 - № structure (LDPCConf), 0 - subchannel(not used), 0 - all field
    struct PostProcessingConf           postProcessing;                         // (x, 16, 0, 0) where x - № channel(1 or 2), 1 - № structure (PostProcessingConf), 0 - subchannel(not used), 0 - all field
    struct OutMuxSubchannelConf         outMux[TDMA_SUBCHANNELS_PER_CHANNEL];   // (x, 17, y, 0) where x - № channel(1 or 2), 1 - № structure (OutMuxSubchannelConf), 0 - subchannel(1 - 8), 0 - all field
                                                                                //      outMux[8]
    struct MiscChannelConf              misc;                                   // (x, 18, 0, 0) where x - № channel(1 or 2), 1 - № structure (MiscChannelConf), 0 - subchannel(not used), 0 - all field
    struct TDMAConf                     tdma[TDMA_SUBCHANNELS_PER_CHANNEL];     // (x, 19, y, 0) where x - № channel(1 or 2), 1 - № structure (TDMAConf), 0 - subchannel (1 - 8), 0 - all field
                                                                                //      tdma[8]
    struct TDMADecoderConf              tdmaDecoder;                            // (x, 20, 0, 0) where x - № channel(1 or 2), 1 - № structure (TDMADecoderConf), 0 - subchannel(not used), 0 - all field
};
/*---------------------------------------------------------------------------*/
/*       ChannelStat - 1,0 for first channel, 2,0  for second channel        */
/*---------------------------------------------------------------------------*/

struct TunerStat                                                        // (x, 101, 0, 0) where x - № channel(1 or 2), 101 - № structure (TunerConf), 0 - subchannel(not used), 0 - all field
{
    unsigned char present;                                              // - not used
    unsigned int  referenceFrequency;                                   // - not used
    unsigned int  serialNumber;                                         // - not used
    unsigned char type;                                                 // - not used
    unsigned char temperature;                                          // - not used
    unsigned char endOfSearch;                                          // - not used
    unsigned char lock;                                                 // - not used
    unsigned char agcError;                                             // - not used
    float         referenceVoltage;                                     // - not used
    float         lBandGain;                                            // - not used
    unsigned int  baseBandGain;                                         // - not used
    float         pin;                                                  // - not used
    unsigned int  lpfCutoffFrequency;                                   // - not used
    unsigned int  maxSpectrumAnalyzerSymbolRate;                        // - not used
};


struct DVBS2Stat                                                        // (x, 102, 0, 0) where x - № channel(1 or 2), 102 - № structure DVBS2Stat (dvb), 0 - subchannel(not used), 0 - all field
{                                                                       // (x, 103, 0, 0) where x - № channel(1 or 2), 103 - № structure DVBS2Stat (tdma), 0 - subchannel(not used), 0 - all field
    unsigned int aeqStatus;                                             // - not used
    unsigned int fsmStatus;                                             // - not used 
    float        snr;                                         // double - not used 
    float        Eb_N0;                                       // double - not used 
    unsigned int carrierOffset;                                         // - not used
    unsigned int symbolRate;                                            // - not used
    unsigned int spectrumInversionStatus;                               // - not used
};

struct SUBChannelStat                                                   // (x, 104, 0, 0) where x - № channel(1 or 2), 104 - № structure (TunerConf), 0 - № subchannel(1-4), 0 - all field
{
    unsigned int fsmStatus;                                             // - not used
    float       snr;                                            // double - not used 
    unsigned int carrierOffset;                                         // - not used
    unsigned int symbolRate;                                            // - not used
    unsigned int ddcDecimationFactor;                                   // - not used
    unsigned int ddcOutputSampleRate;                                   // - not used
    unsigned int dcic;                                                  // - not used
};
    
struct DVBS2DecoderStat                                                 // (x, 105, 0, 0) where x - № channel(1 or 2), 105 - № structure (TunerConf), 0 - subchannel(not used), 0 - all field
{
    unsigned int decoderStatus;                                         // - not used
};


struct TPCDecoderStat                                                   // (x, 106, 0, 0) where x - № channel(1 or 2), 106 - № structure (TPCDecoderStat), 0 - subchannel(not used), 0 - all field
{
    unsigned char frameSync;                                            // - not used
    unsigned int  rawBER;                                               // - not used
    unsigned char spectralInversionDetected;                            // - not used
    unsigned char phaseNumber;                                          // - not used
    unsigned int  blocksInFrame;                                        // - not used
    unsigned int  frameLength;                                          // - not used
    unsigned int  turboCodeRate;                                        // - not used
    unsigned int  coreVersion;                                          // - not used
};

struct CommonDecoderStat                                                // (x, 107, 0, 0) where x - № channel(1 or 2), 107 - № structure (CommonDecoderStat), 0 - subchannel(not used), 0 - all field
{
    unsigned int regStatus;                                             // - not used
    unsigned int rsFrameLengthDetected;                                 // - not used
    unsigned int innerDecoderRawBER;                                    // - not used
    unsigned int outerDecoderRawBER;                                    // - not used
};

struct PostProcessingStat                                               // (x, 108, 0, 0) where x - № channel(1 or 2), 108 - № structure (PostProcessingStat), 0 - subchannel(not used), 0 - all field
{
    unsigned int  regFrmStat0;                                          // - not used
    unsigned int  regFrmStat1;                                          // - not used
    unsigned int  regFrmStat15;                                         // - not used
};


struct LDPCStat                                                         // (x, 109, 0, 0) where x - № channel(1 or 2), 109 - № structure (LDPCStat), 0 - subchannel(not used), 0 - all field
{
    unsigned int  maxIter;                                              // - not used
    unsigned int  iterStatus;                                           // - not used
};


struct LinkStat                                                         // (x, 110, y, 0) where x - № channel(1 or 2), 110 - № structure (LinkStat), y - № subchannel(1-5), 0 - all field
{
    unsigned int                channelSpeed;                           // - not used    
};


struct TDMAStat                                                         // (x, 111, y, 0) where x - № channel(1 or 2), 111 - № structure (TDMAStat), y - № subchannel(1-8), 0 - all field
{
    unsigned int    fsmStatus;                                          // - not used
    int             carrierOffset;                                      // - not used    
    float           ber;                                                // - not used    
    unsigned int    channelSpeed;                                       // - not used        
    unsigned int    statDDCSampleRate;                                  // - not used
    unsigned int    statDecoderTPC;                                     // - not used    
    unsigned int    statPacketDecoderMode;                              // - not used        
    unsigned int    realSymbolRate;                                     // - not used        
    unsigned int    decoderLock;
    unsigned int    decoderPER;
};
/*---------------------------------------------------------------------------*/
struct M2KChainSyncTimeStamp                   // ticket #755
{
    unsigned int TypeInChain;
    unsigned int NumberInChain;
    unsigned int StateChainSync;
};

struct ChannelStat                                                      // (1, 100, 0, 0) where 1 - channel, 0 - all structure ChannelStat, 0 - subchannel(not used), 0 - all field
{                                                                       // (2, 100, 0, 0) where 2 - channel, 0 - all structure ChannelStat, 0 - subchannel(not used), 0 - all field
    struct TunerStat            tuner;                                  // (x, 101, 0, 0) where x - № channel(1 or 2), 101 - № structure (TunerConf), 0 - subchannel(not used), 0 - all field
    struct DVBS2Stat            dvb;                                    // (x, 102, 0, 0) where x - № channel(1 or 2), 102 - № structure (DVBS2Stat dvb), 0 - subchannel(not used), 0 - all field
    struct DVBS2Stat            hdrm;                                   // (x, 103, 0, 0) where x - № channel(1 or 2), 103 - № structure (DVBS2Stat tdma), 0 - subchannel(not used), 0 - all field
    struct SUBChannelStat       hdrmMulti[SUBCHANNELS_PER_CHANNEL - 1]; // (x, 104, 0, 0) where x - № channel(1 or 2), 104 - № structure (SUBChannelStat), 0 - № subchannel(1-4), 0 - all field
                                                                        //                  hdrmMulti[4](minus main hdrm)
    struct DVBS2DecoderStat     dvbDecoder;                             // (x, 105, 0, 0) where x - № channel(1 or 2), 105 - № structure (DVBS2DecoderStat), 0 - subchannel(not used), 0 - all field
    struct TPCDecoderStat       tpcDecoder;                             // (x, 106, 0, 0) where x - № channel(1 or 2), 106 - № structure (TPCDecoderStat), 0 - subchannel(not used), 0 - all field
    struct CommonDecoderStat    commonDecoder;                          // (x, 107, 0, 0) where x - № channel(1 or 2), 107 - № structure (CommonDecoderStat), 0 - subchannel(not used), 0 - all field
    struct PostProcessingStat   postProcessing;                         // (x, 108, 0, 0) where x - № channel(1 or 2), 108 - № structure (PostProcessingStat), 0 - subchannel(not used), 0 - all field
    struct LDPCStat             ldpc;                                   // (x, 109, 0, 0) where x - № channel(1 or 2), 109 - № structure (LDPCStat), 0 - subchannel(not used), 0 - all field
    struct LinkStat             link[TDMA_SUBCHANNELS_PER_CHANNEL];          // (x, 110, y, 0) where x - № channel(1 or 2), 110 - № structure (LinkStat), y - № subchannel(1-5), 0 - all field
                                                                        //                  link[5]
    struct TDMAStat             tdma[TDMA_SUBCHANNELS_PER_CHANNEL];     // (x, 111, y, 0) where x - № channel(1 or 2), 111 - № structure (TDMAStat), y - № subchannel(1-8), 0 - all field
                                                                        //                  tdma[8]
    unsigned int                ethernetLinkStatus;                     // (x, 100, 0, 12) where x - № channel(1 or 2), 0 - structure ChannelStat, 0 - subchannel(not used), 12 - field ethernetLinkStatus
    unsigned int                totalChannelSpeed;                      // (x, 100, 0, 13) where x - № channel(1 or 2), 0 - structure ChannelStat, 0 - subchannel(not used), 13 - field totalChannelSpeed
    unsigned int                statAllChannelsAllowed;                 // (x, 100, 0, 14) where x - № channel(1 or 2), 0 - structure ChannelStat, 0 - subchannel(not used), 14 - field ethernetLinkStatus

    struct M2KChainSyncTimeStamp    m2kChain;                           // #755
};

/*---------------------------------------------------------------------------*/
/*       ChannelConnectionsInfo  -                                           */
/*---------------------------------------------------------------------------*/

struct ConnectionInfo                                                               // not used
{
    char                requestString[REQUEST_BUFSIZE];
    unsigned int        remoteIP;
    unsigned short      remotePort;
    unsigned short      localPort;
    time_t              sessionStartTime;
    unsigned int        txErrors;
    unsigned int        txSpeed;
    unsigned long long  txOctets;
};

struct SUBChannelInfo                                                               // not used
{
    struct ConnectionInfo subChannle[MAX_CONNECTIONS_PER_CHANNEL];
    unsigned int numberSession;
};

struct ChannelConnectionsInfo                                                       // (1, 200, 0, 0) where 1 - channel, 0 - all structure ChannelStat, 0 - subchannel(not used), 0 - all field
{   
    struct SUBChannelInfo ChannelInfo[TDMA_SUBCHANNELS_PER_CHANNEL];                     // (x, 201, y, 0) where x - № channel(1 or 2), 201 - № structure (SUBChannelInfo), y - № subchannel(1-5), 0 - all field
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*
struct FFTConfig {							// (x, 200, 0, 0) where x - channel, 0 - all structure ChannelStat, 0 - subchannel(not used), 0 - all field
   bool   measureSymbolRate;
   unsigned int  powerOfAveraging;
   unsigned int scale;
   unsigned int subChannel;
   unsigned int halfAuto_flag;
   unsigned int tuner_frequency;
   unsigned int tuner_filterBand;
   unsigned int measuredSymbolRate;
};

struct FFTData {
    unsigned int  frameNumber;
    unsigned int   Pin;
    unsigned int  centerFrequency;
    unsigned int  scale;                  
    unsigned int  lpfCutoffFrequency;
    unsigned int  measuredSymbolRate;
    unsigned int  coreVersion;
    unsigned int  measuredScale;
    bool   measureSymbolRate;	
    unsigned int  powerOfAveraging;	
    unsigned int  samples[256];
    unsigned int  leaderStatus;
    unsigned int  subChannel;
    unsigned int  subChannelsNumber;
    unsigned int  adc_decimationRate;          // Decimation_rate        (copy of ChannelConf.adc.decimationRate)
    unsigned int  dem_mode;   
};
*/

struct FFTConfigZMQ {																		// (1, 200, 0, 0) where 1 - channel, 0 - all structure ChannelStat, 0 - subchannel(not used), 0 - all field
   char   measureSymbolRate;
   unsigned int  powerOfAveraging;
   unsigned int scale;
   unsigned int subChannel;        // default = 0
};

struct FFTDataZMQ {
    unsigned int  frameNumber;
    unsigned int  Pin;
    unsigned int  centerFrequency;
    unsigned int  scale;                  
    unsigned int  lpfCutoffFrequency;
    unsigned int  measuredSymbolRate;
    unsigned int  coreVersion;
    unsigned int  measuredScale;
    char          measureSymbolRate;	
    unsigned int  powerOfAveraging;	
    unsigned int  samples[1024];
    unsigned int  leaderStatus;
    unsigned int  subChannel;
    unsigned int  subChannelsNumber;
};


struct ConstellationConfigZMQ {
    unsigned int contrast;
    unsigned int powerOfAveraging;
    unsigned int subChannel;
};

struct ConstellationDataZMQ {
    unsigned int frameNumber;
    unsigned int contrast;	
    unsigned int powerOfAveraging;
    unsigned int samples[16384];
    unsigned int leaderStatus;
    unsigned int subChannel;
    unsigned int subChannelsNumber;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
struct MessageHeader
{
    unsigned int payloadSize;
    unsigned int messageType;
    unsigned int channel;
    unsigned int leadingPart;    
};

/*---------------------------------------------------------------------------*/
//struct for meters

struct HTTPHeader
{
    unsigned char messageType;
    unsigned int leadingPart;
};

struct ZMQManagerHeader
{
		unsigned int IDtime;
        unsigned char Channel;
		unsigned char Nom_structure;
		unsigned char Nom_subchannel;
        unsigned char Nom_field;
		unsigned int  ProcessStatus;
};

struct ZMQServerManager
{
        unsigned char MessageType;
        unsigned char Channel;
		unsigned char Nom_structure;
		unsigned char Nom_subchannel;
        unsigned char Nom_field;
		unsigned int  ProcessStatus;
};

/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
} // extern "C"
#endif

#include "Connection/DataConversion.hpp"
#include "Common/ByteOperations.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Types.h"
#include "Poco/String.h"
/*---------------------------------------------------------------------------*/
#include <arpa/inet.h>
/*---------------------------------------------------------------------------*/

// Defines for AGC Lock status {1.7}
#define STAT_AGC_UNLOCK                 0x00
#define STAT_AGC_LOCK                   0x01
#define STAT_AGC_MAUAL_GAIN_CONTROL     0x02

/*---------------------------------------------------------------------------*/

using Poco::UInt8;
using Poco::UInt16;

using Poco::DateTimeParser;
using Poco::DateTimeFormatter;

using Poco::trim;

using std::string;

// #include "monitord_io.h"
#include "monitord_def.h"
/*---------------------------------------------------------------------------*/
/*
namespace
{

void convertTDMAConfigSNMP2Mon(const TDMAConf& inConf, Settings::TDMASubChannelSettings& outConf)
{
    outConf.setCarrierOffset(inConf.carrierOffset);    
    outConf.setSymbolRate(inConf.symbolRate);
    outConf.setModulationType(inConf.modulationType);
    outConf.setSpectralInversion(inConf.spectralInversion);
    outConf.setTerminalIndex(inConf.terminalName);
    outConf.setOutputMode(inConf.outputMode);
    outConf.setCarrierLBand(inConf.carrierLBand);
}

void convertTDMAConfigMon2SNMP(const Settings::TDMASubChannelSettings& inConf, TDMAConf& outConf)
{
    outConf.modulationType = inConf.getModulationType();
    outConf.carrierOffset = inConf.getCarrierOffset();
    outConf.symbolRate = inConf.getSymbolRate();
    outConf.spectralInversion = inConf.getSpectralInversion();
    outConf.terminalName = inConf.getTerminalIndex();
    outConf.outputMode = inConf.getOutputMode();
    outConf.carrierLBand = inConf.getCarrierLBand();
}

void convertTDMAStatMon2Snmp(const Statistics::TDMAStatistics& inStat, TDMAStat& outStat)
{
    outStat.fsmStatus = inStat.getDemodulatorState();
    outStat.carrierOffset = inStat.getCarrierOffset();
    outStat.ber = inStat.getBer() * 1000000000;                 // BER is multiplied because we can't transfer floats via SNMP;     
    outStat.channelSpeed = inStat.getSubChannelSpeed();
    outStat.statDDCSampleRate = inStat.getDDCSampleRate();
    outStat.statDecoderTPC = inStat.getTDMStatus();
    outStat.statPacketDecoderMode = inStat.getPacketDecoderMode();
    outStat.realSymbolRate = inStat.getSubChannelRealSpeed();
    outStat.decoderLock = inStat.getDecoderLock();
    outStat.decoderPER = inStat.getDecoderPER();
}

void printTDMAConfSNMP(Poco::Logger& logger, const TDMAConf& conf, unsigned int subchannel)
{
    poco_information_f1(logger, "Demodulator (subchannel #%u):", subchannel);
    poco_information_f1(logger, "    Terminal Name:                     %u", conf.terminalName);
    poco_information_f1(logger, "    Symbol rate:                       %u", conf.symbolRate);
    poco_information_f1(logger, "    Spectral inversion:                %b", static_cast<bool>(conf.spectralInversion));
    poco_information_f1(logger, "    Modulation Type:                   %u", conf.modulationType);
    poco_information_f1(logger, "    Output Mode:                       %u", conf.outputMode);
    poco_information_f1(logger, "    Carrier Offset:                    %u", conf.carrierOffset);
    poco_information_f1(logger, "    Carrier LBand:                     %u", conf.carrierLBand);
}

void convertTunerStatMon2SNMP(const Statistics::TunerStatistics& inStat, TunerStat& outStat)
{
    outStat.present             = static_cast<unsigned char>(inStat.getPresence());
    outStat.referenceFrequency  = inStat.getReferenceFrequency();
    outStat.serialNumber        = inStat.getSerialNumber();
    outStat.type                = inStat.getType();
    outStat.temperature         = inStat.getTemperature();
    outStat.endOfSearch         = static_cast<unsigned char>(inStat.getAGCEndOfSearch());

    outStat.lock = STAT_AGC_UNLOCK;
    if(inStat.getAGCLock()) {
        outStat.lock = STAT_AGC_LOCK;
    }
    if(inStat.getAGCManualGainControl()) {
        outStat.lock = STAT_AGC_MAUAL_GAIN_CONTROL;
    }

    outStat.agcError                      = inStat.getAGCError();
    outStat.referenceVoltage              = inStat.getAGCReferenceVoltage();
    outStat.lBandGain                     = inStat.getAGCLBandGain();
    outStat.baseBandGain                  = inStat.getAGCBaseBandGain();
    outStat.pin                           = inStat.getAGCInputSignalLevel();
    outStat.lpfCutoffFrequency            = inStat.getLPFCutoffFrequency();
    outStat.maxSpectrumAnalyzerSymbolRate = inStat.getMaxSpectrumAnalyzerSymbolRate();
}

void convertDVBS2StatMon2SNMP(const Statistics::DVBS2Statistics& inStat, DVBS2Stat& outStat)
{
    outStat.aeqStatus               = inStat.getAEQStatus();
    outStat.fsmStatus               = inStat.getFSMStatus();
    outStat.snr                     = static_cast<float>(inStat.getSNR()  * 10); // SNR is multiplied because we can't transfer floats via SNMP
    outStat.Eb_N0                   = static_cast<float>(inStat.getEbN0() * 10); // Eb/N0 is multiplied because we can't transfer floats via SNMP
    outStat.carrierOffset           = inStat.getCarrierOffset();
    outStat.symbolRate              = inStat.getSymbolRate();
    outStat.spectrumInversionStatus = inStat.getSpectrumInversionStatus();
}

void convertDVBS2DecoderStatMon2SNMP(const Statistics::DVBS2DecoderStatistics& inStat, DVBS2DecoderStat& outStat)
{
    outStat.decoderStatus = inStat.getDecoderStatus();
}

void convertTPCStatMon2SNMP(const Statistics::TPCStatistics& inStat, TPCDecoderStat& outStat)
{
    outStat.frameSync                   = inStat.getFrameSync();                    // [6.2]
    outStat.rawBER                      = inStat.getRawBER();                       // [6.3]
    outStat.spectralInversionDetected   = inStat.getSpectralInversionDetected();    // [6.4]
    outStat.phaseNumber                 = inStat.getPhaseNumber();                  // [6.5]
    outStat.blocksInFrame               = inStat.getBlocksInFrame();                // [6.6]
    outStat.frameLength                 = inStat.getFrameLength();                  // [6.7]
    outStat.turboCodeRate               = inStat.getTurboCodeRate();                // [6.8]
    outStat.coreVersion                 = inStat.getCoreVersion();                  // [6.9]
}

void convertCommonDecoderStatMon2SNMP(const Statistics::CommonDecoderStatistics& inStat, CommonDecoderStat& outStat)
{
    outStat.regStatus               = inStat.getStatus();                           // [7.1]
    outStat.rsFrameLengthDetected   = inStat.getRSFrameLengthDetected();            // [7.2]
    outStat.innerDecoderRawBER      = inStat.getInnerRawBER();                      // [7.3]
    outStat.outerDecoderRawBER      = inStat.getOuterRawBER();                      // [7.4]
}

void convertLDPCStatMon2SNMP(const Statistics::LDPCStatistics& inStat, LDPCStat& outStat)
{
    outStat.maxIter     = inStat.getMaxIter();                                      // [10.1]
    outStat.iterStatus  = inStat.getIterStatus();                                   // [10.3]
}

// ticket #755
void convertM2KChainMon2SNMP(const Statistics::M2KStatistics& inStat, M2KChainSyncTimeStamp& outStat) // Ticket #755
{
    outStat.TypeInChain     = inStat.getTypeInChain();
    outStat.NumberInChain   = inStat.getNumberInChain();
    outStat.StateChainSync  = inStat.getStateChainSync();
}

void convertPostProcessingStatMon2SNMP(const Statistics::PostProcessingStatistics& inStat, PostProcessingStat& outStat)
{
    outStat.regFrmStat0  = inStat.getFrmStat0();                                    // [9.1]
    outStat.regFrmStat1  = inStat.getFrmStat1();                                    // [9.2]
    outStat.regFrmStat15 = inStat.getFrmStat15();                                   // [9.3]
}

void convertMultiHDRMStatMon2Snmp(const Statistics::DVBS2Statistics& inStat, SUBChannelStat& outStat)
{
    outStat.fsmStatus               = inStat.getFSMStatus();
    outStat.snr                     = static_cast<float>(inStat.getSNR()  * 10); // SNR is multiplied because we can't transfer floats via SNMP
    outStat.carrierOffset           = inStat.getCarrierOffset();
    outStat.symbolRate              = inStat.getSymbolRate();
    outStat.ddcDecimationFactor     = inStat.getDDCDecimationFactor();
    outStat.ddcOutputSampleRate     = inStat.getDDCOutputSampleRate();
    outStat.dcic                    = inStat.getDCIC();
}

void convertLinkStatMon2Snmp(const Statistics::ConnectionStatistics& inStat, LinkStat& outStat)
{
    outStat.channelSpeed = inStat.getTransmissionSpeed();
} 


void convertTunerConfigMon2SNMP(const Settings::TunerSettings& inConf, TunerConf& outConf)
{
    outConf.frequency                       = inConf.getFrequency() / 1000; // Convert Hz to kHz
    outConf.filterBand                      = inConf.getFilterBand();
    outConf.manualFilterMode                = static_cast<unsigned char>(inConf.getManualFilterMode());
    outConf.spectrumAnalyzerShift           = inConf.getSpectrumAnalyzerShift();
    outConf.spectrumAnalyzerEnable          = static_cast<unsigned char>(inConf.getSpectrumAnalyzerEnable());
    //outConf.referenceVoltage                = inConf.getReferenceVoltage();     // TODO: Tuner version needed
    outConf.maxSpectrumAnalyzerSymbolRate   = inConf.getMaxSpectrumAnalyzerSymbolRate();
}

void convertAGCConfigMon2SNMP(const Settings::AGCSettings& inConf, AGCConf& outConf)
{
    outConf.initialGain         = inConf.getInitialGain();
    outConf.mode                = inConf.getMode();
    outConf.manualLBandGain     = inConf.getManualLBandGain();
    outConf.manualBaseBandGain  = inConf.getManualBasebandGain();
}

void convertADCConfigMon2SNMP(const Settings::ADCSettings& inConf, const Settings::ResamplerSettings& inRes, ADCConf& outConf)
{
    outConf.coarseGainEnabled   = static_cast<unsigned char>(inConf.getCoarseGainEnabled());
    outConf.decimationRate      = inConf.getDecimationRate();
    outConf.lowLatencyEnabled   = static_cast<unsigned char>(inConf.getLowLatencyEnabled());
    outConf.outputband          = inRes.getOutputBand();
}

void convertDemodulatorConfigMon2SNMP(const Settings::DemodulatorSettings& inConf, DemodulatorSubchannelConf& outConf)
{
    outConf.symbolRate              = inConf.getSymbolRate();
    outConf.spectralInversion       = static_cast<unsigned char>(inConf.getSpectralInversion());
    outConf.rrcAlpha                = inConf.getRrcAlpha();
    outConf.APSKRROI                = static_cast<unsigned int>(inConf.getAPSKRROI());
    outConf.APSKRRMI                = static_cast<unsigned int>(inConf.getAPSKRRMI());
    outConf.aeqMode                 = inConf.getAeqMode();
    outConf.searchRange             = inConf.getSearchRange();
    outConf.mode                    = inConf.getMode();                                                 // {5.2}
    outConf.ddcEnable               = inConf.getDDCEnable();
    outConf.dcic                    = inConf.getDCICOutGain();
    outConf.nonCoherent             = inConf.getNonCoherent();
}

void convertDecoderConfigMon2SNMP(const Settings::DecoderSettings& inConf, DecoderConf& outConf)
{
    outConf.mode                        = inConf.getMode();
    outConf.cutMPEGSync                 = static_cast<unsigned char>(inConf.getCutMPEGSync());
    outConf.innerCodeRate               = inConf.getInnerCodeRate();
    outConf.outerCodeRate               = inConf.getOuterCodeRate();
    outConf.dataInversion               = static_cast<unsigned char>(inConf.getDataInversion());
    outConf.outerFECDeinterleaver       = inConf.getOuterFECDeinterleaver();
    outConf.outerDecoderEnable          = static_cast<unsigned char>(inConf.getOuterDecoderEnable());
    outConf.outerFECDescramblerEnable   = static_cast<unsigned char>(inConf.getOuterFECDescramblerEnable());
    outConf.diffDecoderEnable           = static_cast<unsigned char>(inConf.getDiffDecoderEnable());
    outConf.innerFECDescramblerEnable   = static_cast<unsigned char>(inConf.getInnerFECDescramblerEnable());
    outConf.innerFECDescramblerType     = inConf.getInnerFECDescramblerType();
    outConf.autoSpectralInversionEnable = static_cast<unsigned char>(inConf.getAutoSpectralInversionEnable());
    outConf.autoAmbiguityElimination    = static_cast<unsigned char>(inConf.getAutoAmbiguityElimination());
}

void convertIntelsatViterbiK7ConfigMon2SNMP(const Settings::IntelsatViterbiK7Settings& inConf, IntelsatViterbiK7Conf& outConf)
{
    outConf.delay = inConf.getDelay();
    outConf.delaySubchannel = inConf.getDelaySubchannel();
    outConf.stc16DeinterleaverEnable = static_cast<unsigned char>(inConf.getSTC16DeinterleaverEnable());
}

void convertSequentialConfigMon2SNMP(const Settings::SequentialSettings& inConf, SequentialConf& outConf)
{
    outConf.fanoPolySelect = inConf.getFanoPolySelect();
}

void convertDVBSConfMon2SNMP(DVBSConf& outConf)
{
    outConf.fSyncOff             = 0;
    outConf.fSyncOn              = 0;
}

void convertDVBS2ConfigMon2SNMP(const Settings::DVBS2Settings& inConf, DVBS2DemodulatorConf& outConf)
{
    outConf.sampleRate              = inConf.getSampleRate();
    outConf.ragcEnable              = static_cast<unsigned char>(inConf.getRagcEnable());
    outConf.ragcInvert              = static_cast<unsigned char>(inConf.getRagcInvert());
    outConf.outputAmplitudeVCM      = inConf.getOutputAmplitudeVCM();
    outConf.outputAmplitudeQPSK     = inConf.getOutputAmplitudeQPSK();
    outConf.outputAmplitude8PSK     = inConf.getOutputAmplitude8PSK();
    outConf.outputAmplitude16APSK   = inConf.getOutputAmplitude16APSK();
    outConf.outputAmplitude32APSK   = inConf.getOutputAmplitude32APSK();
    outConf.reacqHoldoff            = static_cast<unsigned char>(inConf.getReacqHoldoff());
    outConf.reacqActivationDelay    = inConf.getReacqActivationDelay();
    outConf.reacqRestartDelay       = inConf.getReacqRestartDelay();
    outConf.falseDetEnable          = static_cast<unsigned char>(inConf.getFalseDetEnable());
    outConf.falseDetOneshot         = static_cast<unsigned char>(inConf.getFalseDetOneshot());
    outConf.falseDetThresh          = inConf.getFalseDetThresh();
    outConf.falseDetPeriod          = inConf.getFalseDetPeriod();
    outConf.aeqAutoSpeed            = inConf.getAeqAutoSpeed();
    outConf.aeqAutoModMask          = inConf.getAeqAutoModMask();
    outConf.aeq2xRate               = static_cast<unsigned char>(inConf.getAeq2xRate());
    outConf.coarseSteps             = inConf.getCoarseSteps();
    outConf.mode                    = inConf.getMode();
    outConf.dvbs2ModCod             = inConf.getDvbs2ModCod();
    outConf.dvbs2FrameType          = inConf.getDvbs2FrameType();
    outConf.dvbs2Pilots             = static_cast<unsigned char>(inConf.getDvbs2Pilots());
    outConf.dvbs2LoopBw             = inConf.getDvbs2LoopBw();
    outConf.dvbs2PLFRAMEDescrambler = static_cast<unsigned char>(inConf.getPLFrameDesc());
}

void convertDVBS2DecoderConfigMon2SNMP(const Settings::DVBS2DecoderSettings& inConf, DVBS2DecoderConf& outConf)
{
    outConf.autoIter                = static_cast<unsigned char>(inConf.getAutoIterEnabled());
    outConf.autoIterMode            = inConf.getAutoIterMode();
    outConf.iterations              = inConf.getIterations();
    outConf.disableFooter           = static_cast<unsigned char>(inConf.getFooterDisabled());
    outConf.disableDFL              = static_cast<unsigned char>(inConf.getDFLDisabled());
    outConf.disableCRCSuppression   = static_cast<unsigned char>(inConf.getCRCSuppressionDisabled());
    outConf.outputMode              = inConf.getOutputMode();
    outConf.disableAS14_15          = static_cast<unsigned char>(inConf.getAS14_15Disabled());
    outConf.isi                     = inConf.getISI();
    outConf.tframeHeader            = inConf.getTFRAMEHeader();
    outConf.outModCod               = inConf.getOutModCod();
}

void convertDEMConfigMon2SNMP(const Settings::DEMSettings& inConf, DEMConf& outConf)
{
    outConf.mode      = inConf.getMode();
    outConf.multiMode = inConf.getMultiMode();
    outConf.numberOfSubchannel = inConf.getNumberOfSubchannel();
    outConf.carrierFormatInMulti = inConf.getCarrierFormatInMulti();
    outConf.carrierFormatInTDMA = inConf.getCarrierFormatInTDMA();
}

void convertHDRMConfigMon2SNMP(const Settings::HDRMSettings& inConf, HDRMSubchannelConf& outConf)
{
    outConf.modulationType = inConf.getModulationType();
    outConf.carrierOffset  = inConf.getCarrierOffset();
    outConf.carrierLBand  = inConf.getCarrierLBand();
}

void convertTPCConfigMon2SNMP(const Settings::TPCSettings& inConf, TPCDecoderConf& outConf)
{
    memset(outConf.codeName, 0, sizeof(outConf.codeName));

    strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   // {8.1}
    outConf.ahaDescramblerEnable = inConf.getAHADescramblerEnable();                                        // {8.3}
}

void convertTDMADecoderConfigMon2SNMP(const Settings::TDMAChannelSettings& inConf, TDMADecoderConf& outConf)
{
    memset(outConf.codeName, 0, sizeof(outConf.codeName));
    strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   
    outConf.ahaDescramblerEnable = inConf.getAHADescramblerEnable();                                        
    outConf.modulationType = inConf.getModulationType();
    outConf.headerValue = inConf.getHeader();
    outConf.portionLength = inConf.getPortionLength() ;
    outConf.timeStampEnable = inConf.getTimestampEnable();
}

void convertTDMADecoderConfigSNMP2Mon(const TDMADecoderConf& inConf, Settings::TDMAChannelSettings& outConf)
{
    outConf.setCodename(trim(static_cast<string>(inConf.codeName)));                                        // {8.1}
    outConf.setAHADescramblerEnable(inConf.ahaDescramblerEnable);                                           // {8.3}
    outConf.setModulationType(inConf.modulationType);
    outConf.setHeader(inConf.headerValue);
    outConf.setPortionLength(inConf.portionLength);
    outConf.setTimestampEnable(inConf.timeStampEnable);
}


void convertMultiTPCConfigMon2SNMP(const Settings::MultiTPCSettings& inConf, MultiTPCDecoderConf& outConf)
{
    outConf.asdEnable                   = inConf.getASDEnable();                                            // {8.19}
    outConf.dataInvAfterDescramblers    = inConf.getDataInvAfterDescramblers();                             // {8.20}
    outConf.customAutoSearchSync        = inConf.getCustomAutoSearchSync();                                 // {8.25}
    outConf.frameLen                    = inConf.getFrameLen();                                             // {8.26}
}

void convertLDPCConfigMon2SNMP(const Settings::LDPCSettings& inConf, LDPCConf& outConf)
{
    outConf.bypassMode              = inConf.getBypassMode();                                               // {12.1}
    outConf.adaptiveBalanserEnable  = inConf.getAdaptiveBalancerEnable();                                   // {12.2}
    outConf.correctionOff           = inConf.getCorrectionsOff();                                           // {12.3}
    outConf.onSync                  = inConf.getOnSync();                                                   // {12.4}
    outConf.offSync                 = inConf.getOffSync();                                                  // {12.5}
    outConf.errorInSyncBeforeLock   = inConf.getErrorInSyncBeforeLock();                                    // {12.6}
    outConf.errorInSyncAfterLock    = inConf.getErrorInSyncAfterLock();                                     // {12.7}
    outConf.maxErrorThreshold       = inConf.getMaxErrThreshold();                                          // {12.8}
}

void convertPostProcessingConfigMon2SNMP(const Settings::PostProcessingSettings& inConf, PostProcessingConf& outConf)
{
    outConf.ssdEnable           = inConf.getSSDEnable();                                                    // {13.1}
    outConf.ssdType             = inConf.getSSDType();                                                      // {13.2}
    outConf.ssdPolynomial       = inConf.getSSDPolynomial();                                                // {13.3}
    outConf.framerEnable        = inConf.getFramerEnable();                                                 // {13.4}
    outConf.framerType          = inConf.getFramerType();                                                   // {13.5}
    outConf.framerSyncWord      = inConf.getFramerSyncWord();                                               // {13.6}
    outConf.framerSyncMask      = inConf.getFramerSyncMask();                                               // {13.7}
    outConf.framerFrameLength   = inConf.getFramerFrameLength();                                            // {13.8}
    outConf.framerSyncOffset    = inConf.getFramerSyncOffset();                                             // {13.9}
    outConf.framerSyncGroup     = inConf.getFramerSyncGroup();                                              // {13.10}
    outConf.outmuxDataInversion = inConf.getOutmuxDataInversion();                                          // {13.14}
    outConf.framerDataInversion = inConf.getFramerDataInversion();                                          // {13.15}
    outConf.adsEnable           = inConf.getADSEnable();                                                    // {13.17}
    outConf.adsType             = inConf.getADSType();                                                      // {13.18}
    outConf.adsPreset           = inConf.getADSPreset();                                                    // {13.19}
    outConf.adsPolynomial       = inConf.getADSPolynomial();                                                // {13.20}
    memset(outConf.outputDataMask, 0, sizeof(outConf.outputDataMask));
    strncpy(outConf.outputDataMask, inConf.getOutputDataMask().c_str(), inConf.getOutputDataMask().size());
//    Poco::Logger& _logger = Poco::Logger::get("_TRACE_Mon2SNMP");
//    poco_debug_f1(_logger, "outputDataMask : %s", std::string(inConf.getOutputDataMask().c_str()));
}

void convertOutMuxConfigMon2SNMP(const Settings::OutMuxSettings& inConf, OutMuxSubchannelConf& outConf)
{
    outConf.portionLength       = inConf.getPortionLength();                                                // {14.1}
    outConf.data                = inConf.getData();                                                         // {14.2}
    outConf.timeStampEnable     = inConf.getTimestampEnable();                                              // {14.3}
    outConf.headerEnable        = inConf.getHeaderEnable();                                                 // {14.4}
    outConf.syncOnMultiframe    = inConf.getSyncOnMultiframe();                                             // {14.5}
    outConf.footerEnable        = inConf.getFooterEnable();                                                 // {14.6}
    outConf.header              = inConf.getHeader();                                                       // {14.7}
}

void convertMiscConfigMon2SNMP(const Settings::MiscSettings& inConf, MiscChannelConf& outConf)
{
    memset(outConf.label, 0, sizeof(outConf.label));
    strncpy(outConf.label, inConf.getLabel().c_str(), inConf.getLabel().size());
}

void convertTunerConfigSNMP2Mon(const TunerConf& inConf, Settings::TunerSettings& outConf)
{
    outConf.setFrequency(inConf.frequency * 1000); // Convert kHz to Hz                                     // {1.1}
    outConf.setFilterBand(inConf.filterBand);                                                               // {1.2}
    outConf.setManualFilterMode(inConf.manualFilterMode);                                                   // {1.3}
    outConf.setSpectrumAnalyzerEnable(inConf.spectrumAnalyzerEnable);                                       // {1.5}
}

void convertAGCConfigSNMP2Mon(const AGCConf& inConf, Settings::AGCSettings& outConf)
{
    outConf.setMode(inConf.mode);                                                                           // {1.9}
    outConf.setManualLBandGain(inConf.manualLBandGain);                                                     // {1.10}
    outConf.setManualBasebandGain(inConf.manualBaseBandGain);                                               // {1.11}
}

void convertADCConfigSNMP2Mon(const ADCConf& inConf, const DEMConf& inDem, Settings::ADCSettings& outConf, Settings::ResamplerSettings& outRes)
{
    outConf.setDecimationRate(inConf.decimationRate);                                                       // {2.2}
    outRes.setStatusEnable(inConf.decimationRate, inDem.mode);
    outRes.setOutputBand(inConf.outputband);
}

void convertDemodulatorConfigSNMP2Mon(const DemodulatorSubchannelConf& inConf, Settings::DemodulatorSettings& outConf)
{
    outConf.setSymbolRate(inConf.symbolRate);                                                               // {3.3}
    outConf.setSpectralInversion(inConf.spectralInversion);                                                 // {3.4}
    outConf.setRrcAlpha(inConf.rrcAlpha);                                                                   // {3.7}
    outConf.setAPSKRROI(inConf.APSKRROI);                                                                   // {3.22}
    outConf.setAPSKRRMI(inConf.APSKRRMI);                                                                   // {3.23}
    outConf.setAeqMode(inConf.aeqMode);                                                                     // {3.24}
    outConf.setSearchRange(inConf.searchRange);                                                             // {3.28}
    outConf.setMode(inConf.mode);                                                                           // {5.2}
    outConf.setDDCEnable(inConf.ddcEnable);
    outConf.setDCIOutGain(inConf.dcic);
    outConf.setNonCoherent(inConf.nonCoherent);
}

void convertDVBS2ConfigSNMP2Mon(const DVBS2DemodulatorConf& inConf, Settings::DVBS2Settings& outConf)
{
    outConf.setMode(inConf.mode);                                                                           // {3.30}
    outConf.setDvbs2ModCod(inConf.dvbs2ModCod);                                                             // {3.31}
    outConf.setDvbs2FrameType(inConf.dvbs2FrameType);                                                       // {3.32}
    outConf.setDvbs2Pilots(inConf.dvbs2Pilots);                                                             // {3.33}
    outConf.setPLFrameDesc(inConf.dvbs2PLFRAMEDescrambler);                                                 // {3.34}
}

void convertDecoderConfigSNMP2Mon(const DecoderConf& inConf, Settings::DecoderSettings& outConf)
{
    outConf.setMode(static_cast<Settings::DecoderSettings::Mode>(inConf.mode));                             // {4.1}
    outConf.setCutMPEGSync(inConf.cutMPEGSync);                                                             // {4.9}

    outConf.setInnerCodeRate(static_cast<Settings::DecoderSettings::InnerCodeRate>(inConf.innerCodeRate));  // {7.1}
    outConf.setOuterCodeRate(inConf.outerCodeRate);                                                         // {7.2}
    outConf.setDataInversion(inConf.dataInversion);                                                         // {7.3}
    outConf.setOuterFECDeinterleaver(inConf.outerFECDeinterleaver);                                         // {7.4}
    outConf.setOuterDecoderEnable(inConf.outerDecoderEnable);                                               // {7.5}
    outConf.setOuterFECDescramblerEnable(inConf.outerFECDescramblerEnable);                                 // {7.6}
    outConf.setDiffDecoderEnable(inConf.diffDecoderEnable);                                                 // {7.7}
    outConf.setInnerFECDescramblerEnable(inConf.innerFECDescramblerEnable);                                 // {7.8}
    outConf.setInnerFECDescramblerType(inConf.innerFECDescramblerType);                                     // {7.9}
    outConf.setAutoSpectralInversionEnable(inConf.autoSpectralInversionEnable);                             // {7.10}
    outConf.setAutoAmbiguityElimination(inConf.autoAmbiguityElimination);                                   // {7.11}
}

void convertIntelsatViterbiK7ConfigSNMP2Mon(const IntelsatViterbiK7Conf& inConf, Settings::IntelsatViterbiK7Settings& outConf)
{
    outConf.setDelay(inConf.delay);
    outConf.setDelaySubchannel(inConf.delaySubchannel);
    outConf.setSTC16DeinterleaverEnable(inConf.stc16DeinterleaverEnable);
}

void convertSequentialConfigSNMP2Mon(const SequentialConf& inConf, Settings::SequentialSettings& outConf)
{
    outConf.setFanoPolySelect(inConf.fanoPolySelect);
}

void convertDVBS2DecoderConfigSNMP2Mon(const DVBS2DecoderConf& inConf, Settings::DVBS2DecoderSettings& outConf)
{
    outConf.setAutoIterEnabled(inConf.autoIter);                                                            // {4.2}
    outConf.setIterations(inConf.iterations);                                                               // {4.4}
    outConf.setFooterDisabled(inConf.disableFooter);                                                        // {4.5}
    outConf.setDFLDisabled(inConf.disableDFL);                                                              // {4.6}
    outConf.setCRCSuppressionDisabled(inConf.disableCRCSuppression);                                        // {4.7}
    outConf.setOutputMode(inConf.outputMode);                                                               // {4.8}
    outConf.setAS14_15Disabled(inConf.disableAS14_15);                                                      // {4.10}
    outConf.setISI(inConf.isi);                                                                             // {4.11}
    outConf.setTFRAMEHeader(inConf.tframeHeader);                                                           // {4.12}
    outConf.setOutModCod(inConf.outModCod);                                                                 // {4.13}
}


void convertDEMConfigSNMP2Mon(const DEMConf& inConf, Settings::DEMSettings& outConf)
{
    outConf.setMode(inConf.mode);                                                                           // {5.1}
    outConf.setMultiMode(inConf.multiMode);
    outConf.setNumberOfSubchannel(inConf.numberOfSubchannel);    
    outConf.setCarrierFormatInMulti(inConf.carrierFormatInMulti);
    outConf.setCarrierFormatInTDMA(inConf.carrierFormatInTDMA);

}

void convertHDRMConfigSNMP2Mon(const HDRMSubchannelConf& inConf, Settings::HDRMSettings& outConf)
{
    outConf.setModulationType(static_cast<Settings::ModulationType>(inConf.modulationType));                // {6.41}
    outConf.setCarrierOffset(inConf.carrierOffset);
    outConf.setCarrierLBand(inConf.carrierLBand);
}

void convertTPCConfigSNMP2Mon(const TPCDecoderConf& inConf, Settings::TPCSettings& outConf)
{
    outConf.setCodename(trim(static_cast<string>(inConf.codeName)));                                        // {8.1}
    outConf.setAHADescramblerEnable(inConf.ahaDescramblerEnable);                                           // {8.3}
}

void convertMultiTPCConfigSNMP2Mon(const MultiTPCDecoderConf& inConf, Settings::MultiTPCSettings& outConf)
{
    outConf.setASDEnable(inConf.asdEnable);                                                                 // {8.19}
    outConf.setDataInvAfterDescramblers(inConf.dataInvAfterDescramblers);                                   // {8.20}
    outConf.setCustomAutoSearchSync(inConf.customAutoSearchSync);                                           // {8.25}
    outConf.setFrameLen(inConf.frameLen);                                                                   // {8.26}
}

void convertLDPCConfigSNMP2Mon(const LDPCConf& inConf, Settings::LDPCSettings& outConf)
{
    outConf.setCorrectionsOff(inConf.correctionOff);                                                        // {12.3}
}

void convertPostProcessingConfigSNMP2Mon(const PostProcessingConf& inConf, Settings::PostProcessingSettings& outConf)
{
    outConf.setSSDEnable(inConf.ssdEnable);                                                                 // {13.1}
    outConf.setSSDType(inConf.ssdType);                                                                     // {13.2}
    outConf.setSSDPolynomial(inConf.ssdPolynomial);                                                         // {13.3}
    outConf.setFramerEnable(inConf.framerEnable);                                                           // {13.4}
    outConf.setFramerType(static_cast<Settings::PostProcessingSettings::FramerType>(inConf.framerType));    // {13.5}
    outConf.setFramerSyncWord(inConf.framerSyncWord);                                                       // {13.6}
    outConf.setFramerSyncMask(inConf.framerSyncMask);                                                       // {13.7}
    outConf.setFramerFrameLength(inConf.framerFrameLength);                                                 // {13.8}
    outConf.setFramerSyncOffset(inConf.framerSyncOffset);                                                   // {13.9}
    outConf.setFramerSyncGroup(inConf.framerSyncGroup);                                                     // {13.10}
    outConf.setOutmuxDataInversion(inConf.outmuxDataInversion);                                             // {13.14}
    outConf.setFramerDataInversion(inConf.framerDataInversion);                                             // {13.15}
    outConf.setADSEnable(inConf.adsEnable);                                                                 // {13.17}
    outConf.setADSType(inConf.adsType);                                                                     // {13.18}
    outConf.setADSPreset(inConf.adsPreset);                                                                 // {13.19}
    outConf.setADSPolynomial(inConf.adsPolynomial);                                                         // {13.20}
    outConf.setOutputDataMask(inConf.outputDataMask);
    // Poco::Logger& _logger = Poco::Logger::get("_TRACE_DataMask");
    // poco_debug_f1(_logger, "outputDataMask : %s", std::string(inConf.outputDataMask));
}

void convertOutMuxConfigSNMP2Mon(const OutMuxSubchannelConf& inConf,const DEMConf& inDem, const DecoderConf& inDec, const DVBS2DecoderConf& inDvb, Settings::OutMuxSettings& outConf)
{
    outConf.setPortionLength(inConf.portionLength);                                                         // {14.1}
    outConf.setData(static_cast<Settings::OutMuxSettings::DataType>(inConf.data));                          // {14.2}
    outConf.setTimestampEnable(inConf.timeStampEnable);                                                     // {14.3}
    outConf.setHeaderEnable(inConf.headerEnable);                                                           // {14.4}
    outConf.setSyncOnMultiframe(inConf.syncOnMultiframe, inDem.mode, inDec.mode, inDvb.outputMode);         // {14.5}
    outConf.setFooterEnable(inConf.footerEnable);                                                           // {14.6}
    outConf.setHeader(inConf.header);                                                                       // {14.7}
}

void convertMiscConfigSNMP2Mon(const MiscChannelConf& inConf, Settings::MiscSettings& outConf)
{
    outConf.setLabel(inConf.label);
}

void printTunerConf(Poco::Logger& logger, const TunerConf& conf)
{
    poco_information(logger, "Tuner:");
    poco_information_f1(logger, "    Frequency:                         %u", conf.frequency);
    poco_information_f1(logger, "    Filter band:                       %u", conf.filterBand);
    poco_information_f1(logger, "    Manual filter mode:                %b", static_cast<bool>(conf.manualFilterMode));
    poco_information_f1(logger, "    Spectrum analyzer shift:           %u", conf.spectrumAnalyzerShift);
    poco_information_f1(logger, "    Spectrum analyzer enalble:         %b", static_cast<bool>(conf.spectrumAnalyzerEnable));
    poco_information_f1(logger, "    Supply voltage:                    %f", static_cast<double>(conf.supplyVoltage));
    poco_information_f1(logger, "    Reference voltage:                 %u", conf.referenceVoltage);
    poco_information_f1(logger, "    Max spectrum analyzer symbol rate: %u", conf.maxSpectrumAnalyzerSymbolRate);
}

void printAGCConf(Poco::Logger& logger, const AGCConf& conf)
{
    poco_information(logger, "AGC:");
    poco_information_f1(logger, "    Initial gain:                      %u", conf.initialGain);
    poco_information_f1(logger, "    Mode:                              %u", conf.mode);
    poco_information_f1(logger, "    Manual LBand gain:                 %u", conf.manualLBandGain);
    poco_information_f1(logger, "    Manual BaseBand gain:              %u", conf.manualBaseBandGain);
}

void printADCConf(Poco::Logger& logger, const ADCConf& conf)
{
    poco_information(logger, "ADC:");
    poco_information_f1(logger, "    Coarse gain enabled:               %b", static_cast<bool>(conf.coarseGainEnabled));
    poco_information_f1(logger, "    Decimation rate:                   %u", conf.decimationRate);
    poco_information_f1(logger, "    Low latency enabled:               %b", static_cast<bool>(conf.lowLatencyEnabled));
}

void printDemodulatorConf(Poco::Logger& logger, const DemodulatorSubchannelConf& conf, unsigned int subchannel)
{
    poco_information_f1(logger, "Demodulator (subchannel #%u):", subchannel);
    poco_information_f1(logger, "    Symbol rate:                       %u", conf.symbolRate);
    poco_information_f1(logger, "    Spectral inversion:                %b", static_cast<bool>(conf.spectralInversion));
    poco_information_f1(logger, "    RRC Alpha:                         %u", conf.rrcAlpha);
//    poco_information_f1(logger, "    APSK RROI:                         %f", conf.APSKRROI);
//    poco_information_f1(logger, "    APSK RRMI:                         %f", conf.APSKRRMI);
    poco_information_f1(logger, "    AEQ mode:                          %u", conf.aeqMode);
    poco_information_f1(logger, "    Search range:                      %u", conf.searchRange);
    poco_information_f1(logger, "    Mode:                              %u", conf.mode);
}

void printDecoderConf(Poco::Logger& logger, const DecoderConf& conf)
{
    poco_information(logger, "Decoder:");
    poco_information_f1(logger, "    Mode:                              %u", conf.mode);
    poco_information_f1(logger, "    Cut MPEG sync:                     %b", static_cast<bool>(conf.cutMPEGSync));
    poco_information_f1(logger, "    Inner code rate:                   %u", conf.innerCodeRate);
    poco_information_f1(logger, "    Outer code rate:                   %u", conf.outerCodeRate);
    poco_information_f1(logger, "    Data inversion:                    %b", static_cast<bool>(conf.dataInversion));
    poco_information_f1(logger, "    Outer FEC deinterleaver:           %u", conf.outerFECDeinterleaver);
    poco_information_f1(logger, "    Outer decoder enable:              %b", static_cast<bool>(conf.outerDecoderEnable));
    poco_information_f1(logger, "    Outer FEC descrambler enable:      %b", static_cast<bool>(conf.outerFECDescramblerEnable));
    poco_information_f1(logger, "    Diff decoder enable:               %b", static_cast<bool>(conf.diffDecoderEnable));
    poco_information_f1(logger, "    Inner FEC descrambler enable:      %b", static_cast<bool>(conf.innerFECDescramblerEnable));
    poco_information_f1(logger, "    Inner FEC descrambler type:        %u", conf.innerFECDescramblerType);
    poco_information_f1(logger, "    Auto specrtal inversion enable:    %b", static_cast<bool>(conf.autoSpectralInversionEnable));
}

void printDVBS2DemodulatorConf(Poco::Logger& logger, const DVBS2DemodulatorConf& conf)
{
    poco_information(logger, "DVBS2 Demodulator");
    poco_information_f1(logger, "    Sample rate:                       %u", conf.sampleRate);
    poco_information_f1(logger, "    RAGC enable:                       %b", static_cast<bool>(conf.ragcEnable));
    poco_information_f1(logger, "    RAGC invert:                       %b", static_cast<bool>(conf.ragcInvert));
    poco_information_f1(logger, "    Output amplitude VCM:              %u", conf.outputAmplitudeVCM);
    poco_information_f1(logger, "    Output amplitude QPSK:             %u", conf.outputAmplitudeQPSK);
    poco_information_f1(logger, "    Output amplitude 8PSK:             %u", conf.outputAmplitude8PSK);
    poco_information_f1(logger, "    Output amplitude 16APSK:           %u", conf.outputAmplitude16APSK);
    poco_information_f1(logger, "    Output amplitude 32APSK:           %u", conf.outputAmplitude32APSK);
    poco_information_f1(logger, "    Reacq holdoff:                     %b", static_cast<bool>(conf.reacqHoldoff));
    poco_information_f1(logger, "    Reacq activation delay:            %f", conf.reacqActivationDelay);
    poco_information_f1(logger, "    Reacq restart delay:               %f", conf.reacqRestartDelay);
    poco_information_f1(logger, "    False detection enable:            %b", static_cast<bool>(conf.falseDetEnable));
    poco_information_f1(logger, "    False detection oneshot:           %b", static_cast<bool>(conf.falseDetOneshot));
    poco_information_f1(logger, "    False detection threshold:         %f", conf.falseDetThresh);
    poco_information_f1(logger, "    False detection period:            %f", conf.falseDetPeriod);
    poco_information_f1(logger, "    AEQ auto speed:                    %u", conf.aeqAutoSpeed);
    poco_information_f1(logger, "    AEQ auto mode mask:                %u", conf.aeqAutoModMask);
    poco_information_f1(logger, "    AEQ 2x rate:                       %b", static_cast<bool>(conf.aeq2xRate));
    poco_information_f1(logger, "    Coarse steps:                      %u", conf.coarseSteps);
    poco_information_f1(logger, "    Mode:                              %u", conf.mode);
    poco_information_f1(logger, "    DVBS2 ModCod:                      %u", conf.dvbs2ModCod);
    poco_information_f1(logger, "    DVBS2 Frame type:                  %u", conf.dvbs2FrameType);
    poco_information_f1(logger, "    DVBS2 Pilots:                      %b", static_cast<bool>(conf.dvbs2Pilots));
    poco_information_f1(logger, "    DVBS2 Loop bandwidth:              %f", conf.dvbs2LoopBw);
    poco_information_f1(logger, "    DVBS2 PLFRAME Descrambler :        %b", static_cast<bool>(conf.dvbs2PLFRAMEDescrambler));
}

void printDVBS2DecoderConf(Poco::Logger& logger, const DVBS2DecoderConf& conf)
{
    poco_information(logger, "DVBS2 Decoder:");
    poco_information_f1(logger, "    Auto iter:                         %b", static_cast<bool>(conf.autoIter));
    poco_information_f1(logger, "    Auto iter mode:                    %u", conf.autoIterMode);
    poco_information_f1(logger, "    Iterations:                        %u", conf.iterations);
    poco_information_f1(logger, "    Disable footer:                    %b", static_cast<bool>(conf.disableFooter));
    poco_information_f1(logger, "    Disable DFL:                       %b", static_cast<bool>(conf.disableDFL));
    poco_information_f1(logger, "    Disable CRC suppression:           %b", static_cast<bool>(conf.disableCRCSuppression));
    poco_information_f1(logger, "    Output mode:                       %u", conf.outputMode);
    poco_information_f1(logger, "    Disable AS14_15:                   %b", static_cast<bool>(conf.disableAS14_15));
    poco_information_f1(logger, "    ISI:                               %u", conf.isi);
    poco_information_f1(logger, "    TFrame Header:                     %u", conf.tframeHeader);
    poco_information_f1(logger, "    Out ModCod:                        %u", conf.outModCod);
}

void printDEMConf(Poco::Logger& logger, const DEMConf& conf)
{
    poco_information(logger, "DEM:");
    poco_information_f1(logger, "    Mode:                              %u", conf.mode);
    poco_information_f1(logger, "    Multi mode:                        %u", conf.multiMode);
}

void printMiscConf(Poco::Logger& logger, const MiscChannelConf& conf)
{
    poco_information(logger, "Misc:");
    poco_information_f1(logger, "    Label:                             %s", std::string(conf.label));
}

} // namespace
*/
/*---------------------------------------------------------------------------*/

namespace SNMP
{
/*
void convertDeviceSettingsMon2SNMP(Settings::CommonSettings& inConf, DeviceSettings& outConf)
{
    memset(outConf.deviceName, 0, sizeof(outConf.deviceName));
    strncpy(outConf.deviceName, inConf.getDeviceName().c_str(), inConf.getDeviceName().size());
}

void convertDeviceSettingsSNMP2Mon(DeviceSettings& inConf, Settings::CommonSettings& outConf)
{
    outConf.setDeviceName(inConf.deviceName);
}

void convertChannelStatMon2SNMP(Statistics::ChannelStatistics& inStat, ChannelStat& outStat)
{
    convertTunerStatMon2SNMP            (inStat.getTunerStatistics(),           outStat.tuner);
    convertDVBS2StatMon2SNMP            (inStat.getDVBS2Statistics(),           outStat.dvb);
    convertDVBS2StatMon2SNMP            (inStat.getHDRMStatistics(0),           outStat.hdrm);
    convertDVBS2DecoderStatMon2SNMP     (inStat.getDVBS2DecoderStatistics(),    outStat.dvbDecoder);
    convertTPCStatMon2SNMP              (inStat.getTPCStatistics(),             outStat.tpcDecoder);
    convertCommonDecoderStatMon2SNMP    (inStat.getCommonDecoderStatistics(),   outStat.commonDecoder);
    convertLDPCStatMon2SNMP             (inStat.getLDPCStatistics(),            outStat.ldpc);
    convertPostProcessingStatMon2SNMP   (inStat.getPostProcessingStatistics(),  outStat.postProcessing);

    for(unsigned int i = 0; i < sizeOfArray(outStat.link); ++i) {
        convertLinkStatMon2Snmp         (inStat.getConnectionStatistics(i),     outStat.link[i]);
    }

    for(unsigned int i = 0; i < sizeOfArray(outStat.hdrmMulti); ++i) {
        convertMultiHDRMStatMon2Snmp    (inStat.getHDRMStatistics(i+1),         outStat.hdrmMulti[i]);
    }

//    Poco::Logger& _logger = Poco::Logger::get("Convertor");
    for(unsigned int i = 0; i < sizeOfArray(outStat.tdma); ++i) { // TDMA_SUBCHANNELS_PER_CHANNEL
        convertTDMAStatMon2Snmp   (inStat.getTDMAStatistics(i),                   outStat.tdma[i]);       
//        poco_debug_f2(_logger, "i = %u, outStat.tdma[i].carrierOffset = %d", i, outStat.tdma[i].carrierOffset);
    }

    outStat.ethernetLinkStatus  = inStat.getConnectionStatistics(0).getEthernetLinkStatus();
    outStat.totalChannelSpeed   = inStat.getConnectionStatistics(0).getChannelTotalSpeed();
    // ticket #755
    convertM2KChainMon2SNMP(inStat.getM2KStatistics(), outStat.m2kChain); // Ticket #755
}
//-------------------------------------------------------------------------------------
void convertChannelConfigMon2SNMP(Settings::ChannelSettings& inConf, ChannelConf& outConf, unsigned char SelectStruct)
{
    if(SelectStruct==0 || SelectStruct==1) {
        convertTunerConfigMon2SNMP              (inConf.getTunerSettings(),             outConf.tuner);
    }
    if(SelectStruct==0 || SelectStruct==2) {
        convertAGCConfigMon2SNMP                (inConf.getAGCSettings(),               outConf.agc);
    }
    if(SelectStruct==0 || SelectStruct==3) {
        convertADCConfigMon2SNMP                (inConf.getADCSettings(), inConf.getResamplerSettings(),  outConf.adc);
    }
    if(SelectStruct==0 || SelectStruct==4) {
        for(unsigned int i = 0; i < sizeOfArray(outConf.demodulator); ++i) {
            convertDemodulatorConfigMon2SNMP    (inConf.getDemodulatorSettings(i),      outConf.demodulator[i]);
        }
    }
    if(SelectStruct==0 || SelectStruct==5) {
        for(unsigned int i = 0; i < sizeOfArray(outConf.hdrm); ++i) {
            convertHDRMConfigMon2SNMP           (inConf.getHDRMSettings(i),             outConf.hdrm[i]);
        }
    }
    if(SelectStruct==0 || SelectStruct==6) {
        convertDVBS2ConfigMon2SNMP              (inConf.getDVBS2Settings(),             outConf.dvb);
    }
    if(SelectStruct==0 || SelectStruct==7) {
        convertDVBS2DecoderConfigMon2SNMP       (inConf.getDVBS2DecoderSettings(),      outConf.dvbDecoder);
    }
    if(SelectStruct==0 || SelectStruct==8) {
        convertDEMConfigMon2SNMP                (inConf.getDEMSettings(),               outConf.dem);
    }
    if(SelectStruct==0 || SelectStruct==9) {
        convertDecoderConfigMon2SNMP            (inConf.getDecoderSettings(),           outConf.decoder);
    }
    if(SelectStruct==0 || SelectStruct==10) {
        convertTPCConfigMon2SNMP                (inConf.getTPCSettings(),               outConf.tpcDecoder);
    }
    if(SelectStruct==0 || SelectStruct==11) {
        convertMultiTPCConfigMon2SNMP           (inConf.getMultiTPCSettings(),          outConf.multiTPCDecoder);
    }
    if(SelectStruct==0 || SelectStruct==12) {
        convertIntelsatViterbiK7ConfigMon2SNMP  (inConf.getIntelsatViterbiK7Settings(), outConf.viterbiK7);
    }
    if(SelectStruct==0 || SelectStruct==13) {
        convertSequentialConfigMon2SNMP         (inConf.getSequentialSettings(),        outConf.sequential);
    }
    if(SelectStruct==0 || SelectStruct==15) {
        convertLDPCConfigMon2SNMP               (inConf.getLDPCSetting(),               outConf.ldpc);
    }
    if(SelectStruct==0 || SelectStruct==16) {
        convertPostProcessingConfigMon2SNMP     (inConf.getPostProcessingSettings(),    outConf.postProcessing);
    }
    if(SelectStruct==0 || SelectStruct==17) {
        for(unsigned int i = 0; i < sizeOfArray(outConf.outMux); ++i) { 
            convertOutMuxConfigMon2SNMP        (inConf.getOutMuxSettings(i),           outConf.outMux[i]);
        }
    }
    if(SelectStruct==0 || SelectStruct==18) {
        convertMiscConfigMon2SNMP               (inConf.getMiscSettings(),              outConf.misc);
    }
    if(SelectStruct==0 || SelectStruct==19) {
        for(unsigned int i = 0; i < sizeOfArray(outConf.tdma); ++i) { 
            convertTDMAConfigMon2SNMP          (inConf.getTDMASubChannelSettings(i),   outConf.tdma[i]);
        }
    }
    if(SelectStruct==0 || SelectStruct==20) {
        convertTDMADecoderConfigMon2SNMP       (inConf.getTDMAChannelSettings(),       outConf.tdmaDecoder);
    }
    
}


void convertChannelConfigSNMP2Mon(ChannelConf& inConf, Settings::ChannelSettings& outConf, unsigned char SelectStruct)
{
    if(SelectStruct==0 || SelectStruct==1) {
        convertTunerConfigSNMP2Mon              (inConf.tuner,           outConf.getTunerSettings());
    }
    if(SelectStruct==0 || SelectStruct==2){
        convertAGCConfigSNMP2Mon                (inConf.agc,             outConf.getAGCSettings());
    }
    if(SelectStruct==0 || SelectStruct==3){
        convertADCConfigSNMP2Mon                (inConf.adc, inConf.dem, outConf.getADCSettings(),  outConf.getResamplerSettings());
    }
    if(SelectStruct==0 || SelectStruct==4) {
        for(unsigned int i = 0; i < sizeOfArray(inConf.demodulator); ++i) {
            convertDemodulatorConfigSNMP2Mon    (inConf.demodulator[i], outConf.getDemodulatorSettings(i));
        }
    }
    if(SelectStruct==0 || SelectStruct==5) {
        for(unsigned int i = 0; i < sizeOfArray(inConf.hdrm); ++i) {
            convertHDRMConfigSNMP2Mon           (inConf.hdrm[i],        outConf.getHDRMSettings(i));
        }
    }
    if(SelectStruct==0 || SelectStruct==6) {
        convertDVBS2ConfigSNMP2Mon              (inConf.dvb,             outConf.getDVBS2Settings());
    }
    if(SelectStruct==0 || SelectStruct==7) {
        convertDVBS2DecoderConfigSNMP2Mon       (inConf.dvbDecoder,      outConf.getDVBS2DecoderSettings());
    }
    if(SelectStruct==0 || SelectStruct==8) {
        convertDEMConfigSNMP2Mon                (inConf.dem,             outConf.getDEMSettings());
    }
    if(SelectStruct==0 || SelectStruct==9) {
        convertDecoderConfigSNMP2Mon            (inConf.decoder,         outConf.getDecoderSettings());
    }
    if(SelectStruct==0 || SelectStruct==10) {
        convertTPCConfigSNMP2Mon                (inConf.tpcDecoder,     outConf.getTPCSettings());
    }
    if(SelectStruct==0 || SelectStruct==11) {
        convertMultiTPCConfigSNMP2Mon           (inConf.multiTPCDecoder,outConf.getMultiTPCSettings());
    }
    if(SelectStruct==0 || SelectStruct==12) {
        convertIntelsatViterbiK7ConfigSNMP2Mon  (inConf.viterbiK7,       outConf.getIntelsatViterbiK7Settings());
    }
    if(SelectStruct==0 || SelectStruct==13) {
        convertSequentialConfigSNMP2Mon         (inConf.sequential,      outConf.getSequentialSettings());
    }
    if(SelectStruct==0 || SelectStruct==15) {
        convertLDPCConfigSNMP2Mon               (inConf.ldpc,           outConf.getLDPCSetting());
    }
    if(SelectStruct==0 || SelectStruct==16) {
        convertPostProcessingConfigSNMP2Mon     (inConf.postProcessing, outConf.getPostProcessingSettings());
    }
    if(SelectStruct==0 || SelectStruct==17){
        for(unsigned int i = 0; i < sizeOfArray(inConf.outMux); ++i) {
            convertOutMuxConfigSNMP2Mon         (inConf.outMux[i], inConf.dem, inConf.decoder, inConf.dvbDecoder, outConf.getOutMuxSettings(i));
        }
    }

    if(SelectStruct==0 || SelectStruct==18){
        convertMiscConfigSNMP2Mon               (inConf.misc,           outConf.getMiscSettings());
    }

    if(SelectStruct==0 || SelectStruct==19){
        for(unsigned int i = 0; i < sizeOfArray(inConf.tdma); ++i) {
            convertTDMAConfigSNMP2Mon           (inConf.tdma[i],        outConf.getTDMASubChannelSettings(i));
        }
    }
    if(SelectStruct==0 || SelectStruct==20){
        convertTDMADecoderConfigSNMP2Mon        (inConf.tdmaDecoder, outConf.getTDMAChannelSettings());
    }
}

void convertDateTimeSNMP2Mon(DeviceSettings& inConf, Poco::DateTime& dateTime)
{
    UInt16* pInt = reinterpret_cast<UInt16*>(inConf.dateAndTime);
    UInt8* pChar = reinterpret_cast<UInt8*>(inConf.dateAndTime);

    int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;

    year = swapBytes(static_cast<UInt16>(*pInt)); // get year

    pChar += 2; // set to month
    month = *pChar; // get month

    ++pChar; // set to day
    day = *pChar;

    ++pChar; // set to hour
    hour = *pChar;

    ++pChar; // set to minutes
    minute = *pChar;

    ++pChar; // set to seconds
    second = *pChar;

    Poco::Logger& _logger = Poco::Logger::get("Convertor");
    //
    poco_debug_f2(_logger, "year  : 0x%08X (%u)", static_cast<unsigned int>(year), static_cast<unsigned int>(year));
    poco_debug_f2(_logger, "month : 0x%08X (%u)", static_cast<unsigned int>(month), static_cast<unsigned int>(month));
    poco_debug_f2(_logger, "day   : 0x%08X (%u)", static_cast<unsigned int>(day), static_cast<unsigned int>(day));
    poco_debug_f2(_logger, "hour  : 0x%08X (%u)", static_cast<unsigned int>(hour), static_cast<unsigned int>(hour));
    poco_debug_f2(_logger, "minute: 0x%08X (%u)", static_cast<unsigned int>(minute), static_cast<unsigned int>(minute));
    poco_debug_f2(_logger, "second: 0x%08X (%u)", static_cast<unsigned int>(second), static_cast<unsigned int>(second));

    if(dateTime.isValid(year, month, day, hour, minute, second)) {
        dateTime.assign(year, month, day, hour, minute, second);
    }
}

void convertDateTimeMon2SNMP(const Poco::DateTime& dateTime, DeviceSettings& outConf)
{
    UInt16* pInt = reinterpret_cast<UInt16*>(outConf.dateAndTime);
    UInt8* pChar = reinterpret_cast<UInt8*>(outConf.dateAndTime);

    *pInt = swapBytes(static_cast<UInt16>(dateTime.year()));

    pChar += 2; // set to month
    *pChar = static_cast<UInt8>(dateTime.month());

    ++pChar; // set to day
    *pChar = static_cast<UInt8>(dateTime.day());

    ++pChar; // set to hours
    *pChar = static_cast<UInt8>(dateTime.hour());

    ++pChar; // set to mintutes
    *pChar = static_cast<UInt8>(dateTime.minute());

    ++pChar; // set to seconds
    *pChar = static_cast<UInt8>(dateTime.second());

    //Poco::Logger& _logger = Poco::Logger::get("Convertor");
    //_logger.dump("Date&Time:", outConf.dateAndTime, 10);
}

void convertConnectionsInfoMon2SNMP(Statistics::ChannelStatistics& inConf, ChannelConnectionsInfo& outConf)
{
    memset(&outConf, 0, sizeof(outConf));

    for(unsigned int subchannel = 0; subchannel < sizeOfArray(outConf.ChannelInfo); ++subchannel) {
        const Statistics::ConnectionStatistics& conStat = inConf.getConnectionStatistics(subchannel);
        const ExchangeAgent::UDPStatistics& udp = conStat.getUDPStatistics();

        outConf.ChannelInfo[subchannel].numberSession = udp.size();

        for(unsigned int conn = 0; conn < udp.size(); ++conn) {
            strncpy(outConf.ChannelInfo[subchannel].subChannle[conn].requestString, udp[conn].getRequestString().c_str(), udp[conn].getRequestString().size());

            outConf.ChannelInfo[subchannel].subChannle[conn].remoteIP = ntohl(reinterpret_cast<const in_addr*>(udp[conn].getClientAddress().host().addr())->s_addr);

            outConf.ChannelInfo[subchannel].subChannle[conn].remotePort       = udp[conn].getClientPortNumber();
            outConf.ChannelInfo[subchannel].subChannle[conn].localPort        = udp[conn].getDevicePortNumber();
            outConf.ChannelInfo[subchannel].subChannle[conn].sessionStartTime = udp[conn].getSessionStartTime().epochTime();
            outConf.ChannelInfo[subchannel].subChannle[conn].txErrors         = udp[conn].getErrorsCount();
            outConf.ChannelInfo[subchannel].subChannle[conn].txSpeed          = udp[conn].getSpeed();
            outConf.ChannelInfo[subchannel].subChannle[conn].txOctets         = udp[conn].getTransmittedBytesCount();
        }
    }
}


void convertDecoderInfoMon2SNMP(const Statistics::DecoderStatistics& inConf, const Settings::TPCCodefileManager& codeFileMgr, DecoderInfo& outConf)
{
    //memset(&outConf, 0, sizeof(outConf)); // it is zeroed by caller function

    strncpy(outConf.modesList, inConf.getModes().c_str(), inConf.getModes().size());                        // [5.1]
    outConf.modesMask = inConf.getModesMask();                                                              // [5.1]

    strncpy(outConf.modulations,   inConf.getModulations().c_str(),   inConf.getModulations().size());      // [5.2]
    strncpy(outConf.innerCodeRate, inConf.getInnerCodeRate().c_str(), inConf.getInnerCodeRate().size());    // [5.3]
    strncpy(outConf.outerCodeRate, inConf.getOuterCodeRate().c_str(), inConf.getOuterCodeRate().size());    // [5.4]

    std::string codes = codeFileMgr.makeCodefileString();

    strncpy(outConf.codes, codes.c_str(), codes.size());                                                    // [6.1]
}


void printChannelConfig(Poco::Logger& logger, const ChannelConf& conf)
{
    printTunerConf(logger, conf.tuner);
//    printAGCConf(logger, conf.agc);
//    printADCConf(logger, conf.adc);
//    printDVBS2DemodulatorConf(logger, conf.dvb);
//    printDVBS2DecoderConf(logger, conf.dvbDecoder);
//    printDecoderConf(logger, conf.decoder);
//    printDEMConf(logger, conf.dem);
}


void convertDateTimeMon2ZMQ(const Poco::DateTime& dateTime, DeviceStatus& outConf)
{
    UInt16* pInt = reinterpret_cast<UInt16*>(outConf.dateAndTime);
    UInt8* pChar = reinterpret_cast<UInt8*>(outConf.dateAndTime);

    *pInt = swapBytes(static_cast<UInt16>(dateTime.year()));

    pChar += 2; // set to month
    *pChar = static_cast<UInt8>(dateTime.month());

    ++pChar; // set to day
    *pChar = static_cast<UInt8>(dateTime.day());

    ++pChar; // set to hours
    *pChar = static_cast<UInt8>(dateTime.hour());

    ++pChar; // set to mintutes
    *pChar = static_cast<UInt8>(dateTime.minute());

    ++pChar; // set to seconds
    *pChar = static_cast<UInt8>(dateTime.second());

    // Poco::Logger& _logger = Poco::Logger::get("Convertor");
    // _logger.dump("Date&Time:", outConf.dateAndTime, 10);
}
*/


//====================================================================================================
//========================          ZMQ             ==================================================
//====================================================================================================


/*
unsigned int convertTunerConfigMon2ZMQ(const TunerConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
            poco_debug_f1(_logger, "sizeof(outVoid)= %u", sizeof(outVoid));
            poco_debug_f1(_logger, "outConf.frequency= %u", outConf.frequency);
        memcpy(outVoid, &outConf.frequency, sizeof(outVoid)); 
            // memcpy(&outConf.frequency, outVoid, sizeof(outVoid));
            //poco_debug_f1(_logger, "outVoid= %u", static_cast<unsigned int> (outConf.frequency));
        break;
    case 2:
        memcpy(outVoid, &outConf.filterBand, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.manualFilterMode, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.spectrumAnalyzerShift, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.spectrumAnalyzerEnable , sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.maxSpectrumAnalyzerSymbolRate, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertAGCConfigMon2ZMQ(const AGCConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.initialGain, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.mode, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.manualLBandGain, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.manualBaseBandGain, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}
*/
/*
unsigned int convertADCConfigMon2ZMQ(const ADCConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.coarseGainEnabled, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.decimationRate, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.lowLatencyEnabled, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDemodulatorConfigMon2ZMQ(const DemodulatorSubchannelConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.symbolRate, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.spectralInversion, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.rrcAlpha, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.APSKRROI, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.APSKRRMI, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.aeqMode, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.searchRange, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.mode, sizeof(outVoid)); 
        break;
    case 9:
        memcpy(outVoid, &outConf.ddcEnable, sizeof(outVoid)); 
        break;
    case 10:
        memcpy(outVoid, &outConf.dcic, sizeof(outVoid)); 
        break;
    case 11:
        memcpy(outVoid, &outConf.nonCoherent, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDecoderConfigMon2ZMQ(const DecoderConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.mode, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.cutMPEGSync, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.innerCodeRate, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.outerCodeRate, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.dataInversion, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.outerFECDeinterleaver, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.outerDecoderEnable, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.outerFECDescramblerEnable, sizeof(outVoid)); 
        break;
    case 9:
        memcpy(outVoid, &outConf.diffDecoderEnable, sizeof(outVoid)); 
        break;
    case 10:
        memcpy(outVoid, &outConf.innerFECDescramblerEnable, sizeof(outVoid)); 
        break;
    case 12:
        memcpy(outVoid, &outConf.autoSpectralInversionEnable, sizeof(outVoid)); 
        break;
    case 13:
        memcpy(outVoid, &outConf.autoAmbiguityElimination, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertIntelsatViterbiK7ConfigMon2ZMQ(const IntelsatViterbiK7Conf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.delay, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.delaySubchannel, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.stc16DeinterleaverEnable, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertSequentialConfigMon2ZMQ(const SequentialConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.fanoPolySelect, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDVBSConfMon2ZMQ(const DVBSConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.fSyncOn, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.fSyncOff, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDVBS2ConfigMon2ZMQ(const DVBS2DemodulatorConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.sampleRate, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.ragcEnable, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.ragcInvert, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.outputAmplitudeVCM, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.outputAmplitudeQPSK, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.outputAmplitude8PSK, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.outputAmplitude16APSK, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.outputAmplitude32APSK, sizeof(outVoid)); 
        break;
    case 9:
        memcpy(outVoid, &outConf.reacqHoldoff, sizeof(outVoid)); 
        break;
    case 10:
        memcpy(outVoid, &outConf.reacqActivationDelay, sizeof(outVoid)); 
        break;
    case 12:
        memcpy(outVoid, &outConf.reacqRestartDelay, sizeof(outVoid)); 
        break;
    case 13:
        memcpy(outVoid, &outConf.falseDetEnable, sizeof(outVoid)); 
        break;
    case 14:
        memcpy(outVoid, &outConf.falseDetOneshot, sizeof(outVoid)); 
        break;
    case 15:
        memcpy(outVoid, &outConf.falseDetThresh, sizeof(outVoid)); 
        break;
    case 16:
        memcpy(outVoid, &outConf.aeqAutoSpeed, sizeof(outVoid)); 
        break;
    case 17:
        memcpy(outVoid, &outConf.aeqAutoModMask, sizeof(outVoid)); 
        break;
    case 18:
        memcpy(outVoid, &outConf.aeq2xRate, sizeof(outVoid)); 
        break;
    case 19:
        memcpy(outVoid, &outConf.coarseSteps, sizeof(outVoid)); 
        break;
    case 20:
        memcpy(outVoid, &outConf.mode, sizeof(outVoid)); 
        break;
    case 21:
        memcpy(outVoid, &outConf.dvbs2ModCod, sizeof(outVoid)); 
        break;
    case 22:
        memcpy(outVoid, &outConf.dvbs2FrameType, sizeof(outVoid)); 
        break;
    case 23:
        memcpy(outVoid, &outConf.dvbs2Pilots, sizeof(outVoid)); 
        break;
    case 24:
        memcpy(outVoid, &outConf.dvbs2LoopBw, sizeof(outVoid)); 
        break;
    case 25:
        memcpy(outVoid, &outConf.dvbs2PLFRAMEDescrambler, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDVBS2DecoderConfigMon2ZMQ(const DVBS2DecoderConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.autoIter, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.autoIterMode, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.iterations, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.disableFooter, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.disableDFL, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.disableCRCSuppression, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.outputMode, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.disableAS14_15, sizeof(outVoid)); 
        break;
    case 9:
        memcpy(outVoid, &outConf.isi, sizeof(outVoid)); 
        break;
    case 10:
        memcpy(outVoid, &outConf.tframeHeader, sizeof(outVoid)); 
        break;
    case 11:
        memcpy(outVoid, &outConf.outModCod, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertDEMConfigMon2ZMQ(const DEMConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
//    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.mode, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.multiMode, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.numberOfSubchannel, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.carrierFormatInMulti, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.carrierFormatInTDMA, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;

}

unsigned int convertHDRMConfigMon2ZMQ(const HDRMSubchannelConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.modulationType, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.carrierOffset, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.carrierLBand, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertTPCConfigMon2ZMQ(const TPCDecoderConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    // Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        // strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   // {8.1}
        memcpy(outVoid, &outConf.codeName, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.ahaDescramblerEnable, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertTDMADecoderConfigMon2ZMQ(const TDMADecoderConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    // Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        // strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   // {8.1}
        memcpy(outVoid, &outConf.codeName, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.ahaDescramblerEnable, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.modulationType, sizeof(outVoid));
        break;
    case 4:
        memcpy(outVoid, &outConf.headerValue, sizeof(outVoid));
        break;
    case 5:
        memcpy(outVoid, &outConf.portionLength, sizeof(outVoid));
        break;
    case 6:
        memcpy(outVoid, &outConf.timeStampEnable, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertMultiTPCConfigMon2ZMQ(const MultiTPCDecoderConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    // Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.asdEnable, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.dataInvAfterDescramblers, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.customAutoSearchSync, sizeof(outVoid));
        break;
    case 4:
        memcpy(outVoid, &outConf.frameLen, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertLDPCConfigMon2ZMQ(const LDPCConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.bypassMode, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.adaptiveBalanserEnable, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.correctionOff, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.onSync, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.offSync, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.errorInSyncBeforeLock, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.errorInSyncAfterLock, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.maxErrorThreshold, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;

}

unsigned int convertPostProcessingConfigMon2ZMQ(const PostProcessingConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.ssdEnable, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.ssdType, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.ssdPolynomial, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.framerEnable, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.framerType, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.framerSyncWord, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.framerSyncMask, sizeof(outVoid)); 
        break;
    case 8:
        memcpy(outVoid, &outConf.framerFrameLength, sizeof(outVoid)); 
        break;
    case 9:
        memcpy(outVoid, &outConf.framerSyncOffset, sizeof(outVoid)); 
        break;
    case 10:
        memcpy(outVoid, &outConf.framerSyncGroup, sizeof(outVoid)); 
        break;
    case 11:
        memcpy(outVoid, &outConf.framerDataInversion, sizeof(outVoid)); 
        break;
    case 12:
        memcpy(outVoid, &outConf.adsEnable, sizeof(outVoid)); 
        break;
    case 13:
        memcpy(outVoid, &outConf.adsType, sizeof(outVoid)); 
        break;
    case 14:
        memcpy(outVoid, &outConf.adsPreset, sizeof(outVoid)); 
        break;
    case 15:
        memcpy(outVoid, &outConf.adsPolynomial, sizeof(outVoid)); 
        break;
    case 16:
        memcpy(outVoid, &outConf.outputDataMask, sizeof(outVoid)); // #814
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertTDMAConfigMon2ZMQ(const TDMAConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.modulationType, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.carrierOffset, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.symbolRate, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.spectralInversion, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.terminalName, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.outputMode, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.carrierLBand, sizeof(outVoid));
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertOutMuxConfigMon2ZMQ(const OutMuxSubchannelConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);;
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(outVoid, &outConf.portionLength, sizeof(outVoid)); 
        break;
    case 2:
        memcpy(outVoid, &outConf.data, sizeof(outVoid));
        break;
    case 3:
        memcpy(outVoid, &outConf.timeStampEnable, sizeof(outVoid)); 
        break;
    case 4:
        memcpy(outVoid, &outConf.headerEnable, sizeof(outVoid)); 
        break;
    case 5:
        memcpy(outVoid, &outConf.syncOnMultiframe, sizeof(outVoid)); 
        break;
    case 6:
        memcpy(outVoid, &outConf.footerEnable, sizeof(outVoid)); 
        break;
    case 7:
        memcpy(outVoid, &outConf.header, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

unsigned int convertMiscConfigMon2ZMQ(const MiscChannelConf& outConf, unsigned char Field, void *outVoid)
{
    unsigned int out_len = sizeof(outVoid);
    //Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        // strncpy(outConf.label, inConf.getLabel().c_str(), inConf.getLabel().size());
        memcpy(outVoid, &outConf.label, sizeof(outVoid)); 
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;

}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
unsigned int convertChannelConfigMon2ZMQ(Settings::ChannelSettings& inConf, ChannelConf& outConf, unsigned char SelectStruct, unsigned char Subchannel, unsigned char Field, void *outVoid)
{
    unsigned int out_len = 0;
    unsigned int countByte = 0;
    Poco::Logger& _logger = Poco::Logger::get("Convertor");
    switch (SelectStruct) {
    case 1 :
        convertTunerConfigMon2SNMP              (inConf.getTunerSettings(),             outConf.tuner);
        if(Field==0){
            memcpy(outVoid, &outConf.tuner, sizeof(outConf.tuner)); 
            out_len = sizeof(outConf.tuner);
        } else {
            out_len = convertTunerConfigMon2ZMQ              (outConf.tuner, Field, outVoid);  
        }
        break;
    case 2 :
        convertAGCConfigMon2SNMP                (inConf.getAGCSettings(),               outConf.agc);
        if(Field==0){
            memcpy(outVoid, &outConf.agc, sizeof(outConf.agc)); 
            out_len = sizeof(outConf.agc);
        } else {
            out_len = convertAGCConfigMon2ZMQ              (outConf.agc, Field, outVoid);  
        }
        break;
    case 3 :
        convertADCConfigMon2SNMP                (inConf.getADCSettings(),     inConf.getResamplerSettings(),       outConf.adc);
        if(Field==0){
            memcpy(outVoid, &outConf.adc, sizeof(outConf.adc)); 
            out_len = sizeof(outConf.adc);
        } else {
            out_len = convertADCConfigMon2ZMQ              (outConf.adc, Field, outVoid);  
        }
        break;
    case 4 :
        // struct DemodulatorSubchannelConf    demodulator[SUBCHANNELS_PER_CHANNEL];   
        // (x, 4, y, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DemodulatorSubchannelConf), 0 - subchannel( 1 - 5 ), 0 - all field
         
        for(unsigned int i = 0; i < sizeOfArray(outConf.demodulator); ++i) {
            convertDemodulatorConfigMon2SNMP    (inConf.getDemodulatorSettings(i),      outConf.demodulator[i]);
            if(Subchannel == 0) { 
                memcpy((char *)outVoid + countByte, &outConf.demodulator[i], sizeof(outConf.demodulator[i])); 
                countByte = countByte + sizeof(outConf.demodulator[i]);
            }
            if(Subchannel ==(i+1)) {
                if(Field==0){
                    memcpy(outVoid, &outConf.demodulator[i], sizeof(outConf.demodulator[i])); 
                    countByte = sizeof(outConf.demodulator[i]);
                } else {
                    countByte = convertDemodulatorConfigMon2ZMQ              (outConf.demodulator[i], Field, outVoid);  
                }
            }
        }
        out_len = countByte;
        break;
    case 5 :
        // struct HDRMSubchannelConf           hdrm[SUBCHANNELS_PER_CHANNEL];          
        // (x, 5, y, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (HDRMSubchannelConf), 0 - subchannel( 1 - 5 ), 0 - all field

        for(unsigned int i = 0; i < sizeOfArray(outConf.hdrm); ++i) {
            convertHDRMConfigMon2SNMP           (inConf.getHDRMSettings(i),             outConf.hdrm[i]);
            if(Subchannel == 0) { 
                poco_debug_f2(_logger, "countByte : %u, sizeof(outConf.hdrm[i]) : %u", static_cast<unsigned int>(countByte), static_cast<unsigned int>(sizeof(outConf.hdrm[i])));
                memcpy((char *)outVoid + countByte, &outConf.hdrm[i], sizeof(outConf.hdrm[i])); 
                countByte = countByte + sizeof(outConf.hdrm[i]);
            }
            if(Subchannel ==(i+1)) {
                if(Field==0){
                    memcpy((char *) outVoid, &outConf.hdrm[i], sizeof(outConf.hdrm[i])); 
                    countByte = sizeof(outConf.hdrm[i]);
                } else {
                    countByte = convertHDRMConfigMon2ZMQ              (outConf.hdrm[i], Field, outVoid);  
                }
            }
        }
        out_len = countByte;
        // poco_debug_f1(_logger, "OK !!! out_len : %u", static_cast<unsigned int>(out_len));
        break;
    case 6 :
        // struct DVBS2DemodulatorConf         dvb;                                    
        // (x, 6, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DVBS2DemodulatorConf ), 0 - subchannel(not used), 0 - all field            
        convertDVBS2ConfigMon2SNMP              (inConf.getDVBS2Settings(),             outConf.dvb);
        if(Field==0){
            memcpy(outVoid, &outConf.dvb, sizeof(outConf)); 
            out_len = sizeof(outConf.dvb);
        } else {
            out_len = convertDVBS2ConfigMon2ZMQ              (outConf.dvb, Field, outVoid);  
        }
        break;
    case 7 :
        // struct DVBS2DecoderConf             dvbDecoder;                             
        // (x, 7, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DVBS2DecoderConf), 0 - subchannel(not used), 0 - all field
        convertDVBS2DecoderConfigMon2SNMP       (inConf.getDVBS2DecoderSettings(),      outConf.dvbDecoder);
        if(Field==0){
            memcpy(outVoid, &outConf.dvbDecoder, sizeof(outConf.dvbDecoder)); 
            out_len = sizeof(outConf.dvbDecoder);
        } else {
            out_len = convertDVBS2DecoderConfigMon2ZMQ              (outConf.dvbDecoder, Field, outVoid);  
        }
        break;
    case 8 :
        // struct DEMConf                      dem;                                    
        // (x, 8, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DEMConf), 0 - subchannel(not used), 0 - all field    
        convertDEMConfigMon2SNMP                (inConf.getDEMSettings(),               outConf.dem);
        if(Field==0){
            memcpy(outVoid, &outConf.dem, sizeof(outConf.dem)); 
            out_len = sizeof(outConf.dem);
        } else {
            out_len = convertDEMConfigMon2ZMQ              (outConf.dem, Field, outVoid);  
        }
        break;
    case 9 :
        // struct DecoderConf                  decoder;                                
        // (x, 9, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DecoderConf), 0 - subchannel(not used), 0 - all field
        convertDecoderConfigMon2SNMP            (inConf.getDecoderSettings(),           outConf.decoder);
        if(Field==0){
            memcpy(outVoid, &outConf.decoder, sizeof(outConf.decoder)); 
            out_len = sizeof(outConf.decoder);
        } else {
            out_len = convertDecoderConfigMon2ZMQ              (outConf.decoder, Field, outVoid);  
        }
        break;
    case 10 :
        // struct TPCDecoderConf               tpcDecoder;                             
        // (x, 10, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (TPCDecoderConf), 0 - subchannel(not used), 0 - all field
        convertTPCConfigMon2SNMP                (inConf.getTPCSettings(),               outConf.tpcDecoder);
        if(Field==0){
            memcpy(outVoid, &outConf.tpcDecoder, sizeof(outConf.tpcDecoder)); 
            out_len = sizeof(outConf.tpcDecoder);
        } else {
            out_len = convertTPCConfigMon2ZMQ              (outConf.tpcDecoder, Field, outVoid);  
        }
        break;
    case 11 :
        // struct MultiTPCDecoderConf          multiTPCDecoder;                        
        // (x, 11, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (MultiTPCDecoderConf), 0 - subchannel(not used), 0 - all field
        convertMultiTPCConfigMon2SNMP           (inConf.getMultiTPCSettings(),          outConf.multiTPCDecoder);
        if(Field==0){
            memcpy(outVoid, &outConf.multiTPCDecoder, sizeof(outConf.multiTPCDecoder)); 
            out_len = sizeof(outConf.multiTPCDecoder);
        } else {
            out_len = convertMultiTPCConfigMon2ZMQ              (outConf.multiTPCDecoder, Field, outVoid);  
        }
        break;
    case 12 :
        // struct IntelsatViterbiK7Conf        viterbiK7;                              
        // (x, 12, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (IntelsatViterbiK7Conf), 0 - subchannel(not used), 0 - all field 
        convertIntelsatViterbiK7ConfigMon2SNMP  (inConf.getIntelsatViterbiK7Settings(), outConf.viterbiK7);
        if(Field==0){
            memcpy(outVoid, &outConf.viterbiK7, sizeof(outConf.viterbiK7)); 
            out_len = sizeof(outConf.viterbiK7);
        } else {
            out_len = convertIntelsatViterbiK7ConfigMon2ZMQ              (outConf.viterbiK7, Field, outVoid);  
        }
        break;
    case 13 :
        // struct SequentialConf               sequential;                             
        // (x, 13, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (SequentialConf), 0 - subchannel(not used), 0 - all field
        convertSequentialConfigMon2SNMP         (inConf.getSequentialSettings(),        outConf.sequential);
        if(Field==0){
            memcpy(outVoid, &outConf.sequential, sizeof(outConf.sequential)); 
            out_len = sizeof(outConf.sequential);
        } else {
            out_len = convertSequentialConfigMon2ZMQ              (outConf.sequential, Field, outVoid);  
        }
        break;
    case 14 :
        // struct DVBSConf                     dvbs;                                   
        // (x, 14, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (DVBSConf), 0 - subchannel(not used), 0 - all field
        convertDVBSConfMon2SNMP   (outConf.dvbs);
        if(Field==0){
            out_len = sizeof(outConf.dvbs);
            memcpy(outVoid, &outConf.dvbs, out_len); 
        } else {
            out_len = convertDVBSConfMon2ZMQ  (outConf.dvbs, Field, outVoid);  
        }
        break;
    case 15 :
        // struct LDPCConf                     ldpc;                                   
        // (x, 15, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (LDPCConf), 0 - subchannel(not used), 0 - all field
        convertLDPCConfigMon2SNMP               (inConf.getLDPCSetting(),               outConf.ldpc);
        if(Field==0){
            memcpy(outVoid, &outConf.ldpc, sizeof(outConf.ldpc)); 
            out_len = sizeof(outConf.ldpc);
        } else {
            out_len = convertLDPCConfigMon2ZMQ              (outConf.ldpc, Field, outVoid);  
        }
        break;
    case 16 :
        // struct PostProcessingConf           postProcessing;                         
        // (x, 16, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (PostProcessingConf), 0 - subchannel(not used), 0 - all field
        convertPostProcessingConfigMon2SNMP     (inConf.getPostProcessingSettings(),    outConf.postProcessing);
        if(Field==0){
            memcpy(outVoid, &outConf.postProcessing, sizeof(outConf.postProcessing)); 
            out_len = sizeof(outConf.postProcessing);
        } else {
            out_len = convertPostProcessingConfigMon2ZMQ              (outConf.postProcessing, Field, outVoid);  
        }
        break;
    case 17 :
        // struct OutMuxSubchannelConf         outMux[TDMA_SUBCHANNELS_PER_CHANNEL];   
        // (x, 17, y, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (OutMuxSubchannelConf), 0 - subchannel(1 - 8), 0 - all field
        for(unsigned int i = 0; i < sizeOfArray(outConf.outMux); ++i) {
            convertOutMuxConfigMon2SNMP        (inConf.getOutMuxSettings(i),           outConf.outMux[i]);
            if(Subchannel == 0) { 
                memcpy((char *)outVoid + countByte, &outConf.outMux[i], sizeof(outConf.outMux[i])); 
                countByte = countByte + sizeof(outConf.outMux[i]);
            }
            if(Subchannel ==(i+1)) {
                if(Field==0){
                    memcpy(outVoid, &outConf.outMux[i], sizeof(outConf.outMux[i])); 
                    countByte = sizeof(outConf.outMux[i]);
                } else {
                    countByte = convertOutMuxConfigMon2ZMQ              (outConf.outMux[i], Field, outVoid);  
                }
            }
        }
        out_len = countByte;
        break;
    case 18 :
        // struct MiscChannelConf              misc;                                   
        // (x, 18, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (MiscChannelConf), 0 - subchannel(not used), 0 - all field
        convertMiscConfigMon2SNMP               (inConf.getMiscSettings(),              outConf.misc);
        if(Field==0){
            memcpy(outVoid, &outConf.misc, sizeof(outConf.misc)); 
            out_len = sizeof(outConf.misc);
        } else {
            out_len = convertMiscConfigMon2ZMQ              (outConf.misc, Field, outVoid);  
        }
        break;
    case 19 :
        // struct TDMAConf                     tdma[TDMA_SUBCHANNELS_PER_CHANNEL];     
        // (x, 19, y, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (TDMAConf), 0 - subchannel (1 - 8), 0 - all field
        for(unsigned int i = 0; i < sizeOfArray(outConf.tdma); ++i) {
            convertTDMAConfigMon2SNMP          (inConf.getTDMASubChannelSettings(i),   outConf.tdma[i]);
            if(Subchannel == 0) { 
                memcpy((char *)outVoid + countByte, &outConf.tdma[i], sizeof(outConf.tdma[i])); 
                countByte = countByte + sizeof(outConf.tdma[i]);
            }
            if(Subchannel ==(i+1)) {
                if(Field==0){
                    memcpy(outVoid, &outConf.tdma[i], sizeof(outConf.tdma[i])); 
                    countByte = sizeof(outConf.tdma[i]);
                } else {
                    countByte = convertTDMAConfigMon2ZMQ              (outConf.tdma[i], Field, outVoid);  
                }
            }
        }
        out_len = countByte;
        break;
    case 20 :
        // struct TDMADecoderConf              tdmaDecoder;                            
        // (x, 20, 0, 0) where x - ¹ channel(1 or 2), 1 - ¹ structure (TDMADecoderConf), 0 - subchannel(not used), 0 - all field
        convertTDMADecoderConfigMon2SNMP       (inConf.getTDMAChannelSettings(),       outConf.tdmaDecoder);
        if(Field==0){
            memcpy(outVoid, &outConf.tdmaDecoder, sizeof(outConf.tdmaDecoder)); 
            out_len = sizeof(outConf.tdmaDecoder);
        } else {
            out_len = convertTDMADecoderConfigMon2ZMQ              (outConf.tdmaDecoder, Field, outVoid);  
        }
        break;
    default:
        out_len = 0;
        break;
    }
    return out_len;
}

//==========================================================================================================================
//=========================     ZMQ copyChannelConfigZMQ2MonH        =======================================================
//==========================================================================================================================
unsigned int copyTunerConfigZMQ2MonH(const void *inVoid, TunerConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    Poco::Logger& _logger = Poco::Logger::get("convertTunerConfigMon2ZM");
    switch (Field) {
    case 1:
        memcpy(&outConf.frequency, inVoid, sizeof(outConf.frequency));
            poco_debug_f1(_logger, "Set inConf.frequency = %u", outConf.frequency);
        break;
    case 2:
        memcpy(&outConf.frequency, inVoid, sizeof(outConf.filterBand));
        break;
    case 3:
        memcpy(&outConf.manualFilterMode, inVoid,  sizeof(outConf.manualFilterMode)); 
        break;
    case 5:
        memcpy(&outConf.spectrumAnalyzerEnable,inVoid,  sizeof(outConf.spectrumAnalyzerEnable)); 
        break;
 
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}


unsigned int copyAGCConfigZMQ2MonH(const void *inVoid, AGCConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
//        memcpy(&outConf.initialGain, inVoid, sizeof(outConf.initialGain));
        break;
    case 2:
        memcpy(&outConf.mode, inVoid, sizeof(outConf.mode));
        break;
    case 3:
        memcpy(&outConf.manualLBandGain, inVoid, sizeof(outConf.manualLBandGain));
        break;
    case 4:
        memcpy(&outConf.manualBaseBandGain, inVoid, sizeof(outConf.manualBaseBandGain));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyADCConfigZMQ2MonH(const void *inVoid, ADCConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;

    switch (Field) {
    case 1:
///        memcpy(&outConf.coarseGainEnabled, inVoid, sizeof(outConf.coarseGainEnabled));
        break;
    case 2:
        memcpy(&outConf.decimationRate, inVoid, sizeof(outConf.decimationRate));
        break;
    case 3:
///        memcpy(&outConf.lowLatencyEnabled, inVoid, sizeof(outConf.lowLatencyEnabled));
        break;
    case 4:
          memcpy(&outConf.outputband, inVoid, sizeof(outConf.outputband));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyDemodulatorConfigZMQ2MonH(const void *inVoid, DemodulatorSubchannelConf& outConf, unsigned char Field)
{
    Poco::Logger& _logger = Poco::Logger::get("convertDemodulatorConfig");
    poco_debug_f1(_logger, " Field = %u",  Field);
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.symbolRate, inVoid, sizeof(outConf.symbolRate));
        poco_debug_f1(_logger, "Set outConf.symbolRate = %u", outConf.symbolRate);
        break;
    case 2:
        memcpy(&outConf.spectralInversion, inVoid, sizeof(outConf.spectralInversion));
        break;
    case 3:
        memcpy(&outConf.rrcAlpha, inVoid, sizeof(outConf.rrcAlpha));
        break;
    case 4:
        memcpy(&outConf.APSKRROI, inVoid, sizeof(outConf.APSKRROI));
        break;
    case 5:
        memcpy(&outConf.APSKRRMI, inVoid, sizeof(outConf.APSKRRMI));
        break;
    case 6:
        memcpy(&outConf.aeqMode, inVoid, sizeof(outConf.aeqMode));
        break;
    case 7:
        memcpy(&outConf.searchRange, inVoid, sizeof(outConf.searchRange));
        break;
    case 8:
        memcpy(&outConf.mode, inVoid, sizeof(outConf.mode));
        break;
    case 9:
        memcpy(&outConf.ddcEnable, inVoid, sizeof(outConf.ddcEnable));
        break;
    case 10:
        memcpy(&outConf.dcic, inVoid, sizeof(outConf.dcic));
        break;
    case 11:
        memcpy(&outConf.nonCoherent, inVoid, sizeof(outConf.nonCoherent));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyDecoderConfigZMQ2MonH(const void *inVoid, DecoderConf& outConf, unsigned char Field)
{
    unsigned int status_error=0;
    switch (Field) {
    case 1:
        memcpy(&outConf.mode, inVoid, sizeof(outConf.mode));
        break;
    case 2:
        memcpy(&outConf.cutMPEGSync, inVoid, sizeof(outConf.cutMPEGSync));
        break;
    case 3:
        memcpy(&outConf.innerCodeRate, inVoid, sizeof(outConf.innerCodeRate));
        break;
    case 4:
        memcpy(&outConf.outerCodeRate, inVoid, sizeof(outConf.outerCodeRate));
        break;
    case 5:
        memcpy(&outConf.dataInversion, inVoid, sizeof(outConf.dataInversion));
        break;
    case 6:
        memcpy(&outConf.outerFECDeinterleaver, inVoid, sizeof(outConf.outerFECDeinterleaver));
        break;
    case 7:
        memcpy(&outConf.outerDecoderEnable, inVoid, sizeof(outConf.outerDecoderEnable));
        break;
    case 8:
        memcpy(&outConf.outerFECDescramblerEnable, inVoid, sizeof(outConf.outerFECDescramblerEnable));
        break;
    case 9:
        memcpy(&outConf.diffDecoderEnable, inVoid, sizeof(outConf.diffDecoderEnable));
        break;
    case 10:
        memcpy(&outConf.innerFECDescramblerEnable, inVoid, sizeof(outConf.innerFECDescramblerEnable));
        break;
    case 12:
        memcpy(&outConf.autoSpectralInversionEnable, inVoid, sizeof(outConf.autoSpectralInversionEnable));
        break;
    case 13:
        memcpy(&outConf.autoAmbiguityElimination, inVoid, sizeof(outConf.autoAmbiguityElimination));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyIntelsatViterbiK7ConfigZMQ2MonH(const void *inVoid, IntelsatViterbiK7Conf& outConf, unsigned char Field)
{
    unsigned int status_error=0;
    switch (Field) {
    case 1:
        memcpy(&outConf.delay, inVoid, sizeof(outConf.delay));
        break;
    case 2:
        memcpy(&outConf.delaySubchannel, inVoid, sizeof(outConf.delaySubchannel));
        break;
    case 3:
        memcpy(&outConf.stc16DeinterleaverEnable, inVoid, sizeof(outConf.stc16DeinterleaverEnable));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copySequentialConfigZMQ2MonH(const void *inVoid, SequentialConf& outConf, unsigned char Field)
{
    unsigned int status_error=0;
    switch (Field) {
    case 1:
        memcpy(&outConf.fanoPolySelect, inVoid, sizeof(outConf.fanoPolySelect));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyDVBS2ConfigZMQ2MonH(const void *inVoid, DVBS2DemodulatorConf& outConf, unsigned char Field)
{
    unsigned int status_error=0;
    switch (Field) {
    case 1:
        memcpy(&outConf.sampleRate, inVoid, sizeof(outConf.sampleRate));
        break;
    case 2:
        memcpy(&outConf.ragcEnable, inVoid, sizeof(outConf.ragcEnable));
        break;
    case 3:
        memcpy(&outConf.ragcInvert, inVoid, sizeof(outConf.ragcInvert));
        break;
    case 4:
        memcpy(&outConf.outputAmplitudeVCM, inVoid, sizeof(outConf.outputAmplitudeVCM));
        break;
    case 5:
        memcpy(&outConf.outputAmplitudeQPSK, inVoid, sizeof(outConf.outputAmplitudeQPSK));
        break;
    case 6:
        memcpy(&outConf.outputAmplitude8PSK, inVoid, sizeof(outConf.outputAmplitude8PSK));
        break;
    case 7:
        memcpy(&outConf.outputAmplitude16APSK, inVoid, sizeof(outConf.outputAmplitude16APSK));
        break;
    case 8:
        memcpy(&outConf.outputAmplitude32APSK, inVoid, sizeof(outConf.outputAmplitude32APSK));
        break;
    case 9:
        memcpy(&outConf.reacqHoldoff, inVoid, sizeof(outConf.reacqHoldoff));
        break;
    case 10:
        memcpy(&outConf.reacqActivationDelay, inVoid, sizeof(outConf.reacqActivationDelay));
        break;
    case 12:
        memcpy(&outConf.reacqRestartDelay, inVoid, sizeof(outConf.reacqRestartDelay));
        break;
    case 13:
        memcpy(&outConf.falseDetEnable, inVoid, sizeof(outConf.falseDetEnable));
        break;
    case 14:
        memcpy(&outConf.falseDetOneshot, inVoid, sizeof(outConf.falseDetOneshot));
        break;
    case 15:
        memcpy(&outConf.falseDetThresh, inVoid, sizeof(outConf.falseDetThresh));
        break;
    case 16:
        memcpy(&outConf.aeqAutoSpeed, inVoid, sizeof(outConf.aeqAutoSpeed));
        break;
    case 17:
        memcpy(&outConf.aeqAutoModMask, inVoid, sizeof(outConf.aeqAutoModMask));
        break;
    case 18:
        memcpy(&outConf.aeq2xRate, inVoid, sizeof(outConf.aeq2xRate));
        break;
    case 19:
        memcpy(&outConf.coarseSteps, inVoid, sizeof(outConf.coarseSteps));
        break;
    case 20:
        memcpy(&outConf.mode, inVoid, sizeof(outConf.mode));
        break;
    case 21:
        memcpy(&outConf.dvbs2ModCod, inVoid, sizeof(outConf.dvbs2ModCod));
        break;
    case 22:
        memcpy(&outConf.dvbs2FrameType, inVoid, sizeof(outConf.dvbs2FrameType));
        break;
    case 23:
        memcpy(&outConf.dvbs2Pilots, inVoid, sizeof(outConf.dvbs2Pilots));
        break;
    case 24:
        memcpy(&outConf.dvbs2LoopBw, inVoid, sizeof(outConf.dvbs2LoopBw));
        break;
    case 25:
        memcpy(&outConf.dvbs2PLFRAMEDescrambler, inVoid, sizeof(outConf.dvbs2PLFRAMEDescrambler));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyDVBS2DecoderConfigZMQ2MonH(const void *inVoid, DVBS2DecoderConf& outConf, unsigned char Field)
{
    unsigned int status_error=0;
    switch (Field) {
    case 1:
        memcpy(&outConf.autoIter, inVoid, sizeof(outConf.autoIter));
        break;
    case 2:
        memcpy(&outConf.autoIterMode, inVoid, sizeof(outConf.autoIterMode));
        break;
    case 3:
        memcpy(&outConf.iterations, inVoid, sizeof(outConf.iterations));
        break;
    case 4:
        memcpy(&outConf.disableFooter, inVoid, sizeof(outConf.disableFooter));
        break;
    case 5:
        memcpy(&outConf.disableDFL, inVoid, sizeof(outConf.disableDFL));
        break;
    case 6:
        memcpy(&outConf.disableCRCSuppression, inVoid, sizeof(outConf.disableCRCSuppression));
        break;
    case 7:
        memcpy(&outConf.outputMode, inVoid, sizeof(outConf.outputMode));
        break;
    case 8:
        memcpy(&outConf.disableAS14_15, inVoid, sizeof(outConf.disableAS14_15));
        break;
    case 9:
        memcpy(&outConf.isi, inVoid, sizeof(outConf.isi));
        break;
    case 10:
        memcpy(&outConf.tframeHeader, inVoid, sizeof(outConf.tframeHeader));
        break;
    case 11:
        memcpy(&outConf.outModCod, inVoid, sizeof(outConf.outModCod));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyDEMConfigZMQ2MonH(const void *inVoid, DEMConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.mode, inVoid, sizeof(outConf.mode));
        break;
    case 2:
        memcpy(&outConf.multiMode, inVoid, sizeof(outConf.multiMode));
        break;
    case 3:
        memcpy(&outConf.numberOfSubchannel, inVoid, sizeof(outConf.numberOfSubchannel));
        break;
    case 4:
        memcpy(&outConf.carrierFormatInMulti, inVoid, sizeof(outConf.carrierFormatInMulti));
        break;
    case 5:
        memcpy(&outConf.carrierFormatInTDMA, inVoid, sizeof(outConf.carrierFormatInTDMA));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyHDRMConfigZMQ2MonH(const void *inVoid, HDRMSubchannelConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.modulationType, inVoid, sizeof(outConf.modulationType));
        break;
    case 2:
        memcpy(&outConf.carrierOffset, inVoid, sizeof(outConf.carrierOffset));
        break;
    case 3:
        memcpy(&outConf.carrierLBand, inVoid, sizeof(outConf.carrierLBand));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyTPCConfigZMQ2MonH(const void *inVoid, TPCDecoderConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        // strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   // {8.1}
        memcpy(&outConf.codeName, inVoid, sizeof(outConf.codeName));
        break;
    case 2:
        memcpy(&outConf.ahaDescramblerEnable, inVoid, sizeof(outConf.ahaDescramblerEnable));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyTDMADecoderConfigZMQ2MonH(const void *inVoid, TDMADecoderConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        // strncpy(outConf.codeName, inConf.getCodename().c_str(), inConf.getCodename().size());                   // {8.1}
        memcpy(&outConf.codeName, inVoid, sizeof(outConf.codeName));
        break;
    case 2:
        memcpy(&outConf.ahaDescramblerEnable, inVoid, sizeof(outConf.ahaDescramblerEnable));
        break;
    case 3:
        memcpy(&outConf.modulationType, inVoid, sizeof(outConf.modulationType));
        break;
    case 4:
        memcpy(&outConf.headerValue, inVoid, sizeof(outConf.headerValue));
        break;
    case 5:
        memcpy(&outConf.portionLength, inVoid, sizeof(outConf.portionLength));
        break;
    case 6:
        memcpy(&outConf.timeStampEnable, inVoid, sizeof(outConf.timeStampEnable));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyMultiTPCConfigZMQ2MonH(const void *inVoid, MultiTPCDecoderConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.asdEnable, inVoid, sizeof(outConf.asdEnable));
        break;
    case 2:
        memcpy(&outConf.dataInvAfterDescramblers, inVoid, sizeof(outConf.dataInvAfterDescramblers));
        break;
    case 3:
        memcpy(&outConf.customAutoSearchSync, inVoid, sizeof(outConf.customAutoSearchSync));
        break;
    case 4:
        memcpy(&outConf.frameLen, inVoid, sizeof(outConf.frameLen));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyLDPCConfigZMQ2MonH(const void *inVoid, LDPCConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.bypassMode, inVoid, sizeof(outConf.bypassMode));
        break;
    case 2:
        memcpy(&outConf.adaptiveBalanserEnable, inVoid, sizeof(outConf.adaptiveBalanserEnable));
        break;
    case 3:
        memcpy(&outConf.correctionOff, inVoid, sizeof(outConf.correctionOff));
        break;
    case 4:
        memcpy(&outConf.onSync, inVoid, sizeof(outConf.onSync));
        break;
    case 5:
        memcpy(&outConf.offSync, inVoid, sizeof(outConf.offSync));
        break;
    case 6:
        memcpy(&outConf.errorInSyncBeforeLock, inVoid, sizeof(outConf.errorInSyncBeforeLock));
        break;
    case 7:
        memcpy(&outConf.errorInSyncAfterLock, inVoid, sizeof(outConf.errorInSyncAfterLock));
        break;
    case 8:
        memcpy(&outConf.maxErrorThreshold, inVoid, sizeof(outConf.maxErrorThreshold));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;

}

unsigned int copyPostProcessingConfigZMQ2MonH(const void *inVoid, PostProcessingConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.ssdEnable, inVoid, sizeof(outConf.ssdEnable));
        break;
    case 2:
        memcpy(&outConf.ssdType, inVoid, sizeof(outConf.ssdType));
        break;
    case 3:
        memcpy(&outConf.ssdPolynomial, inVoid, sizeof(outConf.ssdPolynomial));
        break;
    case 4:
        memcpy(&outConf.framerEnable, inVoid, sizeof(outConf.framerEnable));
        break;
    case 5:
        memcpy(&outConf.framerType, inVoid, sizeof(outConf.framerType));
        break;
    case 6:
        memcpy(&outConf.framerSyncWord, inVoid, sizeof(outConf.framerSyncWord));
        break;
    case 7:
        memcpy(&outConf.framerSyncMask, inVoid, sizeof(outConf.framerSyncMask));
        break;
    case 8:
        memcpy(&outConf.framerFrameLength, inVoid, sizeof(outConf.framerFrameLength));
        break;
    case 9:
        memcpy(&outConf.framerSyncOffset, inVoid, sizeof(outConf.framerSyncOffset));
        break;
    case 10:
        memcpy(&outConf.framerSyncGroup, inVoid, sizeof(outConf.framerSyncGroup));
        break;
    case 11:
        memcpy(&outConf.framerDataInversion, inVoid, sizeof(outConf.framerDataInversion));
        break;
    case 12:
        memcpy(&outConf.adsEnable, inVoid, sizeof(outConf.adsEnable));
        break;
    case 13:
        memcpy(&outConf.adsType, inVoid, sizeof(outConf.adsType));
        break;
    case 14:
        memcpy(&outConf.adsPreset, inVoid, sizeof(outConf.adsPreset));
        break;
    case 15:
        memcpy(&outConf.adsPolynomial, inVoid, sizeof(outConf.adsPolynomial));
        break;
    case 16:
        memcpy(&outConf.outputDataMask, inVoid, sizeof(outConf.outputDataMask)); // #814
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyTDMAConfigZMQ2MonH(const void *inVoid, TDMAConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.modulationType, inVoid, sizeof(outConf.modulationType));
        break;
    case 2:
        memcpy(&outConf.carrierOffset, inVoid, sizeof(outConf.carrierOffset));
        break;
    case 3:
        memcpy(&outConf.symbolRate, inVoid, sizeof(outConf.symbolRate));
        break;
    case 4:
        memcpy(&outConf.spectralInversion, inVoid, sizeof(outConf.spectralInversion));
        break;
    case 5:
        memcpy(&outConf.terminalName, inVoid, sizeof(outConf.terminalName));
        break;
    case 6:
        memcpy(&outConf.outputMode, inVoid, sizeof(outConf.outputMode));
        break;
    case 7:
        memcpy(&outConf.carrierLBand, inVoid, sizeof(outConf.carrierLBand));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyOutMuxConfigZMQ2MonH(const void *inVoid, OutMuxSubchannelConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        memcpy(&outConf.portionLength, inVoid, sizeof(outConf.portionLength));
        break;
    case 2:
        memcpy(&outConf.data, inVoid, sizeof(outConf.data));
        break;
    case 3:
        memcpy(&outConf.timeStampEnable, inVoid, sizeof(outConf.timeStampEnable));
        break;
    case 4:
        memcpy(&outConf.headerEnable, inVoid, sizeof(outConf.headerEnable));
        break;
    case 5:
        memcpy(&outConf.syncOnMultiframe, inVoid, sizeof(outConf.syncOnMultiframe));
        break;
    case 6:
        memcpy(&outConf.footerEnable, inVoid, sizeof(outConf.footerEnable));
        break;
    case 7:
        memcpy(&outConf.header, inVoid, sizeof(outConf.header));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;
}

unsigned int copyMiscConfigZMQ2MonH(const void *inVoid, MiscChannelConf& outConf, unsigned char Field)
{
    unsigned int status_error = 0;
    switch (Field) {
    case 1:
        // strncpy(outConf.label, inConf.getLabel().c_str(), inConf.getLabel().size());
        memcpy(&outConf.label, inVoid, sizeof(outConf.label));
        break;
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }
    return status_error;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------
unsigned int copyChannelConfigZMQ2MonH(ChannelConf& Conf, void *inStructure, unsigned int length, unsigned char SelectStruct, unsigned char Subchannel, unsigned char Field)
{
    unsigned int status_error = 0;
    // convertChannelConfigMon2SNMP(_settingsManager.getChannelSettings(_channel), Conf, SelectStruct);
    // Poco::Logger& _logger = Poco::Logger::get("copyChannelConfigZMQ2MonH");
    switch (SelectStruct) {
    case 0:
        if(Field==0){
            // poco_debug_f1(_logger, "Set Field = %u", SelectStruct);
            memcpy(&Conf, inStructure, length);
        } else {
            status_error = 200;  
        }
        break;
    case 1 :
        if(Field==0){
            memcpy(&Conf.tuner, inStructure, length);
            // memcpy(outVoid, &outConf.tuner, sizeof(outConf.tuner)); 
            // out_len = sizeof(Conf.tuner);
        } else {
            status_error = copyTunerConfigZMQ2MonH   (inStructure, Conf.tuner, Field);  
        }
        break;
    case 2:
        if(Field==0){
            memcpy(&Conf.agc, inStructure, length);
        } else {
            status_error = copyAGCConfigZMQ2MonH   (inStructure, Conf.agc, Field);  
        }
        break;
    case 3 :
        if(Field==0){
            memcpy(&Conf.adc, inStructure, length);
        } else {
            status_error = copyADCConfigZMQ2MonH   (inStructure, Conf.adc, Field);  
        }
        break;
    case 4 :
        if(Subchannel == 0) { 
            if(Field==0){
                memcpy(&Conf.demodulator, inStructure, length);
            } else {
                status_error = 600;  
            }
        } else {
            if(Field==0){
                memcpy(&Conf.demodulator[Subchannel-1], inStructure, length);
            } else {
                status_error = copyDemodulatorConfigZMQ2MonH   (inStructure, Conf.demodulator[Subchannel-1], Field);  
            }
        }
        break;
    case 5 :
        if(Subchannel == 0) { 
            if(Field==0){
                memcpy(&Conf.hdrm, inStructure, length);
            } else {
                status_error = 600;  
            }
        } else {
            if(Field==0){
                memcpy(&Conf.hdrm[Subchannel-1], inStructure, length);
            } else {
                status_error = copyHDRMConfigZMQ2MonH   (inStructure, Conf.hdrm[Subchannel-1], Field);  
            }
        }
//        for(unsigned int i = 0; i < sizeOfArray(Conf.hdrm); ++i) {
       break;
    case 6 :
        if(Field==0){
            memcpy(&Conf.dvb, inStructure, length);
        } else {
            status_error = copyDVBS2ConfigZMQ2MonH   (inStructure, Conf.dvb, Field);  
        }
        break;
    case 7 :
        if(Field==0){
            memcpy(&Conf.dvbDecoder, inStructure, length);
        } else {
            status_error = copyDVBS2DecoderConfigZMQ2MonH   (inStructure, Conf.dvbDecoder, Field);  
        }
        break;
    case 8 :
        if(Field==0){
            memcpy(&Conf.dem, inStructure, length);
        } else {
            status_error = copyDEMConfigZMQ2MonH   (inStructure, Conf.dem, Field);  
        }
        break;
    case 9 :
        if(Field==0){
            memcpy(&Conf.decoder, inStructure, length);
        } else {
            status_error = copyDecoderConfigZMQ2MonH   (inStructure, Conf.decoder, Field);  
        }
        break;
    case 10:
        if(Field==0){
            memcpy(&Conf.tpcDecoder, inStructure, length);
        } else {
            status_error = copyTPCConfigZMQ2MonH   (inStructure, Conf.tpcDecoder, Field);  
        }
        break;
    case 11:
        if(Field==0){
            memcpy(&Conf.multiTPCDecoder, inStructure, length);
        } else {
            status_error = copyMultiTPCConfigZMQ2MonH   (inStructure, Conf.multiTPCDecoder, Field);  
        }
        break;
    case 12:
        if(Field==0){
            memcpy(&Conf.viterbiK7, inStructure, length);
        } else {
            status_error = copyIntelsatViterbiK7ConfigZMQ2MonH   (inStructure, Conf.viterbiK7, Field);  
        }

        break;
    case 13:
        if(Field==0){
            memcpy(&Conf.sequential, inStructure, length);
        } else {
            status_error = copySequentialConfigZMQ2MonH   (inStructure, Conf.sequential, Field);  
        }
        break;
    case 14: // Conf.dvbs -  Not User fSyncOn  fSyncOff
        break;
    case 15:
        if(Field==0){
            memcpy(&Conf.ldpc, inStructure, length);
        } else {
            status_error = copyLDPCConfigZMQ2MonH   (inStructure, Conf.ldpc, Field);  
        }
        break;    
    case 16:
        if(Field==0){
            memcpy(&Conf.postProcessing, inStructure, length);
        } else {
            status_error = copyPostProcessingConfigZMQ2MonH   (inStructure, Conf.postProcessing, Field);  
        }
        break;
    case 17:
        if(Subchannel == 0) { 
            if(Field==0){
                memcpy(&Conf.outMux, inStructure, length);
            } else {
                status_error = 600;  
            }

        } else {
            if(Field==0){
                memcpy(&Conf.outMux[Subchannel-1], inStructure, length);
            } else {
                status_error = copyOutMuxConfigZMQ2MonH   (inStructure, Conf.outMux[Subchannel-1], Field);  
            }

        }
        break;
    case 18:
        if(Field==0){
            memcpy(&Conf.misc, inStructure, length);
        } else {
            status_error = copyMiscConfigZMQ2MonH   (inStructure, Conf.misc, Field);  
        }
        break;
    case 19:
        if(Subchannel == 0) { 
            if(Field==0){
                memcpy(&Conf.tdma, inStructure, length);
            } else {
                status_error = 600;  
            }
        } else {
            if(Field==0){ 
                memcpy(&Conf.tdma[Subchannel-1], inStructure, length);
            } else {
                status_error = copyTDMAConfigZMQ2MonH   (inStructure, Conf.tdma[Subchannel-1], Field);  
            }
        }
        break;
    case 20:
        if(Field==0){
            memcpy(&Conf.tdmaDecoder, inStructure, length);
        } else {
            status_error = copyTDMADecoderConfigZMQ2MonH   (inStructure, Conf.tdmaDecoder, Field);  
        }
    default:
        status_error = MSG_ERROR_EXCEEDED_NUM_FIELD;
        break;
    }

    return status_error;

}
*/
} // namespace SNMP


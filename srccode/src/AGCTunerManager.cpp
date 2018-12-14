#include "AGCTunerManager.hpp"
#include "Common/Defines.hpp"
#include "Common/Helpers.hpp"
#include "Common/ByteOperations.hpp"
#include "Common/Tracer.hpp"
#include "UART/tty.h"
//#include "Defines.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/NumberFormatter.h"
#include "Poco/ScopedLock.h"

using Poco::Logger;
using Poco::ScopedLock;
using Poco::Mutex;
/*---------------------------------------------------------------------------*/
#define ADC_GAIN                    3.5
#define INPUT_IMPENDANCE_MISMATCH   26
/*---------------------------------------------------------------------------*/
TunerManager::TunerManager(const ChannelID& channel, Settings::ChannelSettings& channelSettings, Statistics::ChannelStatistics& channelStatistics)
: _channel(channel)
, _channelSettings(channelSettings)
, _channelStatistics(channelStatistics)
, _device()
, _knownTunerState(false)
, _logger(Logger::get("Monitor.AGCTuner."  + Poco::NumberFormatter::format(channel)))
{
    //TRACE();
    _knownTunerState = 0; //_device.isTunerPresent();
    //_channelSettings.setAGCTunerControlSettings(_device);
    // poco_debug_f1(_logger, "Tuner %s present at start", std::string(_knownTunerState ? "is" : "is not"));
    // printf("open UART");
    fd_UART = 0;
    state_config = false;
}

TunerManager::~TunerManager(){
    if(fd_UART!=0) {
        closeUART(fd_UART);
    }
}
        

void TunerManager::configureConstValues()
{
//    TRACE();
// ---------------------------------------------------------------
    Settings::TunerSettings& tunerSettings = _channelSettings.getTunerSettings();
    Settings::AGCSettings& agcadrfSettings = _channelSettings.getAGCSettings();
// ------------ AGC & Tuner constatns ------------------------------------
    _device.set_Tuner_Fmin(tunerSettings.getTunerFMIN());
    _device.set_Tuner_Fmax(tunerSettings.getTunerFMAX());
    _device.set_Tuner_LPFmin(tunerSettings.getTunerLPFMIN());
    _device.set_Tuner_LPFmax(tunerSettings.getTunerLPFMAX());
// poco_debug_f2(_logger, "!!!tunerSettings.getTunerFMIN() = %u, tunerSettings.getTunerFMAX() = %u", tunerSettings.getTunerFMIN(), tunerSettings.getTunerFMAX());

// ------------ Tuner immutable ------------------------------------
    _device.set_InitialGain2(tunerSettings.getInitialGain2());
    _device.set_InitialGain1(tunerSettings.getInitialGain1());
// poco_debug_f2(_logger, "!!!tunerSettings.getInitialGain1() = %u, tunerSettings.getInitialGain2() = %u", tunerSettings.getInitialGain1(), tunerSettings.getInitialGain2());

// ------------ AGC immutable ------------------------------------
    _device.set_AGC_Uref( agcadrfSettings.get_AGC_UREF());
// poco_debug_f2(_logger, "!!!agcadrfSettings.get_AGC_UREF() = %u, agcadrfSettings.get_AGC_DAC() = %u", agcadrfSettings.get_AGC_UREF(), agcadrfSettings.get_AGC_DAC());

    _device.set_AGC_DAC( agcadrfSettings.get_AGC_DAC());
    _device.set_VPK_delay( agcadrfSettings.get_AGC_VPKDELAY());
    // # AGC.dLim     (0bxxxx11xx.xxxxxxxx) - 0:8*, 1:16, 2:32, 3:64 |Urms-Uref| = Uref/dLim; 
    _device.set_AGC_dLim(agcadrfSettings.get_AGC_DLIM());
    
    _device.set_Filter( agcadrfSettings.get_AGC_ADRF_FILTER());
// poco_debug_f2(_logger, "!!!agcadrfSettings.get_AGC_ADRF_FILTER()= %u, _device.get_Filter = %u", agcadrfSettings.get_AGC_ADRF_FILTER(), _device.get_Filter());
    _device.set_PwrMode( agcadrfSettings.get_AGC_ADRF_PWR_MODE());
    _device.set_VGA1( agcadrfSettings.get_AGC_ADRF_VGA1());
    _device.set_VGA2( agcadrfSettings.get_AGC_ADRF_VGA2());
    _device.set_VGA3( agcadrfSettings.get_AGC_ADRF_VGA3());
// poco_debug_f2(_logger, "!!!agcadrfSettings.get_AGC_ADRF_VGA3()= %u, _device.get_VGA3 = %u", agcadrfSettings.get_AGC_ADRF_VGA3(), _device.get_VGA3());

    _device.set_PostAmp( agcadrfSettings.get_AGC_ADRF_POSTAMP());
    _device.set_DC_offs( agcadrfSettings.get_AGC_ADRF_DC_OFFS());

    _device.set_VGN1( agcadrfSettings.get_AGC_ADRF_VGN1());
    _device.set_VGN2( agcadrfSettings.get_AGC_ADRF_VGN2());
    _device.set_VGN3( agcadrfSettings.get_AGC_ADRF_VGN3());
    _device.set_KOEF_for_Pin( agcadrfSettings.get_AR_ATT_FOR_PIN());
    
}

void TunerManager::collectStatisticsAfterStart()
{
    state_config = true;    
    _device.set_EventLog(true);
    _device.collectAllStatisticAGCTuner(static_cast<unsigned int>(SPI_READ_AGCTUNER_CONF));

   Statistics::TunerStatistics& tuner = _channelStatistics.getTunerStatistics();
   /*
   ѕри включении издели€ однократно прочитываютс€ и вывод€тс€ в соответствующие пол€ статистики. ¬ процессе сеанса работы они не мен€ютс€. 
       Serial number
       HW revision
       // Software version
       // FPGA version
       LO Frequency, Hz
   */
   tuner.setSerialNumber(_device.get_EEPROM_serialNumber()); // Serial number
   tuner.setHWRevisionType(_device.get_EEPROM_version_type());         // HW revision
   tuner.setSoftwareVersion(_device.get_soft_vers());        // Software version
   tuner.setTunerSoftwareRelease(_device.get_release());         // Release
   // FPGA version
   tuner.setReferenceFrequency(_device.get_TSFP_ReferenceFrequency()); // LO Frequency, Hz 
   state_config = false;
}

void TunerManager::configure()
{
    TRACE();
    state_config = true;
    try{
        Settings::TunerSettings& tunerSettings = _channelSettings.getTunerSettings();
        Settings::AGCSettings& agcadrfSettings = _channelSettings.getAGCSettings();
        Settings::DecimationSettings& decimationSettings = _channelSettings.getDecimationSettings();
        
        if(tunerSettings.getAGCMode()==3) { // Factory Mode
            poco_debug(_logger, "Apply ADRF & EEPROM Factory data");
            _device.set_frequency(tunerSettings.getFrequency());
            _device.set_filterBand(tunerSettings.getFilterBandIndex());
            _device.set_spectrumInversion(tunerSettings.getSpectrumInversion()); 
            if(agcadrfSettings.get_ApplySendSPIflag()&0x1) {
                _device.set_ADRF_DAC_1C_4(agcadrfSettings.get_ADRF_DAC_1C_4());
                _device.set_ADRF_REG_16_2(agcadrfSettings.get_ADRF_REG_16_2());
                _device.set_AGC_CTRL_5C_2(agcadrfSettings.get_AGC_CTRL_5C_2());
                _device.set_AGC_Uref_5E_2(agcadrfSettings.get_AGC_Uref_5E_2());

                _device.configureApplyADRFConf();

            }
            if(agcadrfSettings.get_ApplySendSPIflag()&0x80) { //write to EEPROM
                poco_debug(_logger, "ApplySendSPIflag()&0x80)");
                for(unsigned int i=0; i<16; i++) {
                    _device.set_EEPROM_Array16Data(i, agcadrfSettings.get_ArrayEEPROMData(i));
                }
/*              _device.set_EEPROM_ADRFDAC_51_3(agcadrfSettings.get_EEPROM_ADRFDAC_51_3());
                _device.set_EEPROM_ADRFREG_56_2(agcadrfSettings.get_EEPROM_ADRFREG_56_2());
                _device.set_EEPROM_AGCCTR_54_2(agcadrfSettings.get_EEPROM_AGCCTR_54_2());
                _device.set_EEPROM_Project_48_1(agcadrfSettings.get_EEPROM_Project_48_1());
                _device.set_EEPROM_Reference_49_3(agcadrfSettings.get_EEPROM_Reference_49_3());
                _device.set_EEPROM_Release_4E_2(agcadrfSettings.get_EEPROM_Release_4E_2());
                _device.set_EEPROM_SerialNumber_4C_2(agcadrfSettings.get_EEPROM_SerialNumber_4C_2());
                _device.set_EEPROM_VersionNumber_50_1(agcadrfSettings.get_EEPROM_VersionNumber_50_1());
*/
                _device.spi_write_EEPROM();
                usleep(100000); // 100 mck после записи в EEPROM
            }
            // s_device.spi_read_EEPROM(); // пропихнуть значени€ из EEPROM в регистры EEPROM
            //_device.collectAllStatisticAGCTuner(SPI_READ_AGCTUNER_STAT);
            // collectStatisticsAfterStart();
        } else {
            _device.set_frequency(tunerSettings.getFrequency());                     // 1
    poco_debug_f2(_logger, "tunerSettings.getFrequency() = %u, _device.get_frequency() = %u", tunerSettings.getFrequency(), _device.get_frequency());
            _device.set_filterBand(tunerSettings.getFilterBandIndex());                   // 2
    poco_debug_f2(_logger, "tunerSettings.getFilterBand() = %u, _device.get_filterBand() = %u", tunerSettings.getFilterBandIndex(), _device.get_filterBand());
            _device.set_agcMode(tunerSettings.getAGCMode());                         // 3
            _device.set_spectrumInversion(tunerSettings.getSpectrumInversion());

            switch(tunerSettings.getAGCMode()) {
            case 0:
                // # AGC.trigA    (0bxxxxxx11.xxxxxxxx) - 0: Disable* (Auto, One Shot), 1: fixUnlock, 2: forcLock (Manual)
                _device.set_AGC_trig(agcadrfSettings.get_AGC_TRIGA());
                break;
            case 1:
                // # AGC.trigM    (0bxxxxxx11.xxxxxxxx) - 0: Disable (Auto, One Shot), 1: fixUnlock, 2: forcLock* (Manual)
                _device.set_AGC_trig(agcadrfSettings.get_AGC_TRIGM());
                _device.set_agc_manualBaseBandGain(tunerSettings.getManualBasebandGain()); // 4
                poco_debug_f2(_logger, "tunerSettings.getManualBasebandGain() = %u, _device.get_agc_manualBaseBandGain() = %u", tunerSettings.getManualBasebandGain(), _device.get_agc_manualBaseBandGain());
                _device.set_agc_manualLBandGain(tunerSettings.getManualLBandGain());       // 5
                poco_debug_f2(_logger, "tunerSettings.getManualLBandGain() = %u, _device.get_agc_manualLBandGain() = %u", tunerSettings.getManualLBandGain(), _device.get_agc_manualLBandGain());
                break;
            default:
                // # AGC.trigS    (0bxxxxxx11.xxxxxxxx) - 0: Disable* (Auto, One Shot), 1: fixUnlock, 2: forcLock (Manual)
                _device.set_AGC_trig(agcadrfSettings.get_AGC_TRIGS());
                break;
            }
            _device.set_EventLog(false);
            _device.configureAGCTuner();

        } 
            // SET Decimation Rate
        _device.set_DecimationRate(decimationSettings.getDecimationRate());
        _device.configureResampler();
            // SET 10GE
        _device.configure10GE();

        _device.set_EventLog(true);
//        poco_debug_f1(_logger, "_device.collectAllStatisticAGCTuner(SPI_READ_AGCTUNER_CONF) = %u", static_cast<unsigned int>(SPI_READ_AGCTUNER_CONF));
        _device.collectAllStatisticAGCTuner(static_cast<unsigned int>(SPI_READ_AGCTUNER_CONF));
        /*
        ѕри включении издели€ однократно прочитываютс€ и вывод€тс€ в соответствующие пол€ статистики. ¬ процессе сеанса работы они не мен€ютс€. 
            Serial number
            HW revision
            // Software version
            // FPGA version
            LO Frequency, Hz
        */
        Statistics::TunerStatistics& tuner = _channelStatistics.getTunerStatistics();
        tuner.setSerialNumber(_device.get_EEPROM_serialNumber()); // Serial number
        tuner.setHWRevisionType(_device.get_EEPROM_version_type());         // HW revision
        tuner.setSoftwareVersion(_device.get_soft_vers());        // Software version
        tuner.setTunerSoftwareRelease(_device.get_release());         // Release
        // FPGA version
        tuner.setReferenceFrequency(_device.get_TSFP_ReferenceFrequency()); // LO Frequency, Hz 
    }
    catch (const Poco::Exception& exc){
        _logger.log(exc);
    }
    state_config = false;
}

void TunerManager::collectStatistics()
{
    // TRACE();
    if(state_config == false) {
       _device.set_EventLog(false);
       _device.collectAllStatisticAGCTuner(static_cast<unsigned int>(SPI_READ_AGCTUNER_STAT));//SPI_READ_ALL_STATUS  SPI_READ_AGCADRF_CONF SPI_READ_AGCTUNER_STAT);
       _device.collectStatistic10GE();

        Statistics::TunerStatistics& tuner = _channelStatistics.getTunerStatistics();
        Settings::AGCSettings& agcadrfSettings = _channelSettings.getAGCSettings();

        // Settings::TunerSettings& tunerSettings = _channelSettings.getTunerSettings();
        // tuner.setType(_device.get_EEPROM_version_type());         // HW revision
        tuner.setPresence(_device.get_present());
        
    //#0x0E 2.2.„тение параметра Temperature, C
        tuner.setTemperature(_device.get_TSFP_Temperature());
    //#0x1A 2.3. AGC_STS 
        // # Error               (0bx1xxxxxx.xxxxxxxx) 
        tuner.setAGCError(_device.get_AGC_Error());
        // # Too_High       (0bxx1xxxxx.xxxxxxxx)
        // # Too_Low        (0bxxx1xxxx.xxxxxxxx) 
        tuner.setAGCToo_High(_device.get_AGC_TooHigh());
        tuner.setAGCToo_Low (_device.get_AGC_TooLow());
        // # ManualMode  (0bxxxxxxxx.xxxxx1xx)
        tuner.setAGCManualGainControl(_device.get_AGC_Manual());// _device.isManualGainControlMode());
        // # Lock               (0bxxxxxxxx.xxxxxx1x) 
        tuner.setAGCLock(_device.get_AGC_Lock());
        tuner.setAGCEndOfSearch(_device.get_AGC_EndOfSearch());
        // tuner.setAGCUnlock(_device.get_AGC_Unlock());
        tuner.setAGCInit(_device.get_AGC_Init());

    //#0x14 2.4. w14   PeakDetect (ReferenceVoltage)
        tuner.setAGCDetectVoltage(_device.get_TSFP_DetectVoltage());
    //#0x10 2.5. w12     DACoutput           10		R               (struct: BBG:4 (0bxx1111xxxxxxxxxx), DAC:9 (0bxxxxxxx111111111) ) // получаетс€ после калькул€ции DACspecif

    //    if(_channelSettings.getTunerSettings().getAGCMode()==1) {
        tuner.setAGCLBandGain(_device.get_TSFP_GC1_lBandGain());
        tuner.setAGCBaseBandGain(_device.get_TSFP_GC2_baseBandGain());

    //#0x5E 2.6. _AGC_Uref ; pin = TSFP_ReferenceVoltage - _AGC_Uref
    /*    if (getTunerVersion() == 0x15) return 10 * log10(pow(getOutputVoltage(), 2) / 1000) - getLBandGain() - getBaseBandGain() - getADCGain() - getInputImpendanceMismatch() + 1.76;
        if (getTunerVersion() == 0x4)  return 10 * log10(pow(getOutputVoltage(), 2) / 1000) - getLBandGain() - getBaseBandGain() - getADCGain() - getInputImpendanceMismatch() + 12.0;
        return 10 * log10(pow(getOutputVoltage(), 2) / 1000) - getLBandGain() - getBaseBandGain() - getADCGain() - getInputImpendanceMismatch()                                      ;
        getMeanPowerSupplyVoltage() * 1000 / 2047 * sqrt(getRawOutputVoltage())                                                                                                                                                                             ;
    */
       // return 10 * log10(pow(getOutputVoltage(), 2) / 1000) - _device.get_TSFP_GC1_lBandGain() - _device.get_TSFP_GC2_baseBandGain();
        tuner.setAGCInputSignalLevel(_device.get_Pin());
    //#0x30 2.7. F3db / lpfCutoffFrequency
        tuner.setLPFCutoffFrequency(_device.get_lpfCutoffFrequency());

//        tuner.setMaxSpectrumAnalyzerSymbolRate(0);  //_device.get_maxSpectrumAnalyzerSymbolRate());

        agcadrfSettings.set_ADRF_DAC_1C_4(_device.get_ADRF_DAC_1C_4());
        agcadrfSettings.set_ADRF_REG_16_2(_device.get_ADRF_REG_16_2());
        agcadrfSettings.set_AGC_CTRL_5C_2(_device.get_AGC_CTRL_5C_2());
        agcadrfSettings.set_AGC_Uref_5E_2(_device.get_AGC_Uref_5E_2());
        agcadrfSettings.set_EEPROM_Project_48_1(_device.get_EEPROM_Project_48_1());
        agcadrfSettings.set_EEPROM_ADRFDAC_51_3(_device.get_EEPROM_ADRFDAC_51_3());
        agcadrfSettings.set_EEPROM_ADRFREG_56_2(_device.get_EEPROM_ADRFREG_56_2());
        agcadrfSettings.set_EEPROM_AGCCTR_54_2(_device.get_EEPROM_AGCCTR_54_2());
        agcadrfSettings.set_EEPROM_Reference_49_3(_device.get_EEPROM_Reference_49_3());
        agcadrfSettings.set_EEPROM_Release_4E_2(_device.get_EEPROM_Release_4E_2());
        agcadrfSettings.set_EEPROM_SerialNumber_4C_2(_device.get_EEPROM_SerialNumber_4C_2());
        agcadrfSettings.set_EEPROM_VersionNumber_50_1(_device.get_EEPROM_VersionNumber_50_1());

        tuner.setSerialNumber(_device.get_EEPROM_serialNumber()); // Serial number
        tuner.setHWRevisionType(_device.get_EEPROM_version_type());         // HW revision
        tuner.setSoftwareVersion(_device.get_soft_vers());        // Software version
        tuner.setTunerSoftwareRelease(_device.get_release());         // Release
        // FPGA version
        tuner.setReferenceFrequency(_device.get_TSFP_ReferenceFrequency()); // LO Frequency, Hz 

        /*  биты [3:1] = 000 -> Link Up
            биты [3:1] = 001 -> Link Down
            биты [3:1] = 010 -> SFP Module Absent
            биты [3:1] = 011 -> SFP Module Absent
            биты [3:1] = 100 -> Transmission
            биты [3:1] = 101 -> Link Down
            биты [3:1] = 110 -> SFP Module Absent
            биты [3:1] = 111 -> SFP Module Absent   */

        tuner.setEthernetLinkStatus(_device.get_status10GE());
        tuner.setEthernetIPDestination(_device.get_ge_ip_destination());
        tuner.setEthernetDestinationPort(_device.get_ge_destination_port());
        tuner.setEthernetDataPackets(_device.get_ge_data_packets());
    }

    if(fd_UART==0) {
        fd_UART = openUART1();
    } 
    if(fd_UART==0) {
            printf ("Error open UART1");
    } else {
        // printf("OK");
        //strcpy(ini_Uminodiode, "#w1111111111\r\n");
        // writeUART(fd, "#r\r\n");
        char ini_Uminodiode[MAX_LEN_TTY];

        strcpy(ini_Uminodiode, "#w0000000000\r\n");
        ini_Uminodiode[2]='0'; //buzzer
        ini_Uminodiode[3]='2'; //sys - green
        if(_device.get_present()) {
            ini_Uminodiode[4]='2'; //green
        } else {
            ini_Uminodiode[4]='1'; //red
        }

        if(_channelSettings.getTunerSettings().getAGCMode()==1){
            ini_Uminodiode[5]='3'; // orange
        } else {
            if(_device.get_AGC_Lock()) {
                ini_Uminodiode[5]='2'; // Lock
            } else {
                ini_Uminodiode[5]='1'; // red - Unlock
            }
        }

        if(_device.get_status10GE_SFP_MOD_ABSENT()) {
            ini_Uminodiode[10]='1'; // red - SFP Module Absent
            ini_Uminodiode[11]='0';
        } else {
            if(_device.get_status10GE_SFP_LOPC()){
                ini_Uminodiode[10]='3'; // orange - Link Down
                ini_Uminodiode[11]='0'; 
            } else {
                ini_Uminodiode[10]='2'; //green - Link Up
                ini_Uminodiode[11]='2'; 
            }
        }

        writeUART(fd_UART, ini_Uminodiode);
        // printf((char *)ini_Uminodiode);
    }
}



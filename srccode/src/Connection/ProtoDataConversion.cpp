#include "Connection/ProtoDataConversion.hpp"

/*---------------------------------------------------------------------------*/
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Types.h"
#include "Poco/String.h"
using Poco::UInt8;
using Poco::UInt16;

using Poco::DateTimeParser;
using Poco::DateTimeFormatter;
using Poco::trim;

/*---------------------------------------------------------------------------*/
#include <arpa/inet.h>
#include <sstream>
#include <math.h>

using std::string;
/*---------------------------------------------------------------------------*/
//#define MODE_AUTO       0
//#define MODE_MANUAL     1
//#define MODE_ONE_SHOT   3

// Defines for AGC Lock status {1.7}
#define STAT_AGC_UNLOCK                 0x00
#define STAT_AGC_LOCK                   0x01
#define STAT_AGC_MAUAL_GAIN_CONTROL     0x02
// Needed to determine type of agc error (TunerStat::agcError)
#define AGC_LEVEL_TOO_LOW   0x01
#define AGC_LEVEL_TOO_HIGH  0x02



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace 
{
/*
unsigned int getConvert_lpf_to_F3db(unsigned int _lpf){
        unsigned int arrLPF[255];
        unsigned int i = 1;
        // unsigned int val_MIN = 3200000;
        // unsigned int val_MAX = 207200000;
        unsigned int set_val = 3200000;
        for(i=0;i<255;i++) {
            arrLPF[i] = set_val; 
            set_val = set_val + 800000;
            // printf(" set arrLPF[i(%u)] = %u", i, arrLPF[i]);
        }
        printf("arrLPF[_lpf(%u)] = %u\r\n", _lpf, arrLPF[_lpf]);
        return arrLPF[_lpf];
}   
*/
void convertTunerStatMon2Proto(const Statistics::TunerStatistics& inStat, GUI::TunerStat* outStat)
{
//    outStat->set_serialnumber(inStat.getSerialNumber());
//    outStat->set_type(inStat.getType());
//    outStat->set_soft_vers(inStat.getSoftwareVersion()); // TSFP Soft Version
//    outStat->set_referencefrequency(inStat.getReferenceFrequency());
    outStat->set_present(static_cast<unsigned int>(inStat.getPresence()));
    outStat->set_temperature(inStat.getTemperature());
    outStat->set_endofsearch(static_cast<unsigned int>(inStat.getAGCEndOfSearch()));
    if(inStat.getAGCLock()) {
        outStat->set_lock(STAT_AGC_LOCK);
    } else {
        outStat->set_lock(STAT_AGC_UNLOCK);
    }
    if(inStat.getAGCManualGainControl()) {
        outStat->set_lock(STAT_AGC_MAUAL_GAIN_CONTROL);
    }
    outStat->set_agcerror(inStat.getAGCError());
    //outStat->set_referencevoltage(static_cast<float>(inStat.getAGCReferenceVoltage()));
    outStat->set_detectvoltage(inStat.getAGCDetectVoltage());
    outStat->set_lbandgain(static_cast<float>(inStat.getAGCLBandGain()));
    outStat->set_basebandgain(inStat.getAGCBaseBandGain());
    outStat->set_pin(static_cast<float>(inStat.getAGCInputSignalLevel()));
    outStat->set_lpfcutofffrequency(inStat.getLPFCutoffFrequency());
//    outStat->set_maxspectrumanalyzersymbolrate(0);
    outStat->set_agctoolow(inStat.getAGCToo_Low());
    outStat->set_agctoohigh(inStat.getAGCToo_High());
//    outStat->set_init(inStat.getAGCInit());
}

//===============================================================
/*
unsigned int convert_in_lpf(unsigned int _FilterBand, unsigned int _lpfmin, unsigned int _lpfmax){
    unsigned int i = 1;
    unsigned int new_val = 4000000;
    for(i=1;i<_lpfmin;i++) {
        new_val = new_val + 800000;
    }
    for(i=_lpfmin;i<_lpfmax;i++) {
        if(_FilterBand<=new_val) {
            break;
        }
        new_val = new_val + 800000;
    }
    return i;
}
*/
void convertAGCTunerConfigMon2Proto(const Settings::TunerSettings& inConf, GUI::AGCTunerConf* outConf)
{
    outConf->set_frequency(inConf.getFrequency()); // RF Frequency, kHz.  Not Convert Hz to kHz 
    /*
    unsigned int _f3db = inConf.getFilterBand();
    unsigned int _lpf_max = inConf.getTunerLPFMAX();
    unsigned int _lpf_min = inConf.getTunerLPFMIN();
    unsigned int val = getConvert_lpf_to_F3db(_lpf_min);
    unsigned int _lpf = 3200000;
    unsigned int i = 1;
    for(i=_lpf_min;i<_lpf_max;i++) {
        if(val>=_f3db) {
            break;
        }
        val = val + 800000;
    }
    _lpf = i;     
    */
    // FB_INDX = LPF_filter [7:0], ADRF_filter [13:8], FB_mode [15:14],
    // unsigned int FB_index = inConf.getFilterBandIndex();
    // unsigned int LPF_filter = (FB_index&0xFF); 
    // unsigned int ADRF_filter = (FB_index>>8)&3F;
    outConf->set_filterband(inConf.getFilterBandIndex()); 
    outConf->set_agcmode(inConf.getAGCMode()); //Gain Control Mode 
    outConf->set_spectruminversion(inConf.getSpectrumInversion());   // new 02/12/17
    outConf->set_manuallbandgain(inConf.getManualLBandGain());       // L-Band Gain, dB	
    outConf->set_manualbasebandgain(inConf.getManualBasebandGain());    // Baseband Gain, dB
}
/*
void convertMiscConfigMon2Proto(const Settings::MiscSettings& inConf, GUI::MiscChannelConf* outConf)
{
    outConf->set_label(inConf.getLabel());
}
*/
// ======================= new ===============================

void convertEEPROMMon2Proto (const Settings::AGCSettings& inConf, GUI::EEPROM* outConf){
    outConf->set_eepproject(inConf.get_EEPROM_Project_48_1());
    outConf->set_eepreference(inConf.get_EEPROM_Reference_49_3());
    outConf->set_eepserialnumber(inConf.get_EEPROM_SerialNumber_4C_2());
    outConf->set_eeprelease(inConf.get_EEPROM_Release_4E_2());
    outConf->set_eepversionnumber(inConf.get_EEPROM_VersionNumber_50_1());
    outConf->set_eepadrfdac(inConf.get_EEPROM_ADRFDAC_51_3());
    outConf->set_eepagcctr(inConf.get_EEPROM_AGCCTR_54_2());
    outConf->set_eepadrfreg(inConf.get_EEPROM_ADRFREG_56_2());
}

void convertEEPROMProto2Mon (const GUI::EEPROM& inConf, Settings::AGCSettings& outConf){
    /*
    outConf.set_EEPROM_Project_48_1(inConf.eepproject());
    outConf.set_EEPROM_Reference_49_3(inConf.eepreference());
    outConf.set_EEPROM_SerialNumber_4C_2(inConf.eepserialnumber());
    outConf.set_EEPROM_Release_4E_2(inConf.eeprelease());
    outConf.set_EEPROM_VersionNumber_50_1(inConf.eepversionnumber());
    outConf.set_EEPROM_ADRFDAC_51_3(inConf.eepadrfdac());
    outConf.set_EEPROM_AGCCTR_54_2(inConf.eepagcctr());
    outConf.set_EEPROM_ADRFREG_56_2(inConf.eepadrfreg());
    */
    for(unsigned int i = 0; i < 16; ++i){ // inConf.eeparray16().size()
        outConf.set_ArrayEEPROMData(i, inConf.eeparray16(i));
    }
}
/*---------------------------------------------------------------------------*/
void convertAGCTunerConfigProto2Mon(const GUI::AGCTunerConf& inConf, Settings::TunerSettings& outConf)
{
    outConf.setFrequency(inConf.frequency());
    // = 3,2 + 0,8*LPF [7:0] 
    // unsigned int new_band = static_cast<unsigned int>((((inConf.getFilterBand() - 3200000)/1000000)/0.8);
    // unsigned int new_band = static_cast<unsigned int>((3.2 + 0.8*inConf.filterband()) * 1000000);
    unsigned int DB_Index =  inConf.filterband();
    // outConf.setFilterBandIndex(getConvert_lpf_to_F3db(_lpf));
    outConf.setFilterBandIndex(DB_Index);
    outConf.setAGCMode(inConf.agcmode());
    outConf.setManualLBandGain(inConf.manuallbandgain());
    outConf.setManualBasebandGain(inConf.manualbasebandgain());
    outConf.setSpectrumInversion(inConf.spectruminversion());
}

/*---------------------------------------------------------------------------*/
/*
void convertMiscConfigProto2Mon(const GUI::MiscChannelConf& inConf, Settings::MiscSettings& outConf)
{
    outConf.setLabel(inConf.label());
}
*/
void printAGCTunerConfProto(Poco::Logger& logger, const GUI::AGCTunerConf& conf)
{
    poco_information(logger, "AGC & Tuner:");
    poco_information_f1(logger, "    Frequency:                         %u", conf.frequency());
    poco_information_f1(logger, "    FB_index :                         %u ", conf.filterband());     
    poco_information_f1(logger, "         LPF :                         %u ", (conf.filterband()&0xFF));
    poco_information_f1(logger, "        ADRF :                         %u ", (conf.filterband()>>8)&0x3F);
    poco_information_f1(logger, "    Manual LBand gain:                 %u", conf.manuallbandgain());
    poco_information_f1(logger, "    Manual BaseBand gain:              %u", conf.manualbasebandgain());
}

} 
 /*---------------------------------------------------------------------------*/

namespace SNMP
{
    // !!!!!!!!!!!!!!!  DeviceSettings !!!!!!!!!!!!!!
/*
void convertDeviceSettingsMon2Proto(Settings::CommonSettings& inConf, GUI::DeviceSettings& outConf)
{
    Poco::Timestamp currentTime;
    outConf.set_devicename(inConf.getDeviceName());
    outConf.set_dateandtime(currentTime.epochTime());
}

void convertDeviceSettingsProto2Mon(const GUI::DeviceSettings& inConf, Settings::CommonSettings& outConf)
{
    outConf.setDeviceName(inConf.devicename());
}
*/
    // !!!!!!!!!!!!!!!  ChannelConfig !!!!!!!!!!!!!!
void convertChannelConfigMon2Proto(Settings::ChannelSettings& inConf, GUI::ChannelConf& outConf)
{
    convertAGCTunerConfigMon2Proto  (inConf.getTunerSettings(),             outConf.mutable_tuner());
    //  convertMiscConfigMon2Proto      (inConf.getMiscSettings(),              outConf.mutable_misc());
/*
    optional uint32                     AGCCTR         = 10;   //  5C      R/W	(2 байта)
    optional uint32                     AGCUref        = 11;   //  5E      R/W	(2 байта)
    optional uint32                     ADRFREG        = 12;   //  16	   R/W	(2 байта)
    optional uint32                     ADRFDAC        = 13;   //  1C      R/W	(4 байта - NA, VGN1, VGN2, VGN3)
    optional GUI.EEPROM                 eeprom         = 14;   
*/
//    outConf.set_applyadrfconf(0); 
    outConf.set_sendspiflag(0);
    outConf.set_agcctr( inConf.getAGCSettings().get_AGC_CTRL_5C_2());
    outConf.set_agcuref(inConf.getAGCSettings().get_AGC_Uref_5E_2());   
    outConf.set_adrfreg(inConf.getAGCSettings().get_ADRF_REG_16_2());
    outConf.set_adrfdac(inConf.getAGCSettings().get_ADRF_DAC_1C_4());
    convertEEPROMMon2Proto(inConf.getAGCSettings(), outConf.mutable_eeprom());
    outConf.set_decimationrate(inConf.getDecimationSettings().getDecimationRate());
}

void convertChannelConfigProto2Mon(const GUI::ChannelConf& inConf, Settings::ChannelSettings& outConf)
{
    convertAGCTunerConfigProto2Mon           (inConf.tuner(),             outConf.getTunerSettings());
//    convertMiscConfigProto2Mon               (inConf.misc(),              outConf.getMiscSettings());
//    outConf.getAGCSettings().set_ApplyADRFConf(inConf.applyadrfconf());
    outConf.getAGCSettings().set_ApplySendSPIflag(inConf.sendspiflag());
    outConf.getAGCSettings().set_AGC_CTRL_5C_2(inConf.agcctr());
    outConf.getAGCSettings().set_AGC_Uref_5E_2(inConf.agcuref());
    outConf.getAGCSettings().set_ADRF_REG_16_2(inConf.adrfreg());
    outConf.getAGCSettings().set_ADRF_DAC_1C_4(inConf.adrfdac());
    outConf.getDecimationSettings().setDecimationRate(inConf.decimationrate());
    convertEEPROMProto2Mon                  (inConf.eeprom(),       outConf.getAGCSettings());
}
// WORK Ticket #607
// !!!!!!!!!!!!!!!  ChannelStat !!!!!!!!!!!!!!
void convertChannelStatMon2Proto(Statistics::ChannelStatistics& inStat, GUI::ChannelStat& outConf)
{
    convertTunerStatMon2Proto            (inStat.getTunerStatistics(),          outConf.mutable_tuner());
//    outConf.set_leaderstatus(inStat.getLeaderStatus());
//    outConf.set_channelstatus(inStat.getChannelStatus());
}

void printChannelConfigProto(Poco::Logger& logger,const GUI::ChannelConf& conf)
{
    printAGCTunerConfProto(logger, conf.tuner());
//    printMiscConfProto(logger, conf.misc());
//    printTSFPConfProto(logger, conf. .tsfp());
    poco_information(logger, "AGC_ADRF_Conf:");
    poco_information_f1(logger, "    applyadrfcconf:            %u", conf.sendspiflag());
    poco_information_f1(logger, "    REG_AGC_CTR :              %u", conf.agcctr());
    poco_information_f1(logger, "    REG_AGC_UREF:              %u", conf.agcuref());
    poco_information_f1(logger, "    REG_AGC_FREG:              %u", conf.adrfreg());
    poco_information_f1(logger, "    REG_AGC_DAC:               %u", conf.adrfdac());
    poco_information_f1(logger, "    Decimation Rate:           %u", conf.decimationrate());
}

}//namespace SNMP


#pragma once
//#include "agctunersettings.h"
//#include "agctunerstatistics.h"
// #include "interpolate.h"

#include "Common/Tracer.hpp"
#include "Common/Typedefs.hpp"
#include "Device/devmem.h"

#include "Poco/Logger.h"
#include "Poco/Mutex.h"

class AGCTunerControl
{
public:
    AGCTunerControl();

    int configureApplyADRFConf();
    int configureAGCTuner();
    int configure10GE();
    int configureResampler();

    void collectAllStatisticAGCTuner(unsigned int status);
    void collectStatistic10GE();
    void spi_write_EEPROM();
    void set_EventLog(bool val){
        _eventLog = val;
    }

    // ------------ AGC & Tuner constatns ------------------------------------

    void set_Tuner_Fmin(unsigned int _data)
    {
        Tuner_Fmin = _data;
    }
    void set_Tuner_Fmax(unsigned int _data)
    {
        Tuner_Fmax = _data;
    }
    void set_Tuner_LPFmin(unsigned int _data)
    {
        Tuner_LPFmin = _data;
        MinLBand =  static_cast<unsigned int>((3.2 + 0.8*Tuner_LPFmin) * 1000000);
    }
    void set_Tuner_LPFmax(unsigned int _data)
    {
        Tuner_LPFmax = _data;
        MaxLBand =  static_cast<unsigned int>((3.2 + 0.8*Tuner_LPFmax) * 1000000);
    }
    // ------------ Tuner immutable ------------------------------------
    void set_InitialGain2(unsigned int _data)
    {
        ImInitialGain2 = _data;
    }
    void set_InitialGain1(unsigned int _data)
    {
        ImInitialGain1 = _data;
    }
   
    // ------------ AGCTunerConf ------------------------------------
    unsigned int get_frequency() const{
        if(_frequency<Tuner_Fmin) {
            return Tuner_Fmin;
        }
        if(_frequency>Tuner_Fmax) {
            return Tuner_Fmax;
        }
        return _frequency;
    };
    void set_frequency(unsigned int _data){
        if(_data<Tuner_Fmin) {
            _data = Tuner_Fmin;
        }
        if(_data>Tuner_Fmax) {
            _data = Tuner_Fmax;
        }
        _frequency = _data;
    }

    unsigned int get_filterBand() const{
        return _filterBandIndex;
    };
    void set_filterBand(unsigned int _data){
        _filterBandIndex = _data;
    }

    unsigned int get_agcMode() const{
        if(_agcMode>3) {
            return 0;
        }
        if(_agcMode==2) {
            return 0;
        }
        return _agcMode;
    };
    void set_agcMode(unsigned int _data){
        if(_data>3) {
            _data = 0;
        }
        _agcMode = _data;
    }

    void set_spectrumInversion(unsigned int _data){
        if(_data>1) {
            _data = 1;
        }
        _spectrumInversion = _data;
    }
    unsigned int get_spectrumInversion() const{
        if(_spectrumInversion>1) {
            return 1;
        }
        return _spectrumInversion;
    };

    unsigned int get_agc_manualLBandGain() const{
        if(_manualLBandGain==0) {
            return 1;
        }
        if(_manualLBandGain>354) {
            return 354;
        }
        return _manualLBandGain;
    };
    void set_agc_manualLBandGain(unsigned int _data){
        if(_data==0) {
            _data = 1;
        }
        if(_data>354) {
            _data = 354;
        }
        _manualLBandGain = _data;
    }

    unsigned int get_agc_manualBaseBandGain() const{
        if(_manualBaseBandGain==0) {
            return 1;
        }
        if(_manualBaseBandGain>15) {
            return 15;
        }
        return _manualBaseBandGain;
    };
    void set_agc_manualBaseBandGain(unsigned int _data){
        if(_data==0) {
            _data = 1;
        }
        if(_data>15) {
            _data = 15;
        }
        _manualBaseBandGain = _data;
    }

    // ------------------------------------------------
    std::string get_soft_vers(){
        return soft_vers;
    }
    std::string get_release(){
        return release;
    }

    // ------------ AGCADRFConf ------------------------------------

    unsigned int get_present(){
        return present;
    }
    unsigned int get_TSFP_ReferenceFrequency(){
        return TSFP_ReferenceFrequency;
    }
    unsigned int get_EEPROM_serialNumber(){
        return EEPROM_serialNumber;
    }
    unsigned int get_EEPROM_version_type(){
        return EEPROM_version_type;
    }
    unsigned int get_TSFP_Temperature(){
        return TSFP_Temperature;
    }
    unsigned int get_AGC_EndOfSearch(){
        return AGC_EndOfSearch;
    }
    unsigned int get_AGC_Lock(){
        return AGC_Lock;
    }
    unsigned int get_AGC_Unlock(){
        return AGC_Unlock;
    }
    unsigned int get_AGC_Manual(){
        return AGC_ManualMode;
    }
    unsigned int get_AGC_Error(){
        return AGC_Error;
    }
    unsigned int get_TSFP_DetectVoltage(){ // PeakDetect
        return TSFP_DetectVoltage;
    }
    unsigned int get_TSFP_GC1_lBandGain(){
        return TSFP_GC1_lBandGain; // _db
    }
    unsigned int get_TSFP_GC2_baseBandGain(){
        return TSFP_GC2_baseBandGain_db;
    }
    int get_Pin(){
        return Pin;
    }
    unsigned int get_lpfCutoffFrequency(){
        return lpfCutoffFrequency;
    }
    bool get_AGC_TooLow(){
        return AGC_TooLow;
    }
    bool get_AGC_TooHigh(){
        return AGC_TooHigh;
    }
    unsigned int get_AGC_Init(){
        return AGC_Init;
    }

    unsigned int get_AGC_trig(){
        //   ("AGC Block Trig") 0: Disable, 1: fixUnlock(default), 2: forcLock
        if(_AGC_trig>2) {
           return 0;
        }
        return _AGC_trig;
    }
    unsigned int get_AGC_dLim(){
        // AGC_dLim:2;  // [11:10]   0bxxxx11xxxxxxxxxx = 1/64..1/8 Uref                           // debug             "AGC Limits"
        if(_AGC_dLim>3) {
           return 0;
        }
        return _AGC_dLim;
    }
    unsigned int get_VPK_delay(){
        // [14:12]   0bx111xxxxxxxxxxxx = интервал=25<<VPK_delay=25..3200ms        // debug             "Delay"
        if(_VPK_delay>7) {
           return 0;
        }
        return _VPK_delay;
    }
    unsigned int get_AGC_DAC(){
        // _AGC_DAC = 0;
        return 0;
    }
    unsigned int get_DC_offs(){
        // [1]     0bxxxxxxxxxxxxxx1x   0: enable, 1: disable   
        if(_DC_offs>1) {
           return 0;
        }
        return _DC_offs;
    }
    unsigned int get_PostAmp(){  
        // [2]     0bxxxxxxxxxxxxx1xx   0: 3dB,  1: 9dB                                                "PostAmp"
        if(_PostAmp>1) {
           return 0;
        }
        return _PostAmp;
    }
    unsigned int get_VGA3(){     
        // [4:3]   0bxxxxxxxxxxx11xxx   00:21dB, 01:18dB, 10:15dB, 11:12dB                             "VGA3"
        if(_VGA3>3) {
           return 0;
        }
        return _VGA3;
    }
    unsigned int get_VGA2(){    
        // [6:5]   0bxxxxxxxxx11xxxxx   00:21dB, 01:18dB, 10:15dB, 11:12dB                             "VGA2"
        if(_VGA2>3) {
           return 0;
        }
        return _VGA2;
    }
    unsigned int get_VGA1(){ 
        // [8:7]   0bxxxxxxx11xxxxxxx   00:15dB, 01:12dB, 10: 9dB, 11: 9dB                             "VGA1"
        if(_VGA1>3) {
           return 0;
        }
        return _VGA1;
    }
    unsigned int get_PwrMode(){  
        // [9]     0bxxxxxx1xxxxxxxxx   0: low power, 1: high power;                                   "Power"
        if(_PwrMode>1) {
           return 0;
        }
        return _PwrMode;
    }
    unsigned int get_Filter(){  
        // [15:10] 0b111111xxxxxxxxxx   =Code: 1 dB corner in MHz;                                     "Filter"
        if(_Filter>63) {
           return 0;
        }
        return _Filter;
    }
    unsigned int get_VGN3(){
        // [7:0]    подается на VDAC8_4                                                                 "VGN3"
        if(_VGN3>255) {
           return 0;
        }
        return _VGN3;
    }
    unsigned int get_VGN2(){
        // [15:8]   подается на VDAC8_3                                                                 "VGN2"
        if(_VGN2>255) {
           return 0;
        }
        return _VGN2;
    }     
    unsigned int get_VGN1(){
        // [23:16]  подается на VDAC8_2                                                                 "VGN1"
        if(_VGN1>255) {
           return 0;
        }
        return _VGN1;
    } 
    unsigned int get_AGC_Uref(){
        // [31:0]   UrefRMS             AGC_Uref
        if(_AGC_Uref<70) {
           return 70;
        }else {
            if(_AGC_Uref>1700) {
                return 1700;
            }
        }
        return _AGC_Uref;
    }

    void set_AGC_trig(unsigned int _data){
        _AGC_trig = _data;
    }
    void set_AGC_dLim(unsigned int _data){
        _AGC_dLim = _data;
    }
    void set_VPK_delay(unsigned int _data){
        _VPK_delay = _data;
    }
    void set_AGC_DAC(unsigned int _data){
        _AGC_DAC = _data;
    }
    void set_DC_offs(unsigned int _data){  
        _DC_offs = _data;
    }
    void set_PostAmp(unsigned int _data){
        _PostAmp = _data;
    }
    void set_VGA3(unsigned int _data){
        _VGA3 = _data;
    }
    void set_VGA2(unsigned int _data){
        _VGA2 = _data;
    }
    void set_VGA1(unsigned int _data){     
        _VGA1 = _data;
    }
    void set_PwrMode(unsigned int _data){
        _PwrMode = _data;
    }
    void set_Filter(unsigned int _data){
        _Filter = _data;
    }
    void set_VGN3(unsigned int _data){
        _VGN3 = _data;
    }
    void set_VGN2(unsigned int _data){
        _VGN2 = _data;
    }
    void set_VGN1(unsigned int _data){
        _VGN1 = _data;
    }
    void set_AGC_Uref(unsigned int _data){ 
        _AGC_Uref = _data;
    }
    void set_KOEF_for_Pin(float _data){ 
        _koef_for_Pin = _data;
    }

// 10 GE =========================================
    bool                            get_status10GE_SFP_TX_FAULT(){
        return _status10GE_SFP_TX_FAULT;
    }
    bool                            get_status10GE_SFP_LOPC(){
        return _status10GE_SFP_LOPC;
    }
    bool                            get_status10GE_SFP_MOD_ABSENT(){
        return _status10GE_SFP_MOD_ABSENT;
    }
    bool                            get_status10GE_Transmision(){
        return _status10GE_SFP_MOD_ABSENT;
    }

    unsigned int  get_status10GE(){
        return _status10GE;
    }

    unsigned int  get_ge_ip_destination(){
        return ge_ip_destination;
    }
    
    unsigned int  get_ge_destination_port(){
        return ge_destination_port;
    }

    unsigned int  get_ge_data_packets(){
        return ge_data_packets;
    }
// ======================= AGC ADRF DATA  ========================
    unsigned int get_AGC_CTRL_5C_2() const
    {
        return reg_AGC_CTRL_5C_2;
    };
    void set_AGC_CTRL_5C_2(unsigned int data){ 
        reg_AGC_CTRL_5C_2 = data;
    };

    unsigned int get_AGC_Uref_5E_2() const
    {
        return reg_AGC_Uref_5E_2;
    };
    void set_AGC_Uref_5E_2(unsigned int data){ 
        reg_AGC_Uref_5E_2 = data;
    };

    unsigned int get_ADRF_REG_16_2() const
    {
        return reg_ADRF_REG_16_2;
    };
    void set_ADRF_REG_16_2(unsigned int data){ 
        reg_ADRF_REG_16_2 = data;
    };

    unsigned int get_ADRF_DAC_1C_4() const
    {
        return reg_ADRF_DAC_1C_4;
    };
    void set_ADRF_DAC_1C_4(unsigned int data){ 
        reg_ADRF_DAC_1C_4 = data;
    };
// ============ EEPROM REG =====================
    unsigned int get_EEPROM_Project_48_1() const
    {
        return reg_EEPROM_Project_48_1;
    };

    unsigned int get_EEPROM_Reference_49_3() const
    {
        return reg_EEPROM_Reference_49_3;
    };

    unsigned int get_EEPROM_SerialNumber_4C_2() const
    {
        return reg_EEPROM_SerialNumber_4C_2;
    };

    unsigned int get_EEPROM_Release_4E_2() const
    {
        return reg_EEPROM_Release_4E_2;
    };

    unsigned int get_EEPROM_VersionNumber_50_1() const
    {
        return reg_EEPROM_VersionNumber_50_1;
    };

    unsigned int get_EEPROM_ADRFDAC_51_3() const
    {
        return reg_EEPROM_ADRFDAC_51_3;
    };

    unsigned int get_EEPROM_AGCCTR_54_2() const
    {
        return reg_EEPROM_AGCCTR_54_2;
    };

    unsigned int get_EEPROM_ADRFREG_56_2() const
    {
        return reg_EEPROM_ADRFREG_56_2;
    };

    unsigned int get_ApplyADRFConf() const
    {
        return flag_ApplyADRFConf;
    };

    void set_DecimationRate(unsigned int data){ 
        _decimationRate = data;
    };
    void set_EEPROM_Array16Data(unsigned int nom, unsigned int data){ 
        _Array16Data[nom] = data;
    };

private:
    unsigned long spi_read_write_universal(unsigned int _command, unsigned int _datalen, bool status_write, unsigned int data);

    void manualModeRewrite();

    unsigned int Tuner_Fmax, Tuner_Fmin;        // immutable; Tuner.Fmin / Tuner.Fmax : Minimal / maximal tuner frequency, Hz;
    unsigned int Tuner_LPFmin, Tuner_LPFmax;    // immutable; Tuner.LPFmin / Tuner.LPFmax:: Beginning / End of the list LPF band according 
    unsigned int MaxLBand, MinLBand;
    unsigned int _frequency;
    unsigned int _filterBandIndex;
    unsigned int _spectrumInversion;
    unsigned int _agcMode;
    unsigned int _manualLBandGain;
    unsigned int _manualBaseBandGain;
    unsigned int _decimationRate;

    unsigned int ImInitialGain2;
    unsigned int ImInitialGain1;
    unsigned int serial;
    std::string release;
    std::string soft_vers;
    std::string init_EEPROM;


//0x2  message AGCADRFConf
    unsigned int _AGC_trig; // 0bxxxxxx11xxxxxxxx   // [9:8]    ("AGC Block Trig") 0: Disable, 1: fixUnlock(default), 2: forcLock
    unsigned int _AGC_dLim; // 0bxxxx11xxxxxxxxxx   // [11:10]  ("AGC Hysteresis") 0: 1/8(default), 1: 1/16, 2: 1/32, 3: 1/64
    unsigned int _VPK_delay;// 0bx111xxxxxxxxxxxx   // [14:12]  ("VPK Rd Delay") 25 << 0..7 (25..3200ms) // default=2 (100ms)
    unsigned int _AGC_DAC;  // 0b1xxxxxxxxxxxxxxx   // [15]     (новое поле) GC1 or VGN1 (default 0: GC1) // !! ВСЕГДА = 0
    unsigned int _DC_offs;  // 0bxxxxxxxxxxxxxx1x   // 0: enable (default), 1: disable
    unsigned int _PostAmp;  // 0bxxxxxxxxxxxxx1xx   // 0: 3 dB (default), 1: 9 dB
    unsigned int _PostAmp_db;
    unsigned int _VGA3;     // 0bxxxxxxxxxxx11xxx   // 00: 21 dB, 01: 18 dB, 10: 15 dB, 11: 12 dB (default)
    unsigned int _VGA2;     // 0bxxxxxxxxx11xxxxx   // 00: 21 dB, 01: 18 dB, 10: 15 dB, 11: 12 dB (default)
    unsigned int _VGA1;     // 0bxxxxxxx11xxxxxxx   // 00: 15 dB, 01: 12 dB (default), 10: 9 dB, 11: 9 dB
    unsigned int _PwrMode;  // 0bxxxxxx1xxxxxxxxx   // 0: low power (default), 1: high power;
    unsigned int _Filter;   // 0b111111xxxxxxxxxx   // Code = 1 dB corner in MHz; 000000 - filter bypass mode (default)
    unsigned int _VGN3;     // 0b00000000000000000000000011111111 (default=105)
    unsigned int _VGN2;     // 0b00000000000000001111111100000000 (default=105)
    unsigned int _VGN1;     // 0b00000000111111110000000000000000 (default=215)
    unsigned int _AGC_Uref; // !!! только биты [11:0] (default=800)


//0x4  message TunerStat
        // optional  uint32 present;
    unsigned int present; 
    unsigned int present_old_status; 
        //    optional  uint32 referenceFrequency            = 2;
    unsigned int TSFP_ReferenceFrequency;
        //   optional  uint32 serialNumber                  = 3;
    unsigned int EEPROM_serialNumber;
        //    optional  uint32 type                          = 4;
    unsigned int EEPROM_version_type;
        //    optional  uint32 temperature                   = 5;
    unsigned int TSFP_Temperature;
        //    optional  uint32 endOfSearch                   = 6;
    unsigned int AGC_EndOfSearch;
        //    optional  uint32 lock                          = 7;
    unsigned int AGC_Lock;
    unsigned int AGC_Unlock;
    unsigned int AGC_ManualMode;
        //    optional  uint32 agcError                      = 8;
    unsigned int AGC_Error;
        //    optional  float  referenceVoltage              = 9;
    unsigned int TSFP_DetectVoltage;                     // PeakDetect
        //    optional  float  lBandGain                     = 10;
    unsigned int TSFP_GC1_lBandGain;
    unsigned int TSFP_GC1_lBandGain_db;
        //    optional  uint32 baseBandGain                  = 11;
    unsigned int TSFP_GC2_baseBandGain_db;
        //    optional  float  pin                           = 12;
    int Pin;
        //    optional  uint32 lpfCutoffFrequency            = 13;
    unsigned int lpfCutoffFrequency;

        //    optional  bool   agcTooLow                     = 15;
    bool AGC_TooLow; 
        //    optional  bool   agcTooHigh                    = 16;
    bool AGC_TooHigh;
        //    optional  uint32 init                          = 17;
    unsigned int AGC_Init;
    unsigned int _VGA1_db, _VGA2_db, _VGA3_db;
    float   _koef_for_Pin;

    unsigned int flag_ApplyADRFConf;
    unsigned int reg_TUNER_Reference_28_4;
    unsigned int reg_TSFP_Analog_0E_2;
    unsigned int reg_AGC_STS_1A_2;
    unsigned int reg_PeakDetect_14_2;
    unsigned int reg_DACoutput_10_2;
    unsigned int reg_F3db_30_4;
    unsigned int reg_FB_INDX_18_2;

    unsigned int reg_AGC_CTRL_5C_2;
    unsigned int reg_AGC_Uref_5E_2;
    unsigned int reg_ADRF_REG_16_2;
    unsigned int reg_ADRF_DAC_1C_4;

    unsigned int reg_EEPROM_Project_48_1;
    unsigned int reg_EEPROM_Reference_49_3;
    unsigned int reg_EEPROM_SerialNumber_4C_2;
    unsigned int reg_EEPROM_Release_4E_2;
    unsigned int reg_EEPROM_VersionNumber_50_1;
    unsigned int reg_EEPROM_ADRFDAC_51_3;
    unsigned int reg_EEPROM_AGCCTR_54_2;
    unsigned int reg_EEPROM_ADRFREG_56_2;
    unsigned int _Array16Data[16]={0xFF};

//0x7  ALL
    unsigned int                    _status10GE;
    bool                            _status10GE_SFP_TX_FAULT;
    bool                            _status10GE_SFP_LOPC;
    bool                            _status10GE_SFP_MOD_ABSENT;
    bool                            _status10GE_Transmision;
    unsigned int                    ge_ip_destination;
    unsigned int                    ge_destination_port;
    unsigned int                    ge_data_packets;

    Poco::Logger&                   _logger;
    bool                            _eventLog;
    Poco::Mutex                     mutexSPI;
    unsigned int                    _time_out;
    DeviceIO::MemoryDevice          devmem;
};

// #endif // AGCTUNERCONTROL_H

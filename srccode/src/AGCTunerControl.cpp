#include "AGCTunerControl.hpp"
#include "Common/Defines.hpp"
#include "Common/Defines.hpp"
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
#include "Poco/Logger.h"

using Poco::Logger;
using Poco::ScopedLock;
using Poco::Mutex;
/*---------------------------------------------------------------------------*/
#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
  __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
/*
#define MODE_AUTO       0
#define MODE_MANUAL     1
#define MODE_ONE_SHOT   3
*/
// #define MAP_SIZE 0x100000
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define LPF_CUTOFF_LOWER_THRESHOLD  1
#define LPF_CUTOFF_UPPER_THRESHOLD  251

#define BIT_START           0x00000001 << 0
#define BIT_SINGLE          0x00000001 << 1
#define BIT_BLOCK_UNLOCK    0x00000001 << 2
#define BIT_MANUAL_GC       0x00000001 << 3
#define BIT_DISABLE_TSFP_RD 0x00000001 << 4
// #define BIT_MEASURE_POTRION 0x00000001 << 16

#define BIT_END_OF_SEARCH   0x00000001 << 0
#define BIT_LOCK            0x00000001 << 1
#define BIT_MANUAL_MODE     0x00000001 << 2
#define BIT_ERROR_TOO_LOW   0x00000001 << 28
#define BIT_ERROR_TOO_HIGH  0x00000001 << 29
#define BIT_ERROR           0x00000001 << 30

// #define MASK_DATA_PACKETS_SPECTRAL_INVER    0x20
#define MASK_DATA_PACKETS_TRANSMISION       0x80000000

#define ADC_GAIN                    3.5F
#define INPUT_IMPENDANCE_MISMATCH   26.0F

//static float interpolationArgs[] = { 46.0F, 63.0F, 231.0F, 390.0F};
//static float interpolationVals[] = { 70.0F, 67.0F, 30.0F,  0.0F };

//static float bkwrdInterpCoeff = 5.57; // 390/70
// static float bkwrdInterpCoeff = 5.05714; // 354/78
static float bkwrdInterpCoeff = 4.5384; // 354/78
#define MAX_DB_GC1                  84.4

/*---------------------------------------------------------------------------*/
using namespace std;
/*---------------------------------------------------------------------------*/
template <typename T, size_t N>
inline
size_t sizeOfArray( const T(&)[ N ] )
{
  return N;
}
unsigned int backwardInterpolate(unsigned int upperVal, float coeff, unsigned int data)
{
    return static_cast<unsigned int>(static_cast<float>(upperVal - data) * coeff);
}
/*---------------------------------------------------------------------------*/
/*
static inline unsigned short getHiWord(const unsigned int& dword)
{
    return static_cast<unsigned short>((dword >> 16) & 0xFFFF);
}

template <typename T>
unsigned char getByte(const T& dst, const unsigned char byteNum)
{
    return *(reinterpret_cast<const unsigned char*>(&dst) + byteNum);
}

template <typename T>
void setByte(T& dst, const unsigned char& src, const unsigned char byteNum)
{
    *(reinterpret_cast<unsigned char*>(&dst) + byteNum) = src;
}
*/
//===========================================================================
AGCTunerControl::AGCTunerControl()
:_logger(Logger::get("Monitor.AGCTunerControl"))
{
    _frequency  = 0;
    _filterBandIndex = 76;
    _agcMode    = 0;
    _manualLBandGain = 0;
    _manualBaseBandGain = 0;

    ImInitialGain2 = 0;
    ImInitialGain1 = 0;

    soft_vers = "TSFP Soft Versio";
    release = "24/04/1991";
    MinLBand =   3200000;
    MaxLBand = 199200000;
     _AGC_trig  = 0;
     _AGC_dLim  = 0;
     _VPK_delay = 0;
     _AGC_DAC   = 0;
     _DC_offs   = 0;
     _PostAmp   = 0;
     _PostAmp_db = 3;
     _VGA3      = 0;
     _VGA2      = 0;
     _VGA1      = 0;
     _PwrMode   = 0;
     _Filter    = 0;
     _VGN3      = 105;
     _VGN2      = 105;
     _VGN1      = 215;
     _AGC_Uref  = 800;
     _koef_for_Pin = 10.5;
     _VGA1_db = _VGA2_db = _VGA3_db = 12;
     present                    = 3; 
     present_old_status         = 3;
     TSFP_ReferenceFrequency    = 0;
     EEPROM_serialNumber        = 0;
     EEPROM_version_type        = 0;
     TSFP_Temperature = 0;
     AGC_EndOfSearch = 0;
     AGC_Lock = 0;
     AGC_Unlock = 0;
     AGC_ManualMode = 0;
     AGC_Error = 0;

     TSFP_DetectVoltage = 0;
     TSFP_GC1_lBandGain = 0;
     TSFP_GC1_lBandGain_db = 0;
     TSFP_GC2_baseBandGain_db = 0;
    Pin=0;
    lpfCutoffFrequency=0;
    AGC_TooLow=false; 
    AGC_TooHigh=false;
    AGC_Init=0;
    _eventLog = false;
    _time_out =0; 

    ge_ip_destination=1025;
    ge_destination_port=0;
    ge_data_packets=0;

    flag_ApplyADRFConf = 0;
    reg_TUNER_Reference_28_4 = 1;
    reg_TSFP_Analog_0E_2 = 1;
    reg_AGC_STS_1A_2 = 1;
    reg_PeakDetect_14_2 = 1;
    reg_DACoutput_10_2 = 1;
    reg_F3db_30_4 = 1;
    reg_FB_INDX_18_2 = 1;
    reg_AGC_CTRL_5C_2 = 1;
    reg_AGC_Uref_5E_2 = 1;
    reg_ADRF_REG_16_2 = 1;
    reg_ADRF_DAC_1C_4 = 1;

}

//===========================================================================
//                      spi_read_write_universal
//===========================================================================
unsigned long AGCTunerControl::spi_read_write_universal(unsigned int _command, unsigned int _datalen, bool status_write, unsigned int data)
{
    ScopedLock<Mutex> lock(mutexSPI);

    unsigned int mutex_spi = 0;
    unsigned int cnt = 0;
    do {
        mutex_spi = devmem.dev_mem_read(ADDRESS_DEBUG_CTRL);
        if(mutex_spi==1) {
            usleep(500);
            cnt++;
            if(cnt==1000) {
                poco_debug(_logger, "Error! Status SPI is BUSY. ");
                return 0;
            }
        }
    } while (mutex_spi);
    mutex_spi = 1;
    devmem.dev_mem_write(ADDRESS_DEBUG_CTRL, mutex_spi);

/* 
//----------- MAX2112 -------------------------- 
b1      N_Divider_High      00		R/W
b2      N_Divider_Low       01		R/W
b3      Charge_Pump         02		R/W
b4      F_Divider_High      03		R/W
b5      F_Divider_Low       04		R/W
b6      XTAL_Divider        05		R/W
b7      PLL_Reg             06		R/W
b8      VCO_Reg             07		R/W
b9      LPF_Reg             08		R/W
b10     Control             09		R/W
b11     Shutdown            0A		R/W
b12     Test                0B		R/W
b13     Status_High         0C		R
b14     Status_Low          0D		R 
//----------- TSFP ----------------------------- 
TSFP_soft:
1) DACspecif.DAC +/-; DACspecif.BBG +/-;
2) DACfspecif.BBG -> DACoutput.BBG -> MAX2112.ExtControl.BBG;
3) DACspecif.DAC -> DACoutput.DAC -> TSFP_Analog.DACvalue; VDAC8_1_SetValue(TSFP_Analog.DACvalue/2);
w11     TSFP_Analog         0E		R               (struct: Temperature:7 (0b1111111000000000), DACvalue:9 (0b0000000111111111) )
w12     DACoutput           10		R               (struct: BBG:4 (0bxx1111xxxxxxxxxx), DAC:9 (0bxxxxxxx111111111) ) // получается после калькуляции DACspecif
w13     DACspecif           12		R/W	(2 байта)   (struct: BBG:4 (0bxx1111xxxxxxxxxx), DAC:9 (0bxxxxxxx111111111) )
w14     PeakDetect			14		R	(2 байта)
w15     ADRF_REG            16		R/W	(2 байта)
w16     FB_INDX             18      R/W	(2 байта)
w17     AGC_STS             1A		R	(2 байта)
d18     ADRF_DAC			1C		R/W	(4 байта - NA, VGN1, VGN2, VGN3)
d20     MAX_F_trunc			20		R	(4 байта)
d22     MAX_F_remain        24		R	(4 байта)
d24     Reference           28		R	(4 байта fref CONST)
d26     Frequency           2C		R/W	(4 байта)   ("RF Frequency, kHz")
d28     F3db                30		R/W	(4 байта)   ("Filter Band, MHz")
d30     Reserved30          34		R/W	(4 байта)
str1    TSFP_version        38		R	(16 байт "W-01.09/13.09.17")
str2    EEPROM              48      R/W (16 байт)
w48     WordReg48           58      R	(2 байта)
w49     WordReg49           5A      R/W	(2 байта)
w50     AGC_CTR             5C      R/W	(2 байта)
w51     AGC_Uref            5E      R/W	(2 байта)
d52     Reg12_Long52        60		R	(4 байта)
d54     Reg13_Long54        62		R/W	(4 байта)
d56     Reg14_Long56        64		R/W	(4 байта)
d58     Reg15_Long58        66		R/W	(4 байта)
d60     Reg16_Long60        68		R/W	(4 байта)
d62     Reg17_Long62        6A		R/W	(4 байта)
*/
   unsigned int write_date =0;
   unsigned long result_data = 0, read_result=0;
   unsigned long _shift = 0, result_shift = 0;

   unsigned int SPIdataHeader = _command;
   if(status_write) {
       SPIdataHeader += 0x80;
   }
   devmem.dev_mem_write(MAIN_ADDRESS, SPI1_MAIN_ADDRESS);
   devmem.dev_mem_write(MAIN_ADDRESS, (SPI1_MAIN_ADDRESS + SPIdataHeader));
   unsigned int i =0;
   if(status_write==false){ // FF FF - пустые байты
       write_date = SPI1_MAIN_ADDRESS + 0xFF;
       devmem.dev_mem_write(MAIN_ADDRESS, write_date);
       write_date = SPI1_MAIN_ADDRESS + 0xFF;
       devmem.dev_mem_write(MAIN_ADDRESS, write_date);
   }
   _shift = 8*_datalen;
   for(i=0;i<_datalen;i++) {
       if(status_write){
           if(_shift>=8) {
               _shift -= 8;
           }
           write_date = SPI1_MAIN_ADDRESS + ((data&(0xFF<<_shift))>>_shift);
           // printf("write_date =  0x%lX\n", (write_date&0xFF));
       } else {
           write_date = SPI1_MAIN_ADDRESS + 0xFF;
       }
       devmem.dev_mem_write(MAIN_ADDRESS, write_date);
       read_result = devmem.dev_mem_read(MAIN_ADDRESS_SH8);
       if(status_write==false) {
           result_data = (result_data<<result_shift) + (read_result&0xFF);
           // printf("     result_data =  0x%lX, read_result =  0x%lX , result_shift =%u\n", result_data, (read_result&0xFF), result_shift );
           result_shift = 8;
      }
   }

   devmem.dev_mem_write(MAIN_ADDRESS, 0x00000); 
/*
   if(status_write==false) {
        printf(" Result =  0x%lX (%lu)\n", result_data, result_data);
   }
*/
   mutex_spi = 0;
   devmem.dev_mem_write(ADDRESS_DEBUG_CTRL, mutex_spi);
   return result_data;
}

void AGCTunerControl::spi_write_EEPROM()
{
   ScopedLock<Mutex> lock(mutexSPI);
   unsigned int write_data = 0;
   unsigned int SPIdataHeader = 0xC8;
   devmem.dev_mem_write(MAIN_ADDRESS, SPI1_MAIN_ADDRESS);
   //devmem.dev_mem_write(MAIN_ADDRESS, (SPI_LOW_SPEED + SPI1_MAIN_ADDRESS + SPIdataHeader));
   devmem.dev_mem_write(MAIN_ADDRESS, (SPI1_MAIN_ADDRESS + SPIdataHeader));
   for(unsigned int i=0;i<16;i++) {
       //write_data = SPI_LOW_SPEED + SPI1_MAIN_ADDRESS + _Array16Data[i];
       write_data = SPI1_MAIN_ADDRESS + _Array16Data[i];
//       poco_debug_f3(_logger, "get write_data = 0x%X, _Array16Data[%u]=%X", write_data, i, _Array16Data[i]);
       devmem.dev_mem_write(MAIN_ADDRESS, write_data);
       devmem.dev_mem_read(MAIN_ADDRESS_SH8);
   }

   devmem.dev_mem_write(MAIN_ADDRESS, 0x00000); 
}
//===========================================================================
//=========== GET and SET values functions   ================================
//===========================================================================
void AGCTunerControl::manualModeRewrite()
{
    unsigned int data=0;
    // tuner_control -w -a 0x12 2 0x1C82
    data = spi_read_write_universal(ADDR_DACoutput_10, 2, false, 0);;
    spi_read_write_universal(0x12, 2, true, data);
}
//================================================================================
//                  configureApplyADRFConf
//================================================================================
int AGCTunerControl::configureApplyADRFConf(){
    unsigned int result = 0;
    result = spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, true, reg_AGC_CTRL_5C_2);
    result = spi_read_write_universal(ADDR_AGC_Uref_5E, 2, true, reg_AGC_Uref_5E_2);
    result = spi_read_write_universal(ADDR_ADRF_REG_16, 2, true, reg_ADRF_REG_16_2);
    result = spi_read_write_universal(ADDR_ADRF_DAC_1C, 4, true, reg_ADRF_DAC_1C_4);

    // result = spi_read_write_universal(ADDR_EEPROM_48, 1, true, reg_EEPROM_Project_48_1);
    result = spi_read_write_universal(ADDR_EEPROM_Reference_49, 3, true, reg_EEPROM_Reference_49_3);

    result = spi_read_write_universal(ADDR_EEPROM_SerialNumber_4C, 2, true, reg_EEPROM_SerialNumber_4C_2);
    result = spi_read_write_universal(ADDR_EEPROM_Release_4E , 2, true, reg_EEPROM_Release_4E_2);

    result = spi_read_write_universal(ADDR_EEPROM_VersionNumber_50, 1, true, reg_EEPROM_VersionNumber_50_1);
    result = spi_read_write_universal(ADDR_EEPROM_ADRFDAC_51, 3, true, reg_EEPROM_ADRFDAC_51_3);

    result = spi_read_write_universal(ADDR_EEPROM_AGCCTR_54, 2, true, reg_EEPROM_AGCCTR_54_2);
    result = spi_read_write_universal(ADDR_EEPROM_ADRFREG_56, 2, true, reg_EEPROM_ADRFREG_56_2 );
    return result;
}

//================================================================================
//                  configureAGCTuner
//================================================================================
int AGCTunerControl::configureAGCTuner()
{
    // TRACE();
    unsigned int data=0;
    unsigned int AGC_CTR =0;
    unsigned int _AGC_start =0;
    if(present==0) {
        poco_debug(_logger, "Warning! The configuration did not apply! No tuner!");
        return 1;
    } else {
        poco_debug(_logger, "Tuner is present and ready");
    }
    // AGC_CTR = immutable.cfg (AGC.DAC, AGC.VPK_delay, AGC.dLim, AGC.trigA) + AGC_mode=0, AGC_start=0
    data = 0;
    data = data + (_AGC_DAC<<15);           // 0b1xxxxxxx xxxxxxxx   // [15]     (новое поле) GC1 or VGN1 (default 0: GC1) // !! ВСЕГДА = 0
        poco_debug_f2(_logger, "data = 0x%X, _AGC_DAC =%u", data, _AGC_DAC);
    data = data + ((_VPK_delay&0x7)<<12);   // 0bx111xxxx xxxxxxxx   // [14:12]  ("VPK Rd Delay") 25 << 0..7 (25..3200ms) // default=2 (100ms)
        poco_debug_f2(_logger, "data = 0x%X, _VPK_delay =%u", data, _VPK_delay);
    data = data + ((_AGC_dLim&0x3)<<10);    // 0bxxxx11xx xxxxxxxx   // [11:10]  ("AGC Hysteresis") 0: 1/8(default), 1: 1/16, 2: 1/32, 3: 1/64
        poco_debug_f2(_logger, "data = 0x%X, _AGC_dLim =%u", data, _AGC_dLim);
    data = data + ((_AGC_trig&0x3)<<8);     // 0bxxxxxx11 xxxxxxxx   // [9:8]    ("AGC Block Trig") 0: Disable, 1: fixUnlock(default), 2: forcLock
        poco_debug_f2(_logger, "data = 0x%X, _AGC_trig =%u", data, _AGC_trig);
    data = data + ((_agcMode&0x3)<<6);     // 0bxxxxxxxx 11xxxxxx   // [7:6]    ("AGC Mode") 0: Single(Auto)=default, 1: ManGC(Manual), 3: Debug(One Shot, Factory)
        poco_debug_f2(_logger, "data = 0x%X, _agcMode =%u", data, _agcMode);
    data = data + 0;               // 0bxxxxxxxx xxxxxxx1  // [0]      ("Start searching")   default=0   сбрасывается автоматически 
    AGC_CTR = data;
    
    _AGC_start = 0;
    spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, true, (AGC_CTR+_AGC_start));
    
    // --------------------- Frequency -------------------------------------------
    // 1.	Установка частоты RF Frequency
    // # Frequency, Hz 925/2170 MHz (tuner_control -w -a 0x2C 4 1325777000)
    spi_read_write_universal(ADDR_RF_Frequency_2C, 4, true, _frequency);
        poco_debug_f3(_logger, "set _frequency = %u, Tuner_Fmin=%u, Tuner_Fmin=%u", _frequency, Tuner_Fmin, Tuner_Fmax);
    if(_eventLog) {
        data = spi_read_write_universal(ADDR_RF_Frequency_2C, 4, false, 0 );
            poco_debug_f1(_logger, "get _frequency = %u", data);
    }

    // --------------------- F3dB -------------------------------------------
    // 2.	Установка полосы Filter Band, MHz = F3dB
    // # F3dB, Hz LPF = 40/200 MHz
    // spi_read_write_universal(0x30, 4, true, _filterBandIndex);
    // LPF_filter [7:0], ADRF_filter [13:8] и FB_mode [15:14], занимает 2 байта и передаётся в TSFP-регистр FB_INDX (offset = 0x18)
    spi_read_write_universal(ADDR_FB_INDX_18, 2, true, _filterBandIndex);
       poco_debug_f1(_logger, "set _filterBand = 0x%X", _filterBandIndex);
    //data = spi_read_write_universal(0x30, 4, false, 0 );
    if(_eventLog){
        data = spi_read_write_universal(ADDR_FB_INDX_18, 4, false, 0 );
           poco_debug_f1(_logger, "get _filterBandIndex = 0x%X", data);
        data = spi_read_write_universal(ADDR_F3db_30, 4, false, 0 );
           poco_debug_f1(_logger, "get _filterBand (F3db)= 0x%X", data);
    }

    // --------------------- DACspecif ---------------------------------------
    // DACspecif = immutable.cfg (Tuner.InitialGain2; Tuner.InitialGain1)
    //# Tuner.InitialGain2: DACspecif Tuner GC2 BBG:4 (0bxx1111xx.xxxxxxxx) GC2 = 0…15 dB;
    //# Tuner.InitialGain1: DACspecif Tuner GC1 DAC:9 (0bxxxxxxx1.11111111) GC1 = 32..354 (2.7V .. 0.5V)
    if(_agcMode==MODE_MANUAL) {
        manualModeRewrite(); // ?? можно убрать   
        unsigned int internalManualLBandGain = backwardInterpolate(MAX_DB_GC1, bkwrdInterpCoeff, _manualLBandGain); // 70 is upper ext threshold
        data = ((_manualBaseBandGain<<2)<<8) + internalManualLBandGain;
    } else {
        data = ((ImInitialGain2<<2)<<8) + ImInitialGain1;
    }
    spi_read_write_universal(ADDR_DACspecif_12, 2, true, data);
        poco_debug_f1(_logger, "set DACspecif(GC2, GC1) = 0x%X", data);
    if(_eventLog){
        data = spi_read_write_universal(ADDR_DACspecif_12, 2, false, 0 );
            poco_debug_f1(_logger, "get DACspecif(GC2, GC1) = 0x%X ", data);
    }

    // --------------------- ADRF Reg ---------------------------------------
    // # ADRF Reg 0x00F8
    data = 0;
    data = data + ((_DC_offs&0x01)<<0x1); // 0bxxxxxxxx xxxxxx1x  data&0x2    0: enable (default), 1: disable
//        poco_debug_f1(_logger, "data = 0x%X", data);
    data = data + ((_PostAmp&0x01)<<0x2); // 0bxxxxxxxx xxxxx1xx  data&0x4    0: 3 dB (default), 1: 9 dB
//        poco_debug_f1(_logger, "data = 0x%X", data);

    data = data + ((_VGA3&0x03)<<0x3);    // 0bxxxxxxxx xxx11xxx  data&0x18   00: 21 dB, 01: 18 dB, 10: 15 dB, 11: 12 dB (default)
    data = data + ((_VGA2&0x03)<<0x5);    // 0bxxxxxxxx x11xxxxx  data&0x60   00: 21 dB, 01: 18 dB, 10: 15 dB, 11: 12 dB (default)
    data = data + ((_VGA1&0x03)<<0x7);    // 0bxxxxxxx1 1xxxxxxx  data&0x180  00: 15 dB, 01: 12 dB (default), 10: 9 dB, 11: 9 dB
        poco_debug_f3(_logger, "data VGA3 =%u, VGA2 = %u, VGA1 = %u", _VGA3, _VGA2, _VGA1);
    data = data + ((_PwrMode&0x01)<<0x9); // 0bxxxxxx1x xxxxxxxx  data&0x200  0: low power (default), 1: high power;
        poco_debug_f1(_logger, "data _PwrMode =%u", _PwrMode);
    // ADRF_filter [13:8] FB_INDX (offset = 0x18)
        _Filter = (_filterBandIndex>>8)&0x3F;
    data = data + ((_Filter&0x3F)<<0xA);  // 0b111111xx xxxxxxxx  data&0xFC00 Code = 1 dB corner in MHz; 000000 - filter bypass mode (default)
        poco_debug_f1(_logger, "data _Filter =%u", _Filter);

    spi_read_write_universal(ADDR_ADRF_REG_16, 2, true, data);
        poco_debug_f1(_logger, "set TSFP_soft.ADRF_REG = 0x%X", data);
    if(_eventLog){
        data = spi_read_write_universal(ADDR_ADRF_REG_16, 2, false, 0);
            poco_debug_f1(_logger, "get TSFP_soft.ADRF_REG = 0x%X", data);
    }

    // --------------------- ADRF Reg ---------------------------------------
    // tuner_control -w -a 0x1D 3 0xD76969
    // tuner_control -w -a 0x1C 4 0x00D76969
    data = 0;
    data = data + (_VGN1<<16); // 0b0000000011111111 00000000 00000000 (default=215)
    data = data + (_VGN2<<8);  // 0b0000000000000000 11111111 00000000 (default=105)
    data = data + _VGN3;       // 0b0000000000000000 00000000 11111111 (default=105)
    spi_read_write_universal((ADDR_ADRF_DAC_1C+1)/*0x1D*/, 3, true, data);
        poco_debug_f1(_logger, "set TSFP_soft.ADRF_DAC = 0x%X", data);
    if(_eventLog){
        data = spi_read_write_universal(ADDR_ADRF_DAC_1C, 4, false, 0);
            poco_debug_f1(_logger, "TSFP_soft.ADRF_DAC(NA, VGN1, VGN2, VGN3) = 0x%X", data);
    }

    // --------------------- AGC_Uref ---------------------------------------
    // AGC_Uref = immutable.cfg (AGC.Uref)
    // # AGC_Uref 0x02EE: ~750 mV
    // tuner_control -w -a 0x5E 2 750
    spi_read_write_universal(ADDR_AGC_Uref_5E, 2, true, (_AGC_Uref&0xFFF));
        poco_debug_f1(_logger, "set _AGC_Uref = %u", _AGC_Uref);
    if(_eventLog){
        data = spi_read_write_universal(ADDR_AGC_Uref_5E, 2, false, 0 );
            poco_debug_f1(_logger, "get _AGC_Uref = 0x%X", data);
    }

    // --------------------- AGC_Uref ---------------------------------------
    // AGC_CTR = immutable.cfg (AGC.DAC, AGC.VPK_delay, AGC.dLim, AGC.trigA) + AGC_mode=0, AGC_start=0
    data = 0;
    _AGC_start = 0;
    spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, true, (AGC_CTR+_AGC_start));
        poco_debug_f1(_logger, "set ADDR_AGC_CTRL_5C = 0x%X", AGC_CTR);
    if(_eventLog){
        data = spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, false, 0 );
            poco_debug_f1(_logger, "get ADDR_AGC_CTRL_5C = 0x%X", data);
    }
   return 0;
}

//================================================================================
//                  configure10GE
//================================================================================
int AGCTunerControl::configure10GE()
{
    // TRACE();
    unsigned int data=0;

    // --------------------- SET Spectral Inversion 10GE---------------------------------------
    bool _Transmision = false;
    unsigned int read_10ge_824 = devmem.dev_mem_read(ADDRESS_10GE_DATA_PACKETS);
    if(read_10ge_824&MASK_DATA_PACKETS_TRANSMISION) {
        _Transmision = true;   
        read_10ge_824 = read_10ge_824 - MASK_DATA_PACKETS_TRANSMISION; // stop
    }
    if(read_10ge_824&0x20000000) { // регистр [29] всегда 0
        read_10ge_824 = read_10ge_824 - 0x20000000; // stop
    }
    if(read_10ge_824&0x20000000) { // регистр [29] всегда 0
        read_10ge_824 = read_10ge_824 - 0x20000000; // stop
    }
    if(_Transmision) { // if was data transmission 10GE
        data = read_10ge_824 + MASK_DATA_PACKETS_TRANSMISION;
        devmem.dev_mem_write(ADDRESS_10GE_DATA_PACKETS, data);
        poco_debug_f1(_logger, "Set 10GE ADDRESS_10GE_DATA_PACKETS 0x%X = 0x%X", data);
    }
     return data;
}

int AGCTunerControl::configureResampler(){
    unsigned int data = 0; //devmem.dev_mem_read(ADDRESS_RESAMLER);
    unsigned int _dec = 0x10;
    unsigned int _inv = 0;
    switch(_decimationRate) { // 0x4000000C -  Bits(6:4) -  Decimator
    case 1:
        _dec = 0x10;
        break;
    case 2:
        _dec = 0x20;
        break;
    case 3:
        _dec = 0x30;
        break;
    case 4:
        _dec = 0x40;
        break;
    case 5:
        _dec = 0x50;
        break;
    default:
        _dec = 0x10;
        break;
    }
    switch(_spectrumInversion) { // 0x4000000C -  Bits(1:2) -  Tst/SPI
    case 0:
        _inv = 0x0;
        break;
    case 1:
        _inv = 0x2;
        break;
    default:
        _inv = 0x0;
        break;
    }
    data = _dec + _inv;
    devmem.dev_mem_write(ADDRESS_RESAMLER, data);
    return data;
}
//================================================================================
//                  collectAllStatisticAGCTuner
//================================================================================
void AGCTunerControl::collectAllStatisticAGCTuner(unsigned int status)
{
    
    unsigned int data = 0;
    unsigned int i = 0;
//    poco_debug_f1(_logger, "status = %u", status);
    reg_DACoutput_10_2 = spi_read_write_universal(ADDR_DACoutput_10, 2, false, 0);
    //poco_debug_f1(_logger, "reg_DACoutput_10_2 = 0x%X", reg_DACoutput_10_2);
    if(reg_DACoutput_10_2==0xFFFF) {
        present = 0;

        if(present_old_status>0) {
            poco_debug_f1(_logger, "Warning! Tuner missing or not ready! reg_DACoutput_10_2 = 0x%X", reg_DACoutput_10_2);
        }
        
        present_old_status= 0;
        
        TSFP_Temperature = 0;
        TSFP_DetectVoltage = 0;
        lpfCutoffFrequency = 0;
        return;
    } else {
        present = 1; // "Tuner Status"  - 0: Error, 1: Ok // search: TunerStat = new GUI_TunerStat  
        if(present_old_status == 0){
            poco_debug_f2(_logger, "reg_DACoutput_10_2 = 0x%X, present_old_status = %u", reg_DACoutput_10_2, present_old_status);
            configureAGCTuner();
        }
        present_old_status = 1;
    }
    
    reg_TSFP_Analog_0E_2 = spi_read_write_universal(ADDR_TSFP_Analog_0E, 2, false, 0);
    reg_AGC_STS_1A_2 = spi_read_write_universal(ADDR_AGC_STS_1A, 2, false, 0);
    reg_PeakDetect_14_2 = spi_read_write_universal(ADDR_PeakDetect_14, 2, false, 0);
    reg_F3db_30_4 = spi_read_write_universal(ADDR_F3db_30, 4, false, 0);
    reg_FB_INDX_18_2 =  spi_read_write_universal(ADDR_FB_INDX_18, 2, false, 0);
    
    
//---------------------------------------------------------------------------

    if(status&(static_cast<unsigned int>(SPI_READ_AGCTUNER_CONF))){
        // poco_debug_f1(_logger, "status = SPI_READ_AGCTUNER_CONF = %u", status);
        reg_ADRF_REG_16_2 = spi_read_write_universal(ADDR_ADRF_REG_16, 2, false, 0);
        reg_TUNER_Reference_28_4 = spi_read_write_universal(ADDR_TUNER_Reference_28, 4, false, 0); 
        reg_ADRF_DAC_1C_4 = spi_read_write_universal(ADDR_ADRF_DAC_1C, 4, false, 0);
        reg_AGC_CTRL_5C_2 = spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, false, 0);
        reg_AGC_Uref_5E_2 = spi_read_write_universal(ADDR_AGC_Uref_5E, 2, false, 0);

        // Read EEPROM ====================================        
        data = spi_read_write_universal(ADDR_EEPROM_48, 4, false, 0);
        reg_EEPROM_Reference_49_3 = (data&0xFFFFFF);
        reg_EEPROM_Project_48_1 = ((data>>24)&0xFF);

        data = spi_read_write_universal(ADDR_EEPROM_SerialNumber_4C, 4, false, 0);
        reg_EEPROM_Release_4E_2 = (data&0xFFFF);
        reg_EEPROM_SerialNumber_4C_2 = ((data>>16)&0xFFFF);
// poco_debug_f1(_logger, "!!!! EEPROM.Release_4E_2  = 0x%X", data);
        data =  spi_read_write_universal(ADDR_EEPROM_VersionNumber_50, 4, false, 0);
        reg_EEPROM_VersionNumber_50_1 = ((data>>24)&0xFF);
        reg_EEPROM_ADRFDAC_51_3 = (data&0xFFFFFF);

        data = spi_read_write_universal(ADDR_EEPROM_AGCCTR_54, 4, false, 0);
        reg_EEPROM_AGCCTR_54_2 = ((data>>16)&0xFFFF);
        reg_EEPROM_ADRFREG_56_2 = (data&0xFFFF);

        //=======================  EEPROM.CK.VersNumb =======================
        // s1.3.8. "HW Revision", версия TSFP (0x34), Версия ядра АРУ 
        EEPROM_version_type =  reg_EEPROM_VersionNumber_50_1; //spi_read_write_universal(0x50, 1, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "----- EEPROM.Version Number(HW Revision) = 0x%X", data);
        }
        /*
        if(EEPROM_version_type==0x34) { 
            present = 1;     // s1.3.1. "Tuner Status"        // 0: Error, 1: Ok // search: TunerStat = new GUI_TunerStat   
        } else {
            present = 0;
        }
        if(_eventLog) {
            poco_debug_f2(_logger, "status = 0x%X, SPI_READ_AGCTUNER_CONF = %u", status, (unsigned int) SPI_READ_AGCTUNER_CONF);
        } 
        */ 
        //=======================  EEPROM.SerialNumber =======================
        // отображать в разделе "System", подраздел "Tuner Statistic" // s1.3.7. "Serial Number"
        EEPROM_serialNumber = reg_EEPROM_SerialNumber_4C_2;//spi_read_write_universal(0x4C, 2, false, 0);
        
        if(_eventLog) {
            poco_debug_f1(_logger, "----- EEPROM.SerialNumber = 0x%X", EEPROM_serialNumber);
        }

        // =======================  release =======================
        // отображать в разделе "System", подраздел "Tuner Statistic" // s1.3.7. "TSFT last Presents"
        reg_EEPROM_Release_4E_2         = spi_read_write_universal(ADDR_EEPROM_Release_4E, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "----- EEPROM.release = 0x%X", EEPROM_serialNumber);
        }
        //std::string release("24/04/1991");
        char buffer [50];
        sprintf(buffer, "%u.%u.%u", static_cast<unsigned int>((reg_EEPROM_Release_4E_2>>11)&0x1F), static_cast<unsigned int>((reg_EEPROM_Release_4E_2>>7)&0xF), static_cast<unsigned int>((reg_EEPROM_Release_4E_2&0x7F)));
        release = std::string(buffer);

        //======================= TSFP_version =======================
        // отображать в разделе "System", подраздел "Tuner Statistic"	(16 байт "W-01.09/13.09.17") s1.3.15. "TSFP Soft Version"
        // soft_vers("W-01.09/13.09.17");
        for(i=0;i<16;i++) {
            data = spi_read_write_universal((ADDR_TSFP_Soft_Version+i), 1, false, 0);
            buffer[i] = (char) (data&0xFF);
            if(buffer[i]>127) {
                buffer[i]=127;
            }
        }
        buffer[16]=0;
        soft_vers = std::string(buffer);
        if(!present) {
            soft_vers = "TSFP Soft Version";
            release = "24.04.1991";
        }
        if(_eventLog) {
            poco_debug_f1(_logger, "----- soft_vers = %s", soft_vers);
            poco_debug_f1(_logger, "----- release = %s", release);
        }
        //======================= Reference =======================
        //(4 байта fref CONST)
        // s1.3.5. "LO Frequency, Hz"    // TSFP.Reference - кварц
        TSFP_ReferenceFrequency  = reg_TUNER_Reference_28_4; //spi_read_write_universal(0x28, 4, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "----- TSFP_ReferenceFrequency = 0x%X", TSFP_ReferenceFrequency );
        }
    }

//---------------------------------------------------------------------------
    if(status&(static_cast<unsigned int>(SPI_READ_AGCTUNER_STAT))){
        //======================= Temperature, C =======================
            //TSFP_soft:
            //  1) DACspecif.DAC +/-; DACspecif.BBG +/-;
            //  2) DACspecif.BBG -> DACoutput.BBG -> MAX2112.ExtControl.BBG;
            //  3) DACspecif.DAC -> DACoutput.DAC -> TSFP_Analog.DACvalue; VDAC8_1_SetValue(TSFP_Analog.DACvalue/2);
            //w11     TSFP_Analog         0E		R               (struct: Temperature:7 (0b1111111000000000), DACvalue:9 (0b0000000111111111) )
            //             # TSFP_Analog: Temperature, DAC GC1a = GC1/2:7 (0btttttttx.xxxxxxxx)
        data = reg_TSFP_Analog_0E_2;// spi_read_write_universal(0x0E, 2, false, 0);
        if(_eventLog) {
            poco_debug_f3(_logger, "#0x0E 2.2 TSFP_soft.TSFP_Analog = 0x%lX, Temperature = %u, DAC  = %u ", data, (unsigned int)((data&0xFE00)>>9), (unsigned int) (data&0x01FF));
        }
            TSFP_Temperature = ((data&0xFE00)>>9);

        //======================= AGC_STS =======================
        data = reg_AGC_STS_1A_2;//spi_read_write_universal(0x1A, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "#0x1A 2.3. AGC_STS TSFP_soft.AGC_STS = 0x%lX", data);
        }
            AGC_EndOfSearch = data&0x01;   // =6 s1.3.1. (RCV-30)// s1.3.1. (RCV-30)
            AGC_Lock    = ((data&0x06)>>0x1);   // [1]     0bxxxxxxxxxxxxxx1x   (зависимости: Unlock)                                        LED "Lock"
            AGC_ManualMode = ((data&0x06)>>0x2);// [2]     0bxxxxxxxxxxxxx1xx                                                                LED "ManMode"
            AGC_Unlock  = ((data&0x06)>>0x3);   // [3]     0bxxxxxxxxxxxx1xxx   (зависимости: Lock)                                          LED "Unlock"
            AGC_TooLow  = ((data&0x1000)>>0xC); // [12]    0bxxx1xxxxxxxxxxxx                                                                LED "Too Low"
            AGC_TooHigh = ((data&0x2000)>>0xD); // [13]    0bxx1xxxxxxxxxxxxx                                                                LED "Too Hught"
            AGC_Error   = ((data&0x4000)>>0xE); // [14]    0bx1xxxxxxxxxxxxxx   "Error"=TRUE: "Too_Low" или "Too_High" =TRUE
            AGC_Init    = ((data&0x8000)>>0xF); // [15]    0b1xxxxxxxxxxxxxxx   (зависимости: EoSearch)                                      LED "Init"

        //======================= PeakDetect TSFP_ReferenceVoltage =======================
        //      PeakDetect              ->  Reg_Urms (на выходе тюнера) ->  ChannelStat.tuner.referenceVoltage s1.3.4. "Urms, mV" 
       TSFP_DetectVoltage = reg_PeakDetect_14_2;//spi_read_write_universal(0x14, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "#0x14  2.4. TSFP_soft.TSFP_DetectVoltage = 0x%X", TSFP_DetectVoltage);
        }
        //======================= DACoutput (GC1,GC2) =======================
        //(struct: BBG:4 (0bxx1111xxxxxxxxxx), DAC:9 (0bxxxxxxx111111111) ) // получается после калькуляции DACspecif
        //      DACoutput.DAC           ->  Reg_TSFP.DAC, Reg_GC1.GC1   ->  ChannelStat.tuner.lBandGain
        //      DACoutput.BBG           ->  Reg_INIT_GC2, Reg_GC1.GC2   ->  ChannelStat.tuner.baseBandGain 
        data = reg_DACoutput_10_2;// spi_read_write_universal(0x10, 2, false, 0);
        if(_eventLog) {
            poco_debug_f3(_logger, "#0x10 2.5. TSFP_soft.DACoutput = 0x%lX, BBG = %u, DAC  = %u ", data, (unsigned int) ((data&0x3C00)>>10), (unsigned int) (data&0x01FF));
        }
            TSFP_GC1_lBandGain      = (data&0x01FF);    // GC1              // RCV-30: показать // ?! ChannelConf.agc.manualLBandGain
            TSFP_GC2_baseBandGain_db = ((data&0x3C00)>>10);    // GC2              // RCV-30: показать // ?! ChannelConf.agc.manualBaseBandGain
            TSFP_GC1_lBandGain_db   = (MAX_DB_GC1 - (TSFP_GC1_lBandGain/bkwrdInterpCoeff));
            //TSFP_GC1_lBandGain = (84.4 - TSFP_GC1_lBandGain_db) * 5;

        //======================= AGC_Uref ==============================================
        //  5.1. AGC_Uref - опорное напряжение АРУ в миливольтах
/*      data = reg_AGC_Uref_5E_2; //spi_read_write_universal(0x5E, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "AGC_Uref = 0x%lX", data);
        }
            _AGC_Uref = (data&0xFFF); // !!! только биты [11:0] (default=800)
*/
        //======================= ADRF_REG(PostAmp, VGA1, VGA2,VGA3) ==============================================
        data = reg_ADRF_REG_16_2; // spi_read_write_universal(0x16, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "TSFP_soft.ADRF_REG = 0x%lX", data);
        }
        // _DC_offs   = ((data&0x2)>>0x1);     // :1; // 0bxxxxxxxx xxxxxx1x   // 0: enable (default), 1: disable
        // _PostAmp   = ((data&0x4)>>0x2);     // :1; // 0bxxxxxxxx xxxxx1xx   // 0: 3 dB (default), 1: 9 dB
        if(data&0x4) {
            _PostAmp_db = 9;
        } else {
            _PostAmp_db = 3;
        }
    
        unsigned int VGA;
        VGA      = ((data&0x18)>>0x3);  
        switch(VGA) { // 00:21dB, 01:18dB, 10:15dB, 11:12dB 
        case 0:
            _VGA3_db = 21;
            break;
        case 1:
            _VGA3_db = 18;
            break;
        case 2:
            _VGA3_db = 15;
            break;
        case 3:
            _VGA3_db = 12;
            break;
        default:
            _VGA3_db = 12;
            break;
        }
    
        VGA      = ((data&0x60)>>0x5);  // :2; // 0bxxxxxxxx x11xxxxx   // 00: 21 dB, 01: 18 dB, 10: 15 dB, 11: 12 dB (default)
        switch(VGA) { // 00:21dB, 01:18dB, 10:15dB, 11:12dB 
        case 0:
            _VGA2_db = 21;
            break;
        case 1:
            _VGA2_db = 18;
            break;
        case 2:
            _VGA2_db = 15;
            break;
        case 3:
            _VGA2_db = 12;
            break;
        default:
            _VGA2_db = 12;
            break;
        }

        VGA      = ((data&0x180)>>0x7); // :2; // 0bxxxxxxx1 1xxxxxxx   // 00: 15 dB, 01: 12 dB (default), 10: 9 dB, 11: 9 dB
        switch(VGA) {
        case 0:
            _VGA1_db = 15;
            break;
        case 1:
            _VGA1_db = 12;
            break;
        case 2:
            _VGA1_db = 9;
            break;
        case 3:
            _VGA1_db = 9;
            break;
        default:
            _VGA1_db = 12;
            break;
        }

        // _PwrMode   = ((data&0x200)>>0x9); // :1; // 0bxxxxxx1x xxxxxxxx   // 0: low power (default), 1: high power;
        // _Filter    = ((data&0xFC00)>>0xA);// :6; // 0b111111xx xxxxxxxx   // Code = 1 dB corner in MHz; 000000 - filter bypass mode (default)

        // Pin = 10logUad(v) - GC1(дб) - GC2(дб) + VGA1 + VGA2 + VGA3 - Ar+Att
        Pin = (20 * log10(TSFP_DetectVoltage)) - TSFP_GC1_lBandGain_db - TSFP_GC2_baseBandGain_db - _VGA1_db - _VGA2_db - _VGA3_db - _PostAmp_db - _koef_for_Pin;

        //======================= F3db / lpfCutoffFrequency ==============================================
        //  "Filter Band, MHz"
        data = reg_F3db_30_4;// spi_read_write_universal(0x30, 4, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "F3db = 0x%lX", data);
        }

        lpfCutoffFrequency = reg_FB_INDX_18_2; //spi_read_write_universal(0x18, 2, false, 0);
        if(_eventLog) {
            poco_debug_f1(_logger, "FB_Index = 0x%X", lpfCutoffFrequency);
        }

        //======================= restart AGC INIT ==============================================
        if(AGC_ManualMode==0 && AGC_Init==0 && AGC_Lock==0 && AGC_EndOfSearch==1) {
            if(_time_out==0) {
                unsigned int AGC_CTR =0;
                unsigned int _AGC_start = 0;
                // reg_AGC_CTRL_5C_2 = spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, false, 0);
                AGC_CTR = spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, false, 0);
                //spi_read_write_universal(0x5C, 2, false, 0);
                _AGC_start = 1;
                if(AGC_CTR&_AGC_start) {
                    AGC_CTR = AGC_CTR - _AGC_start;
                }
                spi_read_write_universal(ADDR_AGC_CTRL_5C, 2, true, (AGC_CTR + _AGC_start));
                _AGC_start = 0;
                // в ноль падает сам регистр AGC_CTR
                // spi_read_write_universal(0x5C, 2, true, (AGC_CTR + _AGC_start));
                // data = spi_read_write_universal(0x1A, 2, false, 0);
                poco_debug_f2(_logger, "Restart AGC_CTR(0x5C = 0x%X) after Unlock(AGC_STS (#0x1A)  = 0x%X)", AGC_CTR, data);
            }
            _time_out++;
            if(_time_out==4) {
                _time_out = 0;
            }
        } else {
            _time_out=0;
        }
    }
   
    return;
}

//================================================================================
//                  collectStatistic10GE
//================================================================================
void AGCTunerControl::collectStatistic10GE()
{
    unsigned int read_result = devmem.dev_mem_read(ADDRESS_10GE_STATUS_SFP);
    _status10GE = (read_result&0xF);
/*
0 0x1	SFP_TX_FAULT	TX FAULT Status Active High
1 0x2	SFP_LOPC	SFP RX LOS Acveve High
2 0x4	SFP_MOD_ABSENT	SFP MODULE ABSENT Active High
3 0x8	TRANSMISION	Transmision - 1; Stop   - 0
*/
    if(read_result&0x1){
        _status10GE_SFP_TX_FAULT = 1;
    } else {
        _status10GE_SFP_TX_FAULT = 0;
    }
    if(read_result&0x2){
        _status10GE_SFP_LOPC = 1;
    } else {
        _status10GE_SFP_LOPC = 0;
    }
    if(read_result&0x4){
        _status10GE_SFP_MOD_ABSENT = 1;
    } else {
        _status10GE_SFP_MOD_ABSENT = 0;
    }
    if(read_result&0x8){
        _status10GE_Transmision = 1;
    } else {
        _status10GE_Transmision = 0;
    }
// --------------------- IP_DESTINATION (0x40000818) ---------------------------
    read_result = devmem.dev_mem_read(ADDRESS_10GE_IP_DESTINATION);
    ge_ip_destination = read_result; //&0xF);

// --------------------- DESTINATION_PORT (0x40000820) ---------------------------
    read_result = devmem.dev_mem_read(ADDRESS_10GE_DESTINATION_PORT);
    ge_destination_port = (read_result&0xFFFF);

// ---------------------  DATA_PACKETS (0x40000824)  ---------------------------
    read_result = devmem.dev_mem_read(ADDRESS_10GE_DATA_PACKETS);
    ge_data_packets = (read_result&0xFFFF);
}

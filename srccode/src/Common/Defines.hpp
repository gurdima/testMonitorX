#pragma once

//#define CHANNELS_NUMBER     2
#define CHANNELS_NUMBER     1

// #define SUBCHANNELS_NUMBER  5
#define SUBCHANNELS_NUMBER  1

// #define SUBCHANNELS_NUMBER_FOR_TDMA  8
#define SUBCHANNELS_NUMBER_FOR_TDMA  1
//#define MAX_SUBCHANNELS_NUMBER       6  // FOR ExchangeAgentSubsystem
#define MAX_SUBCHANNELS_NUMBER       1 // FOR ExchangeAgentSubsystem

#define MODE_AUTO       0
#define MODE_MANUAL     1
#define MODE_ONE_SHOT   2
#define MODE_FACTORY    3

#define MAIN_ADDRESS    0x40000000
#define MAIN_ADDRESS_SH8    0x40000008

#define SPI1_MAIN_ADDRESS   0x20000
#define SPI_LOW_SPEED       0x40000000

#define SPI_READ_ALL_STATUS    0xF
#define SPI_READ_ALL_CONF_STAT 0x7
#define SPI_READ_AGCTUNER_CONF 0x1
#define SPI_READ_AGCADRF_CONF  0x2
#define SPI_READ_AGCTUNER_STAT 0x4


#define MEMORY_CHANNEL_SHIFT                0x40000

// #define DFLT_ETH_IFACE_NAME     "eth0"

#define ADDR_TSFP_Analog_0E             0x0E
#define ADDR_DACoutput_10               0x10
#define ADDR_PeakDetect_14              0x14
#define ADDR_ADRF_REG_16                0x16 
#define ADDR_FB_INDX_18                 0x18
#define ADDR_AGC_STS_1A                 0x1A
#define ADDR_ADRF_DAC_1C                0x1C
#define ADDR_TUNER_Reference_28         0x28
#define ADDR_F3db_30                    0x30
#define ADDR_RF_Frequency_2C            0x2C
#define ADDR_DACspecif_12               0x12    
#define ADDR_TSFP_Soft_Version          0x38

#define ADDR_EEPROM_48                  0x48
#define ADDR_EEPROM_Reference_49        0x49
#define ADDR_EEPROM_SerialNumber_4C     0x4C
#define ADDR_EEPROM_Release_4E          0x4E
#define ADDR_EEPROM_VersionNumber_50    0x50
#define ADDR_EEPROM_ADRFDAC_51          0x51
#define ADDR_EEPROM_AGCCTR_54           0x54
#define ADDR_EEPROM_ADRFREG_56          0x56

#define ADDR_AGC_CTRL_5C                0x5C
#define ADDR_AGC_Uref_5E                0x5E

#define ADDRESS_RESAMLER                  0x4000000C

#define ADDRESS_10GE_STATUS_SFP           0x40000828
#define ADDRESS_10GE_IP_DESTINATION       0x40000818
#define ADDRESS_10GE_DESTINATION_PORT     0x40000820
#define ADDRESS_10GE_DATA_PACKETS         0x40000824

#define FFT_CTRL_ADDRESS                  0x40000040
#define FFT_SCALE_ADDRESS                 0x4000004C   
#define FFT_VER_ADDRESS                   0x40000050
#define FFT_FRMCNT_ADDRESS                0x40000048

#define ADDRESS_DEBUG_CTRL                0x40000054

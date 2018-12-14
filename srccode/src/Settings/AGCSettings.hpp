#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsHolderBase.hpp"
/*---------------------------------------------------------------------------*/

namespace Settings
{
class AGCSettings : public SettingsHolderBase
{
public:
    friend class ChannelSettings;

    unsigned int get_AGC_UREF() const;
    unsigned int get_AGC_DAC() const;
    unsigned int get_AGC_VPKDELAY() const;
    unsigned int get_AGC_DLIM() const;
    unsigned int get_AGC_TRIGA() const;
    unsigned int get_AGC_TRIGM() const;
    unsigned int get_AGC_TRIGS() const;

    unsigned int get_AGC_ADRF_FILTER() const;
    unsigned int get_AGC_ADRF_PWR_MODE() const;
    unsigned int get_AGC_ADRF_VGA1() const;
    unsigned int get_AGC_ADRF_VGA2() const;
    unsigned int get_AGC_ADRF_VGA3() const;
    unsigned int get_AGC_ADRF_POSTAMP() const;
    unsigned int get_AGC_ADRF_DC_OFFS() const;

    unsigned int get_AGC_ADRF_VGN1() const;
    unsigned int get_AGC_ADRF_VGN2() const;
    unsigned int get_AGC_ADRF_VGN3() const;

    std::string get_AGC_EEPROM_INIT() const;
    bool get_AGC_EEPROM_WRITE() const;

    unsigned int get_AGC_FS_ADC() const;
    unsigned int get_AGC_FS_OUT() const;
    float get_AR_ATT_FOR_PIN() const;

    unsigned int get_MANUAL_LBAND_GAIN_MIN() const;
    unsigned int get_MANUAL_LBAND_GAIN_MAX() const;

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

    unsigned int get_EEPROM_Project_48_1() const
    {
        return reg_EEPROM_Project_48_1;
    };
    void set_EEPROM_Project_48_1(unsigned int data)
    {
        reg_EEPROM_Project_48_1 = data;
    };

    unsigned int get_EEPROM_Reference_49_3() const
    {
        return reg_EEPROM_Reference_49_3;
    };
    void set_EEPROM_Reference_49_3(unsigned int data)
    {
        reg_EEPROM_Reference_49_3 = data;
    };

    unsigned int get_EEPROM_SerialNumber_4C_2() const
    {
        return reg_EEPROM_SerialNumber_4C_2;
    };
    void set_EEPROM_SerialNumber_4C_2(unsigned int data)
    {
        reg_EEPROM_SerialNumber_4C_2 = data;
    };

    unsigned int get_EEPROM_Release_4E_2() const
    {
        return reg_EEPROM_Release_4E_2;
    };
    void set_EEPROM_Release_4E_2(unsigned int data)
    {
        reg_EEPROM_Release_4E_2 = data;
    };

    unsigned int get_EEPROM_VersionNumber_50_1() const
    {
        return reg_EEPROM_VersionNumber_50_1;
    };
    void set_EEPROM_VersionNumber_50_1(unsigned int data)
    {
        reg_EEPROM_VersionNumber_50_1 = data;
    };

    unsigned int get_EEPROM_ADRFDAC_51_3() const
    {
        return reg_EEPROM_ADRFDAC_51_3;
    };
    void set_EEPROM_ADRFDAC_51_3(unsigned int data)
    {
        reg_EEPROM_ADRFDAC_51_3  = data;
    };

    unsigned int get_EEPROM_AGCCTR_54_2() const
    {
        return reg_EEPROM_AGCCTR_54_2;
    };
    void set_EEPROM_AGCCTR_54_2(unsigned int data)
    {
        reg_EEPROM_AGCCTR_54_2 = data;
    };

    unsigned int get_EEPROM_ADRFREG_56_2() const
    {
        return reg_EEPROM_ADRFREG_56_2;
    };
    void set_EEPROM_ADRFREG_56_2(unsigned int data)
    {
        reg_EEPROM_ADRFREG_56_2  = data;
    };

    unsigned int get_ApplySendSPIflag() const
    {
        return flag_ApplySendSPI;
    };
    void set_ApplySendSPIflag(unsigned int data){ 
        flag_ApplySendSPI = data;
    };

    unsigned int get_ArrayEEPROMData(unsigned int nom_byte)
    {
        return dataEEPROM[nom_byte];
    };
    void set_ArrayEEPROMData(unsigned int nom_byte, unsigned int data){ 
        if(nom_byte<16) {
            dataEEPROM[nom_byte] = data;
        }
    };

protected:
    AGCSettings();

    //unsigned int flag_ApplyADRFConf;
    unsigned int flag_ApplySendSPI;

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
    unsigned int dataEEPROM[16];
};

} // namespace Settings

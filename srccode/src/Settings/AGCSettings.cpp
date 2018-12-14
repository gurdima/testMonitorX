#include "Settings/AGCSettings.hpp"
#include "Settings/SettingsParams.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{
AGCSettings::AGCSettings()
: SettingsHolderBase(S_AGC_SECTION_NAME)
{
    // AGC.Mode: 0
    _settings.add(new IntDatum(S_AGC_UREF,      0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_DAC,       0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_VPKDELAY,  0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_DLIM,      0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_TRIGA,     0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_TRIGM,     0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_TRIGS,     0, false)); //  {1.} 

    _settings.add(new IntDatum(S_AGC_ADRF_FILTER,   0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_PWR_MODE, 0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_VGA1,     0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_VGA2,     0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_VGA3,     0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_POSTAMP,  0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_DC_OFFS,  0, false)); //  {1.} 

    _settings.add(new IntDatum(S_AGC_ADRF_VGN1, 0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_VGN2, 0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_ADRF_VGN3, 0, false)); //  {1.} 

    _settings.add(new StringDatum(S_AGC_EEPROM_INIT,   "", false)); //  {1.} 
    _settings.add(new BoolDatum(S_AGC_EEPROM_WRITE,  0, false)); //  {1.} 

    _settings.add(new IntDatum(S_AGC_FS_ADC, 0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_FS_OUT, 0, false)); //  {1.} 
    _settings.add(new IntDatum(S_AGC_MANUAL_LBAND_GAIN_MIN, 1, false));
    _settings.add(new IntDatum(S_AGC_MANUAL_LBAND_GAIN_MAX, 70, false));
    _settings.add(new FloatDatum(S_AGC_AR_ATT_FOR_PIN, 0, false)); //  {1.} 
    // _settings.add(new IntDatum(S_AGC_MEASURE_POTRION, 0, false)); //  {1.12} 
//    _settings.add(new IntDatum(S_DECIMATION_RATE, 1, true));
}

//----------------- immutable -----------------------------

unsigned int AGCSettings::get_AGC_UREF() const
{
    return _settings.getInt(S_AGC_UREF)->getValue();
}
unsigned int AGCSettings::get_AGC_DAC() const
{
    return _settings.getInt(S_AGC_DAC)->getValue();
}
unsigned int AGCSettings::get_AGC_VPKDELAY() const
{
    return _settings.getInt(S_AGC_VPKDELAY)->getValue();
}
unsigned int AGCSettings::get_AGC_DLIM() const
{
    return _settings.getInt(S_AGC_DLIM)->getValue();
}
unsigned int AGCSettings::get_AGC_TRIGA() const
{
    return _settings.getInt(S_AGC_TRIGA)->getValue();
}
unsigned int AGCSettings::get_AGC_TRIGM() const
{
    return _settings.getInt(S_AGC_TRIGM)->getValue();
}
unsigned int AGCSettings::get_AGC_TRIGS() const
{
    return _settings.getInt(S_AGC_TRIGS)->getValue();
}

unsigned int AGCSettings::get_AGC_ADRF_FILTER() const
{
    return _settings.getInt(S_AGC_ADRF_FILTER)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_PWR_MODE() const
{
    return _settings.getInt(S_AGC_ADRF_PWR_MODE)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_VGA1() const
{
    return _settings.getInt(S_AGC_ADRF_VGA1)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_VGA2() const
{
    return _settings.getInt(S_AGC_ADRF_VGA2)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_VGA3() const
{
    return _settings.getInt(S_AGC_ADRF_VGA3)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_POSTAMP() const
{
    return _settings.getInt(S_AGC_ADRF_POSTAMP)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_DC_OFFS() const
{
    return _settings.getInt(S_AGC_ADRF_DC_OFFS)->getValue();
}

unsigned int AGCSettings::get_AGC_ADRF_VGN1() const
{
    return _settings.getInt(S_AGC_ADRF_VGN1)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_VGN2() const
{
    return _settings.getInt(S_AGC_ADRF_VGN2)->getValue();
}
unsigned int AGCSettings::get_AGC_ADRF_VGN3() const
{
    return _settings.getInt(S_AGC_ADRF_VGN3)->getValue();
}

std::string AGCSettings::get_AGC_EEPROM_INIT() const
{
    return _settings.getString(S_AGC_EEPROM_INIT)->getValue();
}

bool AGCSettings::get_AGC_EEPROM_WRITE() const
{
    return _settings.getBool(S_AGC_EEPROM_WRITE)->getValue();
}

unsigned int AGCSettings::get_AGC_FS_ADC() const
{
    return _settings.getInt(S_AGC_FS_ADC)->getValue();
}
unsigned int AGCSettings::get_AGC_FS_OUT() const
{
    return _settings.getInt(S_AGC_FS_OUT)->getValue();
}

float AGCSettings::get_AR_ATT_FOR_PIN() const
{
    return _settings.getFloat(S_AGC_AR_ATT_FOR_PIN)->getValue();
}


unsigned int AGCSettings::get_MANUAL_LBAND_GAIN_MIN() const
{
    return _settings.getInt(S_AGC_MANUAL_LBAND_GAIN_MIN)->getValue();
}

unsigned int AGCSettings::get_MANUAL_LBAND_GAIN_MAX() const
{
    return _settings.getInt(S_AGC_MANUAL_LBAND_GAIN_MAX)->getValue();
}


} // namespace Settings

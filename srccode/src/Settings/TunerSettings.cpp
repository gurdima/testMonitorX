#include "Settings/TunerSettings.hpp"
#include "Settings/SettingsParams.hpp"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

TunerSettings::TunerSettings()
: SettingsHolderBase(S_TUNER_SECTION_NAME)
{
    //Tuner.Frequency: 1678987000
    //Tuner.FilterBand: 152800000
    //Tuner.ManualBasebandGain: 7
    //Tuner.ManualLBandGain: 43
    //Tuner.AGCMode: 0 
    //Tuner.SpectrumInversion: 0 
    _settings.add(new IntDatum  (S_TUNER_FREQUENCY,                   925000, true ));    //  {1.1}
    _settings.add(new IntDatum  (S_TUNER_FILTER_BAND,                 46, true ));    //  {1.2}
    _settings.add(new IntDatum  (S_AGC_MANUAL_BASEBAND_GAIN,          0, true ));    //  {1.2}
    _settings.add(new IntDatum  (S_AGC_MANUAL_L_BAND_GAIN,            0, true ));    //  {1.2}
    _settings.add(new IntDatum  (S_AGC_MODE,                          0, true ));  //  {1.9} 
    _settings.add(new IntDatum  (S_TUNER_SPECTRUM_INVERSION,          0, true ));  //  new 02/12/17
    //Tuner.SupplyVoltage: 1.5
    //Tuner.Reference: 25999956
    //Tuner.InitialGain2: 7
    //Tuner.InitialGain1: 132
    //Tuner.Fmin: 925000000
    //Tuner.Fmax: 2170000000
    //Tuner.LPFmin: 46
    //Tuner.LPFmax: 245
    _settings.add(new FloatDatum(S_TUNER_SUPPLY_VOLTAGE,              1.5, false));    //  {1.6}
    _settings.add(new IntDatum  (S_TUNER_REFERENCE,                   25999956, false));
    _settings.add(new IntDatum(S_AGC_INITIAL_GAIN2,                   1, false));  //  {1.} 
    _settings.add(new IntDatum(S_AGC_INITIAL_GAIN1,                   1, false));  //  {1.} 
    _settings.add(new IntDatum(S_TUNER_FMIN,                          925000, false));  //  {1.} 
    _settings.add(new IntDatum(S_TUNER_FMAX,                         2170000, false));  //  {1.} 
    _settings.add(new IntDatum(S_TUNER_LPFMIN,                        46, false));  //  {1.} 
    _settings.add(new IntDatum(S_TUNER_LPFMAX,                        245, false));  //  {1.} 

//    _settings.add(new IntDatum(S_AGC_MANUAL_L_BAND_GAIN,   0, true ));  //  {1.10}
//    _settings.add(new IntDatum(S_AGC_MANUAL_BASEBAND_GAIN, 0, true ));  //  {1.11}
//    _settings.add(new IntDatum(S_AGC_MEASURE_POTRION,      0, false)); //  {1.12} 
//    _settings.add(new IntDatum  (S_TUNER_SPECTRUM_ANALYZER_SHIFT,     0, false));    //  {1.4}
//    _settings.add(new IntDatum  (S_TUNER_REFERENCE_VOLTAGE_OLD,       0, false));    //  {1.6}
//    _settings.add(new IntDatum  (S_TUNER_REFERENCE_VOLTAGE_NEW,       0, false));    //  {1.6}
//    _settings.add(new IntDatum  (S_TUNER_REFERENCE_VOLTAGE_OLD_NO_SA, 0, false));    //  {1.6}
//    _settings.add(new IntDatum  (S_TUNER_REFERENCE_VOLTAGE_NEW_NO_SA, 0, false));    //  {1.6}
//    _settings.add(new IntDatum  (S_TUNER_MAX_SA_SYMBOL_RATE,          0, false));    //  {1.7}
//    _settings.add(new FloatDatum(S_TUNER_KFBB,                        0, false));    //  {1.12}
}

//----------------- mutable -----------------------------

unsigned int TunerSettings::getFrequency() const
{

    return _settings.getInt(S_TUNER_FREQUENCY)->getValue()*1000;
}

void TunerSettings::setFrequency(unsigned int val)
{
    int new_val = static_cast<int>(val/1000);
    _settings.update(S_TUNER_FREQUENCY, new_val);
}
//----------------------------------------------
unsigned int TunerSettings::getFilterBandIndex() const
{
    // unsigned int new_val = _settings.getInt(S_TUNER_FILTER_BAND)->getValue();
    //unsigned int index_band = (unsigned int) (((new_val-3200000)/1000000)/0.8);
    return _settings.getInt(S_TUNER_FILTER_BAND)->getValue();
}

void TunerSettings::setFilterBandIndex(unsigned int val)
{
    //unsigned int band = ((val*0.8) + 3.2)*1000000;
    _settings.update(S_TUNER_FILTER_BAND, val);
}
//----------------------------------------------
unsigned int TunerSettings::getManualBasebandGain() const
{
    return _settings.getInt(S_AGC_MANUAL_BASEBAND_GAIN)->getValue();
}

void TunerSettings::setManualBasebandGain(unsigned int val)
{
    _settings.update(S_AGC_MANUAL_BASEBAND_GAIN, val);
}
//----------------------------------------------
unsigned int TunerSettings::getManualLBandGain() const
{
    return _settings.getInt(S_AGC_MANUAL_L_BAND_GAIN)->getValue();
}

void TunerSettings::setManualLBandGain(unsigned int val)
{
    _settings.update(S_AGC_MANUAL_L_BAND_GAIN, val);
}
//----------------------------------------------
unsigned int TunerSettings::getAGCMode() const
{
    return _settings.getInt(S_AGC_MODE)->getValue();
}

void TunerSettings::setAGCMode(unsigned int val)
{
    _settings.update(S_AGC_MODE, val);
}
//----------------------------------------------
unsigned int TunerSettings::getSpectrumInversion() const
{
    return _settings.getInt(S_TUNER_SPECTRUM_INVERSION)->getValue();
}

void TunerSettings::setSpectrumInversion(unsigned int val){
    _settings.update(S_TUNER_SPECTRUM_INVERSION, val);
}

//----------------------------------------------
/*
bool TunerSettings::isModeAuto() const
{
    return getMode() == MODE_AUTO;
}

bool TunerSettings::isModeManual() const
{
    return getMode() == MODE_MANUAL;
}

bool TunerSettings::isModeOneShot() const
{
    return getMode() == MODE_ONE_SHOT;
}
*/
//----------------------------------------------
//----------------- immutable -----------------------------

float TunerSettings::getSupplyVoltage() const
{
    return _settings.getFloat(S_TUNER_SUPPLY_VOLTAGE)->getValue();
}

void TunerSettings::setSupplyVoltage(float val)
{
    _settings.update(S_TUNER_SUPPLY_VOLTAGE, val);
}
//----------------------------------------------
unsigned int TunerSettings::getReference() const
{
    return _settings.getInt(S_TUNER_REFERENCE)->getValue();
}

void TunerSettings::setReference(unsigned int val)
{
    _settings.update(S_TUNER_REFERENCE, val);
}
//----------------------------------------------
unsigned int TunerSettings::getInitialGain2() const
{
    return _settings.getInt(S_AGC_INITIAL_GAIN2)->getValue();
}

void TunerSettings::setInitialGain2(unsigned int val)
{
    _settings.update(S_AGC_INITIAL_GAIN2, val);
}
//----------------------------------------------
unsigned int TunerSettings::getInitialGain1() const
{
    return _settings.getInt(S_AGC_INITIAL_GAIN1)->getValue();
}

void TunerSettings::setInitialGain1(unsigned int val)
{
    _settings.update(S_AGC_INITIAL_GAIN1, val);
}
//----------------- new ----------------------------
unsigned int TunerSettings::getTunerFMIN() const
{
    return _settings.getInt(S_TUNER_FMIN)->getValue()*1000;
}

//----------------------------------------------
unsigned int TunerSettings::getTunerFMAX() const
{
    return _settings.getInt(S_TUNER_FMAX)->getValue()*1000;
}

//----------------------------------------------
unsigned int TunerSettings::getTunerLPFMIN() const
{
    return _settings.getInt(S_TUNER_LPFMIN)->getValue();
}

//----------------------------------------------
unsigned int TunerSettings::getTunerLPFMAX() const
{
    return _settings.getInt(S_TUNER_LPFMAX)->getValue();
}

//---------------------------------------
} // namespace Settings


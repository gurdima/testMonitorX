#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsHolderBase.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

class TunerSettings : public SettingsHolderBase
{
public:
    friend class ChannelSettings;
//----------------- mutable -----------------------------
    unsigned int getFrequency() const;
    void setFrequency(unsigned int val);

    unsigned int getFilterBandIndex() const;
    void setFilterBandIndex(unsigned int val);

    unsigned int getManualLBandGain() const;
    void setManualLBandGain(unsigned int val);

    unsigned int getManualBasebandGain() const;
    void setManualBasebandGain(unsigned int val);

    unsigned int getAGCMode() const;
    void setAGCMode(unsigned int val);

    unsigned int getSpectrumInversion() const;
    void setSpectrumInversion(unsigned int val);

/*
    bool isModeAuto() const;
    bool isModeManual() const;
    bool isModeOneShot() const;
*/
//----------------- immutable -----------------------------

    float getSupplyVoltage() const;
    void setSupplyVoltage(float val);

    unsigned int getReference() const;
    void setReference(unsigned int val);

    unsigned int getInitialGain2() const;
    void setInitialGain2(unsigned int val);

    unsigned int getInitialGain1() const;
    void setInitialGain1(unsigned int val);

    unsigned int getTunerFMIN() const;

    unsigned int getTunerFMAX() const;

    unsigned int getTunerLPFMIN() const;

    unsigned int getTunerLPFMAX() const;

    unsigned int getConvert_lpf_to_F3db() const;
    void setConvert_lpf_to_F3db(unsigned int new_lpf);

protected:
    TunerSettings();
};

} // namespace Settings

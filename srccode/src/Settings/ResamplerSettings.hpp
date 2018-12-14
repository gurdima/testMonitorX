#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsHolderBase.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{
class ResamplerSettings : public SettingsHolderBase
{
public:
    friend class ChannelSettings;

    unsigned int getStatusEnable() const;
    void setStatusEnable(unsigned int val,unsigned int demMode);

    unsigned int getOutputBand() const;
    void setOutputBand(unsigned int val);

protected:
    ResamplerSettings();
};

} // namespace Settings

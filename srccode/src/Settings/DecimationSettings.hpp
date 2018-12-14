#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsHolderBase.hpp"
/*---------------------------------------------------------------------------*/

namespace Settings
{
class DecimationSettings : public SettingsHolderBase
{
public:
    friend class ChannelSettings;

    unsigned int getDecimationRate() const;
    void setDecimationRate(unsigned int val);

protected:
    DecimationSettings();
};

} // namespace Settings

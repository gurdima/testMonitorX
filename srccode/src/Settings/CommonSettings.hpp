#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/SettingsHolderBase.hpp"
/*---------------------------------------------------------------------------*/
#include <string>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

class CommonSettings : public SettingsHolderBase
{
public:
    friend class SettingsManager;

    std::string getDeviceName() const;
    void setDeviceName(const std::string& val);
    
protected:
    CommonSettings();
};

} // namespace Settings

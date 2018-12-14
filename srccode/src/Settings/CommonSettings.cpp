#include "Settings/CommonSettings.hpp"
#include "Settings/SettingsParams.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

CommonSettings::CommonSettings()
: SettingsHolderBase(S_COMMON_SECTION_NAME)
{
    _settings.add(new StringDatum(S_COMMON_DEVICE_NAME, "", true ));    
}

std::string CommonSettings::getDeviceName() const
{
    return _settings.getString(S_COMMON_DEVICE_NAME)->getValue();
}

void CommonSettings::setDeviceName(const std::string& val)
{
    _settings.update(S_COMMON_DEVICE_NAME, val);
}

} // namespace Settings

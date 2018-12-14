#pragma once

/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
#include "Common/Typedefs.hpp"
#include "Common/Defines.hpp"
#include "Settings/TunerSettings.hpp"
#include "Settings/AGCSettings.hpp"
// #include "Settings/MiscSettings.hpp"
#include "Settings/DecimationSettings.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
/*---------------------------------------------------------------------------*/
namespace Settings
{

class ChannelSettings : public Noncopyable
{
public:

    friend class SettingsManager;

    ChannelSettings();

    TunerSettings&              getTunerSettings();
    AGCSettings&                getAGCSettings();
//    MiscSettings&               getMiscSettings();
    DecimationSettings&         getDecimationSettings();

    void save(const std::string& prefix, Poco::Util::AbstractConfiguration* config) const;
    void load(const std::string& prefix, Poco::Util::AbstractConfiguration* config);

private:
    TunerSettings               _tunerSettings;
    AGCSettings                 _agcSettings;
//    MiscSettings                _miscSettings;
    DecimationSettings          _decimationSettings;

    Poco::Logger&               _logger;
};

} // namespace Settings

#include "Settings/ChannelSettings.hpp"
#include "Common/Helpers.hpp"
#include "Common/Tracer.hpp"

#include "Poco/NumberFormatter.h"
/*---------------------------------------------------------------------------*/
using std::string;
using Poco::NumberFormatter;
using Poco::Logger;
/*---------------------------------------------------------------------------*/

namespace Settings
{

ChannelSettings::ChannelSettings()
: _logger(Logger::get("Monitor.ChannelSettings"))
{
}

TunerSettings& ChannelSettings::getTunerSettings()
{
    return _tunerSettings;
}

AGCSettings& ChannelSettings::getAGCSettings()
{
    return _agcSettings;
}
/*
MiscSettings& ChannelSettings::getMiscSettings()
{
    return _miscSettings;
}
*/
DecimationSettings& ChannelSettings::getDecimationSettings()
{
    return _decimationSettings;
}

void ChannelSettings::save(const std::string& prefix, Poco::Util::AbstractConfiguration* config) const
{
//    TRACE();
//    _miscSettings.save(prefix, config);
    _agcSettings.save(prefix, config);
    _tunerSettings.save(prefix, config);
    _decimationSettings.save(prefix, config);
}

void ChannelSettings::load(const std::string& prefix, Poco::Util::AbstractConfiguration* config)
{
//     TRACE();
    _tunerSettings.load(prefix, config);
    _agcSettings.load(prefix, config);
//    _miscSettings.load(prefix, config);
    _decimationSettings.load(prefix, config);
}

} // namespace Settings


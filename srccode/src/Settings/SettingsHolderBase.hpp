#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
#include "Settings/SettingsGroup.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Util/AbstractConfiguration.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Settings
{

/**
 * @brief All classes that want to provide setters/getters for 
 *        settings thould be inherited from this class
 */
class SettingsHolderBase : public Noncopyable
{
public:
    virtual ~SettingsHolderBase()
    {
    }

    /**
     * @brief Save settings
     * @param channelId Id of channel this group belongs to
     * @param config Configuration to save data in
     */
    void save(const std::string& prefix, Poco::Util::AbstractConfiguration* config) const
    {
        _settings.save(prefix, config);
    }

    /**
     * @brief Load settings
     * @param channelId Id of channel this group belongs to
     * @param config Configuration to load data from
     */
    void load(const std::string& prefix, Poco::Util::AbstractConfiguration* config)
    {
        _settings.load(prefix, config);
    }

protected:
    SettingsHolderBase(const std::string& domain)
    : _settings(domain)
    {
    }

    SettingsGroup _settings;

};

} // namespace Settings

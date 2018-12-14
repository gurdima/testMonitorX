#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/Datum.hpp"
#include "Common/Typedefs.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Util/AbstractConfiguration.h"
#include "Poco/Logger.h"
/*---------------------------------------------------------------------------*/
#include <map>
#include <string>
/*---------------------------------------------------------------------------*/
#define CHANNEL_ID_COMMON_SETTING   ""
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{

/**
 * @brief Represent group of settings that belongs to one 
 *        domain. E.g. tuner, agc, adc, etc.
 */
class SettingsGroup
{
public:
    /**
     * @brief Constructor
     * @param domain Name of the domain this settings group 
     *               describes
     */
    SettingsGroup(const std::string& domain);

    /**
     * @brief Add setting to the group. Class takes ownership on 
     *        this setting
     * @param setting Poinger to the setting to add
     */
    void add(AbstractDatum* setting);

    /**
     * @brief Get setting by name
     * @param name Name of setting to get
     * @return const AbstractDatum* 
     */
    const AbstractDatum* get(const std::string& name) const;

    /**
     * @brief Get setting
     * @param name Name of setting
     * @return const IntDatum* 
     * @throws InvalidAccessException in the case of real setting 
     *                                type and requested type
     *                                mismatch
     */
    const IntDatum*    getInt(const std::string& name) const;
    const UIntDatum*   getUInt(const std::string& name) const;
    const FloatDatum*  getFloat(const std::string& name) const;
    const BoolDatum*   getBool(const std::string& name) const;
    const StringDatum* getString(const std::string& name) const;
    const DoubleDatum* getDouble(const std::string& name) const;

    /**
     * @brief Update setting value
     * @param name Name of setting
     * @param value Value to set
     */
    void update(const std::string& name, const unsigned int& value);
    void update(const std::string& name, const float& value);
    void update(const std::string& name, const bool& value);
    void update(const std::string& name, const std::string& value);
    void update(const std::string& name, const double& value);
    void update(const std::string& name, const int& value);
    /**
     * @brief Save group
     * @param channelId Id of channel this group belongs to
     * @param config Configuration to save data in
     */
    void save(const std::string& channelId, Poco::Util::AbstractConfiguration* config) const;

    /**
     * @brief Load group. If setting is saveable than it will be 
     *        searched with such name: channelId.domain.settingName,
     *        otherwise with such name: domain.settingName
     * @param channelId Id of channel this group belongs to
     * @param config Configuration to load data from
     */
    void load(const std::string& channelId, Poco::Util::AbstractConfiguration* config);
private:

    
    AbstractDatum* getMutable(const std::string& name);

    std::string makePrefix(const std::string& prefix) const;

    typedef Poco::SharedPtr<AbstractDatum> AbstractDatumPtr;
    typedef std::map<std::string, AbstractDatumPtr> SettingsContainer;

    std::string _domain;
    SettingsContainer _container;
    Poco::Logger& _logger;
};

} // namespace Settings


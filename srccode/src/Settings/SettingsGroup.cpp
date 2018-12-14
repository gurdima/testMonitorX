#include "Settings/SettingsGroup.hpp"
#include "Common/Tracer.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Exception.h"
#include "Poco/AutoPtr.h"

using Poco::AutoPtr;
using Poco::Util::AbstractConfiguration;
using Poco::Logger;
/*---------------------------------------------------------------------------*/
using std::pair;
using std::string;
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Settings
{
SettingsGroup::SettingsGroup(const std::string& domain)
: _domain(domain)
, _logger(Logger::get("Monitor.SettingsGroup"))
{
}

void SettingsGroup::add(AbstractDatum* setting)
{
    _container.insert(pair<string, AbstractDatumPtr>(setting->getName(), AbstractDatumPtr(setting)));
}

const AbstractDatum* SettingsGroup::get(const std::string& name) const
{
    SettingsContainer::const_iterator it = _container.find(name);
    if(it == _container.end()) {
        throw Poco::NotFoundException("Can't find datum " + name);
    }

    return it->second;
}

AbstractDatum* SettingsGroup::getMutable(const std::string& name)
{
    SettingsContainer::iterator it = _container.find(name);
    if(it == _container.end()) {
        throw Poco::NotFoundException("Can't find datum " + name);
    }

    return it->second;
}

const IntDatum* SettingsGroup::getInt(const std::string& name) const
{
    try {
        return dynamic_cast<const IntDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not int datum: " + name);
    }
}

const UIntDatum* SettingsGroup::getUInt(const std::string& name) const
{
    try {
        return dynamic_cast<const UIntDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not int datum: " + name);
    }
}

const FloatDatum* SettingsGroup::getFloat(const std::string& name) const
{
    try {
        return dynamic_cast<const FloatDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not float datum: " + name);
    }
}

const BoolDatum* SettingsGroup::getBool(const std::string& name) const
{
    try {
        return dynamic_cast<const BoolDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not bool datum: " + name);
    }
}

const StringDatum* SettingsGroup::getString(const std::string& name) const
{
    try {
        return dynamic_cast<const StringDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not string datum: " + name);
    }
}

const DoubleDatum* SettingsGroup::getDouble(const std::string& name) const
{
    try {
        return dynamic_cast<const DoubleDatum*>(get(name));
    } catch (...){
        throw Poco::InvalidAccessException("Not string datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const unsigned int& value)
{
    try {
        dynamic_cast<IntDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not int datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const float& value)
{
    try {
        dynamic_cast<FloatDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not float datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const bool& value)
{
    try {
        dynamic_cast<BoolDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not bool datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const std::string& value)
{
    try {
        dynamic_cast<StringDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not string datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const double& value)
{
    try {
        dynamic_cast<DoubleDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not string datum: " + name);
    }
}

void SettingsGroup::update(const std::string& name, const int& value)
{
    try {
        dynamic_cast<IntDatum*>(getMutable(name))->setValue(value);
    } catch (...) {
        throw Poco::InvalidAccessException("Not string datum: " + name);
    }
}

void SettingsGroup::save(const std::string& channelId, Poco::Util::AbstractConfiguration* config) const
{
    AutoPtr<AbstractConfiguration> view = config->createView(makePrefix(channelId));

    for(SettingsContainer::const_iterator it = _container.begin(); it != _container.end(); ++it) {        
        if(_domain=="TDMA") {
            it->second->save(view);
        } else {
            if(it->second->isSaveable()) {
                it->second->save(view);
            }
        }
    }
}

void SettingsGroup::load(const std::string& channelId, Poco::Util::AbstractConfiguration* config)
{
    AutoPtr<AbstractConfiguration> view          = config->createView(makePrefix(channelId));
    AutoPtr<AbstractConfiguration> permanentView = config->createView(makePrefix(""));

    for(SettingsContainer::iterator it = _container.begin(); it != _container.end(); ++it) {        
            AbstractDatum* datum = it->second;
           if(datum->isSaveable()) {
                datum->load(view);
           } else {
                datum->load(permanentView);
           }
    }
}

std::string SettingsGroup::makePrefix(const std::string& prefix) const
{
    if(prefix.empty()) {
        return _domain;
    }

    return prefix + "." + _domain;
}

} // namespace Settings


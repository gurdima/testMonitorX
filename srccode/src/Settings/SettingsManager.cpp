#include "Settings/SettingsManager.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/AutoPtr.h"
#include "Poco/NumberFormatter.h"
#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Util/ConfigurationView.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/AbstractConfiguration.h"

using Poco::AutoPtr;
using Poco::NumberFormatter;
using Poco::Util::LayeredConfiguration;
using Poco::Util::ConfigurationView;
using Poco::Util::PropertyFileConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::Logger;
/*---------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>

#include <sys/mount.h>
#include <sys/reboot.h>
/*---------------------------------------------------------------------------*/
using std::ifstream;
using std::ofstream;
using std::ios;
using std::flush;
/*---------------------------------------------------------------------------*/
/*
static int remountRootFS(bool ro)
{
    unsigned long mountflags = MS_REMOUNT;

    if(ro) {
        mountflags |= MS_RDONLY;
    }

    return mount(NULL, "/", NULL, mountflags, NULL);
}
*/
/*---------------------------------------------------------------------------*/

namespace Settings
{

SettingsManager::SettingsManager(unsigned int channelsNumber)
: _channelsNumber(channelsNumber)
, _mutableConfigFile()
, _savedMutableConfigFile()
, _immutableConfigFile()
, _immutableADCConfigFile()
, _logger(Logger::get("Monitor.SettingsManager"))
{
    poco_debug_f1(_logger, "_channelsNumber=%d", _channelsNumber );
    for(unsigned int i = 0; i < _channelsNumber; ++i) {
        _channelsSettings.push_back(new ChannelSettings());
        poco_debug(_logger, "_channelsSettings.push_back(new ChannelSettings())");
    }
}

void SettingsManager::setMutableConfigFilePath(const std::string& config)
{
    _mutableConfigFile = config;
}

void SettingsManager::setSavedMutableConfigFilePath(const std::string& config)
{
    _savedMutableConfigFile = config;
}

void SettingsManager::setImmutableConfigFilePath(const std::string& config)
{
    _immutableConfigFile = config;
}

void SettingsManager::setImmutableADCConfigFilePath(const std::string& config)
{
    _immutableADCConfigFile = config;
}

void SettingsManager::update()
{
    // TRACE();
    AutoPtr<PropertyFileConfiguration> pConfig          = new PropertyFileConfiguration(_mutableConfigFile);
    AutoPtr<PropertyFileConfiguration> pPermanentConfig = new PropertyFileConfiguration(_immutableConfigFile);
    AutoPtr<LayeredConfiguration> pLayeredConfig        = new LayeredConfiguration();

    pLayeredConfig->add(pPermanentConfig);
    pLayeredConfig->addWriteable(pConfig, 0);

    _channelsSettings[0]->save(CHANNEL_ID_COMMON_SETTING, pLayeredConfig);
    _commonSettings.save(CHANNEL_ID_COMMON_SETTING, pLayeredConfig);
    pConfig->save(_mutableConfigFile);
}

void SettingsManager::saveMutableConfig()
{
    //if(remountRootFS(false) != -1) {
        ifstream src(_mutableConfigFile.c_str(),        ios::binary);
        ofstream dst(_savedMutableConfigFile.c_str(),   ios::binary);

        dst << src.rdbuf() << flush;

        src.close();
        dst.close();

        sync();

        /*if(remountRootFS(true) == -1) {
            poco_error(_logger, "Failed to remount filesystem in RO mode");
        } 
        */ 
    /*} else {
        poco_error(_logger, "Failed to remount filesystem in RW mode");
    } 
    */ 
}

ChannelSettings& SettingsManager::getChannelSettings(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return *(_channelsSettings[channel]);
}

CommonSettings& SettingsManager::getCommonSettings()
{
    return _commonSettings;
}

void SettingsManager::readConfiguration()
{
    // TRACE();
    bool add_root_etc = false;
    try {
        poco_trace_f3(Logger::get("MonitorX"), "_mutableConfigFile = %s, _immutableConfigFile = %s, _immutableADCConfigFile = %s", _mutableConfigFile, _immutableConfigFile, _immutableADCConfigFile); 
        AutoPtr<PropertyFileConfiguration> pConfig          = new PropertyFileConfiguration(_mutableConfigFile);
        AutoPtr<PropertyFileConfiguration> pPermanentADCConfig;
        try {
            pPermanentADCConfig = new PropertyFileConfiguration(_immutableADCConfigFile);    
            add_root_etc = true;
        } catch (...) {
            poco_trace_f1(Logger::get("MonitorX"), "Error read cfg file ( %s )", _immutableADCConfigFile);
            add_root_etc = false;
        }
        
        AutoPtr<PropertyFileConfiguration> pPermanentConfig = new PropertyFileConfiguration(_immutableConfigFile);
        AutoPtr<LayeredConfiguration> pLayeredConfig        = new LayeredConfiguration();
        if(add_root_etc) {
            pLayeredConfig->add(pPermanentADCConfig);
        }
        pLayeredConfig->add(pPermanentConfig);
        pLayeredConfig->addWriteable(pConfig, 0);
        /*
        for(unsigned int i = 0; i < _channelsNumber; ++i) {
            _channelsSettings[i]->load(NumberFormatter::format(i), pLayeredConfig);        
        }
        */
        _channelsSettings[0]->load(CHANNEL_ID_COMMON_SETTING, pLayeredConfig);

        // poco_debug_f1(_logger, "immutableConfigFile - %s", immutableConfigFile);

        _commonSettings.load(CHANNEL_ID_COMMON_SETTING, pLayeredConfig);

        poco_trace(Logger::get("MonitorX"), "OK read cfg file.");
    }catch (...){
        poco_trace(Logger::get("MonitorX"), "Error read cfg file.");
         std::string str_remove;
        //    system("rw");
        str_remove = "rm ";
        str_remove.append(_savedMutableConfigFile.c_str());
        system(str_remove.c_str());
        str_remove = "rm ";
        str_remove.append(_mutableConfigFile.c_str());
        system(str_remove.c_str());
        system("rebootsys"); 
         
        return;
    }
}

} // namespace Settings


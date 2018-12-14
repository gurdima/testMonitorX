#pragma once
/*---------------------------------------------------------------------------*/
#include "Settings/Datum.hpp"
#include "Common/Typedefs.hpp"
#include "Common/Helpers.hpp"
#include "Common/Tracer.hpp"

#include "Settings/ChannelSettings.hpp"
#include "Settings/CommonSettings.hpp"
// #include "Settings/TPCCodefileManager.hpp"
// #include "TDMA/TDMAConfig.hpp"          // _TDMA_
// #include "TDMA/TDMASettings.hpp"          // _TDMA_
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
#include "Poco/SharedPtr.h"
/*---------------------------------------------------------------------------*/
#include <string>
#include <vector>
/*---------------------------------------------------------------------------*/

namespace Settings
{

/**
 * @brief Class to read/write settings of all channels in the 
 *        system
 */
class SettingsManager
{
public:

    explicit SettingsManager(unsigned int channelsNumber);

    void setMutableConfigFilePath(const std::string& config);
    void setSavedMutableConfigFilePath(const std::string& config);
    void setImmutableConfigFilePath(const std::string& config);
    void setImmutableADCConfigFilePath(const std::string& config);
/* 
    void setTPCConfigDirecoryPath(const std::string& path);
    void setTDMANetworkConfigDirecoryPath(const std::string& path); // _TDMA_
    void setTDMATerminalsConfigDirecoryPath(const std::string& path); // _TDMA_    
    void collectTDMAStatistics();
*/
    void readConfiguration();
    void update();
    void saveMutableConfig();
    ChannelSettings& getChannelSettings(const ChannelID& channel);
    CommonSettings& getCommonSettings();

//    TDMASettings& getTDMASettings();
//    const TPCCodefileManager* getTPCCodefileManager(const ChannelID& channel) const;

private:

    typedef Poco::SharedPtr<ChannelSettings> ChannelSettingsPtr;
    typedef std::vector<ChannelSettingsPtr>  ChannelsSettings;
/*
    typedef Poco::SharedPtr<TPCCodefileParamsHolder> TPCCodefileParamsHolderPtr;
    typedef Poco::SharedPtr<TPCCodefileManager> TPCCodefileManagerPtr;
    typedef std::vector<TPCCodefileManagerPtr>  TPCCodefileManagerContainer;
*/
    unsigned int _channelsNumber;

    std::string _mutableConfigFile;
    std::string _savedMutableConfigFile;
    std::string _immutableConfigFile;
    std::string _immutableADCConfigFile;
//    std::string _tpcDirectory;
//    std::string                     _tdmaNetworkConfigFile;         // _TDMA_
//    std::string                     _tdmaTerminalsConfigFile;       // _TDMA_

    ChannelsSettings                _channelsSettings;
    CommonSettings                  _commonSettings;
//    TPCCodefileManagerContainer     _pTPCCodefileManager;    
//    TDMASettings                    _tdmaSettings;
//    TDMAConfig                      _tdmaConfig; 

    Poco::Logger& _logger;
};

} // namespace Settings


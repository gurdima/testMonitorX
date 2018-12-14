#pragma once
/*---------------------------------------------------------------------------*/
#include "Notifications.hpp"
// #include "CodingManager.hpp"
#include "SysMonitor.hpp"

#include "Settings/ChannelSettings.hpp"
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "DeviceConfigurator.hpp"
#include "Common/Defines.hpp"
#include "Common/Tracer.hpp"
#include "Connection/SNMPSubsystem.hpp"
#include "FFT/NetworkSender.hpp"

// #include "ExchangeAgent10GE/AgentX10GESubsystem.hpp"

/*---------------------------------------------------------------------------*/
#include "Poco/SharedPtr.h"
#include "Poco/AutoPtr.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/NotificationCenter.h"
typedef Poco::SharedPtr<SysMonitor>     SysMonitorPtr;
// typedef Poco::SharedPtr<CodingManager>  CodingManagerPtr;
/*---------------------------------------------------------------------------*/
#include <vector>
/*---------------------------------------------------------------------------*/


class MonitorApplication : public Poco::Util::ServerApplication
{
public:
	MonitorApplication();
	~MonitorApplication();

    SysMonitorPtr getSysMonitor() const;
    bool StatusSetThisMode(unsigned int _thisMode);

protected:
	virtual void initialize(Application& self);
	virtual void uninitialize();
	virtual void defineOptions(Poco::Util::OptionSet& options);
	virtual void handleOption(const std::string& name, const std::string& value);
	virtual int main(const std::vector<std::string>& args);	
    
    void displayHelp();
private:

    typedef Poco::SharedPtr<DeviceConfigurator> DeviceConfiguratorPtr;
//    bool CreateCodingList() const;

    bool _helpRequested;
    Poco::NotificationCenter        _centralNC;
    std::string                     _configFile;
    SysMonitorPtr                   _sysMonitor;
    Settings::SettingsManager       _settingsManager;
    Statistics::StatisticsManager   _statisticsManager;
    DeviceConfiguratorPtr           _deviceConfigurator;
};


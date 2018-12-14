#pragma once
/*---------------------------------------------------------------------------*/
#include "BasicDataProvider.hpp" 
#include "Common/Tracer.hpp"
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"

#include "Poco/NotificationCenter.h"
#include "Poco/SharedPtr.h"
/*---------------------------------------------------------------------------*/

namespace MetersSpace {
class BasicSender
{
public:
    BasicSender(Poco::NotificationCenter& nc);

    virtual ~BasicSender() ;

    void getData(Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager);

protected:

    virtual void handleGetData(int port) = 0;
    Poco::SharedPtr<BasicDataProvider> _providerPtr;

private:
    Poco::NotificationCenter&       _externalNC;
};
}//namespace MetersSpace 


#pragma once
/*--------------------------------------------------------------------------*/
#include "Connection/ConnectionHandler.hpp"
#include "Connection/ZMQServer.hpp"
#include "Connection/ZMQPUBServer.hpp"

#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
/*--------------------------------------------------------------------------*/
#include "Poco/Thread.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Logger.h"
#include "Poco/NotificationCenter.h"
/*---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
namespace SNMP
{

class SNMPSubsystem : public Poco::Util::Subsystem
{
public:
    SNMPSubsystem(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~SNMPSubsystem();
    virtual void initialize(Poco::Util::Application& app);
    virtual void uninitialize();
    virtual const char* name() const;
private:
    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    Poco::NotificationCenter&       _nc;

    ConnectionHandler               _connHandler;
//    ZMQServer                       _connZMQServer;
//    ZMQPUBServer                    _connZMQPUBServer;
    Poco::Thread                    _thread1;
//    Poco::Thread                    _thread2;
//    Poco::Thread                    _thread3;

    Poco::Logger&                   _logger;
};

} // namespace SNMP

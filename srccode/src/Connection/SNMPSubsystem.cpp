#include "Connection/SNMPSubsystem.hpp"
/*---------------------------------------------------------------------------*/

#include "Common/Tracer.hpp"
/*---------------------------------------------------------------------------*/
using Poco::Logger;
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace SNMP
{
SNMPSubsystem::SNMPSubsystem(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _nc(nc)
, _connHandler(_settingsManager, _statisticsManager, _nc)
//, _connZMQServer(_settingsManager, _statisticsManager, _nc)
//, _connZMQPUBServer(_settingsManager, _statisticsManager, _nc)
, _thread1("SNMP_HTTP IO Thread")
//, _thread2("ZeroMQ IO Thread")
//, _thread3("ZMQPUB IO Thread")
, _logger(Logger::get("Monitor.SNMP.SNMPSubsystem"))
{
    TRACE();
}

SNMPSubsystem::~SNMPSubsystem()
{
   // TRACE();
}

void SNMPSubsystem::initialize(Poco::Util::Application& app)
{
    TRACE();
    _thread1.start(_connHandler);
//    _thread2.start(_connZMQServer);
//    _thread3.start(_connZMQPUBServer);
    
}

void SNMPSubsystem::uninitialize()
{
    TRACE();
    _connHandler.stop();
//    _connZMQServer.stop();
    _thread1.join();
//    _thread2.join();
}

const char* SNMPSubsystem::name() const
{
    return "SNMP";
}

} // namespace SNMP


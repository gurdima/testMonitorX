#pragma once
/*---------------------------------------------------------------------------*/
#include "Connection/ZMQHandler.hpp"
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Common/Noncopyable.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Runnable.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Logger.h"
#include "Poco/SharedPtr.h"
#include "Poco/Thread.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/Mutex.h"
/*---------------------------------------------------------------------------*/
#include "monitord_io.h"
#include "zmq.hpp"
#include "proto/GUI.pb.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace SNMP
{
    struct initDev
    {
		unsigned int Channel;
		unsigned int IDtime;
		unsigned int ClientIPAddr; 
		unsigned int MessageType;
    };
	struct descrDevTwoCh
	{
		unsigned int DescriptorCh1;
		unsigned int DescriptorCh2;
        unsigned int ClientIPAddrCh1;
        unsigned int ClientIPAddrCh2;
	};

class ZMQServer : public Poco::Runnable, public Noncopyable
{
public:
    ZMQServer(Settings::SettingsManager& settingsManager, Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~ZMQServer();

    virtual void run();
    void stop();

private:

    void processHeader(const ZMQServerManager& header);
    void sendDeviceInfo();
    void sendDeviceStatus();
              
    Settings::SettingsManager&      _settingsManager;
    Statistics::StatisticsManager&  _statisticsManager;
    Poco::NotificationCenter&       _nc;
    bool                            _stop;
    Poco::Logger&                   _logger;
    ZMQHandler                      _connZMQHandler1;
    ZMQHandler                      _connZMQHandler2;
    Poco::Thread                    _threadPoll1;
    Poco::Thread                    _threadPoll2;
    initDev                         pinitDev;
    descrDevTwoCh                   pdescrDev;
    bool                            _threadStart[2];
    Poco::Mutex                     _timerMutex;

    void*                           ZMQContext;
    void*                           ZMQSocket;
};

} // namespace SNMP

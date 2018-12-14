#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"
#include "Poco/Logger.h"
/*---------------------------------------------------------------------------*/
class SysMonitor : public Noncopyable
{
public:
    explicit SysMonitor(long updateInterval); // interval in milliseconds
    ~SysMonitor();

    void start();
    void stop();

    float getCPUUsage() const;
private:

    void updateJiffiesData();
    float calculateCPUUsage() const;

    void onUpdateTimer(Poco::Util::TimerTask& task);

    long _updateInterval;

    unsigned int _totalJiffiesPrev;
    unsigned int _totalJiffiesNext;
    unsigned int _workJiffiesPrev;
    unsigned int _workJiffiesNext;

    float _cpuLoadPercents;

    Poco::Util::Timer   _updateTimer;

    Poco::Logger&   _logger;
};

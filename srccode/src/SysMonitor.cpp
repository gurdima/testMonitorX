/*---------------------------------------------------------------------------*/
#include "SysMonitor.hpp"
/*---------------------------------------------------------------------------*/

#include "Poco/NumberParser.h"
#include "Poco/StringTokenizer.h"
using Poco::StringTokenizer;
using Poco::NumberParser;
using Poco::Logger;
using Poco::Util::TimerTask;
using Poco::Util::TimerTaskAdapter;
/*---------------------------------------------------------------------------*/
#include <string>
#include <fstream>
/*---------------------------------------------------------------------------*/
using std::string;
using std::fstream;
using std::getline;
/*---------------------------------------------------------------------------*/

#define STAT_FILE   "/proc/stat"

/*---------------------------------------------------------------------------*/

static void readJiffies(unsigned int& totalJiffies, unsigned int& workJiffies)
{
    unsigned int total = 0, work = 0;
    string line;
    fstream fs(STAT_FILE);

    getline(fs, line);

    // Line will be of such format
    //      user nice system idle iowait irq softirq steal
    // cpu  444  0    2679   89   3      1   2660    0

    StringTokenizer tok(line, " ", StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);

    // starting from 1 to skip 'cpu'
    for(unsigned int i = 1; i < tok.count(); ++i) {
        unsigned int val = NumberParser::parseUnsigned(tok[i]);

        total += val;

        if (i < 4) { // we want to sum 'user', 'nice', 'system'
            work += val;
        }
    }

    totalJiffies = total;
    workJiffies = work;
}

/*---------------------------------------------------------------------------*/

SysMonitor::SysMonitor(long updateInterval)
: _updateInterval(updateInterval)
, _totalJiffiesPrev(0)
, _totalJiffiesNext(1000)
, _workJiffiesPrev(0)
, _workJiffiesNext(1)
, _cpuLoadPercents(0.0)
, _logger(Logger::get("Monitor.SysMonitor"))
{
}

SysMonitor::~SysMonitor()
{
    stop();
}

void SysMonitor::start()
{
    TimerTask::Ptr pTask = new TimerTaskAdapter<SysMonitor>(*this, &SysMonitor::onUpdateTimer);
    _updateTimer.schedule(pTask, 0, _updateInterval);
}

void SysMonitor::stop()
{
    _updateTimer.cancel();
}

void SysMonitor::updateJiffiesData()
{
    _totalJiffiesPrev = _totalJiffiesNext;
    _workJiffiesPrev  = _workJiffiesNext;

    readJiffies(_totalJiffiesNext, _workJiffiesNext);
}

float SysMonitor::getCPUUsage() const
{
    return _cpuLoadPercents;
}

float SysMonitor::calculateCPUUsage() const
{
    unsigned int workOverPeriod  = _workJiffiesNext  - _workJiffiesPrev;
    unsigned int totalOverPeriod = _totalJiffiesNext - _totalJiffiesPrev;

    return static_cast<float>(workOverPeriod) / totalOverPeriod * 100;
}

void SysMonitor::onUpdateTimer(Poco::Util::TimerTask& task)
{
    updateJiffiesData();
    _cpuLoadPercents = calculateCPUUsage();

    //poco_information_f1(_logger, "CPU Load: %.2f%%", static_cast<double>(_cpuLoadPercents));
}


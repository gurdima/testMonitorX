#include "MonitorApplication.hpp"
#include "UART/tty.h"

/*---------------------------------------------------------------------------*/
// using ExchangeAgent::ExchangeAgentSubsystem;
/*---------------------------------------------------------------------------*/
#include "Poco/FileChannel.h"
#include "Poco/ConsoleChannel.h"

#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/ThreadPool.h"
#include "Poco/Process.h"
#include "Poco/File.h"

using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Logger;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Process;
using Poco::ProcessHandle;
using Poco::File;
using Poco::Path;
/*---------------------------------------------------------------------------*/
#include <fstream>
#include <iostream>     // std::cout, std::ostream, std::ios
#include <sys/mount.h>
#include <string>
#include <set>
#include <sys/reboot.h>

using std::string;
using std::ifstream;
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
static int remountRootRW(bool ro)
{
    unsigned long mountflags = MS_REMOUNT;
    if(ro) {
        mountflags |= MS_RDONLY;
        mountflags |= MS_SYNCHRONOUS;
    }
    return mount(NULL, "/", NULL, mountflags, NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
MonitorApplication::MonitorApplication()
: _helpRequested(false)
, _configFile()
, _settingsManager(CHANNELS_NUMBER)
, _statisticsManager(CHANNELS_NUMBER)
{
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%H:%M:%S.%i %P %I %q %s: %t");
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pCons));
    Logger::root().setChannel(pFC);
    Logger::root().setLevel("trace");
    Logger::get("MonitorX").information("Logger configured");

    Poco::ThreadPool::defaultPool().addCapacity(101);

    // addSubsystem(new ExchangeAgentSubsystem(CHANNELS_NUMBER, _statisticsManager, _centralNC));
    // addSubsystem(new AgentX10GESubsystem(_statisticsManager, _centralNC));
    addSubsystem(new SNMP::SNMPSubsystem(_settingsManager, _statisticsManager, _centralNC));
    addSubsystem(new MetersSpace::NetworkSender(CHANNELS_NUMBER, _settingsManager, _statisticsManager, _centralNC));
}

MonitorApplication::~MonitorApplication()
{
    poco_trace(Logger::get("MonitorX"), __func__);
}

void MonitorApplication::initialize(Application& self)
{
    
    poco_trace(Logger::get("MonitorX"), __func__);
    poco_trace(Logger::get("MonitorX"), "initialize(Application& self)");

    if (_configFile.empty()) {
        poco_trace(Logger::get("MonitorX"), "Using default configuration");
        loadConfiguration(); // load default configuration files, if present
    } else {
        poco_trace_f1(Logger::get("Monitor"), "Using configuration from file: %s", _configFile);
        loadConfiguration(_configFile);
    }

    // string cfgFilePath = config().getString("Coding.CfgFilePath", "/system/etc/codings.cfg");
    // string keyFilePath = config().getString("Coding.KeyFilePath", "/system/etc/codings.key");
    
    // _codingManager = new CodingManager(cfgFilePath, keyFilePath);
    // bool enabled_mod1 = _codingManager->isModulationEnabled("multichannel_dem");        
    //bool enabled_mod2 = _codingManager->isModulationEnabled("tdma_mode");   
    // bool enabled_b2ge = _codingManager->isModulationEnabled("bypass_2ge");   
    // bool enabled_FFT64k =  _codingManager->isModulationEnabled("fft_64k");   
    // _setAddModesMask = enabled_mod1 + enabled_mod2*2 + enabled_b2ge*4 + enabled_FFT64k*8;

    string mutableFilePath      = config().getString("Config.MutableFilePath",         "/system/ramdisk/mutable.cfg");
    string savedMutableFilePath = config().getString("Config.SavedMutableFilePath",    "/system/etc/mutable.cfg");
    string permanentFilePath    = config().getString("Config.ImmutableFilePath",       "/system/etc/immutable.cfg");
    string permanentADCFilePath = config().getString("Config.ImmutableADCFilePath",       "/etc/immutable_this_dev.cfg");
    // string tpcConfigsDirPath    = config().getString("Config.TPCConfigDirectoryPath",  "/system/etc/tpc_codes");
    try{
        _settingsManager.setMutableConfigFilePath(mutableFilePath);
        _settingsManager.setSavedMutableConfigFilePath(savedMutableFilePath);
        _settingsManager.setImmutableADCConfigFilePath(permanentADCFilePath); 
        _settingsManager.setImmutableConfigFilePath(permanentFilePath);
        // _settingsManager.setTPCConfigDirecoryPath(tpcConfigsDirPath);
    }catch (...){
        if(remountRootRW(false) != -1) {
            poco_trace(Logger::get("Monitor"), "Error read cfg file");
            unlink(savedMutableFilePath.c_str());
            unlink(mutableFilePath.c_str());
            sync();
            sleep(1);
            // reboot(RB_AUTOBOOT);
            return;
        }
    }
     poco_trace(Logger::get("MonitorXX"), "_settingsManager.readConfiguration()");
    _settingsManager.readConfiguration();   
      
    /*
    if(StatusSetThisMode(_settingsManager.getChannelSettings(0).getDEMSettings().getMode())==false || StatusSetThisMode(_settingsManager.getChannelSettings(1).getDEMSettings().getMode())==false){
        if(remountRootRW(false) != -1) {
            unlink(savedMutableFilePath.c_str());
            unlink(mutableFilePath.c_str());
            sync();
            sleep(1);
            reboot(RB_AUTOBOOT);
            return;
        }
    } 
    
    string tdmaNetConfigsDirPath= config().getString("Config.TDMANetConfigDirectoryPath", "/system/etc/tdma_networks.cfg"); // _TDMA_
    string tdmaTerConfigsDirPath= config().getString("Config.TDMATerConfigDirectoryPath", "/system/etc/tdma_terminals.cfg"); // _TDMA_

    _settingsManager.setTDMANetworkConfigDirecoryPath(tdmaNetConfigsDirPath); // _TDMA_
    _settingsManager.setTDMATerminalsConfigDirecoryPath(tdmaTerConfigsDirPath); // _TDMA_
    _settingsManager.collectTDMAStatistics();
    
    string decodersFilePath     = config().getString("Config.DecodersFilePath",     "/system/etc/decoders.cfg");
    string demodulatorsFilePath = config().getString("Config.DemodulatorsFilePath", "/system/etc/demodulators.cfg");
    _statisticsManager.setDecodersConfigFilePath(decodersFilePath);
    _statisticsManager.setDemodulatorsConfigFilePath(demodulatorsFilePath);
    _statisticsManager.collectImmutableStatistics(*_codingManager);
    */

    ServerApplication::initialize(self);
}

void MonitorApplication::uninitialize()
{
    poco_trace(Logger::get("Monitor"), __func__);
    ServerApplication::uninitialize();
}

void MonitorApplication::defineOptions(OptionSet& options)
{
    ServerApplication::defineOptions(options);

    options.addOption(Option("help", "h", "display help information on command line arguments")
                     .required(false)
                     .repeatable(false));

    options.addOption(Option("config", "c", "load configuration data from a file")
                     .required(false)
                     .repeatable(false)
                     .argument("FILE"));
}

void MonitorApplication::handleOption(const std::string& name, const std::string& value)
{
    poco_trace(Logger::get("Monitor"), __func__);

    ServerApplication::handleOption(name, value);

    if (name == "help") {
        _helpRequested = true;
    }

    if (name == "config") {
        _configFile = value;
    }
}

void MonitorApplication::displayHelp()
{
    poco_trace(Logger::get("Monitor"), __func__);

    HelpFormatter helpFormatter(options());
    helpFormatter.setCommand(commandName());
    helpFormatter.setUsage("OPTIONS");
    helpFormatter.setHeader("Monitor application.");
    helpFormatter.format(std::cout);
}

SysMonitorPtr MonitorApplication::getSysMonitor() const
{
    return _sysMonitor;
}


int MonitorApplication::main(const std::vector<std::string>& args)
{
    poco_trace(Logger::get("MonitorX"), __func__);
    
    if (_helpRequested) {
        displayHelp();
    } else {
        /*
        int fd = openUART1();	
        if(fd==0) {
            printf ("Error open UART0");
        } else {
            char Uminodiode[14];
            strcpy(Uminodiode, "#w0220000000\r\n");
            writeUART(fd, Uminodiode);
            closeUART(fd);
        }
        */
        poco_information_f1(Logger::get("MonitorX"), "MonitiorX is running in %s mode", string(isInteractive() ? "INTERACTIVE" : "DAEMON"));
    
        try {
            _sysMonitor = new SysMonitor(5000);
            _sysMonitor->start();

            _deviceConfigurator = new DeviceConfigurator(_settingsManager, _statisticsManager, _centralNC);
            _deviceConfigurator->initDevice();

           // SNMP starting code
//            Process::Args args;
//            args.push_back("-M");
//            poco_information(Logger::get("Monitor"), "Starting SNMPD");
//            ProcessHandle snmpHandle = Process::launch("/system/bin/Rcv20Table_subagent", args);
        }
        catch (const Poco::Exception& exc) {
            Logger::get("MonitorX").log(exc);
            //Logger::get("MonitorX").debug(memctl_convert_status_to_string(exc.code()));
        }
    
        // wait for CTRL-C or kill
        waitForTerminationRequest();
    }

    return Application::EXIT_OK;
}

POCO_SERVER_MAIN(MonitorApplication)

#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Statistics
{

class CommonStatistics : public Noncopyable
{
public:

    friend class StatisticsManager;

    unsigned char getConfigurationPending() const
    {
        return _configurationPending;
    }

    void setConfigurationPending(unsigned char pending)
    {
        // poco_debug_f1(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "Configuration pending = %u", static_cast<unsigned int>(pending));
        _configurationPending = pending;
    }

    unsigned char getVirtualPending() const
    {
        return _virtualPending;
    }

    void setVirtualPending(unsigned char pending)
    {
         // poco_debug_f1(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "Virtual pending = %u", static_cast<unsigned int>(pending));
        _virtualPending = pending;
    }

    bool statusChannelControlConnection(unsigned int channel) const
    {
//        poco_debug_f1(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "TRACE 1. statusChannelControlUDP. ControlUDP = %u", _controlUDP);
       if(channel==0){
           for(int i=0;i<MAX_SUBCHANNELS_NUMBER;i++) {
               if(controlUDP_Ch0[i]>0){
                    return true;
               }
           }
           return false;
       }
       if(channel==1){
           for(int i=0;i<MAX_SUBCHANNELS_NUMBER;i++) {
               if(controlUDP_Ch1[i]>0){
                    return true;
               }
           }
           return false;
        }
       return false;
    }
         
    bool addStatusControlConnection(unsigned int channel,  unsigned int subchannel)
    {
//        poco_debug_f2(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "addControlUDP. channel=%u, subchannel=%u", channel, subchannel);
        if(channel==0 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            controlUDP_Ch0[subchannel]++;
            return true;
        }
        if(channel==1 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            controlUDP_Ch1[subchannel]++;
            return true;
        }
        return false;
    }

    bool lessStatusControlConnection(unsigned int channel,  unsigned int subchannel)
    {
//        poco_debug_f2(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "lessControlUDP. channel=%u, subchannel=%u", channel, subchannel);
        if(channel==0 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            controlUDP_Ch0[subchannel]--;
            return true;
        }
        if(channel==1 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            controlUDP_Ch1[subchannel]--;
            return true;
        }
        return false;        
    }

    void addStatusSetNewConnect(unsigned int channel,  unsigned int subchannel)
    {
//        poco_debug_f2(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "+ StatusSetNewConnect. channel=%u, subchannel=%u", channel, subchannel);
        if(channel==0 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            connnectCh0[subchannel] = true;
        }
        if(channel==1 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            connnectCh1[subchannel] = true;
        }
    }

    void lessStatusSetNewConnect(unsigned int channel,  unsigned int subchannel)
    {
//        poco_debug_f2(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "- StatusSetNewConnect. channel=%u, subchannel=%u", channel, subchannel);
        if(channel==0 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            connnectCh0[subchannel] = false;
        }
        if(channel==1 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            connnectCh1[subchannel] = false;
        }
    }

    bool getStatusSetNewConnect(unsigned int channel,  unsigned int subchannel)
    {
        if(channel==0 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            return connnectCh0[subchannel];
        }
        if(channel==1 && subchannel < MAX_SUBCHANNELS_NUMBER) {
            return connnectCh1[subchannel];
        }
        return false;
    }

    unsigned int getConfigurationLeader(unsigned int _channel)  const
    {
        if(_channel==0) {
            return DescriptorLiderCh1;
        }
        if(_channel==1) {
            return DescriptorLiderCh2;
        }
        return false;
    }

    void setConfigurationLeader(unsigned int _channel, unsigned int _descriptor)
    {
        if(_channel==0) {
            DescriptorLiderCh1 = _descriptor;
        }
        if(_channel==1) {
            DescriptorLiderCh2 = _descriptor;
        }
    }

    void setTunerLock(unsigned int _channel, bool _tunerLock) 
    {
        if(_channel==0) {
            TunerLockCh1 = _tunerLock;
        }
        if(_channel==1) {
            TunerLockCh2 = _tunerLock;
        }
    }

    bool getTunerLock(unsigned int _channel)  const
    {
        if(_channel==0) {
            return TunerLockCh1;
        }
        if(_channel==1) {
            return TunerLockCh2;
        }
        return false;
    }

    void setFirstTunerConfig(unsigned char channel, bool value)
    {
        if(value) {
            if(!(_firstTunerConfig&Registr16[channel])) {
                _firstTunerConfig = _firstTunerConfig + Registr16[channel];
            }
        } else {
            if(_firstTunerConfig&Registr16[channel]) {
                _firstTunerConfig = _firstTunerConfig - Registr16[channel];
            }
        }
        // poco_debug_f2(Poco::Logger::get("Monitor.Statistics.CommonStatistics"), "Configuration channel %u firstTunerConfig = %u", static_cast<unsigned int>(channel), static_cast<unsigned int>(_firstTunerConfig));
    }

    bool getFirstTunerConfig(unsigned char channel) const
    {
        if(_firstTunerConfig&Registr16[channel]) {
            return true;
        }
        return false;
    }

private:
    CommonStatistics()
    : _configurationPending(false)
    {
        Registr16[0] = 1;
        Registr16[1] = 2;
        Registr16[2] = 4;
        Registr16[3] = 8;
        Registr16[4] = 16;
        Registr16[5] = 32;
        Registr16[6] = 64;
        Registr16[7] = 128;
        Registr16[8] = 256;
        Registr16[9] = 512;
        Registr16[10] = 1024;
        Registr16[11] = 2048;
        Registr16[12] = 4096;
        Registr16[13] = 8192;
        Registr16[14] = 16384;
        Registr16[15] = 32768;
        _configurationPending = 0;
        _firstTunerConfig=0;
        _virtualPending=0;

        for(int i=0;i<MAX_SUBCHANNELS_NUMBER;i++) {
            connnectCh0[i]=false;
            connnectCh1[i]=false;
            controlUDP_Ch0[i]=0;
            controlUDP_Ch1[i]=0;
        }
    }

    unsigned char _configurationPending;
    unsigned char _virtualPending;
    unsigned char _firstTunerConfig;

    unsigned int DescriptorLiderCh1;
    unsigned int DescriptorLiderCh2;
    unsigned int Registr16[16];
    bool  TunerLockCh1;
    bool  TunerLockCh2;
    bool  connnectCh0[MAX_SUBCHANNELS_NUMBER];
    bool  connnectCh1[MAX_SUBCHANNELS_NUMBER];
    unsigned int  controlUDP_Ch0[MAX_SUBCHANNELS_NUMBER];
    unsigned int  controlUDP_Ch1[MAX_SUBCHANNELS_NUMBER];
};

} // namespace Statistics

#pragma once

/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
/*---------------------------------------------------------------------------*/
#include <iostream>
#include <string.h>
using std::string;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Statistics
{

class TunerStatistics : public Noncopyable
{
public:
    friend class ChannelStatistics;

    void setPresence(bool present);
    bool getPresence() const;

    void setReferenceFrequency(unsigned int frequency);
    unsigned int getReferenceFrequency() const;

    void setLPFCutoffFrequency(unsigned int frequency);
    unsigned int getLPFCutoffFrequency() const;

    void setSerialNumber(unsigned int number);
    unsigned int getSerialNumber() const;

    void setHWRevisionType(unsigned int number);
    unsigned int getHWRevisionType() const;

    void setTemperature(unsigned int number);
    unsigned int getTemperature() const;

    void setAGCEndOfSearch(bool end);           //
    bool getAGCEndOfSearch() const;             //
                                                //
    void setAGCLock(bool lock);                 //
    bool getAGCLock() const;                    //

    void setAGCManualGainControl(bool manual);
    bool getAGCManualGainControl() const;
                                                //
    void setAGCError(unsigned int error);       //
    unsigned int getAGCError() const;           //

    void setAGCToo_High(unsigned int error);    
    unsigned int getAGCToo_High() const;        

    void setAGCToo_Low(unsigned int error);    
    unsigned int getAGCToo_Low() const;        

    void setAGCDetectVoltage(unsigned int voltage);
    unsigned int getAGCDetectVoltage() const;

    void setAGCLBandGain(unsigned int gain);
    unsigned int getAGCLBandGain() const;

    void setAGCBaseBandGain(unsigned int gain);
    unsigned int getAGCBaseBandGain() const;

    void setAGCInputSignalLevel(int level);
    int getAGCInputSignalLevel() const;

    void setAGCInit(unsigned int lock);
    unsigned int getAGCInit() const;

    std::string getSoftwareVersion() const;
    void setSoftwareVersion(std::string new_name);

    std::string getTunerSoftwareRelease() const;
    void setTunerSoftwareRelease(std::string new_name);

    void setEthernetLinkStatus(unsigned int val);
    unsigned int getEthernetLinkStatus() const;

    unsigned int getEthernetIPDestination() const;
    void setEthernetIPDestination(unsigned int val);

    unsigned int getEthernetDestinationPort() const;
    void setEthernetDestinationPort(unsigned int val);

    unsigned int getEthernetDataPackets() const;
    void setEthernetDataPackets(unsigned int val);

private:
    TunerStatistics();

    bool            _presence;
    unsigned int    _refFrequency;
    unsigned int    _serialNumber;
    unsigned char   _type;
    unsigned char   _temperature;
    bool            _endOfSearch;
    bool            _lock;
    bool            _manual;
    unsigned int    _agcError;
    unsigned int    _agcTooHigh;
    unsigned int    _agcTooLow;
    unsigned int    _lpfCutoffFrequency;
    unsigned int    _detectVoltage;
    unsigned int    _lBandGain;
    unsigned int    _baseBandGain;
    unsigned int    _inputSignalLevel;
    unsigned int    _agcInit;
    std::string     _soft_version;
    std::string     _soft_release;
    unsigned int    _ethernetLinkStatus;
    unsigned int    _ip_destination;
    unsigned int    _destination_port;
    unsigned int    _data_packets;
};

} // namespace Statistics


#include "Statistics/AGCTunerStatistics.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Statistics
{

TunerStatistics::TunerStatistics()
: _presence(false)
, _refFrequency(0)
, _serialNumber(0)
, _type(0)
, _temperature(0)
, _endOfSearch(false)
, _lock(false)
, _manual(false)
, _agcError(0)
, _agcTooHigh(0)
, _agcTooLow(0)
, _lpfCutoffFrequency(0)
, _detectVoltage(0)
, _lBandGain(0)
, _baseBandGain(0)
, _inputSignalLevel(0)
, _agcInit(0)
, _ethernetLinkStatus(0)
, _ip_destination(0)
, _destination_port(0)
, _data_packets(0)
{

}

void TunerStatistics::setPresence(bool present)
{
    _presence = present;
}

bool TunerStatistics::getPresence() const
{
    return _presence;
}

void TunerStatistics::setReferenceFrequency(unsigned int frequency)
{
    _refFrequency = frequency;
}

unsigned int TunerStatistics::getReferenceFrequency() const
{
    return _refFrequency;
}

void TunerStatistics::setLPFCutoffFrequency(unsigned int frequency)
{
    _lpfCutoffFrequency = frequency;
}

unsigned int TunerStatistics::getLPFCutoffFrequency() const
{
    return _lpfCutoffFrequency;
}

void TunerStatistics::setSerialNumber(unsigned int number)
{
    _serialNumber = number;
}

unsigned int TunerStatistics::getSerialNumber() const
{
    return _serialNumber;
}

void TunerStatistics::setHWRevisionType(unsigned int type)
{
    _type = type;
}

unsigned int TunerStatistics::getHWRevisionType() const
{
    return _type;
}

void TunerStatistics::setTemperature(unsigned int temp)
{
    _temperature = temp;
}

unsigned int TunerStatistics::getTemperature() const
{
    return _temperature;
}

void TunerStatistics::setAGCEndOfSearch(bool end)
{
    _endOfSearch = end;
}

bool TunerStatistics::getAGCEndOfSearch() const
{
    return _endOfSearch;
}

void TunerStatistics::setAGCLock(bool lock)
{
    _lock = lock;
}

bool TunerStatistics::getAGCLock() const
{
    return _lock;
}


void TunerStatistics::setAGCManualGainControl(bool manual)
{
    _manual = manual;
}

bool TunerStatistics::getAGCManualGainControl() const
{
    return _manual;
}


void TunerStatistics::setAGCError(unsigned int error)
{
    _agcError = error;
}

unsigned int TunerStatistics::getAGCError() const
{
    return _agcError;
}


void TunerStatistics::setAGCToo_High(unsigned int val)
{
    _agcTooHigh = val;
}
unsigned int TunerStatistics::getAGCToo_High() const
{
    return _agcTooHigh;
}


void TunerStatistics::setAGCToo_Low(unsigned int val)
{
    _agcTooLow = val;
}
unsigned int TunerStatistics::getAGCToo_Low() const
{
    return _agcTooLow;
}

void TunerStatistics::setAGCDetectVoltage(unsigned int voltage)
{
    _detectVoltage = voltage;
}

unsigned int TunerStatistics::getAGCDetectVoltage() const
{
    return _detectVoltage;
}

void TunerStatistics::setAGCLBandGain(unsigned int gain)
{
    _lBandGain = gain;
}

unsigned int TunerStatistics::getAGCLBandGain() const
{
    return _lBandGain;
}

void TunerStatistics::setAGCBaseBandGain(unsigned int gain)
{
    _baseBandGain = gain;
}

unsigned int TunerStatistics::getAGCBaseBandGain() const
{
    return _baseBandGain;
}

void TunerStatistics::setAGCInputSignalLevel(int level)
{
    _inputSignalLevel = level;
}

int TunerStatistics::getAGCInputSignalLevel() const
{
    return _inputSignalLevel;
}

void TunerStatistics::setAGCInit(unsigned int val)
{
    _agcInit = val;
}
unsigned int TunerStatistics::getAGCInit() const
{
    return _agcInit;
}

void TunerStatistics::setSoftwareVersion(std::string new_name){
    _soft_version = new_name;
}
std::string TunerStatistics::getSoftwareVersion() const
{
    return _soft_version;
};

void TunerStatistics::setTunerSoftwareRelease(std::string new_name){
    _soft_release = new_name;
}
std::string TunerStatistics::getTunerSoftwareRelease() const
{
    return _soft_release;
};

void TunerStatistics::setEthernetLinkStatus(unsigned int val)
{
    _ethernetLinkStatus = val;
}
unsigned int TunerStatistics::getEthernetLinkStatus() const
{
    return _ethernetLinkStatus;
}

unsigned int TunerStatistics::getEthernetIPDestination() const
{
    return _ip_destination;
}
void TunerStatistics::setEthernetIPDestination(unsigned int val)
{
    _ip_destination = val;
}

unsigned int TunerStatistics::getEthernetDestinationPort() const
{
    return _destination_port;
}
void TunerStatistics::setEthernetDestinationPort(unsigned int val)
{
    _destination_port = val;
}

unsigned int TunerStatistics::getEthernetDataPackets() const
{
    return _data_packets;
}
void TunerStatistics::setEthernetDataPackets(unsigned int val)
{
    _data_packets = val;
}

} // namespace Statistics


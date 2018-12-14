#pragma once
/*---------------------------------------------------------------------------*/
#include "Common/Noncopyable.hpp"
// #include "ExchangeAgent/UDPPortStatistics.hpp"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Statistics
{

class ConnectionStatistics : public Noncopyable
{
public:
    friend class ChannelStatistics;
    /*
    void setUDPStatistics(const ExchangeAgent::UDPStatistics& udp)
    {
        _udpStat = udp;
    }

    const ExchangeAgent::UDPStatistics& getUDPStatistics() const
    {
        return _udpStat;
    }


    void setEthernetLinkStatus(unsigned int status)
    {
        _etherLinkStat = status;
    }

    unsigned int getEthernetLinkStatus() const
    {
        return _etherLinkStat;
    }


    void setTransmissionSpeed(unsigned int speed)
    {
        _speed = speed;
    }

    unsigned int getTransmissionSpeed() const
    {
        return _speed;
    }

    void setChannelTotalSpeed(unsigned int speed)
    {
        _channelTotalSpeed = speed;
    }

    unsigned int getChannelTotalSpeed() const
    {
        return _channelTotalSpeed;
    }
    */
private:
    ConnectionStatistics()
    : _udpStat()
    , _speed(0)
    , _etherLinkStat(0)
    , _channelTotalSpeed(0)
    {
    }

    // ExchangeAgent::UDPStatistics _udpStat;
    unsigned int _speed;
    unsigned int _etherLinkStat;
    unsigned int _channelTotalSpeed;
};

} // namespace Statistics



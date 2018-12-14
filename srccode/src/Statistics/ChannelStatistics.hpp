#pragma once
/*---------------------------------------------------------------------------*/

#include "Statistics/AGCTunerStatistics.hpp"
// #include "Statistics/DVBS2Statistics.hpp"
// #include "Statistics/DVBS2DecoderStatistics.hpp"
// #include "Statistics/CommonDecoderStatistics.hpp"
// #include "Statistics/TPCStatistics.hpp"
// #include "Statistics/LDPCStatistics.hpp"
// #include "Statistics/PostProcessingStatistics.hpp"
// #include "Statistics/ConnectionStatistics.hpp"
// #include "Statistics/M2KStatistics.hpp"
// #include "TDMA/TDMAStatistics.hpp"
#include "Common/Noncopyable.hpp"
#include "Common/Defines.hpp"
#include "Common/Typedefs.hpp"
#include "Common/Helpers.hpp"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Statistics
{

class ChannelStatistics : public Noncopyable
{
public:
    friend class StatisticsManager;

    TunerStatistics& getTunerStatistics()
    {
        return _tuner;
    }
    /*
    DVBS2Statistics& getDVBS2Statistics()
    {
        return _dvbs2;
    }

    DVBS2Statistics& getHDRMStatistics(const SubchannelID& subchannel)
    {
        checkSubchannelNumber(subchannel);
        return _hdrm[subchannel];
    }

    DVBS2DecoderStatistics& getDVBS2DecoderStatistics()
    {
        return _dvbs2Decoder;
    }

    CommonDecoderStatistics& getCommonDecoderStatistics()
    {
        return _decoder;
    }

    TPCStatistics& getTPCStatistics()
    {
        return _tpc;
    }

    LDPCStatistics& getLDPCStatistics()
    {
        return _ldpc;
    }

    PostProcessingStatistics& getPostProcessingStatistics()
    {
        return _postProcessing;
    }

    ConnectionStatistics& getConnectionStatistics(const SubchannelID& subchannel)
    {
        return _connectionStat[subchannel];
    }

    TDMAStatistics& getTDMAStatistics(const SubchannelTDMAID& subchannel)
    {
        return _tdma[subchannel];
    }
     
    M2KStatistics& getM2KStatistics()  
    {
        return _m2k;
    } 
    */
    void setLeaderStatus(unsigned int _leaderStatus)
    {   
            _idLeading = _leaderStatus;
    }

    unsigned int getLeaderStatus()
    {
        return  _idLeading;
    }

    void addChannelStatus(unsigned int _status)
    {   
        if(!(_channelStatus  & _status)) {
             _channelStatus = _channelStatus + _status;
        }
    }

    void lessChannelStatus(unsigned int _status)
    {   
        if(_channelStatus  & _status) {
             _channelStatus =_channelStatus - _status;
        }
    }

    unsigned int getChannelStatus()
    {
        return  _channelStatus;
    }

private:
    ChannelStatistics()
    {
    }

    TunerStatistics             _tuner;
/* 
    DVBS2Statistics             _dvbs2;
    DVBS2Statistics             _hdrm[SUBCHANNELS_NUMBER]; // it is suitable for hdrm too
    DVBS2DecoderStatistics      _dvbs2Decoder;
    CommonDecoderStatistics     _decoder;
    TPCStatistics               _tpc;
    LDPCStatistics              _ldpc;
    PostProcessingStatistics    _postProcessing;
    TDMAStatistics              _tdma[SUBCHANNELS_NUMBER_FOR_TDMA]; // it is suitable for hdrm too
    M2KStatistics               _m2k;
*/
    // ConnectionStatistics        _connectionStat[SUBCHANNELS_NUMBER_FOR_TDMA];
    unsigned int                _idLeading;
    unsigned int                _channelStatus;
};

} // namespace Statistics


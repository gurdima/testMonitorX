#pragma once
/*---------------------------------------------------------------------------*/
// #include "CodingManager.hpp"

#include "Common/Typedefs.hpp"
#include "Common/Helpers.hpp"

#include "Statistics/ChannelStatistics.hpp"
// #include "Statistics/DecoderStatistics.hpp"
#include "Statistics/CommonStatistics.hpp"

//#include "Decoder/DecoderConfigParser.hpp"
//#include "Demodulator/DemodulatorConfigParser.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/SharedPtr.h"
/*---------------------------------------------------------------------------*/
#include <vector>
#include <string>
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
namespace Statistics
{

class StatisticsManager
{
public:

    StatisticsManager(unsigned int channelsNumber);

//    void setDecodersConfigFilePath(const std::string& config);
//    void setDemodulatorsConfigFilePath(const std::string& config);
//    void collectImmutableStatistics(const CodingManager& codingMgr);
    ChannelStatistics& getChannelStatistics(const ChannelID& channel);
//    const DecoderStatistics& getDecoderStatistics() const;
//    Decoder::DecoderConfigParser& getDecoderConfigParser();
//    Demodulator::DemodulatorConfigParser& getDemodulatorConfigParser();
    CommonStatistics& getCommonStatistics();

private:
    typedef Poco::SharedPtr<ChannelStatistics> ChannelStatisticsPtr;
    typedef std::vector<ChannelStatisticsPtr>  ChannelsStatistics;
/*    unsigned int makeEnabledDecodersBitMask(const CodingManager& codingMgr);
    std::string                             _decodersConfigFile;
    std::string                             _demodulatorsConfigFile;
    Decoder::DecoderConfigParser            _decoderConfigParser;
    Demodulator::DemodulatorConfigParser    _demodulatorConfigParser;
    DecoderStatistics               _decoderStat;
*/
    CommonStatistics                _commonStat;
    ChannelsStatistics              _stat;

};

} // namespace Statistics


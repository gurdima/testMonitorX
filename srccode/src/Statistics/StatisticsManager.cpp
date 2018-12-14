#include "Statistics/StatisticsManager.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"

using Poco::Logger;
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Statistics
{
StatisticsManager::StatisticsManager(unsigned int channelsNumber)
//: _decodersConfigFile()
//, _decoderConfigParser()
// , _decoderStat()
: _commonStat()
, _stat()
{
    // ????????????????!!!!!!!!!!
    for(unsigned int i = 0; i < channelsNumber; ++i) {
        _stat.push_back(new ChannelStatistics());
    } 
}
/*
void StatisticsManager::setDecodersConfigFilePath(const std::string& config)
{
    _decodersConfigFile = config;
}

void StatisticsManager::setDemodulatorsConfigFilePath(const std::string& config)
{
    _demodulatorsConfigFile = config;
}

void StatisticsManager::collectImmutableStatistics(const CodingManager& codingMgr)
{
    _decoderConfigParser.parse(_decodersConfigFile);
    _demodulatorConfigParser.parse(_demodulatorsConfigFile);
    
    _decoderStat.setModes(_decoderConfigParser.makeDecodersString());
    _decoderStat.setModesMask(makeEnabledDecodersBitMask(codingMgr));

    _decoderStat.setModulations(_decoderConfigParser.makeModulationsString());
    _decoderStat.setInnerCodeRate(_decoderConfigParser.makeInnerCodeRatesString());
    _decoderStat.setOuterCodeRate(_decoderConfigParser.makeOuterCodeRatesString());
}
*/
ChannelStatistics& StatisticsManager::getChannelStatistics(const ChannelID& channel)
{
    checkChannelNumber(channel);

    return *(_stat[channel]);
}
/*
const DecoderStatistics& StatisticsManager::getDecoderStatistics() const
{
    return _decoderStat;
}

Decoder::DecoderConfigParser& StatisticsManager::getDecoderConfigParser()
{
    return _decoderConfigParser;
}

Demodulator::DemodulatorConfigParser& StatisticsManager::getDemodulatorConfigParser()
{
    return _demodulatorConfigParser;
}
*/
CommonStatistics& StatisticsManager::getCommonStatistics()
{
    return _commonStat;
}
/*
unsigned int StatisticsManager::makeEnabledDecodersBitMask(const CodingManager& codingMgr)
{
    unsigned int mask = 0, id;
    for(Decoder::DecoderConfigParser::DecodersNamesContainer::const_iterator it = _decoderConfigParser.getAvailableDecoders().begin();
        it != _decoderConfigParser.getAvailableDecoders().end();
        ++it) {
        
        if(codingMgr.isCodingEnabled(*it)) {
            id = _decoderConfigParser.getId(*it);
            mask |= 0x1UL << id;
        }
    }
    return mask;
}
*/
} // namespace Statistics


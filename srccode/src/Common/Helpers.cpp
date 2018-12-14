#include "Common/Helpers.hpp"
#include "Common/Defines.hpp"
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
#include "Poco/Exception.h"
#include "Poco/Format.h"

using Poco::format;
using Poco::Logger;
/*---------------------------------------------------------------------------*/
#include <fstream>
/*---------------------------------------------------------------------------*/

#define SW_VERSION_PATH "/system/etc/sw_version"
#define DFLT_VERSION "0.0.0"

/*---------------------------------------------------------------------------*/

std::string getSoftwareVersion()
{
    std::fstream verStream(SW_VERSION_PATH, std::ios_base::in);
    std::string version = DFLT_VERSION;

    if(!verStream.eof() && !verStream.fail()) {
        std::getline(verStream, version);
    }

    return version;
}

/*---------------------------------------------------------------------------*/

bool isValidChannelNumber(const ChannelID& channel)
{
    if(channel >= SUBCHANNELS_NUMBER_FOR_TDMA) {
        return false;
    }

    return true;
}

void checkChannelNumber(const ChannelID& channel)
{
    if (!isValidChannelNumber(channel)) {
        throw Poco::RangeException(format("Invalid channel number - %u", channel));
    }
}

bool isValidSubchannelNumber(const SubchannelID& subchannel)
{
    if(subchannel >= SUBCHANNELS_NUMBER_FOR_TDMA) { // SUBCHANNELS_NUMBER
        return false;
    }

    return true;
}

void checkSubchannelNumber(const SubchannelID& subchannel)
{
    if (!isValidSubchannelNumber(subchannel)) {
        throw Poco::RangeException(format("Invalid subchannel number - %u", subchannel));
    }
}

/*---------------------------------------------------------------------------*/

bool readUntilNewline(Poco::Net::StreamSocket& socket, std::string& collector)
{
    for(;;) {
        char buffer[64] = { 0 };
        int n = socket.receiveBytes(buffer, sizeOfArray(buffer));

        if(n > 0) {
            collector += buffer;
            if(collector.find(0x0D) != std::string::npos ||
               collector.find(0x0A) != std::string::npos) {
                return true;
            }
        } else {
            return false;
        }
    }

    return false; // should never get here
}


void sendString(Poco::Net::StreamSocket& socket, std::string& data)
{
    socket.sendBytes(data.c_str(), data.length());
}

bool readBytes(Poco::Net::StreamSocket& socket, char* buffer, const size_t number)
{
    size_t bytesRead = 0;

    for(;;) {
        int n = socket.receiveBytes(buffer + bytesRead, number - bytesRead);
        if(n > 0) {
            bytesRead += n;
            if(number == bytesRead) {
                return true;
            }
        } else {
            return false;
        }
    }

    return false;
}

/** This file is licensed under GPLv2.
 **
 ** This program is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU General Public License as
 ** published by the Free Software Foundation; either version 2 of the
 ** License, or (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful, but
 ** WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 ** USA
 **/

#include "BitFileParser.hpp"

#include "Poco/Path.h"
#include "Poco/Format.h"
#include "Poco/InflatingStream.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <fstream>

/*---------------------------------------------------------------------------*/

using Poco::InflatingInputStream;
using Poco::InflatingStreamBuf;
using Poco::Path;

/*---------------------------------------------------------------------------*/

#define BUFSIZE     1024

#define SWAP_2(X) (((X >> 8) & 0x00FF) | ((X << 8) & 0xFF00))

/*---------------------------------------------------------------------------*/

static bool isZippedFile(const std::string& file)
{
    Path path(file);
    if(path.getExtension() != "bit") { // Here we assume that all that is not bit is zipped bit
        return true;
    }

    return false;
}

/*---------------------------------------------------------------------------*/


BitFileParser::BitFileParser()
{
}

const std::string& BitFileParser::getDesignName() const
{
    return _design;
}

const std::string& BitFileParser::getPartName() const
{
    return _part;
}

const std::string& BitFileParser::getDate() const
{
    return _date;
}

const std::string& BitFileParser::getTime() const
{
    return _time;
}

bool BitFileParser::parse(const std::string& path)
{
    if(isZippedFile(path)) {
        return parseZippedBit(path);
    }else{
        return parseBit(path);
    }
}

bool BitFileParser::parseZippedBit(const std::string& path)
{
    char buffer[BUFSIZE] = {0};

    std::ifstream istr(path.c_str(), std::ios::binary);
    InflatingInputStream inflater(istr, InflatingStreamBuf::STREAM_GZIP);

    inflater.read(buffer, sizeof(buffer));

    istr.close();

    extractBitInfoFromBuffer(buffer);

    return true;
}

bool BitFileParser::parseBit(const std::string& path)
{
    char buffer[BUFSIZE] = {0};
    
    int fd = open(path.c_str(), O_RDONLY);
    if(fd < 0) {
        return false;
    }

    ssize_t n = read(fd, buffer, sizeof(buffer));
    if(n < 0) {
        close(fd);
        return false;
    }

    close(fd);

    extractBitInfoFromBuffer(buffer);

    return true;
}


void BitFileParser::extractBitInfoFromBuffer(char* pBuffer)
{
    char* pbuf = pBuffer;
    unsigned short len = 0; // length to read

    memcpy(&len, pbuf, 2);  // reading length of first header field
    len = SWAP_2(len); // TODO: Change to be16toh
    pbuf += 2; // skip length
    pbuf += len; // skip first field

    memcpy(&len, pbuf, 2);  // reading length of 'a' letter field
    len = SWAP_2(len);      // TODO: Change to be16toh
    pbuf += 2;
    pbuf += len;

    memcpy(&len, pbuf, 2);  // reading length of design name field
    len = SWAP_2(len);      // TODO: Change to be16toh
    pbuf += 2;
    _design = pbuf;
    pbuf += len;

    pbuf += 1; // skip 'b' letter field

    memcpy(&len, pbuf, 2);  // reading length of part name field
    len = SWAP_2(len);      // TODO: Change to be16toh
    pbuf += 2; 
    _part = pbuf;
    pbuf += len;

    pbuf += 1; // skip 'c' letter field

    memcpy(&len, pbuf, 2);  // reading length of date field
    len = SWAP_2(len);      // TODO: Change to be16toh
    pbuf += 2; 
    _date = pbuf;
    pbuf += len;

    pbuf += 1;  // skip 'd' letter field

    memcpy(&len, pbuf, 2);  // reading length of time field
    len = SWAP_2(len);      // TODO: Change to be16toh
    pbuf += 2; 
    _time = pbuf;
    pbuf += len;

}

/*---------------------------------------------------------------------------*/
std::string getBitFileVersion(const std::string& fileName)
{
    BitFileParser parser;
    parser.parse(fileName);
    Path path(fileName);

    return Poco::format("%s (%s %s)", path.getBaseName(), parser.getDate(), parser.getTime() );
}

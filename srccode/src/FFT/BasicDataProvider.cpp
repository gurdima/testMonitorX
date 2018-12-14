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

#include "BasicDataProvider.hpp"

namespace MetersSpace
{

BasicDataProvider::BasicDataProvider()
{
}
BasicDataProvider::~BasicDataProvider()
{
}

void BasicDataProvider::getData(int channel)
{
    handleGetData(channel);
}

void BasicDataProvider::makeProto(int channel, const HTTPHeader& header)
{
    handleMakeProto(channel, header);
}

void BasicDataProvider::makeProtoStream(std::ostream& ost)
{
    handleMakeProtoStream(ost);
}

int BasicDataProvider::getDataSize()
{
    return handleGetDataSize();
}

void BasicDataProvider::clearData()
{
    handleClearData();
}

void BasicDataProvider::setConfig(int channel, const char* config, int size, const HTTPHeader& header)
{
    handleSetConfig(channel, config, size, header);
}
}//namespace MetersSpace

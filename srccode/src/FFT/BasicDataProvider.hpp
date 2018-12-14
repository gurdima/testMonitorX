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

#pragma once

#include <ostream>

#include "monitord_io.h"
/*---------------------------------------------------------------------------*/
namespace MetersSpace
{
class BasicDataProvider
{
public:
    BasicDataProvider();
    virtual ~BasicDataProvider();

    void getData(int channel);
    void makeProtoStream(std::ostream& ost);
    int  getDataSize();
    void makeProto(int channel, const HTTPHeader& header);
    void clearData();
    void setConfig(int channel,const char* config, int size, const HTTPHeader& header);

protected:
    virtual void handleMakeProtoStream(std::ostream& ost) = 0;
    virtual void handleGetData(int channel) = 0 ;
    virtual void handleMakeProto(int channel, const HTTPHeader& header) = 0;
    virtual int  handleGetDataSize() const = 0;
    virtual void handleClearData() = 0;
    virtual void handleSetConfig(int channel, const char* config, int size, const HTTPHeader& header) = 0;
};
} //namespace MetersSpace 

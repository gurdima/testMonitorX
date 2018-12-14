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

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Connection.hpp"
#include "Poco/SharedPtr.h"
/*---------------------------------------------------------------------------*/

namespace MetersSpace{
class ConnectionFactory:public Poco::Net::TCPServerConnectionFactory
{
public:
    ConnectionFactory(const Poco::SharedPtr<BasicDataProvider>& providerPtr,const unsigned int channelsNumber)
    :_providerPtr(providerPtr)
    ,_channelsNumber(channelsNumber)
    {
    }
    ~ConnectionFactory()
    {
    }

    Connection* createConnection(const Poco::Net::StreamSocket& socket)
    {
        return new Connection(socket,_providerPtr,_channelsNumber);
    }
private:
    Poco::SharedPtr<BasicDataProvider>        _providerPtr;
    const unsigned int                        _channelsNumber;
};
} //MetersSpace

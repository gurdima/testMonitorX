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

/*---------------------------------------------------------------------------*/
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Timespan.h"
#include "Poco/Util/Timer.h"
#include "Poco/Util/TimerTaskAdapter.h"
#include "BasicDataProvider.hpp"
#include "Poco/SharedPtr.h"
#include "Poco/Net/StreamSocket.h"
#include "proto/FFT.pb.h"
#include "Poco/Logger.h"
#include "Common/Tracer.hpp"

namespace MetersSpace {
class Connection:public Poco::Net::TCPServerConnection
{
public:
    Connection(const Poco::Net::StreamSocket& socket,const Poco::SharedPtr<BasicDataProvider>& providerPtr,const unsigned int channelsNumber);
    ~Connection();

    void run();
    void sendDataMessage();

private:
    Poco::Logger&                        _logger;
    Poco::SharedPtr<BasicDataProvider>   _providerPtr;
    const unsigned int                   _channelsNumber;
};
} //namespace MetersSpace


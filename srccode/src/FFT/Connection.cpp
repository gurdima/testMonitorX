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

#include "monitord_io.h"

#include "Connection.hpp"

#include "Poco/Net/SocketStream.h"
#include "Poco/Timespan.h"

/*---------------------------------------------------------------------------*/

using Poco::Net::SocketStream;
using Poco::Net::StreamSocket;
using Poco::SharedPtr;

/*---------------------------------------------------------------------------*/

namespace MetersSpace {

Connection::Connection(const StreamSocket& socket,const SharedPtr<BasicDataProvider>& providerPtr,const unsigned int channelsNumber)
:TCPServerConnection(socket)
, _logger(Poco::Logger::get("Monitor.TCP.Connection"))
,_providerPtr(providerPtr)
,_channelsNumber(channelsNumber)
{
  //  TRACE();
}

Connection::~Connection()
{
   // TRACE();
}

void Connection::run()
{
  //  TRACE();
    sendDataMessage();
}

void Connection::sendDataMessage()
{
//    TRACE();

    HTTPHeader header;
    socket().receiveBytes(&header, sizeof(header));
// poco_information_f1(_logger, "sendDataMessage() eader.messageType =  %u", (unsigned int) header.messageType);

    if(header.messageType == HTTP_MSG_TYPE_NOT_CONFIG){
        SocketStream socketStream(socket());

        _providerPtr->makeProto(_channelsNumber, header);
        _providerPtr->makeProtoStream(socketStream);

        socketStream << std::flush;

    } else {
        char buffer[64] = {0};

        int bytes = socket().receiveBytes(&buffer, sizeof(buffer));
        poco_information_f1(_logger, "bytes = socket().receiveBytes(&buffer, sizeof(buffer)) =  %u", (unsigned int) bytes );
        _providerPtr->setConfig(_channelsNumber, buffer, bytes, header);
    }
}
} //namespace MetersSpace


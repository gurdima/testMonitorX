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
#include "BasicSender.hpp"
#include "Settings/SettingsManager.hpp"
#include "Statistics/StatisticsManager.hpp"
#include "Common/Tracer.hpp"

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Util/Subsystem.h"
#include "Poco/SharedPtr.h"

#include <vector>

/*---------------------------------------------------------------------------*/
namespace MetersSpace{
class NetworkSender: public BasicSender,public Poco::Util::Subsystem
{
public:
    NetworkSender(const unsigned int channelsNumber, Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc);
    ~NetworkSender();

    virtual void handleGetData(int port);
protected:
    virtual void initialize(Poco::Util::Application& app);
    virtual void uninitialize();
    virtual const char* name() const;
private:
    const unsigned int                                  _channelsNumber;
    Settings::SettingsManager&                          _settingsManager;
    Statistics::StatisticsManager&                      _statisticsManager;
    typedef Poco::SharedPtr<Poco::Net::TCPServer>       TCPServerPtr;
    typedef Poco::SharedPtr<Poco::Net::ServerSocket>    ServerSocketPtr;

    typedef std::vector<TCPServerPtr>                   TCPServerContainer;
    typedef std::vector<ServerSocketPtr>                ServerSocketContainer;

    TCPServerContainer              _tcpServers;
    ServerSocketContainer           _serverSockets;
    Poco::Logger&                   _logger;

};
}//namespace MetersSpace

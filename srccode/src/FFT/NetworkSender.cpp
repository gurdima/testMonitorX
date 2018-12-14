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

#include "NetworkSender.hpp"
#include "ConnectionFactory.hpp"

using Poco::Net::ServerSocket;
using Poco::Net::TCPServer;

namespace MetersSpace{
NetworkSender::NetworkSender(const unsigned int channelsNumber, Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager, Poco::NotificationCenter& nc)
: BasicSender(nc)
, _channelsNumber(channelsNumber)
, _settingsManager(settingsManager)
, _statisticsManager(statisticsManager)
, _logger(Poco::Logger::get("Monitor.NetworkSender"))
{
    TRACE();
}

NetworkSender::~NetworkSender()
{
    TRACE();
}

void NetworkSender::handleGetData(int port)
{
    //TRACE();
    for(unsigned int i = 0; i < _channelsNumber; ++i){
        ServerSocketPtr pServSocket = new ServerSocket(port + i);
        _serverSockets.push_back(pServSocket);

        _providerPtr->getData(i);

        TCPServerPtr tcpSvr = new TCPServer(new ConnectionFactory(_providerPtr,i), *pServSocket);        
        tcpSvr->start();

        _tcpServers.push_back(tcpSvr);
    }
}

void NetworkSender::initialize(Poco::Util::Application& app)
{
    TRACE();
    getData(_settingsManager,_statisticsManager);
}

void NetworkSender::uninitialize()
{
    TRACE();
    for(unsigned int i = 0; i < _tcpServers.size(); ++i) {
        _tcpServers[i]->stop();    
    }

    _tcpServers.clear();
    _serverSockets.clear();
}

const char* NetworkSender::name() const
{
    return "Network";
}
}//namespace MetersSpace

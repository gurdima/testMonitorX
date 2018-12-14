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

#include "BasicSender.hpp"
#include "FFTDataProvider.hpp"
//#include "ConstellationDataProvider.hpp"
//#include "QQDataProvider.hpp"
//#include "BERDataProvider.hpp"

#define FFT_FIRST_PORT   9000
#define Const_FIRST_PORT 9010
#define QQ_FIRST_PORT    9020  
#define BER_FIRST_PORT   9030

namespace MetersSpace {

BasicSender::BasicSender(Poco::NotificationCenter& nc)
: _externalNC(nc)
{
}

BasicSender::~BasicSender()
{
}

void BasicSender::getData(Settings::SettingsManager& settingsManager,Statistics::StatisticsManager& statisticsManager)
{
    _providerPtr = new FFTDataProvider(settingsManager,statisticsManager, _externalNC);
    handleGetData(FFT_FIRST_PORT);
/*    _providerPtr = new ConstellationDataProvider(settingsManager, statisticsManager);
    handleGetData(Const_FIRST_PORT);
    _providerPtr = new QQDataProvider;
    handleGetData(QQ_FIRST_PORT);
    _providerPtr = new BERDataProvider;
    handleGetData(BER_FIRST_PORT);
*/
}
}//namespace MetersSpace 

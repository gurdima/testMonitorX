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

#include "Settings/MiscSettings.hpp"
#include "Settings/SettingsParams.hpp"

/*---------------------------------------------------------------------------*/

namespace Settings
{

MiscSettings::MiscSettings()
: SettingsHolderBase(S_MISC_SECTION_NAME)
{
    _settings.add(new StringDatum(S_MISC_CHANNEL_LABEL, "", true ));
}

std::string MiscSettings::getLabel() const
{
    return _settings.getString(S_MISC_CHANNEL_LABEL)->getValue();
}

void MiscSettings::setLabel(const std::string& val)
{
    _settings.update(S_MISC_CHANNEL_LABEL, val);
}


} // namespace Settings

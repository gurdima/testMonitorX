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

#include "Common/LoadedNotification.hpp"
#include "Common/Typedefs.hpp"

/*---------------------------------------------------------------------------*/

struct ApplyChannelConfigurationNotificationPayload
{
    ApplyChannelConfigurationNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<ApplyChannelConfigurationNotificationPayload> ApplyChannelConfigurationNotification;
/*---------------------------------------------------------------------------*/
struct ApplyFadingChannelConfigurationNotificationPayload
{
    ApplyFadingChannelConfigurationNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<ApplyFadingChannelConfigurationNotificationPayload> ApplyFadingChannelConfigurationNotification;

/*---------------------------------------------------------------------------*/

struct ChannelReconfigurationNotificationPayload
{
    ChannelReconfigurationNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<ChannelReconfigurationNotificationPayload> ChannelReconfigurationNotification;

/*---------------------------------------------------------------------------*/

struct ApplyDeviceSettingsNotificationPayload
{
    ApplyDeviceSettingsNotificationPayload(bool save)
    : saveConfig(save)
    {
    }

    bool saveConfig;
};

typedef LoadedNotification<ApplyDeviceSettingsNotificationPayload> ApplyDeviceSettingsNotification;


/*---------------------------------------------------------------------------*/
struct ApplyTunerConfigurationNotificationPayload
{
    ApplyTunerConfigurationNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<ApplyTunerConfigurationNotificationPayload> ApplyTunerConfigurationNotification;
/*---------------------------------------------------------------------------*/
struct TeminateChannelDataConnectionsNotificationPayload
{
    TeminateChannelDataConnectionsNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<TeminateChannelDataConnectionsNotificationPayload> TeminateChannelDataConnectionsNotification;

/*---------------------------------------------------------------------------*/
struct ApplyMetersSettingsNotificationPayload
{
    ApplyMetersSettingsNotificationPayload(ChannelID& ch)
    : channel(ch)
    {
    }

    ChannelID   channel;
};

typedef LoadedNotification<ApplyMetersSettingsNotificationPayload> ApplyMetersSettingsNotification;

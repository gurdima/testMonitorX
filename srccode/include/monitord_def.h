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


#define MSG_ERROR_EXCEEDED_DESCRIPTOR       10
#define MSG_ERROR_EXCEEDED_NUM_CHANNEL      20
#define MSG_ERROR_EXCEEDED_NUM_STRUCT       21
#define MSG_ERROR_EXCEEDED_NUM_SUBCHAN      22
#define MSG_ERROR_EXCEEDED_NUM_FIELD        23

#define MSG_ERROR_EXCEEDED_SIZE_STRUCT      200  // Error length of structure
#define MSG_ERROR_NUM_STRUCT_NULL           201  // Error 
#define MSG_ERROR_MASMATCH_I_O_SIZE         202
#define MSG_ERROR_EXCEEDED_SIZE_RF          MSG_ERROR_EXCEEDED_SIZE_STRUCT + 101
        
#define MSG_ERROR_BUSY_PENDING              50
#define MSG_ERROR_BUSY_PENDING_CHANNEL1     51
#define MSG_ERROR_BUSY_PENDING_CHANNEL2     52
// #define MSG_ERROR_BUSY_PENDING_DEVICE       53
#define MSG_ERROR_TUNER_UNLOCK              54

#define MSG_ERROR_SIZE_STRUCTURE            10000

#define MSG_ERROR_NOT_CHANGE_THIS_DATA      20000

#define MAX_COUNT_OF_STRUCTURE_CONF         21  // Количество членов (структур) в ChannelConf
#define MAX_COUNT_OF_FIELDS_IN_CONF         26  // Количество структур 

/*---------------------------------------------------------------------------*/

#define CHANNEL_STATUS_MSG_MASTER             0x100
#define CHANNEL_STATUS_MSG_SLAVE              0x200
#define CHANNEL_STATUS_MSG_LEADER_DLL         0x1000


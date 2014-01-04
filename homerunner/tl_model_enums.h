/****************************************************************************
 **
 ** Copyright (C) 2013 Tristian Celestin
 ** All rights reserved.
 ** Contact: tristian.celestin@outlook.com
 **
 ** This file is part of the Homerunner plugin.
 **
 ** GNU Lesser General Public License Usage
 ** This file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Tristian Celestin at tristian.celestin@outlook.com
 **
 ****************************************************************************/

#ifndef TL_MODEL_ENUMS_H
#define TL_MODEL_ENUMS_H

/* This is the structure of the GtkListStore returned by get_devices */
enum
{
	TL_DEVICE_MODEL_NAME_COLUMN = 0,
	TL_DEVICE_MODEL_DEV_ID_COLUMN,
	TL_DEVICE_MODEL_N_COLUMNS
};

/* This is the structure of the GtkListStore returned by scan_channels & load_channels */
enum
{
	TL_CHANNEL_MODEL_VCHANNEL_COLUMN = 0,
	TL_CHANNEL_MODEL_STATION_COLUMN,
	TL_CHANNEL_MODEL_FREQ_COLUMN,
	TL_CHANNEL_MODEL_PROGRAM_ID_COLUMN,
	TL_CHANNEL_MODEL_N_COLUMNS
};

#endif

#ifndef TL_HDHR_CHANNEL_MANAGER_H
#define TL_HDHR_CHANNEL_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif
#include "tl_hdhr_manager.h"

#define TL_HDHR_CHANNEL_MANAGER_XML_NAMESPACE "http://www.titaniclistener.com/homerunner/1.0"

G_BEGIN_DECLS

#define TL_TYPE_HDHR_CHANNEL_MANAGER				(tl_hdhr_channel_manager_get_type ())
#define TL_HDHR_CHANNEL_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TL_TYPE_HDHR_CHANNEL_MANAGER, TLHDHRChannelManager))
#define TL_HDHR_CHANNEL_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TL_TYPE_HDHR_CHANNEL_MANAGER, TLHDHRChannelManagerClass))
#define TL_IS_HDHR_CHANNEL_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TL_TYPE_HDHR_CHANNEL_MANAGER))
#define TL_IS_HDHR_CHANNEL_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TL_TYPE_HDHR_CHANNEL_MANAGER))
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

#define TL_HDHR_CHANNEL_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TL_TYPE_HDHR_CHANNEL_MANAGER, TLHDHRChannelManagerClass))

typedef struct _TLHDHRChannelManager TLHDHRChannelManager;
typedef struct _TLHDHRChannelManagerClass TLHDHRChannelManagerClass;
typedef struct _TLHDHRChannelManagerPrivate TLHDHRChannelManagerPrivate;

struct _TLHDHRChannelManager
{
	TLHDHRManager parent_instance;
};

struct _TLHDHRChannelManagerClass
{
	TLHDHRManagerClass parent_class;
};

TLHDHRChannelManager *tl_hdhr_channel_manager_new();

/* Channel Scanning Methods */

GtkListStore *tl_hdhr_channel_manager_test_scan_channels(TLHDHRChannelManager *self);
GtkListStore *tl_hdhr_channel_manager_scan_channels(TLHDHRChannelManager *self, uint32_t device_id);
void tl_hdhr_channel_manager_save_channels_to_xml_file(TLHDHRChannelManager *self, GtkListStore *channel_store, gchar *path, gchar *schema_path, uint32_t device_id);
GtkListStore *tl_hdhr_channel_manager_load_channels_from_xml_file(TLHDHRChannelManager *self, gchar *path, gchar *schema_path);

G_END_DECLS

#endif

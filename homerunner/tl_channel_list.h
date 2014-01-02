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

#ifndef TL_CHANNEL_LIST_H
#define TL_CHANNEL_LIST_H

#include <glib.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif

G_BEGIN_DECLS

#define TL_TYPE_CHANNEL_LIST				(tl_channel_list_get_type ())
#define TL_CHANNEL_LIST(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TL_TYPE_CHANNEL_LIST, TLChannelList))
#define TL_CHANNEL_LIST_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TL_TYPE_CHANNEL_LIST, TLChannelListClass))
#define TL_IS_CHANNEL_LIST(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TL_TYPE_CHANNEL_LIST))
#define TL_IS_CHANNEL_LIST_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TL_TYPE_CHANNEL_LIST))
#define TL_CHANNEL_LIST_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TL_TYPE_CHANNEL_LIST, TLChannelListClass))

typedef struct _TLChannelList TLChannelList;
typedef struct _TLChannelListClass TLChannelListClass;
typedef struct _TLChannelListPrivate TLChannelListPrivate;

struct _TLChannelList
{
	/*< private >*/
	GtkBox box;
	TLChannelListPrivate *priv;
};

struct _TLChannelListClass
{
	GtkBoxClass parent_class;

	/* signals */
	void (* play_channel)(TLChannelList *self, guint frequency, guint program_id, gpointer user_data);
	void (* set_device)(TLChannelList *self, guint32 device_id, gpointer user_data);
};

GtkWidget *tl_channel_list_new();
void tl_channel_list_set_channel_model(TLChannelList *self, GtkListStore *channel_store);
GtkListStore *tl_channel_list_get_channel_model(TLChannelList *self);
void tl_channel_list_set_device_model(TLChannelList *self, GtkListStore *device_store);
GtkListStore *tl_channel_list_get_device_model(TLChannelList *self);

G_END_DECLS

#endif

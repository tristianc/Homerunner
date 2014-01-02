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

#ifndef TL_HDHR_PLAYBACK_MANAGER_H
#define TL_HDHR_PLAYBACK_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif
#include "tl_hdhr_manager.h"

G_BEGIN_DECLS

#define TL_TYPE_HDHR_PLAYBACK_MANAGER				(tl_hdhr_playback_manager_get_type ())
#define TL_HDHR_PLAYBACK_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TL_TYPE_HDHR_PLAYBACK_MANAGER, TLHDHRPlaybackManager))
#define TL_HDHR_PLAYBACK_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TL_TYPE_HDHR_PLAYBACK_MANAGER, TLHDHRPlaybackManagerClass))
#define TL_IS_HDHR_PLAYBACK_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TL_TYPE_HDHR_PLAYBACK_MANAGER))
#define TL_IS_HDHR_PLAYBACK_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TL_TYPE_HDHR_PLAYBACK_MANAGER))
#define TL_HDHR_PLAYBACK_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TL_TYPE_HDHR_PLAYBACK_MANAGER, TLHDHRPlaybackManagerClass))

typedef struct _TLHDHRPlaybackManager TLHDHRPlaybackManager;
typedef struct _TLHDHRPlaybackManagerClass TLHDHRPlaybackManagerClass;
typedef struct _TLHDHRPlaybackManagerPrivate TLHDHRPlaybackManagerPrivate;

struct _TLHDHRPlaybackManager
{
	TLHDHRManager parent_instance;
	TLHDHRPlaybackManagerPrivate *priv;
};

struct _TLHDHRPlaybackManagerClass
{
	TLHDHRManagerClass parent_class;
};

TLHDHRPlaybackManager *tl_hdhr_playback_manager_new();
gboolean tl_hdhr_playback_manager_stream_channel_to_ip(TLHDHRPlaybackManager *self, guint32 frequency, guint32 program_id, uint32_t device_id, gchar *ip, guint port);
void tl_hdhr_playback_manager_stop_streaming(TLHDHRPlaybackManager *self);

G_END_DECLS

#endif

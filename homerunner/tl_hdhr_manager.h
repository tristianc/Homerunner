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

#ifndef TL_HDHR_MANAGER_H
#define TL_HDHR_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif

G_BEGIN_DECLS

#define TL_TYPE_HDHR_MANAGER				(tl_hdhr_manager_get_type ())
#define TL_HDHR_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TL_TYPE_HDHR_MANAGER, TLHDHRManager))
#define TL_HDHR_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TL_TYPE_HDHR_MANAGER, TLHDHRManagerClass))
#define TL_IS_HDHR_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TL_TYPE_HDHR_MANAGER))
#define TL_IS_HDHR_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TL_TYPE_HDHR_MANAGER))
#define TL_HDHR_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TL_TYPE_HDHR_MANAGER, TLHDHRManagerClass))

typedef struct _TLHDHRManager TLHDHRManager;
typedef struct _TLHDHRManagerClass TLHDHRManagerClass;
typedef struct _TLHDHRManagerPrivate TLHDHRManagerPrivate;

struct _TLHDHRManager
{
	GObject parent_instance;
	TLHDHRManagerPrivate *priv;
};

struct _TLHDHRManagerClass
{
	GObjectClass parent_class;
};

TLHDHRManager *tl_hdhr_manager_new();

/* Discovery Methods */

GtkListStore *tl_hdhr_manager_get_devices(TLHDHRManager *self);

/* Tuner Methods */

struct hdhomerun_device_t *tl_hdhr_manager_get_tuner(TLHDHRManager *self, uint32_t id);

/* Utility Methods */

gchar *tl_hdhr_manager_get_host_ip_relative_to_device(TLHDHRManager *self, uint32_t device_id);

G_END_DECLS

#endif

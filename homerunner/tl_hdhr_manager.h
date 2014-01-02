/*
 * hdhr_manager.h
 *
 *  Created on: Oct 26, 2013
 *      Author: tristian
 */

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

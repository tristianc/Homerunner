/*
 * hdhr_manager.h
 *
 *  Created on: Oct 26, 2013
 *      Author: tristian
 */

#ifndef TJ_HDHR_MANAGER_H
#define TJ_HDHR_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif

G_BEGIN_DECLS

#define TJ_TYPE_HDHR_MANAGER				(tj_hdhr_manager_get_type ())
#define TJ_HDHR_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TJ_TYPE_HDHR_MANAGER, TJHDHRManager))
#define TJ_HDHR_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TJ_TYPE_HDHR_MANAGER, TJHDHRManagerClass))
#define TJ_IS_HDHR_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TJ_TYPE_HDHR_MANAGER))
#define TJ_IS_HDHR_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TJ_TYPE_HDHR_MANAGER))
#define TJ_HDHR_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TJ_TYPE_HDHR_MANAGER, TJHDHRManagerClass))

typedef struct _TJHDHRManager TJHDHRManager;
typedef struct _TJHDHRManagerClass TJHDHRManagerClass;
typedef struct _TJHDHRManagerPrivate TJHDHRManagerPrivate;

struct _TJHDHRManager
{
	GObject parent_instance;
	TJHDHRManagerPrivate *priv;
};

struct _TJHDHRManagerClass
{
	GObjectClass parent_class;
};

TJHDHRManager *tj_hdhr_manager_new();

/* Discovery Methods */

GtkListStore *tj_hdhr_manager_get_devices(TJHDHRManager *self);

/* Tuner Methods */

struct hdhomerun_device_t *tj_hdhr_manager_get_tuner(TJHDHRManager *self, uint32_t id);

G_END_DECLS

#endif

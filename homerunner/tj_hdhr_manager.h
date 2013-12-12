/*
 * hdhr_manager.h
 *
 *  Created on: Oct 26, 2013
 *      Author: tristian
 */

#ifndef HDHR_MANAGER_H_
#define HDHR_MANAGER_H_

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif

/* This is the structure of the GtkListStore returned by get_devices */
enum
{
	TJ_HDHR_MANAGER_DEV_ID_COLUMN = 0,
	TJ_HDHR_MANAGER_DEV_TYPE_COLUMN,
	TJ_HDHR_MANAGER_IP_ADDR_COLUMN,
	TJ_HDHR_MANAGER_TUNER_COUNT
};

/* This is the structure of the GtkListStore returned by scan_channels & load_channels */
enum
{
	TJ_HDHR_MANAGER_VCHANNEL_COLUMN = 0,
	TJ_HDHR_MANAGER_STATION_COLUMN,
	TJ_HDHR_MANAGER_FREQ_COLUMN,
	TJ_HDHR_MANAGER_PROGRAM_ID_COLUMN,
	TJ_HDHR_MANAGER_N_COLUMNS
};

#define TJ_HDHR_MANAGER_XML_NAMESPACE "http://www.thoughtjacked.net/homerunner/1.0"

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
void tj_hdhr_manager_init_devices(TJHDHRManager *self);
GtkListStore *tj_hdhr_manager_get_devices(TJHDHRManager *self);
struct hdhomerun_device_t *tj_hdhr_get_device_from_liststore(
		TJHDHRManager *self, GtkListStore *device_store,
		GtkTreeIter *device_iter, struct hdhomerun_debug_t *debug);
GtkListStore *tj_hdhr_manager_test_scan_channels(TJHDHRManager *self);
GtkListStore *tj_hdhr_manager_scan_channels(TJHDHRManager *self);
void tj_hdhr_manager_save_channels_to_xml_file(TJHDHRManager *self,
		GtkListStore *channel_store, gchar *path, gchar *schema_path);
GtkListStore *tj_hdhr_manager_load_channels_from_xml_file(TJHDHRManager *self,
		gchar *path, gchar *schema_path);
void tj_hdhr_manager_stream_channel_to_uri(TJHDHRManager *self,
		struct hdhomerun_device_t *dev, guint32 frequency, guint32 program_id,
		gchar* uri);
void tj_hdhr_manager_stop_streaming(TJHDHRManager *self, struct hdhomerun_device_t *dev);

G_END_DECLS

#endif /* HDHR_MANAGER_H_ */

#ifndef TJ_HDHR_CHANNEL_MANAGER_H
#define TJ_HDHR_CHANNEL_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif
#include "tj_hdhr_manager.h"

#define TJ_HDHR_CHANNEL_MANAGER_XML_NAMESPACE "http://www.titaniclistener.com/homerunner/1.0"

G_BEGIN_DECLS

#define TJ_TYPE_HDHR_CHANNEL_MANAGER				(tj_hdhr_channel_manager_get_type ())
#define TJ_HDHR_CHANNEL_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TJ_TYPE_HDHR_CHANNEL_MANAGER, TJHDHRChannelManager))
#define TJ_HDHR_CHANNEL_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TJ_TYPE_HDHR_CHANNEL_MANAGER, TJHDHRChannelManagerClass))
#define TJ_IS_HDHR_CHANNEL_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TJ_TYPE_HDHR_CHANNEL_MANAGER))
#define TJ_IS_HDHR_CHANNEL_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TJ_TYPE_HDHR_CHANNEL_MANAGER))
#define TJ_HDHR_CHANNEL_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TJ_TYPE_HDHR_CHANNEL_MANAGER, TJHDHRChannelManagerClass))

typedef struct _TJHDHRChannelManager TJHDHRChannelManager;
typedef struct _TJHDHRChannelManagerClass TJHDHRChannelManagerClass;
typedef struct _TJHDHRChannelManagerPrivate TJHDHRChannelManagerPrivate;

struct _TJHDHRChannelManager
{
	TJHDHRManager parent_instance;
};

struct _TJHDHRChannelManagerClass
{
	TJHDHRManagerClass parent_class;
};

TJHDHRChannelManager *tj_hdhr_channel_manager_new();

/* Channel Scanning Methods */

GtkListStore *tj_hdhr_channel_manager_test_scan_channels(TJHDHRChannelManager *self);
GtkListStore *tj_hdhr_channel_manager_scan_channels(TJHDHRChannelManager *self, uint32_t device_id);
void tj_hdhr_channel_manager_save_channels_to_xml_file(TJHDHRChannelManager *self, GtkListStore *channel_store, gchar *path, gchar *schema_path);
GtkListStore *tj_hdhr_channel_manager_load_channels_from_xml_file(TJHDHRChannelManager *self, gchar *path, gchar *schema_path);

G_END_DECLS

#endif

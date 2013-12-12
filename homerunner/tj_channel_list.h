#ifndef TJ_CHANNEL_LIST_H_
#define TJ_CHANNEL_LIST_H_

#include <glib.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif

G_BEGIN_DECLS

#define TJ_TYPE_CHANNEL_LIST				(tj_channel_list_get_type ())
#define TJ_CHANNEL_LIST(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TJ_TYPE_CHANNEL_LIST, TJChannelList))
#define TJ_CHANNEL_LIST_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TJ_TYPE_CHANNEL_LIST, TJChannelListClass))
#define TJ_IS_CHANNEL_LIST(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TJ_TYPE_CHANNEL_LIST))
#define TJ_IS_CHANNEL_LIST_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TJ_TYPE_CHANNEL_LIST))
#define TJ_CHANNEL_LIST_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TJ_TYPE_CHANNEL_LIST, TJChannelListClass))

typedef struct _TJChannelList TJChannelList;
typedef struct _TJChannelListClass TJChannelListClass;
typedef struct _TJChannelListPrivate TJChannelListPrivate;

struct _TJChannelList
{
	/*< private >*/
	GtkBox box;
	TJChannelListPrivate *priv;
};

struct _TJChannelListClass
{
	GtkBoxClass parent_class;
};

GtkWidget *tj_channel_list_new();
void tj_channel_list_set_channels(TJChannelList *self, GtkListStore *channel_store);
void tj_channel_list_set_devices(TJChannelList *self, GtkListStore *device_store);

G_END_DECLS

#endif

#ifndef TJ_CHANNEL_LIST_H
#define TJ_CHANNEL_LIST_H

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

        /* virtual functions */
        /*
         * ToDo:
         * Finish refactoring the render function so that it is a virtual function
         * with a default implementation. Somehow, this function has to provide
         * a cell renderer and a GtkCellLayoutDataFunc
         */ 
        //void (*render_device_name)(GtkCellRenderer *renderer, GtkCellLayoutDataFunc);

	/* signals */
	void (* play_channel)(TJChannelList *self, guint frequency, guint program_id, gpointer user_data);
	void (* set_device)(TJChannelList *self, guint32 device_id, gpointer user_data);
};

GtkWidget *tj_channel_list_new();
void tj_channel_list_set_channel_model(TJChannelList *self, GtkListStore *channel_store);
GtkListStore *tj_channel_list_get_channel_model(TJChannelList *self);
void tj_channel_list_set_device_model(TJChannelList *self, GtkListStore *device_store);
GtkListStore *tj_channel_list_get_device_model(TJChannelList *self);

G_END_DECLS

#endif

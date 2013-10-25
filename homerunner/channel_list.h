#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TOTEM_TYPE_CHANNEL_LIST                 (totem_channel_list_get_type ())
#define TOTEM_CHANNEL_LIST (o)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), TOTEM_TYPE_CHANNEL_LIST, TotemChannelList))
#define TOTEM_CHANNEL_LIST_CLASS (k)			(G_TYPE_CHECK_CLASS_CAST ((klass), TOTEM_TYPE_CHANNEL_LIST, TotemChannelListClass))
#define TOTEM_IS_CHANNEL_LIST (o)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TOTEM_TYPE_CHANNEL_LIST))
#define TOTEM_IS_CHANNEL_LIST_CLASS (k)			(G_TYPE_CHECK_CLASS_TYPE ((klass), TOTEM_TYPE_CHANNEL_LIST))
#define TOTEM_CHANNEL_LIST_GET_CLASS (o)       	(G_TYPE_INSTANCE_GET_CLASS ((obj), TOTEM_TYPE_CHANNEL_LIST, TotemChannelListClass))

typedef struct _TotemChannelList             TotemChannelList;
typedef struct _TotemChannelListClass        TotemChannelListClass;
typedef struct _TotemChannelListPrivate      TotemChannelListPrivate;

struct _TotemChannelList
{
	/*< private >*/
	GtkBox box;
	TotemChannelListPrivate *priv;
};

struct _TotemChannelListClass
{
	GtkBoxClass parent_class;
};

void populate_devices ();
void populate_channels ();
void start_playback ();
GtkWidget *totem_channel_list_new ();

G_END_DECLS

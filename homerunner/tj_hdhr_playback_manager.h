#ifndef TJ_HDHR_PLAYBACK_MANAGER_H
#define TJ_HDHR_PLAYBACK_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif
#include "tj_hdhr_manager.h"

G_BEGIN_DECLS

#define TJ_TYPE_HDHR_PLAYBACK_MANAGER				(tj_hdhr_playback_manager_get_type ())
#define TJ_HDHR_PLAYBACK_MANAGER(o)        			(G_TYPE_CHECK_INSTANCE_CAST ((o), TJ_TYPE_HDHR_PLAYBACK_MANAGER, TJHDHRPlaybackManager))
#define TJ_HDHR_PLAYBACK_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_CAST ((k), TJ_TYPE_HDHR_PLAYBACK_MANAGER, TJHDHRPlaybackManagerClass))
#define TJ_IS_HDHR_PLAYBACK_MANAGER(o)				(G_TYPE_CHECK_INSTANCE_TYPE ((o), TJ_TYPE_HDHR_PLAYBACK_MANAGER))
#define TJ_IS_HDHR_PLAYBACK_MANAGER_CLASS(k)			(G_TYPE_CHECK_CLASS_TYPE ((k), TJ_TYPE_HDHR_PLAYBACK_MANAGER))
#define TJ_HDHR_PLAYBACK_MANAGER_GET_CLASS(o)       	(G_TYPE_INSTANCE_GET_CLASS ((o), TJ_TYPE_HDHR_PLAYBACK_MANAGER, TJHDHRPlaybackManagerClass))

typedef struct _TJHDHRPlaybackManager TJHDHRPlaybackManager;
typedef struct _TJHDHRPlaybackManagerClass TJHDHRPlaybackManagerClass;
typedef struct _TJHDHRPlaybackManagerPrivate TJHDHRPlaybackManagerPrivate;

struct _TJHDHRPlaybackManager
{
	TJHDHRManager parent_instance;
	TJHDHRPlaybackManagerPrivate *priv;
};

struct _TJHDHRPlaybackManagerClass
{
	TJHDHRManagerClass parent_class;
};

TJHDHRPlaybackManager *tj_hdhr_playback_manager_new();
gboolean tj_hdhr_playback_manager_stream_channel_to_ip(TJHDHRPlaybackManager *self, guint32 frequency, guint32 program_id, uint32_t device_id, gchar *ip, guint port);
void tj_hdhr_playback_manager_stop_streaming(TJHDHRPlaybackManager *self);

G_END_DECLS

#endif

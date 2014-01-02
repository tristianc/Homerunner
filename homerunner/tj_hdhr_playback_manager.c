#include "tj_hdhr_playback_manager.h"


struct _TJHDHRPlaybackManagerPrivate
{
	struct hdhomerun_device_t *playback_device;
};

G_DEFINE_TYPE_WITH_PRIVATE(TJHDHRPlaybackManager, tj_hdhr_playback_manager, TJ_TYPE_HDHR_MANAGER);


static void tj_hdhr_playback_manager_class_init(TJHDHRPlaybackManagerClass *k)
{
}

static void tj_hdhr_playback_manager_init(TJHDHRPlaybackManager *self)
{
	self->priv = tj_hdhr_playback_manager_get_instance_private(self);
	self->priv->playback_device = NULL;
}

TJHDHRPlaybackManager *tj_hdhr_playback_manager_new()
{
	return g_object_new(TJ_TYPE_HDHR_PLAYBACK_MANAGER, NULL);
}

static void tj_hdhr_playback_manager_dispose(GObject *object)
{
	TJHDHRPlaybackManager *self = TJ_HDHR_PLAYBACK_MANAGER(object);

	/* In dispose(), you are supposed to free all types referenced from this
	 * object which might themselves hold a reference to self. Generally,
	 * the most simple solution is to unref all members on which you own a
	 * reference.
	 */

	/* dispose() might be called multiple times, so we must guard against
	 * calling g_object_unref() on an invalid GObject by setting the member
	 * NULL; g_clear_object() does this for us, atomically.
	 */
	//g_clear_object (&self->priv->an_object);
	/* Always chain up to the parent class; there is no need to check if
	 * the parent class implements the dispose() virtual function: it is
	 * always guaranteed to do so
	 */
	tj_hdhr_playback_manager_stop_streaming(TJ_HDHR_PLAYBACK_MANAGER(object));
	G_OBJECT_CLASS (tj_hdhr_playback_manager_parent_class)->dispose(object);
}

static void tj_hdhr_playback_manager_finalize(GObject *object)
{
	TJHDHRPlaybackManager *self = TJ_HDHR_PLAYBACK_MANAGER(object);

	/* Always chain up to the parent class; as with dispose(), finalize()
	 * is guaranteed to exist on the parent's class virtual function table
	 */
	G_OBJECT_CLASS (tj_hdhr_playback_manager_parent_class)->finalize(object);
}


gboolean tj_hdhr_playback_manager_stream_channel_to_ip(TJHDHRPlaybackManager *self, guint32 frequency, guint32 program_id, uint32_t device_id, gchar *ip, guint port)
{
	gboolean succeeded;
	gchar *url;
	gchar *freq_str;
	gchar *pid_str;

	g_assert(self != NULL);
	g_assert(ip != NULL);

	succeeded = FALSE;
	if (self->priv->playback_device != NULL) {
		tj_hdhr_playback_manager_stop_streaming(self);
	}
	self->priv->playback_device = tj_hdhr_manager_get_tuner(TJ_HDHR_MANAGER(self), device_id);
	if (self->priv->playback_device == NULL) {
		g_error("Could not acquire tuner.");
	} else {
		g_debug("Streaming using device %X and tuner %d", device_id, hdhomerun_device_get_tuner(self->priv->playback_device));
		freq_str = g_strdup_printf("%d", frequency);
		pid_str = g_strdup_printf("%d", program_id);
		url = g_strdup_printf("udp://%s:%d", ip, port);
		hdhomerun_device_set_tuner_channel(self->priv->playback_device, freq_str);
		hdhomerun_device_set_tuner_program(self->priv->playback_device, pid_str);
		succeeded = (gboolean)hdhomerun_device_set_tuner_target(self->priv->playback_device, url);
		g_free(freq_str);
		g_free(pid_str);
	}
	g_free(url);
	return succeeded;
}

void tj_hdhr_playback_manager_stop_streaming(TJHDHRPlaybackManager *self)
{
	g_assert(self != NULL);

	hdhomerun_device_set_tuner_target(self->priv->playback_device, "none");
	hdhomerun_device_tuner_lockkey_release(self->priv->playback_device);
	self->priv->playback_device = NULL;
}

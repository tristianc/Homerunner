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

#include "tl_hdhr_playback_manager.h"

struct _TLHDHRPlaybackManagerPrivate
{
	struct hdhomerun_device_t *playback_device;
};

G_DEFINE_TYPE_WITH_PRIVATE(TLHDHRPlaybackManager, tl_hdhr_playback_manager, TL_TYPE_HDHR_MANAGER);

static void
tl_hdhr_playback_manager_class_init(TLHDHRPlaybackManagerClass *k)
{
}

static void
tl_hdhr_playback_manager_init(TLHDHRPlaybackManager *self)
{
	self->priv = tl_hdhr_playback_manager_get_instance_private(self);
	self->priv->playback_device = NULL;
}

TLHDHRPlaybackManager *
tl_hdhr_playback_manager_new()
{
	return g_object_new(TL_TYPE_HDHR_PLAYBACK_MANAGER, NULL);
}

static void
tl_hdhr_playback_manager_dispose(GObject *object)
{
	TLHDHRPlaybackManager *self = TL_HDHR_PLAYBACK_MANAGER(object);

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
	tl_hdhr_playback_manager_stop_streaming(TL_HDHR_PLAYBACK_MANAGER(object));
	G_OBJECT_CLASS (tl_hdhr_playback_manager_parent_class)->dispose(object);
}

static void
tl_hdhr_playback_manager_finalize(GObject *object)
{
	TLHDHRPlaybackManager *self = TL_HDHR_PLAYBACK_MANAGER(object);

	/* Always chain up to the parent class; as with dispose(), finalize()
	 * is guaranteed to exist on the parent's class virtual function table
	 */
	G_OBJECT_CLASS (tl_hdhr_playback_manager_parent_class)->finalize(object);
}

gboolean
tl_hdhr_playback_manager_stream_channel_to_ip(TLHDHRPlaybackManager *self, guint32 frequency,
	guint32 program_id, uint32_t device_id, gchar *ip, guint port)
{
	gboolean succeeded;
	gchar *url;
	gchar *freq_str;
	gchar *pid_str;

	g_assert(self != NULL);
	g_assert(ip != NULL);

	succeeded = FALSE;
	if (self->priv->playback_device != NULL) {
		tl_hdhr_playback_manager_stop_streaming(self);
	}
	self->priv->playback_device = tl_hdhr_manager_get_tuner(TL_HDHR_MANAGER(self), device_id);
	if (self->priv->playback_device == NULL) {
		g_error("Could not acquire tuner.");
	} else {
		g_debug("Streaming using device %X and tuner %d", device_id,
			hdhomerun_device_get_tuner(self->priv->playback_device));
		freq_str = g_strdup_printf("%d", frequency);
		pid_str = g_strdup_printf("%d", program_id);
		url = g_strdup_printf("udp://%s:%d", ip, port);
		hdhomerun_device_set_tuner_channel(self->priv->playback_device, freq_str);
		hdhomerun_device_set_tuner_program(self->priv->playback_device, pid_str);
		succeeded = (gboolean) hdhomerun_device_set_tuner_target(self->priv->playback_device, url);
		g_free(freq_str);
		g_free(pid_str);
	}
	g_free(url);
	return succeeded;
}

void
tl_hdhr_playback_manager_stop_streaming(TLHDHRPlaybackManager *self)
{
	g_assert(self != NULL);

	hdhomerun_device_set_tuner_target(self->priv->playback_device, "none");
	hdhomerun_device_tuner_lockkey_release(self->priv->playback_device);
	self->priv->playback_device = NULL;
}


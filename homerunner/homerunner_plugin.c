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

#include <glib.h>
#include <glib-object.h>
#include <totem.h>
#include <totem-plugin.h>
#include <arpa/inet.h>
#include "tl_channel_list.h"
#include "tl_hdhr_channel_manager.h"
#include "tl_hdhr_playback_manager.h"

#define TOTEM_TYPE_HOMERUNNER_PLUGIN			(totem_homerunner_plugin_get_type ())
#define TOTEM_HOMERUNNER_PLUGIN(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin))
#define TOTEM_HOMERUNNER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))
#define TOTEM_IS_HOMERUNNER_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_IS_HOMERUNNER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_HOMERUNNER_PLUGIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))

typedef struct
{
	GtkWidget *channel_list;
	TLHDHRChannelManager *channel_manager;
	TLHDHRPlaybackManager *playback_manager;
	gchar* plugin_config_path;
	gchar* channel_path;
	gint play_channel_handler_id;
	gint set_device_handler_id;
	guint32 device_id;
} TotemHomerunnerPluginPrivate;

TOTEM_PLUGIN_REGISTER(TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin,
		totem_homerunner_plugin);

static void on_set_device(TLChannelList *list, guint32 device_id, TotemHomerunnerPlugin *plugin)
{
	gboolean success;
	gchar *schema_path;
	gchar *filename;
	gchar *abs_filename;
	GtkListStore *channel_store;
	GFile *channel_file;
	GFileInputStream *channel_is;
	GError *error;

	g_assert(plugin != NULL);
	g_assert(list != NULL);

	g_debug("Got device id %X", device_id);
	plugin->priv->device_id = device_id;
	schema_path = totem_plugin_find_file("homerunner", "schema.xsd");
	filename = g_strdup_printf("%X.xml", device_id);
	abs_filename = g_build_filename(plugin->priv->channel_path, filename);
	channel_file = g_file_new_for_path(abs_filename);
	error = NULL;
	g_debug("Attempting to read from %s", abs_filename);
	channel_is = g_file_read(channel_file, NULL, &error);
	if (error != NULL) {
		g_debug("Could not read channels for device: %s", error->message);
		channel_store = tl_hdhr_channel_manager_scan_channels(plugin->priv->channel_manager, device_id);
		tl_hdhr_channel_manager_save_channels_to_xml_file(plugin->priv->channel_manager, channel_store, abs_filename, schema_path, device_id);
		g_error_free(error);
		error = NULL;
	}
	else {
		g_object_unref(channel_is);
		channel_store = tl_hdhr_channel_manager_load_channels_from_xml_file(plugin->priv->channel_manager, abs_filename, schema_path);
	}
	tl_channel_list_set_channel_model(TL_CHANNEL_LIST(plugin->priv->channel_list), channel_store);
	g_free(filename);
	g_free(abs_filename);
	g_free(schema_path);
	g_object_unref(channel_store);
	g_object_unref(channel_file);
}

static void on_play_channel(TLChannelList *list, guint frequency, guint program_id, TotemHomerunnerPlugin *plugin)
{
	TotemObject *totem;
	gchar *channel_url;
	gchar *ip;
	guint port;
	gboolean succeeded;

	g_assert(plugin != NULL);
	g_assert(list != NULL);

	ip = tl_hdhr_manager_get_host_ip_relative_to_device(TL_HDHR_MANAGER(plugin->priv->playback_manager), plugin->priv->device_id);
	if (ip == NULL) {
		g_debug("Could not get local IP address to stream to.");
		return;
	}
	port = 5000;
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	channel_url = g_strdup_printf("udp://%s:%d", ip, port);

	g_debug("Running play-channel handler with frequency %d and program_id %d", frequency, program_id);

	totem_object_add_to_playlist_and_play(totem, channel_url, "TV!");

	succeeded = tl_hdhr_playback_manager_stream_channel_to_ip(plugin->priv->playback_manager, frequency, program_id, plugin->priv->device_id, ip, port);
	if (succeeded == FALSE) {
		g_debug("Was not successful in streaming channel to %s", channel_url);
	}
	g_free(channel_url);
	g_free(ip);
}

static void verify_config_paths(TotemHomerunnerPlugin *self)
{
	const gchar *config_path;
	GFile *plugin_config_dir;
	GFile *channels_dir;
	GError *error;

	g_assert(self != NULL);

	error = NULL;
	config_path = g_get_user_config_dir();
	self->priv->plugin_config_path = g_build_filename(config_path, "homerunner", NULL);
	plugin_config_dir = g_file_new_for_path(self->priv->plugin_config_path);
	g_file_make_directory(plugin_config_dir, NULL, &error);
	if (error != NULL) {
		if (error->code != G_IO_ERROR_EXISTS) {
			g_critical("Could not read from plugin configuration directory: %s", error->message);
		}
		g_error_free(error);
		error = NULL;
	}

	self->priv->channel_path = g_build_filename(config_path, "homerunner", "channels", NULL);
	channels_dir = g_file_new_for_path(self->priv->channel_path);
	g_file_make_directory(channels_dir, NULL, &error);
	if (error != NULL) {
		if (error->code != G_IO_ERROR_EXISTS) {
			g_critical("Could not read from channels directory: %s", error->message);
		}
		g_error_free(error);
		error = NULL;
	}

	g_object_unref(plugin_config_dir);
	g_object_unref(channels_dir);
}

static void impl_activate(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self;
	TotemObject *totem;
	gchar *path;
	GtkListStore *device_store;
	GtkListStore *channel_store;

	g_assert(plugin != NULL);

	self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	self->priv->channel_list = tl_channel_list_new();
	self->priv->channel_manager = tl_hdhr_channel_manager_new();
	self->priv->playback_manager = tl_hdhr_playback_manager_new();
	self->priv->device_id = -1;

	self->priv->play_channel_handler_id = g_signal_connect(
			G_OBJECT(self->priv->channel_list), "play_channel",
			G_CALLBACK(on_play_channel), self);

	self->priv->play_channel_handler_id = g_signal_connect(
			G_OBJECT(self->priv->channel_list), "set-device",
			G_CALLBACK(on_set_device), self);

	verify_config_paths(self);
	gtk_widget_show(self->priv->channel_list);
	totem_object_add_sidebar_page(totem, "channel_list", "Homerunner", self->priv->channel_list);
	device_store = tl_hdhr_manager_get_devices(TL_HDHR_MANAGER(self->priv->channel_manager));
	tl_channel_list_set_device_model(TL_CHANNEL_LIST(self->priv->channel_list), device_store);
	g_object_unref(device_store);
	g_free(path);
}

static void impl_deactivate(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self;
	TotemObject *totem;

	g_assert(plugin != NULL);

	/* Destroy resources created in impl_activate() here. e.g. Disconnect from signals
	 * and remove menu entries and UI. */
	self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	g_free(self->priv->channel_path);
	g_free(self->priv->plugin_config_path);
	g_object_unref(self->priv->channel_manager);
	g_object_unref(self->priv->playback_manager);
	g_signal_handler_disconnect(plugin, self->priv->play_channel_handler_id);
	g_signal_handler_disconnect(plugin, self->priv->set_device_handler_id);
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	totem_object_remove_sidebar_page(totem, "channel_list");

}


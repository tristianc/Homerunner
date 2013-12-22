#include <glib.h>
#include <glib-object.h>
#include <totem.h>
#include <totem-plugin.h>
#include <arpa/inet.h>
#include "tj_channel_list.h"
#include "tj_hdhr_channel_manager.h"
#include "tj_hdhr_playback_manager.h"
#include "bacon-video-widget.h"

#define TOTEM_TYPE_HOMERUNNER_PLUGIN			(totem_homerunner_plugin_get_type ())
#define TOTEM_HOMERUNNER_PLUGIN(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin))
#define TOTEM_HOMERUNNER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))
#define TOTEM_IS_HOMERUNNER_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_IS_HOMERUNNER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_HOMERUNNER_PLUGIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))

typedef struct
{
	GtkWidget *channel_list;
	TJHDHRChannelManager *channel_manager;
	TJHDHRPlaybackManager *playback_manager;
	gchar* plugin_config_path;
	gchar* channels_path;
	gint play_channel_handler_id;
	gint set_device_handler_id;
	gint device_id;
} TotemHomerunnerPluginPrivate;

TOTEM_PLUGIN_REGISTER(TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin,
		totem_homerunner_plugin);

static void on_set_device(TJChannelList *list, guint32 device_id, TotemHomerunnerPlugin *plugin)
{
	gboolean success;

	g_debug("Got device id %X", device_id);
	plugin->priv->device_id = device_id;
}

static void on_play_channel(TJChannelList *list, guint frequency, guint program_id, TotemHomerunnerPlugin *plugin)
{
	TotemObject *totem;
	BaconVideoWidget *bvw;
	gchar *uri;
	gchar *ip;
	gchar *port;
	gboolean status;
	uint32_t local_ip;

	g_assert(plugin != NULL);
	g_assert(list != NULL);

	ip = g_strdup("127.0.0.1");
	port = g_strdup("5000");
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	bvw = BACON_VIDEO_WIDGET (totem_get_video_widget(totem));
	status = FALSE;
	uri = g_strdup_printf("udp://%s:%s", ip, port);

	g_debug("Running play-channel handler with frequency %d and program_id %d", frequency, program_id);
	status = tj_hdhr_playback_manager_stream_channel_to_uri(plugin->priv->playback_manager, frequency, program_id, uri, plugin->priv->device_id);
	if (status == FALSE) {
		g_debug("Was no successful in streaming channel to uri");
	}
	bacon_video_widget_open(bvw, uri);
	bacon_video_widget_play(bvw, NULL);
	g_object_unref(bvw);
	g_free(uri);
	g_free(port);
	g_free(ip);
}

//static void on_stop_playing()
//{
//
//}

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

	self->priv->channels_path = g_build_filename(config_path, "homerunner", "channels", NULL);
	channels_dir = g_file_new_for_path(self->priv->channels_path);
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
	gchar *schema_path;
	GtkListStore *device_store;
	GtkListStore *channel_store;

	g_assert(plugin != NULL);

	/*
	 * ToDo:
	 * Write and read channels using channels path
	 */
	self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	verify_config_paths(self);
	path = g_strdup("/home/tristian/channels.xml");
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	schema_path = totem_plugin_find_file("homerunner", "schema.xsd");

	self->priv->channel_list = tj_channel_list_new();
	self->priv->channel_manager = tj_hdhr_channel_manager_new();
	self->priv->playback_manager = tj_hdhr_playback_manager_new();
	self->priv->device_id = -1;

	self->priv->play_channel_handler_id = g_signal_connect(
			G_OBJECT(self->priv->channel_list), "play_channel",
			G_CALLBACK(on_play_channel), self);

	self->priv->play_channel_handler_id = g_signal_connect(
			G_OBJECT(self->priv->channel_list), "set-device",
			G_CALLBACK(on_set_device), self);

	gtk_widget_show(self->priv->channel_list);
	totem_object_add_sidebar_page(totem, "channel_list", "Homerunner", self->priv->channel_list);

	device_store = tj_hdhr_manager_get_devices(TJ_HDHR_MANAGER(self->priv->channel_manager));
	//channel_store = tj_hdhr_manager_scan_channels(self->priv->manager);
	//self->priv->channel_store = tj_hdhr_manager_test_scan_channels(self->priv->manager);
	channel_store = tj_hdhr_channel_manager_load_channels_from_xml_file(self->priv->channel_manager, "/home/tristian/channels.xml", schema_path);

	tj_channel_list_set_device_model(TJ_CHANNEL_LIST(self->priv->channel_list), device_store);
	tj_channel_list_set_channel_model(TJ_CHANNEL_LIST(self->priv->channel_list), channel_store);
	//tj_hdhr_manager_save_channels_to_xml_file(self->priv->manager, channel_store, path, schema_path);

	g_object_unref(channel_store);
	g_object_unref(device_store);
	g_free(path);
	g_free(schema_path);
}

static void impl_deactivate(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self;
	TotemObject *totem;

	g_assert(plugin != NULL);

	/* Destroy resources created in impl_activate() here. e.g. Disconnect from signals
	 * and remove menu entries and UI. */
	self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	g_free(self->priv->channels_path);
	g_free(self->priv->plugin_config_path);
	g_object_unref(self->priv->channel_manager);
	g_object_unref(self->priv->playback_manager);
	g_signal_handler_disconnect(plugin, self->priv->play_channel_handler_id);
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	totem_object_remove_sidebar_page(totem, "channel_list");

}


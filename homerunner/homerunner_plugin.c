#include <glib.h>
#include <glib-object.h>
#include <totem.h>
#include <totem-plugin.h>
#include <arpa/inet.h>
#ifndef HDHOMERUN_H
#define HDHOMERUN_H
#include <hdhomerun/hdhomerun.h>
#endif
#include "tj_channel_list.h"
#include "tj_hdhr_manager.h"
#include "bacon-video-widget.h"

#define TOTEM_TYPE_HOMERUNNER_PLUGIN			(totem_homerunner_plugin_get_type ())
#define TOTEM_HOMERUNNER_PLUGIN(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin))
#define TOTEM_HOMERUNNER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))
#define TOTEM_IS_HOMERUNNER_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_IS_HOMERUNNER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_HOMERUNNER_PLUGIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))

typedef struct
{
	struct hdhomerun_debug_t *dbg;
	struct hdhomerun_device_t *current;
	GtkWidget *channel_list;
	TJHDHRManager *manager;
	gchar* plugin_config_path;
	gchar* channels_path;
	gint play_channel_handler_id;
} TotemHomerunnerPluginPrivate;

TOTEM_PLUGIN_REGISTER(TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin,
		totem_homerunner_plugin);

static void on_select_device(PeasActivatable *plugin)
{

}

static void on_play_channel(PeasActivatable *plugin, guint frequency,
		guint program_id, TJChannelList *list)
{
	TotemHomerunnerPlugin *self;
	TotemObject *totem;
	BaconVideoWidget *bvw;
	gchar *uri;
	gchar* ip;
	gchar* port;

	g_assert(plugin != NULL);
	g_assert(list != NULL);

	ip = g_strdup("127.0.0.1");
	port = g_strdup("5000");
	self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	bvw = BACON_VIDEO_WIDGET (totem_get_video_widget(totem));

	uri = g_strdup_printf("%s:%s", ip, port);
	//tj_hdhr_manager_stream_channel_to_uri(self->priv->manager, self->priv->current, frequency, program_id, uri);
	g_debug("Running handler with frequency %d and program_id %d", frequency, program_id);
	bacon_video_widget_open(bvw, uri);
	bacon_video_widget_play(bvw, NULL);
	g_object_unref(bvw);
	g_free(uri);
	g_free(port);
	g_free(ip);
}

static void verify_config_paths(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	const gchar *config_path;
	GFile *plugin_config_dir;
	GFile *channels_dir;
	GError *error;

	g_assert(plugin != NULL);

	error = NULL;
	config_path = g_get_user_config_dir();
	self->priv->plugin_config_path = g_build_filename(config_path, "homerunner", NULL);
	plugin_config_dir = g_file_new_for_path(self->priv->plugin_config_path);
	g_file_make_directory(plugin_config_dir, NULL, &error);
	if (error != NULL) {
		if (error->code != G_IO_ERROR_EXISTS) {
			g_critical("Could not read from plugin configuration directory: %s", error->message);
			//impl_deactivate(plugin);
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
			//impl_deactivate(plugin);
		}
		g_error_free(error);
		error = NULL;
	}

	g_object_unref(plugin_config_dir);
	g_object_unref(channels_dir);
}

static void impl_activate(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN(plugin);
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
	verify_config_paths(plugin);
	path = g_strdup("/home/tristian/channels.xml");

	totem = g_object_get_data(G_OBJECT(plugin), "object");
	schema_path = totem_plugin_find_file("homerunner", "schema.xsd");
	self->priv->channel_list = tj_channel_list_new();
	self->priv->manager = tj_hdhr_manager_new();
	self->priv->play_channel_handler_id = g_signal_connect_swapped(
			G_OBJECT(self->priv->channel_list), "play_channel",
			G_CALLBACK(on_play_channel), self);

	gtk_widget_show(self->priv->channel_list);
	totem_object_add_sidebar_page(totem, "channel_list", "Homerunner", self->priv->channel_list);
	tj_hdhr_manager_init_devices(self->priv->manager);

	device_store = tj_hdhr_manager_get_devices(self->priv->manager);
	//channel_store = tj_hdhr_manager_scan_channels(self->priv->manager);
	//self->priv->channel_store = tj_hdhr_manager_test_scan_channels(self->priv->manager);
	channel_store = tj_hdhr_manager_load_channels_from_xml_file(self->priv->manager, "/home/tristian/channels.xml", schema_path);

	tj_channel_list_set_devices(TJ_CHANNEL_LIST(self->priv->channel_list), device_store);
	tj_channel_list_set_channels(TJ_CHANNEL_LIST(self->priv->channel_list), channel_store);
	//tj_hdhr_manager_save_channels_to_xml_file(self->priv->manager, channel_store, path, schema_path);

	g_object_unref(channel_store);
	g_object_unref(device_store);
	g_free(path);
	g_free(schema_path);
}

static void impl_deactivate(PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN(plugin);
	TotemObject *totem;

	g_assert(plugin != NULL);

	/* Destroy resources created in impl_activate() here. e.g. Disconnect from signals
	 * and remove menu entries and UI. */
	g_free(self->priv->channels_path);
	g_free(self->priv->plugin_config_path);
	g_object_unref(self->priv->manager);
	//if (g_signal_handler_is_connected(G_OBJECT(plugin), self->priv->play_channel_handler_id)) {
	//	g_signal_handler_disconnect(plugin, self->priv->play_channel_handler_id);
	//}
	totem = g_object_get_data(G_OBJECT(plugin), "object");
	totem_object_remove_sidebar_page(totem, "channel_list");

}


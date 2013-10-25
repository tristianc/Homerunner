#include <glib.h>
#include <glib-object.h>
#include <totem.h>
#include <totem-plugin.h>
#include <hdhomerun/hdhomerun.h>
#include <arpa/inet.h>
#include "channel_list.h"

#define TOTEM_TYPE_HOMERUNNER_PLUGIN			(totem_homerunner_plugin_get_type ())
#define TOTEM_HOMERUNNER_PLUGIN(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin))
#define TOTEM_HOMERUNNER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))
#define TOTEM_IS_HOMERUNNER_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_IS_HOMERUNNER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_HOMERUNNER_PLUGIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))

typedef struct {
	struct hdhomerun_debug_t *dbg;
} TotemHomerunnerPluginPrivate;

TOTEM_PLUGIN_REGISTER (TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin, totem_homerunner_plugin);

static gchar*
ipbits_to_str (uint32_t binary_address)
{
	gchar* string = malloc(sizeof(guchar) * 16);
	guchar octet[4];

	int i;
	for (i = 0; i < 4; i ++) {
		octet[i] = (guchar) (binary_address >> (i*8)) & 0xFF;
	}
	sprintf(string, "%u.%u.%u.%u\0", octet[3], octet[2], octet[1], octet[0]);
	g_debug("IP address is: %s", string);
	return string;
}

//static struct homerun_discover_device_t*
//get_devices (TotemHomerunnerPlugin *self)
//{
//	g_debug ("In find_devices");
//	//TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN (plugin);
//	int MAX_COUNT = 16;
//	struct hdhomerun_discover_device_t* discoveries = malloc(sizeof(struct homerun_discover_device_t) * MAX_COUNT);
//	int count =  hdhomerun_discover_find_devices_custom (0, HDHOMERUN_DEVICE_TYPE_TUNER, HDHOMERUN_DEVICE_ID_WILDCARD, discoveries, MAX_COUNT);
//	if (count == -1) {
//		g_debug ("Device discovery failed.");
//		return NULL;
//	}
//
//	uint i;
//	for (i = 0; i < count; i ++) {
//		gchar* ip_addr_str = ipbits_to_str(discoveries[i].ip_addr);
//		g_debug ("Discovered device with at address %s with id %X. It has %u tuners", ip_addr_str, discoveries[i].device_id, discoveries[i].tuner_count);
//	}
//	return discoveries;
//}

static void
get_channel_list ()
{
}

static void
stream_channel ()
{
}

static void
stop_channel ()
{
}

static void
impl_activate (PeasActivatable *plugin)
{
	g_debug ("In impl_activate");
	TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN (plugin);
	TotemHomerunnerPluginPrivate *priv = self->priv;
	TotemObject *totem;
	GtkWidget* channel_list;


	/* Initialise resources, connect to events, create menu items and UI, etc., here.
	 * Note that impl_activate() and impl_deactivate() can be called multiple times in one
	 * Totem instance, though impl_activate() will always be followed by impl_deactivate() before
	 * it is called again. Similarly, impl_deactivate() cannot be called twice in succession. */
	totem = g_object_get_data (G_OBJECT (plugin), "object");
	channel_list = totem_channel_list_new ();


	// find all device IDs on network; keep track of them so we can switch between them
	// store device IDs in drop down so we can select between them
	// add channels on device to playlist

	//get_devices (TOTEM_HOMERUNNER_PLUGIN (self));
	gtk_widget_show (channel_list);
	totem_object_add_sidebar_page (totem, "channel_list", "Channels", channel_list);
}

static void
impl_deactivate (PeasActivatable *plugin)
{
	TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN (plugin);
	TotemObject *totem;

	/* Destroy resources created in impl_activate() here. e.g. Disconnect from signals
	 * and remove menu entries and UI. */
	totem = g_object_get_data (G_OBJECT (plugin), "object");
	totem_object_remove_sidebar_page (totem, "channel_list");

}

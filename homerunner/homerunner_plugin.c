#include <glib.h>
#include <glib-object.h>
#include <totem.h>
#include <totem-plugin.h>



#define TOTEM_TYPE_HOMERUNNER_PLUGIN			(totem_Homerunner_plugin_get_type ())
#define TOTEM_HOMERUNNER_PLUGIN(o)				(G_TYPE_CHECK_INSTANCE_CAST ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin))
#define TOTEM_HOMERUNNER_PLUGIN_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))
#define TOTEM_IS_HOMERUNNER_PLUGIN(o)			(G_TYPE_CHECK_INSTANCE_TYPE ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_IS_HOMERUNNER_PLUGIN_CLASS(k)     (G_TYPE_CHECK_CLASS_TYPE ((k), TOTEM_TYPE_HOMERUNNER_PLUGIN))
#define TOTEM_HOMERUNNER_PLUGIN_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPluginClass))

typedef struct {
    /* Plugin private member variables */
} TotemHomerunnerPluginPrivate;

TOTEM_PLUGIN_REGISTER (TOTEM_TYPE_HOMERUNNER_PLUGIN, TotemHomerunnerPlugin, totem_homerunner_plugin);

static void
impl_activate (PeasActivatable *plugin)
{
    TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN (plugin);
    TotemHomerunnerPluginPrivate *priv = self->priv;
    TotemObject *totem = g_object_get_data (G_OBJECT (plugin), "object");

    /* Initialise resources, connect to events, create menu items and UI, etc., here.
     * Note that impl_activate() and impl_deactivate() can be called multiple times in one
     * Totem instance, though impl_activate() will always be followed by impl_deactivate() before
     * it is called again. Similarly, impl_deactivate() cannot be called twice in succession. */
}

static void
impl_deactivate (PeasActivatable *plugin)
{
    TotemHomerunnerPlugin *self = TOTEM_HOMERUNNER_PLUGIN (plugin);

    /* Destroy resources created in impl_activate() here. e.g. Disconnect from signals
     * and remove menu entries and UI. */
}

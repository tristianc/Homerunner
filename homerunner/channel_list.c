#include "channel_list.h"

struct _TotemChannelListPrivate
{
	GtkWidget* box1;
	GtkWidget* comboboxtext1;
	GtkWidget* scrolledwindow1;
	GtkWidget* treeview1;
};

G_DEFINE_TYPE_WITH_PRIVATE (TotemChannelList, totem_channel_list, GTK_TYPE_BOX);

void
populate_devices (TotemChannelList *widget)
{

}

void
populate_channels ();

void
start_playback ();

static void
totem_channel_list_class_init (TotemChannelListClass *k)
{
	GtkWidgetClass* widget_class;

	widget_class = GTK_WIDGET_CLASS (k);
	gtk_widget_class_set_template_from_resource (widget_class, "/org/thoughtjacked/homerunner/channel_list.ui");
	gtk_widget_class_bind_template_child_private (widget_class, TotemChannelList, box1);
	gtk_widget_class_bind_template_child_private (widget_class, TotemChannelList, comboboxtext1);
	gtk_widget_class_bind_template_child_private (widget_class, TotemChannelList, scrolledwindow1);
	gtk_widget_class_bind_template_child_private (widget_class, TotemChannelList, treeview1);
}

static void
totem_channel_list_init (TotemChannelList *widget)
{
	widget->priv = G_TYPE_INSTANCE_GET_PRIVATE (widget, TOTEM_TYPE_CHANNEL_LIST, TotemChannelListPrivate);
	gtk_widget_init_template (GTK_WIDGET (widget));
	//gtk_widget_set_has_window (GTK_WIDGET (widget), FALSE);
}

GtkWidget *
totem_channel_list_new ()
{
	return g_object_new (TOTEM_TYPE_CHANNEL_LIST, NULL);
}

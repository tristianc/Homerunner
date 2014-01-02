#include "tl_channel_list.h"
#include "marshaller.h"
#include "tl_model_enums.h"

enum
{
	CHANNEL_COLUMN = 0,
	STATION_COLUMN
};

enum
{
	PLAY_CHANNEL = 0,
	SET_DEVICE,
	LAST_SIGNAL
};

static guint tl_channel_list_signals[LAST_SIGNAL];

struct _TLChannelListPrivate
{
	GtkWidget *box1;
	GtkWidget *combobox1;
	GtkWidget *scrolledwindow1;
	GtkWidget *treeview1;
	gint changed_handler_id;
};

G_DEFINE_TYPE_WITH_PRIVATE(TLChannelList, tl_channel_list, GTK_TYPE_BOX);


static void on_combobox_changed(GtkComboBox *box, TLChannelList *user_data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	uint32_t id;

	g_assert(box != NULL);
	g_assert(user_data != NULL);

	id = -1;
	gtk_combo_box_get_active_iter(box, &iter);
	model = gtk_combo_box_get_model(box);
	if (model != NULL) {
		gtk_tree_model_get(model, &iter, TL_DEVICE_MODEL_DEV_ID_COLUMN, &id, -1);
	}
	g_signal_emit(user_data, tl_channel_list_signals[SET_DEVICE], 0, id);
}

static void on_row_activated(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *column, TLChannelList *user_data)
{
        guint32 frequency;
        guint program_id;
        GtkTreeModel *model;
        GtkTreeIter iter;

        g_assert(view != NULL);
        g_assert(path != NULL);
        g_assert(user_data != NULL);

        model = gtk_tree_view_get_model(view);
        if (model != NULL) {
        	gtk_tree_model_get_iter(model, &iter, path);
        	gtk_tree_model_get(model, &iter, TL_CHANNEL_MODEL_FREQ_COLUMN, &frequency, TL_CHANNEL_MODEL_PROGRAM_ID_COLUMN, &program_id, -1);
        	g_debug("Received frequency %d and program id %d", frequency, program_id);
        	g_signal_emit(user_data, tl_channel_list_signals[PLAY_CHANNEL], 0, frequency, program_id);
        }
}

static void default_device_renderer(GtkTreeViewColumn *column, GtkCellRenderer *renderer, GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *value;

	g_assert(column != NULL);
	g_assert(renderer != NULL);
	g_assert(model != NULL);
	g_assert(iter != NULL);

	gtk_tree_model_get(model, iter, TL_DEVICE_MODEL_NAME_COLUMN, &value, -1);
	g_object_set(renderer, "text", value, NULL);
	g_free(value);
}

static void tl_channel_list_add_columns(TLChannelList *self)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *channel_column;
	GtkTreeViewColumn *station_column;
	GtkTreeViewColumn *column;

	g_assert(self != NULL);

	renderer = gtk_cell_renderer_text_new();
	channel_column = gtk_tree_view_column_new_with_attributes("Channel",
		renderer, "text", CHANNEL_COLUMN, NULL);
	station_column = gtk_tree_view_column_new_with_attributes("Station",
		renderer, "text", STATION_COLUMN, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(self->priv->treeview1),
		channel_column);
	gtk_tree_view_append_column(GTK_TREE_VIEW(self->priv->treeview1),
		station_column);

	column = gtk_tree_view_get_column(GTK_TREE_VIEW(self->priv->treeview1), CHANNEL_COLUMN);
	gtk_tree_view_column_set_sort_column_id(column, TL_CHANNEL_MODEL_VCHANNEL_COLUMN);
	column = gtk_tree_view_get_column(GTK_TREE_VIEW(self->priv->treeview1), STATION_COLUMN);
	gtk_tree_view_column_set_sort_column_id(column, TL_CHANNEL_MODEL_STATION_COLUMN);
}

static gint tl_channel_list_sort_channel_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer user_data)
{
	gint ret = 0;
	gchar *left_channel;
	gchar *right_channel;
	gfloat left_val;
	gfloat right_val;
	int result;

	result = 0;
	gtk_tree_model_get(model, a, TL_CHANNEL_MODEL_VCHANNEL_COLUMN, &left_channel, -1);
	gtk_tree_model_get(model, b, TL_CHANNEL_MODEL_VCHANNEL_COLUMN, &right_channel, -1);
	if (left_channel == NULL || right_channel == NULL) {
		if (left_channel == NULL && right_channel == NULL) {
			result = 0;
		}
		else {
			result = (left_channel == NULL) ? -1 : 1;
		}
	}
	else {
		left_val = atof(left_channel);
		right_val = atof(right_channel);
		result = left_val - right_val;
	}

	g_free(left_channel);
	g_free(right_channel);
	return result;
}

static void tl_channel_list_class_init(TLChannelListClass *k)
{
	GtkWidgetClass *widget_class;

	widget_class = GTK_WIDGET_CLASS(k);
	gtk_widget_class_set_template_from_resource(widget_class,
		"/org/titaniclistener/homerunner/tl_channel_list.ui");
	gtk_widget_class_bind_template_child_private(widget_class, TLChannelList,
		box1);
	gtk_widget_class_bind_template_child_private(widget_class, TLChannelList,
		combobox1);
	gtk_widget_class_bind_template_child_private(widget_class, TLChannelList,
		scrolledwindow1);
	gtk_widget_class_bind_template_child_private(widget_class, TLChannelList,
		treeview1);

	tl_channel_list_signals[PLAY_CHANNEL] = g_signal_new("play-channel",
		G_TYPE_FROM_CLASS(k),
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		G_STRUCT_OFFSET (TLChannelListClass, play_channel),
		NULL,
		NULL,
		g_cclosure_user_marshal_VOID__UINT_UINT,
		G_TYPE_NONE,
		2,
		G_TYPE_UINT,
		G_TYPE_UINT);

	tl_channel_list_signals[SET_DEVICE] = g_signal_new("set-device",
		G_TYPE_FROM_CLASS(k),
		G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		G_STRUCT_OFFSET (TLChannelListClass, set_device),
		NULL,
		NULL,
		g_cclosure_marshal_VOID__INT,
		G_TYPE_NONE,
		1,
		G_TYPE_UINT);
}

static void tl_channel_list_init(TLChannelList *self)
{
	GtkCellRenderer *device_renderer;

	g_assert(self != NULL);

	self->priv = tl_channel_list_get_instance_private(self);
	gtk_widget_init_template(GTK_WIDGET(self));
	tl_channel_list_add_columns(self);
	g_signal_connect(self->priv->treeview1, "row-activated",
			G_CALLBACK (on_row_activated), self);

	device_renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(self->priv->combobox1),
			device_renderer, TRUE);
	gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(self->priv->combobox1),
			device_renderer, (GtkCellLayoutDataFunc)default_device_renderer, NULL, NULL);
	self->priv->changed_handler_id = g_signal_connect(
			G_OBJECT(self->priv->combobox1), "changed",
			G_CALLBACK(on_combobox_changed), self);
}

GtkWidget *tl_channel_list_new()
{
	return g_object_new(TL_TYPE_CHANNEL_LIST, NULL);
}

void tl_channel_list_set_channel_model(TLChannelList *self, GtkListStore *channel_model)
{
	g_assert(self != NULL);
	g_assert(channel_model != NULL);

	gtk_tree_view_set_model(GTK_TREE_VIEW(self->priv->treeview1),
		GTK_TREE_MODEL(channel_model));
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(channel_model), TL_CHANNEL_MODEL_VCHANNEL_COLUMN, tl_channel_list_sort_channel_func, NULL, NULL);
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(channel_model), TL_CHANNEL_MODEL_VCHANNEL_COLUMN, GTK_SORT_ASCENDING);
}

GtkListStore *tl_channel_list_get_channel_model(TLChannelList *self)
{
        GtkTreeModel *channel_model;

        g_assert(self != NULL);
        g_assert(channel_model != NULL);

        channel_model = gtk_tree_view_get_model(GTK_TREE_VIEW(self->priv->treeview1));
        return GTK_LIST_STORE(channel_model);
}

void tl_channel_list_set_device_model(TLChannelList *self, GtkListStore *device_model)
{
	g_assert(self != NULL);
	g_assert(device_model != NULL);

	gtk_combo_box_set_model(GTK_COMBO_BOX(self->priv->combobox1), GTK_TREE_MODEL(device_model));
}

GtkListStore *tl_channel_list_get_device_model(TLChannelList *self)
{
        GtkTreeModel *device_model;

        g_assert(self != NULL);

        device_model = NULL;
        device_model = gtk_combo_box_get_model(GTK_COMBO_BOX(self->priv->combobox1));
        return GTK_LIST_STORE(device_model);
}


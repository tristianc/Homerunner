#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#define LIBXML_SCHEMAS_ENABLED
#include <libxml/xmlschemastypes.h>
#include "tj_hdhr_channel_manager.h"
#include "tj_model_enums.h"

#define ENCODING "UTF-8"

G_DEFINE_TYPE(TJHDHRChannelManager, tj_hdhr_channel_manager, TJ_TYPE_HDHR_MANAGER);

static void tj_hdhr_channel_manager_class_init(TJHDHRChannelManagerClass *k)
{
}

static void tj_hdhr_channel_manager_init(TJHDHRChannelManager *self)
{
}

TJHDHRChannelManager *tj_hdhr_channel_manager_new()
{
	return g_object_new(TJ_TYPE_HDHR_CHANNEL_MANAGER, NULL);
}

static void tj_hdhr_channel_manager_dispose(GObject *object)
{
	TJHDHRChannelManager *self = TJ_HDHR_CHANNEL_MANAGER(object);

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
	G_OBJECT_CLASS (tj_hdhr_channel_manager_parent_class)->dispose(object);
}

static void tj_hdhr_channel_manager_finalize(GObject *object)
{
	TJHDHRChannelManager *self = TJ_HDHR_CHANNEL_MANAGER(object);

	/* Always chain up to the parent class; as with dispose(), finalize()
	 * is guaranteed to exist on the parent's class virtual function table
	 */
	G_OBJECT_CLASS (tj_hdhr_channel_manager_parent_class)->finalize(object);
}

GtkListStore *tj_hdhr_channel_manager_test_scan_channels(TJHDHRChannelManager *self)
{
	GtkListStore *test_channel_store;
	GtkTreeIter channel_iter;

	g_assert(self != NULL);
	test_channel_store = gtk_list_store_new(TJ_CHANNEL_MODEL_N_COLUMNS,
			G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);
	gtk_list_store_insert_with_values(test_channel_store, &channel_iter, -1,
			TJ_CHANNEL_MODEL_VCHANNEL_COLUMN, "88.1",
			TJ_CHANNEL_MODEL_STATION_COLUMN, "TristinTV",
			TJ_CHANNEL_MODEL_FREQ_COLUMN, 0000,
			TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN, 1111,
			-1);
	gtk_list_store_insert_with_values(test_channel_store, &channel_iter, -1,
			TJ_CHANNEL_MODEL_VCHANNEL_COLUMN, "92.Q",
			TJ_CHANNEL_MODEL_STATION_COLUMN, "OniTV",
			TJ_CHANNEL_MODEL_FREQ_COLUMN, 0001,
			TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN, 2222,
			-1);
	return test_channel_store;
}

GtkListStore *tj_hdhr_channel_manager_scan_channels(TJHDHRChannelManager *self, uint32_t device_id)
{
	guint i;
	guint j;
	guint k;
	gchar *channelmap;
	gchar *virtual_channel;
	const gchar *channelmap_scan_group;
	GtkListStore *channel_store;
	GtkTreeIter channel_iter;
	struct hdhomerun_device_t *scanner_device;
	int ret;

	g_assert(self != NULL);
	channel_store = gtk_list_store_new(TJ_CHANNEL_MODEL_N_COLUMNS, G_TYPE_STRING,
	G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);

	scanner_device = tj_hdhr_manager_get_tuner(TJ_HDHR_MANAGER(self), device_id);

	if (scanner_device == NULL)
	{
		g_debug("Failed to set tuner.");
	}

	if (hdhomerun_device_get_tuner_channelmap(scanner_device, &channelmap) <= 0)
	{
		g_debug("Failed to query channelmap from device");
	}

	channelmap_scan_group = hdhomerun_channelmap_get_channelmap_scan_group(channelmap);
	if (channelmap_scan_group == NULL)
	{
		g_debug("Unknown channelmap '%s'", channelmap);
	}

	if (hdhomerun_device_channelscan_init(scanner_device, channelmap_scan_group) <= 0)
	{
		g_debug("Failed to initialize channel scan");
	}

	while (TRUE)
	{
		struct hdhomerun_channelscan_result_t result;
		ret = hdhomerun_device_channelscan_advance(scanner_device, &result);
		if (ret <= 0)
		{
			break;
		}

		ret = hdhomerun_device_channelscan_detect(scanner_device, &result);
		if (ret <= 0)
		{
			break;
		}

		for (k = 0; k < result.program_count; k++)
		{

			if ((result.programs[k].virtual_major != 0)
					&& (strcmp(result.programs[k].name, "") != 0))
			{
				if (result.programs[k].virtual_minor > 0)
				{
					virtual_channel = g_strdup_printf("%hu.%hu",
							result.programs[k].virtual_major,
							result.programs[k].virtual_minor);
				}
				else
				{
					virtual_channel = g_strdup_printf("%hu",
							result.programs[k].virtual_major);
				}

				gtk_list_store_insert_with_values(channel_store, &channel_iter,
						-1,
						TJ_CHANNEL_MODEL_VCHANNEL_COLUMN, virtual_channel,
						TJ_CHANNEL_MODEL_STATION_COLUMN, result.programs[k].name,
						TJ_CHANNEL_MODEL_FREQ_COLUMN, result.frequency,
						TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN, result.programs[k].program_number,
						-1);
				g_debug("NAME %s", result.programs[k].name);
				g_debug("PROGRAM %s", result.programs[k].program_str);

				g_free(virtual_channel);
			}
		}
	}

	if (ret < 0)
	{
		g_debug("Communication error sending request to HDHomerun device");
	}

	hdhomerun_device_tuner_lockkey_release(scanner_device);
	return channel_store;
}

static void tj_hdhr_channel_manager_add_channel_to_xml(TJHDHRChannelManager *self,
		xmlNodePtr parent, gchar *vchannel, gchar *station, guint32 frequency,
		guint32 program_id)
{
	int rc;
	gchar *vchannel_content;
	gchar *station_content;
	gchar *freq_content;
	gchar *pid_content;
	xmlNodePtr local_parent;
	xmlNodePtr node;

	g_assert(self != NULL);
	g_assert(parent != NULL);
	g_assert(vchannel != NULL);
	g_assert(station != NULL);

	g_debug("Constructing channel node");
	g_debug("Frequency: %d", frequency);
	g_debug("Program ID: %d", program_id);
	g_debug("Station: %s", station);
	g_debug("Virtual Channel: %s", vchannel);

	local_parent = xmlNewChild(parent, NULL, BAD_CAST "channel", NULL);
	if (local_parent == NULL) {
		g_debug("Error creating channel element");
		goto cleanup;
	}

	vchannel_content = g_strdup_printf("%s", vchannel);
	node = xmlNewChild(local_parent, NULL, BAD_CAST "vchan",
			BAD_CAST vchannel_content);
	if (node == NULL) {
		g_debug("Error creating virtual channel element");
		goto cleanup;
	}

	station_content = g_strdup_printf("%s", station);
	node = xmlNewChild(local_parent, NULL, BAD_CAST "station",
			BAD_CAST station_content);
	if (node == NULL) {
		g_debug("Error creating station element");
		goto cleanup;
	}

	freq_content = g_strdup_printf("%d", frequency);
	node = xmlNewTextChild(local_parent, NULL, BAD_CAST "frequency",
			BAD_CAST freq_content);
	if (node == NULL) {
		g_debug("Error creating frequency element");
		goto cleanup;
	}

	pid_content = g_strdup_printf("%d", program_id);
	node = xmlNewTextChild(local_parent, NULL, BAD_CAST "pid",
			BAD_CAST pid_content);
	if (node == NULL) {
		g_debug("Error creating program id element");
		goto cleanup;
	}

	g_debug("Finished constructing channel node");
cleanup:
	g_free(vchannel_content);
	g_free(station_content);
	g_free(freq_content);
	g_free(pid_content);
}

void tj_hdhr_channel_manager_save_channels_to_xml_file(TJHDHRChannelManager *self,
		GtkListStore *channel_store, gchar *path, gchar *schema_path)
{
	int rc;
	xmlTextWriterPtr writer;
	xmlDocPtr doc;
	xmlNsPtr namespace;
	xmlNodePtr device_node;
	xmlNodePtr channels_node;
	xmlNodePtr tree_root;
	xmlAttrPtr id_attr;
	xmlChar *tmp;
	GtkTreeIter iter;
	gboolean valid;
	gchar *vchannel;
	gchar *station;
	guint32 frequency;
	guint32 program_id;

	LIBXML_TEST_VERSION

	g_assert(self != NULL);
	g_assert(channel_store != NULL);
	g_assert(path != NULL);

	doc = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);
	if (doc == NULL) {
		g_debug("Could not create XML document tree");
		goto cleanup;
	}

	device_node = xmlNewDocNode(doc, NULL, BAD_CAST "device", NULL);
	if (device_node == NULL) {
		g_debug("Could not create new XML device node");
		goto cleanup;
	}
	namespace = xmlNewNs(device_node, BAD_CAST TJ_HDHR_CHANNEL_MANAGER_XML_NAMESPACE,
			NULL);
	if (namespace == NULL) {
		g_debug("Could not write namespace to device node");
		goto cleanup;
	}

	/*
	 * ToDo:
	 * Add real device id
	 */
	id_attr = xmlNewProp(device_node, BAD_CAST "id", BAD_CAST "ABCDEFG");
	if (id_attr == NULL) {
		g_debug("Could not add id attribute to device node");
		goto cleanup;
	}

	channels_node = xmlNewChild(device_node, NULL, BAD_CAST "channels", NULL);
	if (device_node == NULL) {
		g_debug("Could not create new channels node");
		goto cleanup;
	}

	/* Set the root node of the tree */
	xmlDocSetRootElement(doc, device_node);

	valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(channel_store), &iter);
	while (valid) {
		gtk_tree_model_get(GTK_TREE_MODEL(channel_store), &iter,
				TJ_CHANNEL_MODEL_VCHANNEL_COLUMN, &vchannel,
				TJ_CHANNEL_MODEL_STATION_COLUMN, &station,
				TJ_CHANNEL_MODEL_FREQ_COLUMN, &frequency,
				TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN, &program_id,
				-1);

		tj_hdhr_channel_manager_add_channel_to_xml(self, channels_node, vchannel,
				station, frequency, program_id);
		valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(channel_store), &iter);
		g_free(vchannel);
		g_free(station);
	}

cleanup:
	xmlSaveFormatFileEnc(path, doc, ENCODING, 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
}

static void tj_hdhr_parse_channel_from_xml_file(TJHDHRChannelManager *self,
		xmlNodePtr channels, GtkListStore *channel_store)
{
	xmlNodePtr child;
	xmlNodePtr data;
	gchar *vchannel;
	gchar *station;
	gchar *freq_str;
	gchar *pid_str;
	guint32 frequency;
	guint32 program_id;
	GtkTreeIter channel_iter;

	g_assert(self != NULL);
	g_assert(channels != NULL);
	g_assert(channel_store != NULL);

	for (child = channels->children; child != NULL; child = child->next) {
		if (xmlStrcmp(child->name, BAD_CAST "channel") == 0) {
			for (data = child->children; data != NULL; data = data->next) {
				if (xmlStrcmp(data->name, BAD_CAST "vchan") == 0) {
					vchannel = (gchar *)xmlNodeGetContent(data);
					g_debug("vchannel is %s", vchannel);
				}
				else if (xmlStrcmp(data->name, BAD_CAST "station") == 0) {
					station = (gchar *)xmlNodeGetContent(data);
					g_debug("station is %s", station);
				}
				else if (xmlStrcmp(data->name, BAD_CAST "frequency") == 0) {
					freq_str = (gchar *)xmlNodeGetContent(data);
					sscanf(freq_str, "%d", &frequency);
					g_free(freq_str);
					g_debug("frequency is %d", frequency);
				}
				else if (xmlStrcmp(data->name, BAD_CAST "pid") == 0) {
					pid_str = (gchar *)xmlNodeGetContent(data);
					sscanf((gchar *)xmlNodeGetContent(data), "%d", &program_id);
					g_free(pid_str);
					g_debug("program_id is %d", program_id);
				}
			}
			gtk_list_store_insert_with_values(channel_store, &channel_iter, -1,
					TJ_CHANNEL_MODEL_VCHANNEL_COLUMN, vchannel,
					TJ_CHANNEL_MODEL_STATION_COLUMN, station,
					TJ_CHANNEL_MODEL_FREQ_COLUMN, frequency,
					TJ_CHANNEL_MODEL_PROGRAM_ID_COLUMN, program_id,
					-1);
			g_free(vchannel);
			g_free(station);
		}
	}
}

static void tj_hdhr_channel_manager_parse_channels_from_xml_file(TJHDHRChannelManager *self,
		xmlDocPtr doc, GtkListStore *channel_store)
{
	xmlNodePtr current;
	xmlNodePtr root;
	xmlNodePtr child;
	xmlAttrPtr attribute;
	xmlChar *device_id;

	g_assert(self != NULL);
	g_assert(doc != NULL);
	g_assert(channel_store != NULL);

	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		g_debug("Could not get root element");
	}

	for (current = root; current != NULL; current = current->next) {
		if (xmlStrcmp(current->name, BAD_CAST "device") == 0) {
			device_id = xmlGetProp(current, BAD_CAST "id");
			if (device_id == NULL) {
				g_debug("No id attribute for device");
			}
			for (child = current->children; child != NULL; child =
					child->next) {
				if (xmlStrcmp(child->name, BAD_CAST "channels") == 0) {
					tj_hdhr_parse_channel_from_xml_file(self, child,
							channel_store);
				}
			}
			g_free(device_id);
		}
	}
}

GtkListStore *tj_hdhr_channel_manager_load_channels_from_xml_file(TJHDHRChannelManager *self, gchar *path,
		gchar *schema_path)
{
	GtkListStore *channel_store;
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlSchemaParserCtxtPtr parse_context;
	xmlSchemaValidCtxtPtr valid_context;
	xmlSchemaPtr schema;
	int rc;

	LIBXML_TEST_VERSION

	g_assert(self != NULL);
	g_assert(path != NULL);
	g_assert(schema_path != NULL);

	/*
	 * ToDo:
	 * Use GIO to provide file data to libxml2, so remote locations can be written to/read from
	 */
	doc = xmlReadFile(path, ENCODING, 0);
	if (doc == NULL) {
		g_debug("Could not parse file %s", path);
		channel_store = NULL;
		goto cleanup;
	}

	parse_context = xmlSchemaNewParserCtxt(schema_path);
	if (parse_context == NULL) {
		g_debug("Could not acquire parse context for %s", schema_path);
		channel_store = NULL;
		goto cleanup;
	}

	xmlSchemaSetParserErrors(parse_context,
			(xmlSchemaValidityErrorFunc) fprintf,
			(xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(parse_context);
	if (schema == NULL) {
		g_debug("Could not acquire schema for %s", schema_path);
		channel_store = NULL;
		goto cleanup;
	}

	valid_context = xmlSchemaNewValidCtxt(schema);
	if (valid_context == NULL) {
		g_debug("Could not acquire validation context for %s", schema_path);
		channel_store = NULL;
		goto cleanup;
	}

	xmlSchemaSetValidErrors(valid_context, (xmlSchemaValidityErrorFunc) fprintf,
			(xmlSchemaValidityWarningFunc) fprintf, stderr);

	rc = xmlSchemaValidateDoc(valid_context, doc);
	if (rc > 0) {
		g_debug("%s is not a valid XML tuner file", path);
		channel_store = NULL;
		goto cleanup;
	}
	else if (rc < 0) {
		g_debug("%s validation generated an internal error", path);
		channel_store = NULL;
		goto cleanup;
	}

	channel_store = gtk_list_store_new(TJ_CHANNEL_MODEL_N_COLUMNS, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_UINT, G_TYPE_UINT);
	tj_hdhr_channel_manager_parse_channels_from_xml_file(self, doc, channel_store);

cleanup:
	xmlSchemaFreeParserCtxt(parse_context);
	xmlSchemaFreeValidCtxt(valid_context);
	xmlSchemaCleanupTypes();
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return channel_store;
}


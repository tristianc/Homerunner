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

#include "tl_hdhr_manager.h"
#include "tl_model_enums.h"

#define MAX_DEVICES 256
#define ENCODING "UTF-8"

struct _TLHDHRManagerPrivate
{
	gint discovery_count;
	struct hdhomerun_discover_device_t *discoveries;
	GHashTable *tuner_pool;
};

G_DEFINE_TYPE_WITH_PRIVATE(TLHDHRManager, tl_hdhr_manager, G_TYPE_OBJECT);


static gchar *ipbits_to_str(uint32_t binary_address)
{
	gchar *str;
	guchar octet[4];
	gint i;

	for (i = 0; i < 4; i++) {
		octet[i] = (guchar) (binary_address >> (i * 8)) & 0xFF;
	}
	str = g_strdup_printf("%u.%u.%u.%u", octet[3], octet[2], octet[1], octet[0]);
	return str;
}

static void destroy_selector(gpointer selector) {
	hdhomerun_device_selector_destroy((struct hdhomerun_device_selector_t *)selector, TRUE);
}

static void tl_hdhr_manager_class_init(TLHDHRManagerClass *k)
{
}

static void tl_hdhr_manager_init(TLHDHRManager *self)
{
	self->priv = tl_hdhr_manager_get_instance_private(self);
	self->priv->discovery_count = -1;
	self->priv->discoveries = g_new0(struct hdhomerun_discover_device_t, MAX_DEVICES);
	self->priv->tuner_pool = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, destroy_selector);
}

TLHDHRManager *tl_hdhr_manager_new()
{
	return g_object_new(TL_TYPE_HDHR_MANAGER, NULL);
}

static void tl_hdhr_manager_dispose(GObject *object)
{
	TLHDHRManager *self = TL_HDHR_MANAGER(object);

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
	G_OBJECT_CLASS (tl_hdhr_manager_parent_class)->dispose(object);
}

static void tl_hdhr_manager_finalize(GObject *object)
{
	TLHDHRManager *self = TL_HDHR_MANAGER(object);

	g_free(self->priv->discoveries);
	g_hash_table_unref(self->priv->tuner_pool);
	/* Always chain up to the parent class; as with dispose(), finalize()
	 * is guaranteed to exist on the parent's class virtual function table
	 */
	G_OBJECT_CLASS (tl_hdhr_manager_parent_class)->finalize(object);
}

static void tl_hdhr_manager_discover_devices(TLHDHRManager *self)
{
	g_assert(self != NULL);

	g_free(self->priv->discoveries);
	self->priv->discoveries = g_new0(struct hdhomerun_discover_device_t, MAX_DEVICES);
	self->priv->discovery_count = hdhomerun_discover_find_devices_custom(0,
			HDHOMERUN_DEVICE_TYPE_TUNER, HDHOMERUN_DEVICE_ID_WILDCARD,
			self->priv->discoveries,
			MAX_DEVICES);
	if (self->priv->discovery_count == -1) {
		g_debug("Device discovery failed.");
	}
}

GtkListStore *tl_hdhr_manager_get_devices(TLHDHRManager *self)
{
	GtkListStore *device_store;
	GtkTreeIter device_iter;
	guint i;
	gchar *ip_addr_str;
	gchar *device_name;

	g_assert(self != NULL);

	device_store = gtk_list_store_new(TL_DEVICE_MODEL_N_COLUMNS, G_TYPE_STRING,
			G_TYPE_UINT);

	tl_hdhr_manager_discover_devices(self);
	for (i = 0; i < self->priv->discovery_count; i++)
	{
		ip_addr_str = ipbits_to_str(self->priv->discoveries[i].ip_addr);
		device_name = g_strdup_printf("%X", self->priv->discoveries[i].device_id);
		g_debug("Discovered device with at address %s with id %X. It has %u tuners",
				ip_addr_str, self->priv->discoveries[i].device_id,
				self->priv->discoveries[i].tuner_count);
		gtk_list_store_insert_with_values(device_store, &device_iter, -1,
				TL_DEVICE_MODEL_NAME_COLUMN, device_name,
				TL_DEVICE_MODEL_DEV_ID_COLUMN,
				self->priv->discoveries[i].device_id, -1);
		g_free(ip_addr_str);
		g_free(device_name);
	}
	return device_store;
}

struct hdhomerun_device_t *tl_hdhr_manager_get_tuner(TLHDHRManager *self, uint32_t device_id)
{
	int i;
	int j;
	struct hdhomerun_device_selector_t *selector;
	struct hdhomerun_device_t *device;
	struct hdhomerun_discover_device_t *discovery;

	g_assert(self != NULL);

	device = NULL;
	selector = (struct hdhomerun_device_selector_t *)g_hash_table_lookup(self->priv->tuner_pool, GUINT_TO_POINTER(device_id));
	if (selector == NULL) {
		tl_hdhr_manager_discover_devices(self);
		for (i = 0; i < self->priv->discovery_count; i++) {
			if (self->priv->discoveries[i].device_id == device_id) {
				discovery = &(self->priv->discoveries[i]);
				selector = hdhomerun_device_selector_create(NULL);
				for (j = 0; j < discovery->tuner_count; j++)
				{
					device = hdhomerun_device_create(device_id, discovery->ip_addr, j, NULL);
					g_debug("Created device with with id %X. It has %u tuners",
							self->priv->discoveries[i].device_id,
							self->priv->discoveries[i].tuner_count);
					hdhomerun_device_selector_add_device(selector, device);
				}
				g_hash_table_insert(self->priv->tuner_pool, GUINT_TO_POINTER(device_id), (gpointer) selector);
				device = hdhomerun_device_selector_choose_and_lock(selector, NULL);
				break;
			}
		}
	} else {
		device = hdhomerun_device_selector_choose_and_lock(selector, NULL);
	}
	return device;
}

gchar *tl_hdhr_manager_get_host_ip_relative_to_device(TLHDHRManager *self, uint32_t device_id)
{
	uint32_t ip;
	gchar *ip_str;
	gchar *dev_str;
	struct hdhomerun_device_t *dev;

	g_assert(self != NULL);

	ip_str = NULL;
	dev_str = g_strdup_printf("%X", device_id);
	dev = hdhomerun_device_create_from_str(dev_str, NULL);
	if (dev != NULL) {
		ip = hdhomerun_device_get_local_machine_addr(dev);
		if (ip > 0) {
			ip_str = ipbits_to_str(ip);
		}
		hdhomerun_device_destroy(dev);
	}
	g_free(dev_str);

	return ip_str;
}

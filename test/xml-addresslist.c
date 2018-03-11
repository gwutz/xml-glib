/* xml-addresslist.c
 *
 * Copyright 2018 Guenther Wutz <info@gunibert.de>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "xml-addresslist.h"
#include "xml-glib.h"

struct _XmlAddresslist
{
  GObject parent_instance;

  GPtrArray *addresses;
};

static void xml_addresslist_serializable_init (XmlSerializableInterface *iface);

G_DEFINE_TYPE_WITH_CODE (XmlAddresslist, xml_addresslist, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (XML_TYPE_SERIALIZABLE, xml_addresslist_serializable_init))

enum {
  PROP_0,
  PROP_ADDRESSES,
  N_PROPS
};

XmlAddresslist *
xml_addresslist_new (void)
{
  return g_object_new (XML_TYPE_ADDRESSLIST, NULL);
}

static void
xml_addresslist_finalize (GObject *object)
{
  XmlAddresslist *self = (XmlAddresslist *)object;

  g_ptr_array_unref (self->addresses);

  G_OBJECT_CLASS (xml_addresslist_parent_class)->finalize (object);
}

static void
xml_addresslist_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  XmlAddresslist *self = XML_ADDRESSLIST (object);

  switch (prop_id)
    {
    case PROP_ADDRESSES:
      g_value_set_boxed (value, self->addresses);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xml_addresslist_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  XmlAddresslist *self = XML_ADDRESSLIST (object);

  switch (prop_id)
    {
    case PROP_ADDRESSES:
      self->addresses = g_value_get_boxed (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xml_addresslist_class_init (XmlAddresslistClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = xml_addresslist_finalize;
  object_class->get_property = xml_addresslist_get_property;
  object_class->set_property = xml_addresslist_set_property;

  g_object_class_install_property (object_class, PROP_ADDRESSES, g_param_spec_boxed ("addresses", "Addresses", "the Addresses", G_TYPE_PTR_ARRAY, G_PARAM_READWRITE));
}

static void
xml_addresslist_init (XmlAddresslist *self)
{
  self->addresses = g_ptr_array_new_with_free_func (g_object_unref);
}

static xmlNodePtr
xml_addresslist_serialize_property (XmlSerializable *serializable,
                                    const gchar     *property_name,
                                    const GValue    *value,
                                    GParamSpec      *pspec)
{
  if (g_strcmp0 (property_name, "addresses") == 0) {
    GPtrArray *arr = g_value_get_boxed (value);
    xmlNodePtr first = NULL;

    for (int i = 0; i < arr->len; i++) {
      XmlAddress *a = g_ptr_array_index (arr, i);

      xmlNodePtr n = xml_gobject_serialize (G_OBJECT (a));
      if (i == 0) first = n;
      else xmlAddSibling (first, n);
    }

    return first;
  }

  return xml_serializable_default_serialize_property (serializable, property_name, value, pspec);
}

static
void xml_addresslist_serializable_init (XmlSerializableInterface *iface)
{
  iface->serialize_property = xml_addresslist_serialize_property;
}

void
xml_addresslist_add_address (XmlAddresslist *self,
                             XmlAddress     *address)
{
  g_ptr_array_add (self->addresses, address);
}

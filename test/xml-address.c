/* xml-address.c
 *
 * Copyright 2018 Günther Wutz <info@gunibert.de>
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

#include "xml-address.h"

struct _XmlAddress
{
  GObject parent_instance;

  gint house_number;
  gchar *street_name;
  gchar *city;
};

G_DEFINE_TYPE (XmlAddress, xml_address, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_HOUSE_NUMBER,
  PROP_STREET_NAME,
  PROP_CITY,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

XmlAddress *
xml_address_new (gint house_number, gchar *street_name, gchar *city)
{
  return g_object_new (XML_TYPE_ADDRESS,
                       "house-number", house_number,
                       "street-name", street_name,
                       "city", city,
                       NULL);
}

static void
xml_address_finalize (GObject *object)
{
  XmlAddress *self = (XmlAddress *)object;

  g_free (self->street_name);
  g_free (self->city);

  G_OBJECT_CLASS (xml_address_parent_class)->finalize (object);
}

static void
xml_address_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  XmlAddress *self = XML_ADDRESS (object);

  switch (prop_id)
    {
    case PROP_HOUSE_NUMBER:
      g_value_set_int (value, self->house_number);
      break;
    case PROP_STREET_NAME:
      g_value_set_string (value, self->street_name);
      break;
    case PROP_CITY:
      g_value_set_string (value, self->city);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xml_address_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  XmlAddress *self = XML_ADDRESS (object);

  switch (prop_id)
    {
    case PROP_HOUSE_NUMBER:
      self->house_number = g_value_get_int (value);
      break;
    case PROP_STREET_NAME:
      g_free (self->street_name);
      self->street_name = g_value_dup_string (value);
      break;
    case PROP_CITY:
      g_free (self->city);
      self->city = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
xml_address_class_init (XmlAddressClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = xml_address_finalize;
  object_class->get_property = xml_address_get_property;
  object_class->set_property = xml_address_set_property;

  properties[PROP_HOUSE_NUMBER] =
    g_param_spec_int ("house-number",
                      "House-Number",
                      "the house number",
                      1,
                      G_MAXINT,
                      1,
                      G_PARAM_READWRITE);

  properties[PROP_STREET_NAME] =
    g_param_spec_string ("street-name",
                         "Street-Name",
                         "the street name",
                         "",
                         G_PARAM_READWRITE);

  properties[PROP_CITY] =
    g_param_spec_string ("city",
                         "City",
                         "the city",
                         "",
                         G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
xml_address_init (XmlAddress *self)
{
}

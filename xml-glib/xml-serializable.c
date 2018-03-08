/* xml-serializable.c
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

#include "xml-serializable.h"
#include "xml-gobject-private.h"

G_DEFINE_INTERFACE (XmlSerializable, xml_serializable, G_TYPE_OBJECT)

xmlNodePtr
xml_serializable_serialize_property (XmlSerializable *serializable,
                                     const gchar     *property_name,
                                     const GValue    *value,
                                     GParamSpec      *pspec)
{
  XmlSerializableInterface *iface;

  g_return_val_if_fail (XML_IS_SERIALIZABLE (serializable), NULL);
  g_return_val_if_fail (property_name != NULL, NULL);
  g_return_val_if_fail (value != NULL, NULL);
  g_return_val_if_fail (pspec != NULL, NULL);

  iface = XML_SERIALIZABLE_GET_IFACE (serializable);

  return iface->serialize_property (serializable, property_name, value, pspec);
}

gboolean
xml_serializable_deserialize_property (XmlSerializable *serializable,
                                       const gchar     *property_name,
                                       GValue          *value,
                                       GParamSpec      *pspec,
                                       xmlNodePtr       node)
{
  XmlSerializableInterface *iface;

  g_return_val_if_fail (XML_IS_SERIALIZABLE (serializable), FALSE);
  g_return_val_if_fail (property_name != NULL, FALSE);
  g_return_val_if_fail (value != NULL, FALSE);
  g_return_val_if_fail (pspec != NULL, FALSE);
  g_return_val_if_fail (node != NULL, FALSE);

  iface = XML_SERIALIZABLE_GET_IFACE (serializable);

  return iface->deserialize_property (serializable, property_name, value, pspec, node);
}

xmlNodePtr
xml_serialize_property_real (XmlSerializable *serializable,
                             const gchar     *property_name,
                             const GValue    *value,
                             GParamSpec      *pspec)
{
  if (g_param_value_defaults (pspec, (GValue *)value))
    return NULL;

  return xml_serialize_pspec (pspec, value);
}

gboolean
xml_deserialize_property_real (XmlSerializable *serializable,
                               const gchar     *property_name,
                               GValue          *value,
                               GParamSpec      *pspec,
                               xmlNodePtr       node)
{
  return xml_deserialize_pspec (value, pspec, node);
}

const gchar *
xml_serializable_get_root_name (XmlSerializable *serializable)
{
  XmlSerializableInterface *iface;

  g_return_val_if_fail (XML_IS_SERIALIZABLE (serializable), G_OBJECT_CLASS_NAME (serializable));

  iface = XML_SERIALIZABLE_GET_IFACE (serializable);

  return iface->get_root_name (serializable);
}

static void
xml_serializable_default_init (XmlSerializableInterface *iface)
{
  iface->serialize_property = xml_serialize_property_real;
  iface->deserialize_property = xml_deserialize_property_real;
}

xmlNodePtr
xml_serializable_default_serialize_property (XmlSerializable *serializable,
                                             const gchar     *property_name,
                                             const GValue    *value,
                                             GParamSpec      *pspec)
{
  g_return_val_if_fail (XML_IS_SERIALIZABLE (serializable), NULL);
  g_return_val_if_fail (property_name != NULL, NULL);
  g_return_val_if_fail (value != NULL, NULL);
  g_return_val_if_fail (pspec != NULL, NULL);

  return xml_serialize_property_real (serializable, property_name, value, pspec);
}

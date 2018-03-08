/* xml-serializable.h
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

#pragma once

#include <glib-object.h>
#include <libxml/tree.h>

G_BEGIN_DECLS

#define XML_TYPE_SERIALIZABLE (xml_serializable_get_type ())

G_DECLARE_INTERFACE (XmlSerializable, xml_serializable, XML, SERIALIZABLE, GObject)

struct _XmlSerializableInterface
{
  GTypeInterface parent;

  xmlNodePtr (* serialize_property)   (XmlSerializable *serializable,
                                       const gchar     *property_name,
                                       const GValue    *value,
                                       GParamSpec      *pspec);
  gboolean   (* deserialize_property) (XmlSerializable *serializable,
                                       const gchar     *property_name,
                                       GValue          *value,
                                       GParamSpec      *pspec,
                                       xmlNodePtr       node);
  const gchar *(* get_root_name)      (XmlSerializable *serializable);
};

xmlNodePtr xml_serializable_serialize_property (XmlSerializable *serializable,
                                              const gchar     *property_name,
                                              const GValue    *value,
                                              GParamSpec      *pspec);

gboolean xml_serializable_deserialize_property (XmlSerializable *serializable,
                                                const gchar     *property_name,
                                                GValue          *value,
                                                GParamSpec      *pspec,
                                                xmlNodePtr       node);
xmlNodePtr xml_serializable_default_serialize_property (XmlSerializable *serializable,
                                                        const gchar     *property_name,
                                                        const GValue    *value,
                                                        GParamSpec      *pspec);

const gchar * xml_serializable_get_root_name (XmlSerializable *serializable);
G_END_DECLS

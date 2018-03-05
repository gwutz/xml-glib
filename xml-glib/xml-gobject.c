/* xml-gobject.c
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

#include "xml-gobject.h"
#include <glib/gprintf.h>
#include "xml-serializable.h"

xmlNodePtr
xml_serialize_pspec (GParamSpec   *pspec,
                     const GValue *real_value)
{
  gchar tmp[100];
  xmlNodePtr child = NULL;

  switch (G_TYPE_FUNDAMENTAL (G_VALUE_TYPE (real_value))) {
  case G_TYPE_INT:
    g_sprintf (tmp, "%d", g_value_get_int (real_value));
    child = xmlNewDocNode (NULL, NULL, BAD_CAST pspec->name, BAD_CAST tmp);
    break;

  case G_TYPE_INT64:
    g_sprintf (tmp, "%ld", g_value_get_int64 (real_value));
    child = xmlNewDocNode (NULL, NULL, BAD_CAST pspec->name, BAD_CAST tmp);
    break;

  case G_TYPE_BOOLEAN:
    g_sprintf (tmp, "%d", g_value_get_boolean (real_value));
    child = xmlNewDocNode (NULL, NULL, BAD_CAST pspec->name, BAD_CAST tmp);
    break;

  case G_TYPE_DOUBLE:
    g_sprintf (tmp, "%f", g_value_get_double (real_value));
    child = xmlNewDocNode (NULL, NULL, BAD_CAST pspec->name, BAD_CAST tmp);
    break;

  case G_TYPE_STRING:
    child = xmlNewDocNode (NULL, NULL, BAD_CAST pspec->name, BAD_CAST g_value_get_string (real_value));
    break;

  case G_TYPE_OBJECT:
    child = xml_gobject_serialize (g_value_get_object (real_value));
    break;

  default:
    g_warning ("Unsupported type `%s'", g_type_name (G_VALUE_TYPE (real_value)));
    break;
  }

  return child;
}

gboolean
xml_deserialize_pspec (GValue     *value,
                       GParamSpec *pspec,
                       xmlNodePtr  node)
{

  if (g_type_is_a (G_VALUE_TYPE(value), G_TYPE_OBJECT)) {
    // TODO how to deserialize objects
    GObject *obj = xml_gobject_deserialize (G_VALUE_TYPE (value), node);
    if (obj != NULL)
      g_value_take_object (value, obj);
    else
      g_value_set_object (value, NULL);

    return TRUE;
  }

  gchar *buf;
  buf = (gchar *)xmlNodeGetContent (node);
  switch (G_TYPE_FUNDAMENTAL (G_VALUE_TYPE (value))) {
  case G_TYPE_INT:
    g_value_set_int (value, atoi (buf));
    break;
  case G_TYPE_INT64:
    g_value_set_int64 (value, atol (buf));
    break;
  case G_TYPE_BOOLEAN:
    g_value_set_boolean (value, atoi (buf));
    break;
  case G_TYPE_DOUBLE:
    g_value_set_double (value, atof (buf));
    break;
  case G_TYPE_STRING:
    g_value_set_string (value, g_strdup (buf));
    break;
  }

  xmlFree (buf);
  return TRUE;
}

xmlNodePtr
xml_gobject_serialize (GObject *gobject)
{
  xmlNodePtr root;
  GParamSpec **pspecs;
  guint n_pspecs, i;
  gboolean serialize_property = FALSE;
  gboolean get_root_name = FALSE;
  XmlSerializable *serializable;
  XmlSerializableInterface *iface;

  GObjectClass *class = G_OBJECT_GET_CLASS (gobject);
  if (XML_IS_SERIALIZABLE (gobject))
    {
      serializable = XML_SERIALIZABLE (gobject);
      iface = XML_SERIALIZABLE_GET_IFACE (serializable);
      serialize_property = (iface->serialize_property != NULL);
      get_root_name = (iface->get_root_name != NULL);
    }

  if (get_root_name)
    root = xmlNewNode (NULL, BAD_CAST xml_serializable_get_root_name (serializable));
  else
    root = xmlNewNode (NULL, BAD_CAST G_OBJECT_CLASS_NAME (class));

  pspecs = g_object_class_list_properties (class, &n_pspecs);


  for (i = 0; i < n_pspecs; i++)
    {
      GParamSpec *pspec = pspecs[i];
      GValue value = { 0, };
      xmlNodePtr child = NULL;

      if (!(pspec->flags & G_PARAM_READABLE))
        continue;

      g_value_init (&value, G_PARAM_SPEC_VALUE_TYPE (pspec));
      g_object_get_property (gobject, pspec->name, &value);

      if (serialize_property)
        child = xml_serializable_serialize_property (serializable, pspec->name, &value, pspec);
      else
        child = xml_serialize_pspec (pspec, &value);
      xmlAddChild (root, child);
    }

  return root;
}

GObject *
xml_gobject_deserialize (GType      type,
                         xmlNodePtr node)
{
#if GLIB_VERSION_2_54
  gint i = 0;
#else
  GArray *construct_params;
#endif
  gint n_children;
  GObjectClass *klass;
  klass = g_type_class_ref (type);
  xmlNodePtr cur;

  n_children = xmlChildElementCount (node);
#if GLIB_VERSION_2_54
  GValue values[n_children];
  const gchar *names[n_children];
#else
  construct_params = g_array_sized_new (FALSE, FALSE, sizeof (GParameter), n_children);
#endif
  for (cur = node->children; cur != NULL; cur = cur->next) {
    GParamSpec *pspec;

    pspec = g_object_class_find_property (klass, (gchar *)cur->name);
    if (!pspec)
      continue;

    if (!(pspec->flags & G_PARAM_WRITABLE))
      continue;

#if GLIB_VERSION_2_54
    g_value_init (&values[i], G_PARAM_SPEC_VALUE_TYPE (pspec));

    xml_deserialize_pspec (&values[i], pspec, cur);
    names[i] = g_strdup ((gchar *)cur->name);
    i += 1;
#else
    GParameter parameter = { NULL, };
    g_value_init (&parameter.value, G_PARAM_SPEC_VALUE_TYPE (pspec));

    xml_deserialize_pspec (&parameter.value, pspec, cur);
    parameter.name = g_strdup ((gchar *)cur->name);
    g_array_append_val (construct_params, parameter);
#endif

  }

#if GLIB_VERSION_2_54
  GObject *obj = g_object_new_with_properties (type, n_children, names, values);
#else
  GObject *obj = g_object_newv (type, n_children, (GParameter *)construct_params->data);
#endif

  g_type_class_unref (klass);
  return obj;
}

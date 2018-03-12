/* simple-gobject.c
 *
 * Copyright 2018 Günther Wutz <info@gunibert.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "simple-gobject.h"

struct _SimpleGobject
{
  GObject parent_instance;

  gint integer;
  char *string;
};

G_DEFINE_TYPE (SimpleGobject, simple_gobject, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_INT,
  PROP_STRING,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

SimpleGobject *
simple_gobject_new (int integer, char *string)
{
  return g_object_new (SIMPLE_TYPE_GOBJECT,
                       "integer", integer,
                       "string", string,
                       NULL);
}

static void
simple_gobject_finalize (GObject *object)
{
  SimpleGobject *self = (SimpleGobject *)object;

  g_free (self->string);

  G_OBJECT_CLASS (simple_gobject_parent_class)->finalize (object);
}

static void
simple_gobject_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  SimpleGobject *self = SIMPLE_GOBJECT (object);

  switch (prop_id)
    {
    case PROP_INT:
      g_value_set_int (value, self->integer);
      break;
    case PROP_STRING:
      g_value_set_string (value, self->string);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
simple_gobject_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  SimpleGobject *self = SIMPLE_GOBJECT (object);

  switch (prop_id)
    {
    case PROP_INT:
      self->integer = g_value_get_int (value);
      break;
    case PROP_STRING:
      g_free (self->string);
      self->string = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
simple_gobject_class_init (SimpleGobjectClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = simple_gobject_finalize;
  object_class->get_property = simple_gobject_get_property;
  object_class->set_property = simple_gobject_set_property;

  properties [PROP_INT] = g_param_spec_int ("integer",
                                            "Integer",
                                            "My super integer",
                                            0,
                                            100,
                                            10,
                                            G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);

  properties [PROP_STRING] = g_param_spec_string ("string",
                                                  "String",
                                                  "My super string",
                                                  "",
                                                  G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
simple_gobject_init (G_GNUC_UNUSED SimpleGobject *self)
{
}

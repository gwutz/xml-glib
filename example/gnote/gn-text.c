/* gn-text.c
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

#include "gn-text.h"

struct _GnText
{
  GObject parent_instance;

  gchar *notecontent;
};

G_DEFINE_TYPE (GnText, gn_text, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_NOTE_CONTENT,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

GnText *
gn_text_new (gchar *content)
{
  return g_object_new (GN_TYPE_TEXT,
                       "note-content", content,
                       NULL);
}

static void
gn_text_finalize (GObject *object)
{
  GnText *self = (GnText *)object;

  g_free (self->notecontent);

  G_OBJECT_CLASS (gn_text_parent_class)->finalize (object);
}

static void
gn_text_get_property (GObject    *object,
                      guint       prop_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
  GnText *self = GN_TEXT (object);

  switch (prop_id)
    {
    case PROP_NOTE_CONTENT:
      g_value_set_string (value, self->notecontent);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gn_text_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
  GnText *self = GN_TEXT (object);

  switch (prop_id)
    {
    case PROP_NOTE_CONTENT:
      g_free (self->notecontent);
      self->notecontent = g_value_dup_string (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gn_text_class_init (GnTextClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gn_text_finalize;
  object_class->get_property = gn_text_get_property;
  object_class->set_property = gn_text_set_property;

  properties [PROP_NOTE_CONTENT] =
    g_param_spec_string ("note-content",
                         "NoteContent",
                         "NoteContent",
                         "",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_NOTE_CONTENT,
                                   properties [PROP_NOTE_CONTENT]);
}

static void
gn_text_init (GnText *self)
{
}

gchar *
gn_text_get_content (GnText *self)
{
  return self->notecontent;
}

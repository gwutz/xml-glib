/* gn-note.c
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

#include "gn-note.h"
#include "gn-text.h"

struct _GnNote
{
  GObject parent_instance;

  gchar *title;
  GnText *text;
  gchar *last_change_date;
  gchar *last_metadata_change_date;
  gchar *create_date;

};

G_DEFINE_TYPE (GnNote, gn_note, G_TYPE_OBJECT)

enum {
  PROP_0,
  PROP_TITLE,
  PROP_TEXT,
  PROP_LAST_CHANGE_DATE,
  PROP_LAST_METADATA_CHANGE_DATE,
  PROP_CREATE_DATE,
  N_PROPS
};

static GParamSpec *properties [N_PROPS];

GnNote *
gn_note_new (void)
{
  return g_object_new (GN_TYPE_NOTE, NULL);
}

static void
gn_note_finalize (GObject *object)
{
  GnNote *self = (GnNote *)object;

  g_free (self->title);
  g_clear_object (&self->text);
  g_free (self->last_change_date);
  g_free (self->last_metadata_change_date);
  g_free (self->create_date);

  G_OBJECT_CLASS (gn_note_parent_class)->finalize (object);
}

static void
gn_note_get_property (GObject    *object,
                      guint       prop_id,
                      GValue     *value,
                      GParamSpec *pspec)
{
  GnNote *self = GN_NOTE (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      g_value_set_string (value, self->title);
      break;
    case PROP_TEXT:
      g_value_set_object (value, self->text);
      break;
    case PROP_LAST_CHANGE_DATE:
      g_value_set_string (value, self->last_change_date);
      break;
    case PROP_LAST_METADATA_CHANGE_DATE:
      g_value_set_string (value, self->last_metadata_change_date);
      break;
    case PROP_CREATE_DATE:
      g_value_set_string (value, self->create_date);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gn_note_set_property (GObject      *object,
                      guint         prop_id,
                      const GValue *value,
                      GParamSpec   *pspec)
{
  GnNote *self = GN_NOTE (object);

  switch (prop_id)
    {
    case PROP_TITLE:
      g_free (self->title);
      self->title = g_value_dup_string (value);
      break;
    case PROP_TEXT:
      g_clear_object (&self->text);
      self->text = g_value_get_object (value);
      break;
    case PROP_LAST_CHANGE_DATE:
      g_free (self->last_change_date);
      self->last_change_date = g_value_dup_string (value);
      break;
    case PROP_LAST_METADATA_CHANGE_DATE:
      g_free (self->last_metadata_change_date);
      self->last_metadata_change_date = g_value_dup_string (value);
      break;
    case PROP_CREATE_DATE:
      g_free (self->create_date);
      self->create_date = g_value_dup_string (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gn_note_class_init (GnNoteClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gn_note_finalize;
  object_class->get_property = gn_note_get_property;
  object_class->set_property = gn_note_set_property;

  properties [PROP_TITLE] =
    g_param_spec_string ("title",
                         "Title",
                         "Title",
                         "",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));
  properties [PROP_TEXT] =
    g_param_spec_object ("text",
                         "Text",
                         "Text",
                         GN_TYPE_TEXT,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_LAST_CHANGE_DATE] =
    g_param_spec_string ("last-change-date",
                         "LastChangeDate",
                         "LastChangeDate",
                         "",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_LAST_METADATA_CHANGE_DATE] =
    g_param_spec_string ("last-metadata-change-date",
                         "LastMetadataChangeDate",
                         "LastMetadataChangeDate",
                         "",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  properties [PROP_CREATE_DATE] =
    g_param_spec_string ("create-date",
                         "CreateDate",
                         "CreateDate",
                         "",
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));

  g_object_class_install_properties (object_class, N_PROPS, properties);
}

static void
gn_note_init (GnNote *self)
{
}

gchar *
gn_note_get_title (GnNote *self)
{
  return self->title;
}

gchar *
gn_note_get_text  (GnNote *self)
{
  return gn_text_get_content (self->text);
}

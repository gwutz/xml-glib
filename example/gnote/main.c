/* main.c
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
#include "xml-glib.h"
#include "gn-note.h"
#include <string.h>

gchar *notexml =
"<?xml version=\"1.0\"?>\n"
"<note version=\"0.3\" xmlns:link=\"http://beatniksoftware.com/tomboy/link\" xmlns:size=\"http://beatniksoftware.com/tomboy/size\" xmlns=\"http://beatniksoftware.com/tomboy\">\n"
"  <title>Neue Notizbuchvorlage</title>\n"
"  <text xml:space=\"preserve\">\n"
"    <note-content version=\"0.1\" xmlns:link=\"http://beatniksoftware.com/tomboy/link\" xmlns:size=\"http://beatniksoftware.com/tomboy/size\">Neue Notizbuchvorlage\n"
"      Beschreiben Sie ihre neue Notiz hier.\n"
"    </note-content>\n"
"  </text>\n"
"  <last-change-date>2017-11-16T21:34:41.979417Z</last-change-date>\n"
"  <last-metadata-change-date>2017-11-16T21:34:41.892018Z</last-metadata-change-date>\n"
"  <create-date>2017-11-06T18:07:02.374252Z</create-date>\n"
"</note>\n";

gint
main (gint argc, gchar *argv[])
{
  const gchar *entry;
  GError *error = NULL;
  g_autofree gchar *gnotedir;

  if (argc > 1) {
    gnotedir = g_strdup (argv[1]);
  } else {
    gnotedir = g_build_path ("/", g_get_home_dir (), ".local/share/gnote/", NULL);
  }

  GDir *dir = g_dir_open (gnotedir, 0, &error);
  if (error != NULL) {
    g_error ("%s", error->message);
  }

  for (entry = g_dir_read_name (dir); entry != NULL; entry = g_dir_read_name (dir))
    {
      if (!g_str_has_suffix (entry, ".note")) {
        continue;
      }

      g_autofree gchar *notefile = g_build_path ("/", gnotedir, entry, NULL);
      xmlDocPtr doc = xmlParseFile (notefile);
      xmlNodePtr node = xmlDocGetRootElement (doc);

      GObject *obj = xml_gobject_deserialize (GN_TYPE_NOTE, node);
      GnNote *note = GN_NOTE (obj);

      g_print ("-------------------------------\nTitle: %s\n\n%s\n", gn_note_get_title (note), gn_note_get_text (note));
    }

  g_dir_close (dir);

  return 0;
}

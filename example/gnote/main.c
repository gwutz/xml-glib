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
"</note>\n";

gint
main (gint argc, gchar *argv[])
{
  xmlDocPtr doc = xmlParseMemory (notexml, strlen (notexml));
  xmlNodePtr node = xmlDocGetRootElement (doc);

  GObject *obj = xml_gobject_deserialize (GN_TYPE_NOTE, node);
  GnNote *note = GN_NOTE (obj);
  return 0;
}

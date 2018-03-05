/* xml-address.h
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

G_BEGIN_DECLS

#define XML_TYPE_ADDRESS (xml_address_get_type())

G_DECLARE_FINAL_TYPE (XmlAddress, xml_address, XML, ADDRESS, GObject)

XmlAddress *xml_address_new (gint house_number, gchar *street_name, gchar *city);

G_END_DECLS

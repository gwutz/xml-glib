/* xml-gobject-private.h
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

#include "xml-gobject.h"

G_BEGIN_DECLS


xmlNodePtr xml_serialize_pspec (GParamSpec   *pspec,
                                const GValue *real_value);
gboolean xml_deserialize_pspec (GValue     *value,
                                GParamSpec *pspec,
                                xmlNodePtr  node);

G_END_DECLS

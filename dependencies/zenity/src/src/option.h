/*
 * option.h
 *
 * Copyright (C) 2002 Sun Microsystems, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Glynn Foster <glynn.foster@sun.com>
 *          Lucas Rocha <lucasr@im.ufba.br>
 */

#ifndef OPTION_H
#define OPTION_H

#include "zenity.h"
#include <glib.h>
#include <langinfo.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

typedef enum { MODE_ENTRY, MODE_LIST, MODE_LAST } ZenityDialogMode;

typedef enum {
	ERROR_SYNTAX,
	ERROR_SUPPORT,
	ERROR_DIALOG,
	ERROR_LAST
} ZenityError;

typedef struct {
	ZenityDialogMode mode;
	ZenityData *data;

	ZenityEntryData *entry_data;
	ZenityTreeData *tree_data;
} ZenityParsingOptions;

void zenity_option_error (gchar *string, ZenityError error);

ZenityParsingOptions *zenity_option_parse (gint argc, gchar **argv);

void zenity_option_free (void);

#endif /* OPTION_H */

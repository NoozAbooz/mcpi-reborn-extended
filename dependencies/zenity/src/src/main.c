/*
 * main.c
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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Glynn Foster <glynn.foster@sun.com>
 */

#include "option.h"
#include "zenity.h"

#include <glib.h>
#include <gtk/gtk.h>
#include <langinfo.h>
#include <stdlib.h>
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

gint
main (gint argc, gchar **argv) {
	ZenityParsingOptions *results;
	gint retval;

#ifdef HAVE_LOCALE_H
	setlocale (LC_ALL, "");
#endif

	gtk_init (&argc, &argv);

	results = zenity_option_parse (argc, argv);

	switch (results->mode) {
		case MODE_ENTRY:
			results->entry_data->data = (const gchar **) argv + 1;
			zenity_entry (results->data, results->entry_data);
			break;
		case MODE_LIST:
			results->tree_data->data = (const gchar **) argv + 1;
			zenity_tree (results->data, results->tree_data);
			break;
		case MODE_LAST:
			g_printerr ("You must specify a dialog type. See 'zenity "
						"--help' for details\n");
			zenity_option_free ();
			exit (-1);
		default:
			g_assert_not_reached ();
			zenity_option_free ();
			exit (-1);
	}

	retval = results->data->exit_code;

	zenity_option_free ();

	exit (retval);
}

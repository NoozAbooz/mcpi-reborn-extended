/*
 * util.c
 *
 * Copyright (C) 2002 Sun Microsystems, Inc.
 *           (C) 1999, 2000 Red Hat Inc.
 *           (C) 1998 James Henstridge
 *           (C) 1995-2002 Free Software Foundation
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
 *          Havoc Pennington <hp@redhat.com>
 *          James Henstridge <james@daa.com.au>
 *          Tom Tromey <tromey@redhat.com>
 */

#include "util.h"
#include "zenity.h"
#include <errno.h>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZENITY_OK_DEFAULT 0
#define ZENITY_CANCEL_DEFAULT 1
#define ZENITY_ESC_DEFAULT 1
#define ZENITY_ERROR_DEFAULT -1
#define ZENITY_EXTRA_DEFAULT 127

GtkBuilder *
zenity_util_load_ui_file (const gchar *root_widget, ...) {
	va_list args;
	gchar *arg = NULL;
	GPtrArray *ptrarray;
	GtkBuilder *builder = gtk_builder_new ();
	GError *error = NULL;
	gchar **objects;
	guint result = 0;

	/* We have at least the root_widget and a NULL */
	ptrarray = g_ptr_array_sized_new (2);

	g_ptr_array_add (ptrarray, g_strdup (root_widget));

	va_start (args, root_widget);

	arg = va_arg (args, gchar *);

	while (arg) {
		g_ptr_array_add (ptrarray, g_strdup (arg));
		arg = va_arg (args, gchar *);
	}
	va_end (args);

	/* Enforce terminating NULL */
	g_ptr_array_add (ptrarray, NULL);
	objects = (gchar **) g_ptr_array_free (ptrarray, FALSE);

	result = gtk_builder_add_objects_from_resource (
		builder, "/zenity.ui", objects, &error);

	g_strfreev (objects);

	if (result == 0) {
		g_warning ("Could not load ui data: %s", error->message);
		g_error_free (error);
		g_object_unref (builder);
		return NULL;
	}

	return builder;
}
gchar *
zenity_util_strip_newline (gchar *string) {
	gsize len;

	g_return_val_if_fail (string != NULL, NULL);

	len = strlen (string);
	while (len--) {
		if (string[len] == '\n')
			string[len] = '\0';
		else
			break;
	}

	return string;
}

gboolean
zenity_util_fill_file_buffer (GtkTextBuffer *buffer, const gchar *filename) {
	GtkTextIter iter, end;
	FILE *f;
	gchar buf[2048];
	gint remaining = 0;

	if (filename == NULL)
		return FALSE;

	f = fopen (filename, "r");

	if (f == NULL) {
		g_warning ("Cannot open file '%s': %s", filename, g_strerror (errno));
		return FALSE;
	}

	gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);

	while (!feof (f)) {
		gint count;
		const char *leftover;
		int to_read = 2047 - remaining;

		count = fread (buf + remaining, 1, to_read, f);
		buf[count + remaining] = '\0';

		g_utf8_validate (buf, count + remaining, &leftover);

		g_assert (g_utf8_validate (buf, leftover - buf, NULL));
		gtk_text_buffer_insert (buffer, &iter, buf, leftover - buf);

		remaining = (buf + remaining + count) - leftover;
		memmove (buf, leftover, remaining);

		if (remaining > 6 || count < to_read)
			break;
	}

	if (remaining) {
		g_warning (
			"Invalid UTF-8 data encountered reading file '%s'", filename);
		return FALSE;
	}

	/* We had a newline in the buffer to begin with. (The buffer always contains
	 * a newline, so we delete to the end of the buffer to clean up.
	 */

	gtk_text_buffer_get_end_iter (buffer, &end);
	gtk_text_buffer_delete (buffer, &iter, &end);

	gtk_text_buffer_set_modified (buffer, FALSE);

	return TRUE;
}

void
zenity_util_show_help (GError **error) {
	gchar *tmp;
	tmp = g_find_program_in_path ("yelp");

	if (tmp) {
		g_free (tmp);
		g_spawn_command_line_async ("yelp help:zenity", error);
	}
}

gint
zenity_util_return_exit_code (ZenityExitCode value) {

	const gchar *env_var = NULL;
	gint retval;

	switch (value) {

		case ZENITY_OK:
			env_var = g_getenv ("ZENITY_OK");
			if (!env_var)
				env_var = g_getenv ("DIALOG_OK");
			if (!env_var)
				retval = ZENITY_OK_DEFAULT;
			break;

		case ZENITY_CANCEL:
			env_var = g_getenv ("ZENITY_CANCEL");
			if (!env_var)
				env_var = g_getenv ("DIALOG_CANCEL");
			if (!env_var)
				retval = ZENITY_CANCEL_DEFAULT;
			break;

		case ZENITY_ESC:
			env_var = g_getenv ("ZENITY_ESC");
			if (!env_var)
				env_var = g_getenv ("DIALOG_ESC");
			if (!env_var)
				retval = ZENITY_ESC_DEFAULT;
			break;

		case ZENITY_EXTRA:
			env_var = g_getenv ("ZENITY_EXTRA");
			if (!env_var)
				env_var = g_getenv ("DIALOG_EXTRA");
			if (!env_var)
				retval = ZENITY_EXTRA_DEFAULT;
			break;

		case ZENITY_ERROR:
			env_var = g_getenv ("ZENITY_ERROR");
			if (!env_var)
				env_var = g_getenv ("DIALOG_ERROR");
			if (!env_var)
				retval = ZENITY_ERROR_DEFAULT;
			break;

		case ZENITY_TIMEOUT:
			env_var = g_getenv ("ZENITY_TIMEOUT");
			if (!env_var)
				env_var = g_getenv ("DIALOG_TIMEOUT");
			if (!env_var)
				retval = ZENITY_TIMEOUT;
			break;

		default:
			retval = 1;
	}

	if (env_var)
		retval = atoi (env_var);
	return retval;
}

void
zenity_util_exit_code_with_data (ZenityExitCode value, ZenityData *zen_data) {
	zen_data->exit_code = zenity_util_return_exit_code (value);
}

void
zenity_util_show_dialog (GtkWidget *dialog) {
	gtk_widget_realize (dialog);
	gtk_widget_show (dialog);
}

gboolean
zenity_util_timeout_handle (gpointer data) {
	GtkDialog *dialog = GTK_DIALOG (data);
	if (dialog != NULL)
		gtk_dialog_response (dialog, ZENITY_TIMEOUT);
	else {
		gtk_main_quit ();
		exit (ZENITY_TIMEOUT);
	}
	return FALSE;
}

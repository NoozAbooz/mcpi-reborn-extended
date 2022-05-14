/*
 * text.c
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

#include "util.h"
#include "zenity.h"
#include <gio/gio.h>

static ZenityTextData *zen_text_data;

static void zenity_text_dialog_response (
	GtkWidget *widget, int response, gpointer data);

static gboolean
zenity_text_scroll_to_end (gpointer user_data) {
	GtkTextIter iter;
	gtk_text_buffer_get_end_iter (
		gtk_text_view_get_buffer (GTK_TEXT_VIEW (user_data)), &iter);
	gtk_text_view_scroll_to_iter (
		GTK_TEXT_VIEW (user_data), &iter, 0.0, FALSE, 0, 0);
	return G_SOURCE_REMOVE;
}

static gboolean
zenity_text_handle_stdin (
	GIOChannel *channel, GIOCondition condition, gpointer data) {
	static GtkTextBuffer *buffer;
	static GtkTextView *text_view;
	gchar buf[1024];

	gsize len;

	text_view = GTK_TEXT_VIEW (data);
	buffer = gtk_text_view_get_buffer (text_view);

	if ((condition & G_IO_IN) || (condition & (G_IO_IN | G_IO_HUP))) {
		GError *error = NULL;
		gint status;

		while (channel->is_readable != TRUE)
			;

		do {
			status = g_io_channel_read_chars (channel, buf, 1024, &len, &error);

			while (gtk_events_pending ())
				gtk_main_iteration ();

		} while (status == G_IO_STATUS_AGAIN);

		if (status != G_IO_STATUS_NORMAL) {
			if (error) {
				g_warning ("zenity_text_handle_stdin () : %s", error->message);
				g_error_free (error);
				error = NULL;
			}
			return FALSE;
		}

		if (len > 0) {
			GtkTextIter end;
			gchar *utftext;
			gsize localelen;
			gsize utflen;

			gtk_text_buffer_get_end_iter (buffer, &end);

			if (!g_utf8_validate (buf, len, NULL)) {
				utftext = g_convert_with_fallback (buf,
					len,
					"UTF-8",
					"ISO-8859-1",
					NULL,
					&localelen,
					&utflen,
					NULL);
				gtk_text_buffer_insert (buffer, &end, utftext, utflen);
				g_free (utftext);
			} else {
				gtk_text_buffer_insert (buffer, &end, buf, len);
			}

			zenity_text_scroll_to_end (text_view);
		}
	}

	return TRUE;
}

static void
zenity_text_fill_entries_from_stdin (GtkTextView *text_view) {
	GIOChannel *channel;

	channel = g_io_channel_unix_new (0);
	g_io_channel_set_encoding (channel, "UTF-8", NULL);
	g_io_channel_set_flags (channel, G_IO_FLAG_NONBLOCK, NULL);
	g_io_add_watch (
		channel, G_IO_IN | G_IO_HUP, zenity_text_handle_stdin, text_view);
}

void
zenity_text (ZenityData *data, ZenityTextData *text_data) {
	GtkBuilder *builder;
	GtkWidget *dialog;
	GtkWidget *ok_button;

	GObject *text_view;
	GtkTextBuffer *text_buffer;

	GObject *text_label;

	zen_text_data = text_data;
	builder =
		zenity_util_load_ui_file ("zenity_text_dialog", "textbuffer1", NULL);

	if (builder == NULL) {
		data->exit_code = zenity_util_return_exit_code (ZENITY_ERROR);
		return;
	}

	gtk_builder_connect_signals (builder, NULL);

	dialog =
		GTK_WIDGET (gtk_builder_get_object (builder, "zenity_text_dialog"));

	ok_button = GTK_WIDGET (
		gtk_builder_get_object (builder, "zenity_text_close_button"));

	g_signal_connect (G_OBJECT (dialog),
		"response",
		G_CALLBACK (zenity_text_dialog_response),
		data);

	if (data->dialog_title)
		gtk_window_set_title (GTK_WINDOW (dialog), data->dialog_title);

	gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_CLOSE);

	text_buffer = gtk_text_buffer_new (NULL);
	text_view = gtk_builder_get_object (builder, "zenity_text_view");
	gtk_text_view_set_buffer (GTK_TEXT_VIEW (text_view), text_buffer);

	if (text_data->no_wrap)
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view), GTK_WRAP_NONE);

	G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	if (text_data->font) {
		PangoFontDescription *fontDesc =
			pango_font_description_from_string (text_data->font);
		gtk_widget_override_font (GTK_WIDGET (text_view), fontDesc);
	}
	G_GNUC_END_IGNORE_DEPRECATIONS

	if (text_data->uri) {
		zenity_util_fill_file_buffer (text_buffer, text_data->uri);
		g_idle_add (zenity_text_scroll_to_end, text_view);
	} else
		zenity_text_fill_entries_from_stdin (GTK_TEXT_VIEW (text_view));

	if (data->extra_label) {
		gint i = 0;
		while (data->extra_label[i] != NULL) {
			gtk_dialog_add_button (
				GTK_DIALOG (dialog), data->extra_label[i], i);
			i++;
		}
	}

	if (data->ok_label) {
		gtk_button_set_label (GTK_BUTTON (ok_button), data->ok_label);
	}

	if (data->width > -1 || data->height > -1)
		gtk_window_set_default_size (
			GTK_WINDOW (dialog), data->width, data->height);
	else
		gtk_window_set_default_size (GTK_WINDOW (dialog), 300, 400);

	if (data->modal)
		gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

	text_label = gtk_builder_get_object (builder, "zenity_text_label");

	if (text_data->dialog_text)
        gtk_label_set_markup (
			GTK_LABEL (text_label), g_strcompress (text_data->dialog_text));

	zenity_util_show_dialog (dialog);

	g_object_unref (builder);

	if (data->timeout_delay > 0) {
		g_timeout_add_seconds (data->timeout_delay,
			(GSourceFunc) zenity_util_timeout_handle,
			dialog);
	}

	gtk_main ();
}

static void
zenity_text_dialog_response (GtkWidget *widget, int response, gpointer data) {
	ZenityData *zen_data = data;

	switch (response) {
		case GTK_RESPONSE_CLOSE:
			zen_data->exit_code = zenity_util_return_exit_code (ZENITY_OK);
			break;

		case ZENITY_TIMEOUT:
			zen_data->exit_code = zenity_util_return_exit_code (ZENITY_TIMEOUT);
			break;

		default:
			if (zen_data->extra_label &&
				response < g_strv_length (zen_data->extra_label))
				printf ("%s\n", zen_data->extra_label[response]);
			zenity_util_exit_code_with_data (ZENITY_ESC, zen_data);
			break;
	}
	gtk_main_quit ();
}

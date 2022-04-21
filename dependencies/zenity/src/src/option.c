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
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Glynn Foster <glynn.foster@sun.com>
 *          Lucas Rocha <lucasr@im.ufba.br>
 */

#include "option.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* General Options */
static gchar *zenity_general_dialog_title;
static int zenity_general_width;
static int zenity_general_height;
static gchar *zenity_general_dialog_text;
static gchar *zenity_general_separator;
static gboolean zenity_general_multiple;
static gboolean zenity_general_editable;
static gchar *zenity_general_uri;
static gboolean zenity_general_dialog_no_wrap;
static gboolean zenity_general_dialog_no_markup;
static gint zenity_general_timeout_delay;
static gchar *zenity_general_ok_button;
static gchar *zenity_general_cancel_button;
static gchar **zenity_general_extra_buttons;
static gboolean zenity_general_modal;

/* Entry Dialog Options */
static gboolean zenity_entry_active;
static gchar *zenity_entry_entry_text;
static gboolean zenity_entry_hide_text;

/* List Dialog Options */
static gboolean zenity_list_active;
static gchar **zenity_list_columns;
static gboolean zenity_list_checklist;
static gboolean zenity_list_radiolist;
static gchar *zenity_list_print_column;
static gchar *zenity_list_hide_column;
static gboolean zenity_list_hide_header;
static gboolean zenity_list_imagelist;
static gboolean zenity_list_mid_search;

static GOptionEntry general_options[] = {{"title",
											 '\0',
											 0,
											 G_OPTION_ARG_STRING,
											 &zenity_general_dialog_title,
											 "Set the dialog title",
											 "TITLE"},
	{"width",
		'\0',
		0,
		G_OPTION_ARG_INT,
		&zenity_general_width,
		"Set the width",
		"WIDTH"},
	{"height",
		'\0',
		0,
		G_OPTION_ARG_INT,
		&zenity_general_height,
		"Set the height",
		"HEIGHT"},
	{"timeout",
		'\0',
		0,
		G_OPTION_ARG_INT,
		&zenity_general_timeout_delay,
		"Set dialog timeout in seconds",
		/* Timeout for closing the dialog */
		"TIMEOUT"},
	{"ok-label",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_STRING,
		&zenity_general_ok_button,
		"Set the label of the OK button",
		"TEXT"},
	{"cancel-label",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_STRING,
		&zenity_general_cancel_button,
		"Set the label of the Cancel button",
		"TEXT"},
	{"extra-button",
		'\0',
		0,
		G_OPTION_ARG_STRING_ARRAY,
		&zenity_general_extra_buttons,
		"Add an extra button",
		"TEXT"},
	{"modal",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_NONE,
		&zenity_general_modal,
		"Set the modal hint",
		NULL},
	{NULL}};

static GOptionEntry entry_options[] = {{"entry",
										   '\0',
										   G_OPTION_FLAG_IN_MAIN,
										   G_OPTION_ARG_NONE,
										   &zenity_entry_active,
										   "Display text entry dialog",
										   NULL},
	{"text",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_STRING,
		&zenity_general_dialog_text,
		"Set the dialog text",
		"TEXT"},
	{"entry-text",
		'\0',
		0,
		G_OPTION_ARG_STRING,
		&zenity_entry_entry_text,
		"Set the entry text",
		"TEXT"},
	{"hide-text",
		'\0',
		0,
		G_OPTION_ARG_NONE,
		&zenity_entry_hide_text,
		"Hide the entry text",
		NULL},
	{NULL}};

static GOptionEntry list_options[] = {{"list",
										  '\0',
										  G_OPTION_FLAG_IN_MAIN,
										  G_OPTION_ARG_NONE,
										  &zenity_list_active,
										  "Display list dialog",
										  NULL},
	{"text",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_STRING,
		&zenity_general_dialog_text,
		"Set the dialog text",
		"TEXT"},
	{"column",
		'\0',
		0,
		G_OPTION_ARG_STRING_ARRAY,
		&zenity_list_columns,
		"Set the column header",
		"COLUMN"},
	{"checklist",
		'\0',
		0,
		G_OPTION_ARG_NONE,
		&zenity_list_checklist,
		"Use check boxes for the first column",
		NULL},
	{"radiolist",
		'\0',
		0,
		G_OPTION_ARG_NONE,
		&zenity_list_radiolist,
		"Use radio buttons for the first column",
		NULL},
	{"imagelist",
		'\0',
		0,
		G_OPTION_ARG_NONE,
		&zenity_list_imagelist,
		"Use an image for the first column",
		NULL},
	{"separator",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_STRING,
		&zenity_general_separator,
		"Set output separator character",
		"SEPARATOR"},
	{"multiple",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_NONE,
		&zenity_general_multiple,
		"Allow multiple rows to be selected",
		NULL},
	{"editable",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_NONE,
		&zenity_general_editable,
		"Allow changes to text",
		NULL},
	{"print-column",
		'\0',
		0,
		G_OPTION_ARG_STRING,
		&zenity_list_print_column,
		"Print a specific column (Default is 1. 'ALL' can be used to print "
		"all columns)",
		/* Column index number to print out on a list dialog */
		"NUMBER"},
	{"hide-column",
		'\0',
		0,
		G_OPTION_ARG_STRING,
		&zenity_list_hide_column,
		"Hide a specific column",
		"NUMBER"},
	{"hide-header",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_NONE,
		&zenity_list_hide_header,
		"Hide the column headers",
		NULL},
	{"mid-search",
		'\0',
		G_OPTION_FLAG_NOALIAS,
		G_OPTION_ARG_NONE,
		&zenity_list_mid_search,
		"Change list default search function searching for text in the "
		"middle, not on the beginning",
		NULL},
	{NULL}};

static ZenityParsingOptions *results;
static GOptionContext *ctx;

static void
zenity_option_init (void) {

	results = g_new0 (ZenityParsingOptions, 1);

	/* Initialize the various dialog structures */
	results->mode = MODE_LAST;
	results->data = g_new0 (ZenityData, 1);
	results->entry_data = g_new0 (ZenityEntryData, 1);
	results->tree_data = g_new0 (ZenityTreeData, 1);
}

void
zenity_option_free (void) {
	if (zenity_general_dialog_title)
		g_free (zenity_general_dialog_title);
	if (zenity_general_dialog_text)
		g_free (zenity_general_dialog_text);
	if (zenity_general_uri)
		g_free (zenity_general_uri);
	g_free (zenity_general_separator);
	if (zenity_general_ok_button)
		g_free (zenity_general_ok_button);
	if (zenity_general_cancel_button)
		g_free (zenity_general_cancel_button);
	if (zenity_general_extra_buttons)
		g_strfreev (zenity_general_extra_buttons);

	if (zenity_entry_entry_text)
		g_free (zenity_entry_entry_text);

	if (zenity_list_columns)
		g_strfreev (zenity_list_columns);
	if (zenity_list_print_column)
		g_free (zenity_list_print_column);
	if (zenity_list_hide_column)
		g_free (zenity_list_hide_column);

	g_option_context_free (ctx);
}

static void
zenity_option_set_dialog_mode (gboolean is_active, ZenityDialogMode mode) {
	if (is_active == TRUE) {
		if (results->mode == MODE_LAST)
			results->mode = mode;
		else
			zenity_option_error (NULL, ERROR_DIALOG);
	}
}

static gchar *
zenity_option_get_name (GOptionEntry *entries, gpointer arg_data) {
	int i;

	for (i = 1; entries[i].long_name != NULL; i++) {
		if (entries[i].arg_data == arg_data)
			return (gchar *) entries[i].long_name;
	}
	return NULL;
}

/* Error callback */
static void
zenity_option_error_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	zenity_option_error (NULL, ERROR_SYNTAX);
}

/* Pre parse callbacks set the default option values */

static gboolean
zenity_general_pre_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	zenity_general_dialog_title = NULL;
	zenity_general_width = -1;
	zenity_general_height = -1;
	zenity_general_dialog_text = NULL;
	zenity_general_separator = g_strdup ("|");
	zenity_general_multiple = FALSE;
	zenity_general_editable = FALSE;
	zenity_general_uri = NULL;
	zenity_general_ok_button = NULL;
	zenity_general_cancel_button = NULL;
	zenity_general_extra_buttons = NULL;
	zenity_general_dialog_no_wrap = FALSE;
	zenity_general_dialog_no_markup = FALSE;
	zenity_general_timeout_delay = -1;
	zenity_general_modal = FALSE;

	return TRUE;
}

static gboolean
zenity_entry_pre_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	zenity_entry_active = FALSE;
	zenity_entry_entry_text = NULL;
	zenity_entry_hide_text = FALSE;

	return TRUE;
}

static gboolean
zenity_list_pre_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	zenity_list_active = FALSE;
	zenity_list_columns = NULL;
	zenity_list_checklist = FALSE;
	zenity_list_radiolist = FALSE;
	zenity_list_imagelist = FALSE;
	zenity_list_hide_header = FALSE;
	zenity_list_print_column = NULL;
	zenity_list_hide_column = NULL;
	zenity_list_mid_search = FALSE;

	return TRUE;
}

/* Post parse callbacks assign the option values to
   parsing result and makes some post condition tests */

static gboolean
zenity_general_post_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	results->data->dialog_title = zenity_general_dialog_title;
	results->data->width = zenity_general_width;
	results->data->height = zenity_general_height;
	results->data->timeout_delay = zenity_general_timeout_delay;
	results->data->ok_label = zenity_general_ok_button;
	results->data->cancel_label = zenity_general_cancel_button;
	results->data->extra_label = zenity_general_extra_buttons;
	results->data->modal = zenity_general_modal;

	return TRUE;
}

static gboolean
zenity_entry_post_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	zenity_option_set_dialog_mode (zenity_entry_active, MODE_ENTRY);

	if (results->mode == MODE_ENTRY) {
		results->entry_data->dialog_text = zenity_general_dialog_text;
		results->entry_data->entry_text = zenity_entry_entry_text;
		results->entry_data->hide_text = zenity_entry_hide_text;
	} else {
		if (zenity_entry_entry_text)
			zenity_option_error (zenity_option_get_name (
									 entry_options, &zenity_entry_entry_text),
				ERROR_SUPPORT);

		if (zenity_entry_hide_text)
			zenity_option_error (
				zenity_option_get_name (entry_options, &zenity_entry_hide_text),
				ERROR_SUPPORT);
	}

	return TRUE;
}

static gboolean
zenity_list_post_callback (GOptionContext *context, GOptionGroup *group,
	gpointer data, GError **error) {
	int i = 0;
	gchar *column;

	zenity_option_set_dialog_mode (zenity_list_active, MODE_LIST);

	if (results->mode == MODE_LIST) {
		results->tree_data->dialog_text = zenity_general_dialog_text;

		if (zenity_list_columns) {
			column = zenity_list_columns[0];
			while (column != NULL) {
				results->tree_data->columns =
					g_slist_append (results->tree_data->columns, column);
				column = zenity_list_columns[++i];
			}
		}

		results->tree_data->checkbox = zenity_list_checklist;
		results->tree_data->radiobox = zenity_list_radiolist;
		results->tree_data->imagebox = zenity_list_imagelist;
		results->tree_data->multi = zenity_general_multiple;
		results->tree_data->editable = zenity_general_editable;
		results->tree_data->print_column = zenity_list_print_column;
		results->tree_data->hide_column = zenity_list_hide_column;
		results->tree_data->hide_header = zenity_list_hide_header;
		results->tree_data->separator = zenity_general_separator;
		results->tree_data->mid_search = zenity_list_mid_search;
	} else {
		if (zenity_list_columns)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_columns),
				ERROR_SUPPORT);

		if (zenity_list_checklist)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_checklist),
				ERROR_SUPPORT);

		if (zenity_list_radiolist)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_radiolist),
				ERROR_SUPPORT);

		if (zenity_list_imagelist)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_imagelist),
				ERROR_SUPPORT);

		if (zenity_list_print_column)
			zenity_option_error (zenity_option_get_name (
									 list_options, &zenity_list_print_column),
				ERROR_SUPPORT);

		if (zenity_list_hide_column)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_hide_column),
				ERROR_SUPPORT);

		if (zenity_list_hide_header)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_hide_header),
				ERROR_SUPPORT);
		if (zenity_list_mid_search)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_list_mid_search),
				ERROR_SUPPORT);
	}

	return TRUE;
}

static GOptionContext *
zenity_create_context (void) {
	GOptionContext *tmp_ctx;
	GOptionGroup *a_group;

	tmp_ctx = g_option_context_new (NULL);

	/* Adds general option entries */
	a_group = g_option_group_new (
		"general", "General options", "Show general options", NULL, NULL);
	g_option_group_add_entries (a_group, general_options);
	g_option_group_set_parse_hooks (
		a_group, zenity_general_pre_callback, zenity_general_post_callback);
	g_option_group_set_error_hook (a_group, zenity_option_error_callback);
	g_option_context_add_group (tmp_ctx, a_group);

	/* Adds entry option entries */
	a_group = g_option_group_new (
		"entry", "Text entry options", "Show text entry options", NULL, NULL);
	g_option_group_add_entries (a_group, entry_options);
	g_option_group_set_parse_hooks (
		a_group, zenity_entry_pre_callback, zenity_entry_post_callback);
	g_option_group_set_error_hook (a_group, zenity_option_error_callback);
	g_option_context_add_group (tmp_ctx, a_group);

	/* Adds list option entries */
	a_group = g_option_group_new (
		"list", "List options", "Show list options", NULL, NULL);
	g_option_group_add_entries (a_group, list_options);
	g_option_group_set_parse_hooks (
		a_group, zenity_list_pre_callback, zenity_list_post_callback);
	g_option_group_set_error_hook (a_group, zenity_option_error_callback);
	g_option_context_add_group (tmp_ctx, a_group);

	/* Adds gtk option entries */
	a_group = gtk_get_option_group (TRUE);
	g_option_context_add_group (tmp_ctx, a_group);

	/* Enable help options */
	g_option_context_set_help_enabled (tmp_ctx, TRUE);
	g_option_context_set_ignore_unknown_options (tmp_ctx, FALSE);

	return tmp_ctx;
}

void
zenity_option_error (gchar *string, ZenityError error) {
	switch (error) {
		case ERROR_SYNTAX:
			g_printerr ("This option is not available. Please see --help "
						"for all possible usages.\n");
			zenity_option_free ();
			exit (-1);
		case ERROR_SUPPORT:
			g_printerr ("--%s is not supported for this dialog\n", string);
			zenity_option_free ();
			exit (-1);
		case ERROR_DIALOG:
			g_printerr ("Two or more dialog options specified\n");
			zenity_option_free ();
			exit (-1);
		default:
			return;
	}
}

ZenityParsingOptions *
zenity_option_parse (gint argc, gchar **argv) {
	GError *error = NULL;

	zenity_option_init ();

	ctx = zenity_create_context ();

	g_option_context_parse (ctx, &argc, &argv, &error);

	/* Some option pointer a shared among more than one group and don't
	   have their post condition tested. This test is done here. */

	if (strcmp (zenity_general_separator, "|") != 0)
		if (results->mode != MODE_LIST)
			zenity_option_error (zenity_option_get_name (
									 list_options, &zenity_general_separator),
				ERROR_SUPPORT);

	if (zenity_general_multiple)
		if (results->mode != MODE_LIST)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_general_multiple),
				ERROR_SUPPORT);

	if (zenity_general_editable)
		if (results->mode != MODE_LIST)
			zenity_option_error (
				zenity_option_get_name (list_options, &zenity_general_editable),
				ERROR_SUPPORT);

	return results;
}

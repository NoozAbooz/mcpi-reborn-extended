#ifndef UTIL_H
#define UTIL_H

#include "zenity.h"
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ZENITY_UI_FILE_FULLPATH ZENITY_DATADIR "/zenity.ui"
#define ZENITY_UI_FILE_RELATIVEPATH "./zenity.ui"

GtkBuilder *zenity_util_load_ui_file (
	const gchar *widget_root, ...) G_GNUC_NULL_TERMINATED;
gchar *zenity_util_strip_newline (gchar *string);
gboolean zenity_util_fill_file_buffer (
	GtkTextBuffer *buffer, const gchar *filename);
void zenity_util_show_help (GError **error);
gint zenity_util_return_exit_code (ZenityExitCode value);
void zenity_util_exit_code_with_data (ZenityExitCode value, ZenityData *data);
void zenity_util_show_dialog (GtkWidget *widget);

gboolean zenity_util_timeout_handle (gpointer data);

G_END_DECLS

#endif /* UTIL_H */

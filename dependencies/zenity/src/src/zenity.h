#ifndef ZENITY_H
#define ZENITY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct {
	gchar *dialog_title;
	gchar *ok_label;
	gchar *cancel_label;
	gchar **extra_label;
	gint width;
	gint height;
	gint exit_code;
	gint timeout_delay;
	gboolean modal;
	guintptr attach;
} ZenityData;

typedef enum {
	ZENITY_OK,
	ZENITY_CANCEL,
	ZENITY_ESC,
	ZENITY_ERROR,
	ZENITY_EXTRA,
	ZENITY_TIMEOUT
} ZenityExitCode;

typedef struct {
	gchar *dialog_text;
	gchar *entry_text;
	gboolean hide_text;
	gboolean only_numerical;
	const gchar **data;
} ZenityEntryData;

typedef struct {
	gchar *dialog_text;
	GSList *columns;
	gboolean checkbox;
	gboolean radiobox;
	gboolean hide_header;
	gboolean imagebox;
	gchar *separator;
	gboolean multi;
	gboolean editable;
	gboolean mid_search;
	gchar *print_column;
	gchar *hide_column;
	const gchar **data;
} ZenityTreeData;

void zenity_entry (ZenityData *data, ZenityEntryData *entry_data);
void zenity_tree (ZenityData *data, ZenityTreeData *tree_data);

G_END_DECLS

#endif /* ZENITY_H */

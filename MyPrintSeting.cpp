/*
 * MyPrintSeting.cpp
 *
 *  Created on: 21/11/2012
 *      Author: vrafael
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <locale.h>
//#include <math.h>
#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "MyPrintSeting.h"


void on_wbegin_print (GtkPrintOperation *operation,
	     GtkPrintContext   *context,
	     gpointer           user_data)
{

	gtk_print_operation_set_n_pages (operation,1);

}

void on_wend_print (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gpointer           user_data)
{

}


void on_wdraw_page (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gint               page_nr,
	   gpointer           user_data)
{

}

MyPrintSeting::MyPrintSeting()
{

}

MyPrintSeting::~MyPrintSeting()
{

}

void MyPrintSeting::w_Setprint(void* vxParent)
{
	GtkPrintOperation *operation;
	GtkPrintSettings *settings;
	gchar *dir, *ext;
	GError *error = NULL;


	operation = gtk_print_operation_new ();

	g_signal_connect (G_OBJECT (operation), "begin-print",
			G_CALLBACK (on_wbegin_print), this);
	g_signal_connect (G_OBJECT (operation), "draw-page",
			G_CALLBACK (on_wdraw_page), this);
	g_signal_connect (G_OBJECT (operation), "end-print",
			G_CALLBACK (on_wend_print), this);

	gtk_print_operation_set_use_full_page (operation, FALSE);
	gtk_print_operation_set_unit (operation, GTK_UNIT_POINTS);
	gtk_print_operation_set_embed_page_setup (operation, TRUE);

	settings = gtk_print_settings_new ();
	dir = (gchar*)g_get_user_special_dir (G_USER_DIRECTORY_DOCUMENTS);
	if (dir == NULL)
	{
		dir = (gchar*)g_get_home_dir ();
	}
	if (g_strcmp0 (gtk_print_settings_get (settings, GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT), "ps") == 0)
	{
		ext = ".ps";
	}
	else
	{
		if (g_strcmp0 (gtk_print_settings_get (settings, GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT), "svg") == 0)
		{
			ext = ".svg";
		}
		else
		{
			ext = ".pdf";
		}
	}

	char curi[1024];
	snprintf(curi,1024,"file://%s/PrintSeting%s",dir,ext);
	gtk_print_settings_set (settings, GTK_PRINT_SETTINGS_OUTPUT_URI, curi);
	gtk_print_operation_set_print_settings (operation, settings);

	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (vxParent), &error);
//	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT,NULL,NULL);

	g_object_unref (operation);
	g_object_unref (settings);

	if (error)
	{
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW (vxParent),
					   GTK_DIALOG_DESTROY_WITH_PARENT,
					   GTK_MESSAGE_ERROR,
					   GTK_BUTTONS_CLOSE,
					   "%s", error->message);
		g_error_free (error);

		g_signal_connect (dialog, "response",
			G_CALLBACK (gtk_widget_destroy), NULL);

		gtk_widget_show (dialog);
	}

}


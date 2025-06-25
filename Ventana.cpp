/*
 * Ventana.cpp
 *
 *  Created on: 15/09/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif

#include "Ventana.h"


gint on_deleteVentana(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	gtk_main_quit();
    return 1;
}

Ventana::Ventana()
{

}

Ventana::~Ventana()
{

}

void* Ventana::w_Create()
{

    GtkWidget* pWidget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    if(pWidget == NULL) return NULL;

//    GtkWindow* pwWindow =(GtkWindow*) pWidget;
    gtk_window_set_title((GtkWindow*)pWidget,"Gas");
//    gtk_window_set_default_size((GtkWindow*)pWidget,600,600);
    gtk_window_set_default_size((GtkWindow*)pWidget,850,600);
//    gtk_window_resize((GtkWindow*)pWidget,900,-1);
    gtk_window_move((GtkWindow*)pWidget,100,30);
//    gtk_window_maximize((GtkWindow*)pWidget);

    gtk_signal_connect (GTK_OBJECT (pWidget), "delete_event",
                        GTK_SIGNAL_FUNC (on_deleteVentana), this);

    gtk_widget_show(pWidget);

	return pWidget;
}

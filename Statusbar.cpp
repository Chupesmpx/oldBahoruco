/*
 * Statusbar.cpp
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

#include "Statusbar.h"



Statusbar::Statusbar()
{

}

Statusbar::~Statusbar()
{

}

void* Statusbar::w_Create()
{

	GtkWidget* pStatus = gtk_statusbar_new();
	voidStatusbar = pStatus;

    g_object_set_data(G_OBJECT(pStatus), "unMsg", (gpointer) "1");
    ggid = gtk_statusbar_get_context_id(GTK_STATUSBAR(pStatus), "unMsg");

    gtk_widget_show (pStatus);

	return pStatus;
}

void Statusbar::w_PushItem(char* lpxTexto)
{
	gtk_statusbar_push( GTK_STATUSBAR(voidStatusbar),(guint) &ggid,lpxTexto);
}

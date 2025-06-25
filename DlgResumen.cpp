/*
 * DlgResumen.cpp
 *
 *  Created on: 29/11/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
	#include <gdk/gdkkeysyms.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
	#include <gtk-2.0/gdk/gdkkeysyms.h>
#endif

#include "main.h"
#include "DlgResumen.h"

gboolean on_EventKeyDlgResumen(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgResumen* pDialogo = (DlgResumen*) data;

	if(event->keyval == GDK_Return)
	{
		if(widget == pDialogo->vInicial)
		{
			gtk_widget_grab_focus((GtkWidget*)pDialogo->vFinal);
			return 1;
		}

		if(widget == pDialogo->vFinal)
		{
			gtk_dialog_response((GtkDialog*)pDialogo->voidMyDlg,GTK_RESPONSE_OK);
			return 1;
		}

	}
	return 0;

}

DlgResumen::DlgResumen()
{

}

DlgResumen::~DlgResumen()
{

}

bool DlgResumen::w_Create(double* dxInicial,double* dxFinal)
{

	voidMyDlg = NULL;

	GtkWidget* pWidget =  gtk_dialog_new_with_buttons(
							"RESUMEN",
							(GtkWindow*)w_GetMain(),
							GTK_DIALOG_MODAL,
							GTK_STOCK_OK,GTK_RESPONSE_OK,
							GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
							NULL);
	voidMyDlg = pWidget;
	w_Controles();

//	gtk_widget_set_usize (pWidget,540,370);

	bool bRetorno = false;
	gint result = gtk_dialog_run(( GtkDialog*)pWidget);
	if(result == GTK_RESPONSE_OK)
	{
		char* lpInicial = (char*)gtk_entry_get_text((GtkEntry*)vInicial);
		char* lpFinal   = (char*)gtk_entry_get_text((GtkEntry*)vFinal);
		*dxInicial = atof(lpInicial);
		*dxFinal   = atof(lpFinal);
		bRetorno = true;
	}

	gtk_widget_destroy(pWidget);

	return bRetorno;
}

void DlgResumen::w_Controles()
{
	GtkWidget* pLabel1 = gtk_label_new("Lectura Inicial:");
	GtkWidget* pLabel2 = gtk_label_new("Lectura Final:");
	gtk_widget_set_size_request(pLabel1,105,-1);
	gtk_widget_set_size_request(pLabel2,105,-1);
	gtk_misc_set_alignment((GtkMisc*)pLabel1,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel2,0,0.5);
	gtk_widget_show (pLabel1);
	gtk_widget_show (pLabel2);

	GtkWidget* pEdit1 = gtk_entry_new();
	GtkWidget* pEdit2 = gtk_entry_new();
	gtk_entry_set_max_length((GtkEntry*)pEdit1,16);
	gtk_entry_set_max_length((GtkEntry*)pEdit2,16);
	vInicial = pEdit1;
	vFinal   = pEdit2;
	gtk_widget_set_size_request(pEdit1,100,-1);
	gtk_widget_set_size_request(pEdit2,100,-1);
	gtk_widget_show (pEdit1);
	gtk_widget_show (pEdit2);

	GtkWidget* pCajax= gtk_vbox_new (false, 4);

	GtkWidget* pCaja1 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pEdit1, false, false, 0);

	GtkWidget* pCaja2 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pLabel2, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pEdit2, false, false, 0);

	gtk_container_add (GTK_CONTAINER (pCajax), pCaja1);
	gtk_container_add (GTK_CONTAINER (pCajax), pCaja2);

	gtk_container_set_border_width( GTK_CONTAINER (pCajax), 20 );

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidMyDlg)->vbox), pCajax );

	gtk_widget_show (pCaja1);gtk_widget_show (pCajax);
	gtk_widget_show (pCaja2);

	gtk_signal_connect(GTK_OBJECT(vInicial),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyDlgResumen), this);
	gtk_signal_connect(GTK_OBJECT(vFinal),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyDlgResumen), this);

//	gtk_entry_set_text((GtkEntry*)vNameClte,"AL CONTADO");
}


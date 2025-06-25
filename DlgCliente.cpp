/*
 * DlgCliente.cpp
 *
 *  Created on: 28/11/2012
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
#include "DlgCliente.h"


gboolean on_EventKeyDlgCliente(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgCliente* pDialogo = (DlgCliente*) data;

	if(event->keyval == GDK_Return)
	{
		if(widget == pDialogo->vNameClte)
		{
			gtk_dialog_response((GtkDialog*)pDialogo->voidMyDlg,GTK_RESPONSE_OK);
			return 1;
		}

	}
	return 0;

}

DlgCliente::DlgCliente()
{

}

DlgCliente::~DlgCliente()
{

}

bool DlgCliente::w_Create(char* lpxCliente,long lxCliente)
{
	if(lpxCliente == NULL) return 0;
	memset(lpxCliente,0,lxCliente);

	voidMyDlg = NULL;

	GtkWidget* pWidget =  gtk_dialog_new_with_buttons(
							"FACTURA",
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
		char* buff = (char*)gtk_entry_get_text((GtkEntry*)vNameClte);
		long lwBuff = strlen(buff);
		if(lwBuff >= lxCliente) lwBuff = lxCliente - 1;
		memcpy(lpxCliente,buff,lwBuff);
		bRetorno = true;
	}

	gtk_widget_destroy(pWidget);

	return bRetorno;
}

void DlgCliente::w_Controles()
{
	GtkWidget* pLabel1 = gtk_label_new("Cliente:  ");
//	gtk_widget_set_size_request(pLabel1,80,-1);
	gtk_misc_set_alignment((GtkMisc*)pLabel1,0,0.5);
	gtk_widget_show (pLabel1);

	GtkWidget* pEdit1 = gtk_entry_new();
	gtk_entry_set_max_length((GtkEntry*)pEdit1,63);
	vNameClte = pEdit1;
	gtk_widget_set_size_request(pEdit1,230,-1);
	gtk_widget_show (pEdit1);

	GtkWidget* pCajax= gtk_vbox_new (false, 4);

	GtkWidget* pCaja1 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pEdit1, false, false, 0);

	gtk_container_add (GTK_CONTAINER (pCajax), pCaja1);

	gtk_container_set_border_width( GTK_CONTAINER (pCajax), 20 );

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidMyDlg)->vbox), pCajax );

	gtk_widget_show (pCaja1);gtk_widget_show (pCajax);

	gtk_signal_connect(GTK_OBJECT(vNameClte),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyDlgCliente), this);

	gtk_entry_set_text((GtkEntry*)vNameClte,"AL CONTADO");
}


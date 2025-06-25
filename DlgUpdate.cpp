/*
 * DlgUpdate.cpp
 *
 *  Created on: 12/11/2012
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
#include "DlgUpdate.h"
#include "MyListview.h"
#include "myworklv.h"
#include "MySave.h"
#include "CargarDet.h"

gboolean on_EventKeyCalcularGas(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgUpdate* pDialogo = (DlgUpdate*) data;
	pDialogo->w_CalcularGas();
	return 0;
}
gboolean on_EventKeyCalcularTotal(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgUpdate* pDialogo = (DlgUpdate*) data;
	pDialogo->w_CalcularTotal();
	return 0;
}
gboolean on_EventKeyCalcularEfectivo(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgUpdate* pDialogo = (DlgUpdate*) data;
	pDialogo->w_CalcularDevuelta();
	return 0;
}

gboolean on_EventKeyGas(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgUpdate* pDialogo = (DlgUpdate*) data;

//	#define GDK_Return 0xff0d
	//GDK_ENTER_NOTIFY


	if(event->keyval == GDK_Return)
	{
		if(widget == pDialogo->vGalones)
		{
			gtk_widget_grab_focus((GtkWidget*)pDialogo->vTotal);
			return 1;
		}
		if(widget == pDialogo->vTotal)
		{
			gtk_widget_grab_focus((GtkWidget*)pDialogo->vEfectivo);
			return 1;
		}
		if(widget == pDialogo->vEfectivo)
		{
			gtk_dialog_response((GtkDialog*)pDialogo->voidMyDlg,GTK_RESPONSE_OK);
			return 1;
		}

	}
	return 0;

}

DlgUpdate::DlgUpdate()
{

}

DlgUpdate::~DlgUpdate()
{

}

void DlgUpdate::w_Create(long lxTipo)
{
	voidMyDlg = NULL;
	gglTipo = lxTipo;

	MyListview* pView = (MyListview*) w_GetListview();

	MyWorkLV wxx;
	if(wxx.w_GetLineasLV(pView->pTreeTickets) < 1)
	{
		pView->w_CargaInicial();
	}
	else
	{
		char* buff1;char* buff2;
		if(w_GetLV(5,&buff1,&buff2))
		{
			if(strlen(buff1) > 1)
			{
//				CargarDet cargarDet;
//				cargarDet.w_Cargar();

				wxx.w_UnSelectPosLV(pView->pTreeDetalle,-1);

				pView->w_CargaInicial();
			}
			free(buff1);free(buff2);
		}
	}

	const char* lpwTitulo = "Adicionado";
//	if(lxAccion == 2) lpwTitulo = "Modificando";

	GtkWidget* pWidget =  gtk_dialog_new_with_buttons(
							lpwTitulo,
							(GtkWindow*)w_GetMain(),
							GTK_DIALOG_MODAL,
							GTK_STOCK_PRINT,GTK_RESPONSE_YES,
							GTK_STOCK_OK,GTK_RESPONSE_OK,
							GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
							NULL);
	voidMyDlg = pWidget;
	w_Controles(lxTipo);
	w_Cargar(lxTipo);

//	gtk_widget_set_usize (pWidget,540,370);

	gint result = gtk_dialog_run(( GtkDialog*)pWidget);
	if(result == GTK_RESPONSE_YES)
	{
		w_Save(true);
	}
	if(result == GTK_RESPONSE_OK)
	{
		w_Save(false);
	}


	gtk_widget_destroy(pWidget);

}

void DlgUpdate::w_Controles(long lxTipo)
{
	GtkWidget* pLabel1 = gtk_label_new("Galones:");
	GtkWidget* pLabel2 = gtk_label_new("Descripcion:");
//	GtkWidget* pLabel3 = gtk_label_new("Monto:");
	GtkWidget* pLabel4 = gtk_label_new("Total:");
	GtkWidget* pLabel5 = gtk_label_new("Efectivo:");
	GtkWidget* pLabel6 = gtk_label_new("      Devuelta:");

	gtk_widget_set_size_request(pLabel1,90,-1);
	gtk_widget_set_size_request(pLabel2,90,-1);
//	gtk_widget_set_size_request(pLabel3,90,-1);
	gtk_widget_set_size_request(pLabel4,90,-1);
	gtk_widget_set_size_request(pLabel5,90,-1);
	gtk_widget_set_size_request(pLabel6,90,-1);

	gtk_misc_set_alignment((GtkMisc*)pLabel1,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel2,0,0.5);
//	gtk_misc_set_alignment((GtkMisc*)pLabel3,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel4,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel5,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel6,0,0.5);

	gtk_widget_show (pLabel1);gtk_widget_show (pLabel2);
	gtk_widget_show (pLabel4);gtk_widget_show (pLabel5);gtk_widget_show (pLabel6);


	GtkWidget* pEdit1 = gtk_entry_new();
	GtkWidget* pEdit2 = gtk_combo_box_new_text();
//	GtkWidget* pEdit3 = gtk_entry_new();
	GtkWidget* pEdit4 = gtk_entry_new();
	GtkWidget* pEdit5 = gtk_entry_new();
	GtkWidget* pEdit6 = gtk_entry_new();

	gtk_entry_set_max_length((GtkEntry*)pEdit1,16);
//	gtk_entry_set_max_length((GtkEntry*)pEdit2,32);
//	gtk_entry_set_max_length((GtkEntry*)pEdit3,16);
	gtk_entry_set_max_length((GtkEntry*)pEdit4,16);
	gtk_entry_set_max_length((GtkEntry*)pEdit5,16);
	gtk_entry_set_max_length((GtkEntry*)pEdit6,16);

	vGalones  = pEdit1;
	vCombo    = pEdit2;
//	vMonto    = pEdit3;
	vTotal    = pEdit4;
	vEfectivo = pEdit5;
	vDevuelta = pEdit6;

	gtk_editable_set_editable((GtkEditable*)vDevuelta,0);
	if(lxTipo != 1)
	{
		gtk_widget_set_sensitive((GtkWidget*)vEfectivo,0);
		gtk_widget_set_sensitive((GtkWidget*)vDevuelta,0);
	}

	gtk_combo_box_append_text((GtkComboBox*)vCombo,"Contado");
	gtk_combo_box_append_text((GtkComboBox*)vCombo,"BonoGas");
	gtk_combo_box_append_text((GtkComboBox*)vCombo,"Tarjeta");

	gtk_combo_box_set_active((GtkComboBox*)vCombo,lxTipo-1);
	gtk_widget_set_sensitive((GtkWidget*)vCombo,0);


	gtk_widget_set_size_request(pEdit1,90,-1);
	gtk_widget_set_size_request(pEdit2,120,-1);
//	gtk_widget_set_size_request(pEdit3,90,-1);
	gtk_widget_set_size_request(pEdit4,90,-1);
	gtk_widget_set_size_request(pEdit5,90,-1);
	gtk_widget_set_size_request(pEdit6,90,-1);

//	GtkWidget* pBoton1 = gtk_button_new_with_label("...");
//	gtk_widget_set_size_request(pBoton1,25,-1);
//	gtk_widget_show (pBoton1);


	gtk_widget_show (pEdit1);gtk_widget_show (pEdit2);
	gtk_widget_show (pEdit4);gtk_widget_show (pEdit5);gtk_widget_show (pEdit6);

//	GtkWidget* pBoton = gtk_button_new_with_label("...");
//	gtk_widget_show (pBoton);

	GtkWidget* pSep1 = gtk_hseparator_new();
	gtk_widget_set_size_request(pSep1,-1,26);
	gtk_widget_show (pSep1);

	GtkWidget* pCajax= gtk_vbox_new (false, 4);

	GtkWidget* pCaja1 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pEdit1, false, false, 0);

	GtkWidget* pCaja2 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pLabel2, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pEdit2, false, false, 0);

//	GtkWidget* pCaja3 = gtk_hbox_new (false, 0);
//	gtk_box_pack_start (GTK_BOX (pCaja3), pSep1, false, false, 0);
//	gtk_box_pack_start (GTK_BOX (pCaja3), pLabel3, false, false, 0);
//	gtk_box_pack_start (GTK_BOX (pCaja3), pEdit3, false, false, 0);

	GtkWidget* pCaja4 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja4), pLabel4, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja4), pEdit4, false, false, 0);

	GtkWidget* pCaja5 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja5), pLabel5, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja5), pEdit5, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja5), pLabel6, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja5), pEdit6, false, false, 0);

	gtk_container_add (GTK_CONTAINER (pCajax), pCaja1);
	gtk_container_add (GTK_CONTAINER (pCajax), pCaja2);
//	gtk_container_add (GTK_CONTAINER (pCajax), pCaja3);
	gtk_container_add (GTK_CONTAINER (pCajax), pCaja4);
	gtk_box_pack_start (GTK_BOX (pCajax), pSep1, false, false, 0);

	gtk_container_add (GTK_CONTAINER (pCajax), pCaja5);

	gtk_container_set_border_width( GTK_CONTAINER (pCajax), 20 );

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidMyDlg)->vbox), pCajax );


	gtk_widget_show (pCaja1);gtk_widget_show (pCaja2);
	gtk_widget_show (pCaja4);gtk_widget_show (pCaja5);gtk_widget_show (pCajax);


//	gtk_signal_connect(GTK_OBJECT(voidPago),"changed", GTK_SIGNAL_FUNC(on_ChangeFormaFact), this);
//	gtk_signal_connect(GTK_OBJECT(pBoton),"clicked", GTK_SIGNAL_FUNC(on_BotonFechaFact), this);

	gtk_signal_connect(GTK_OBJECT(vGalones),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyGas), this);
	gtk_signal_connect(GTK_OBJECT(vTotal),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyGas), this);
	gtk_signal_connect(GTK_OBJECT(vEfectivo),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyGas), this);

	gtk_signal_connect(GTK_OBJECT(vGalones),"key-release-event", GTK_SIGNAL_FUNC(on_EventKeyCalcularGas), this);
	gtk_signal_connect(GTK_OBJECT(vTotal),"key-release-event", GTK_SIGNAL_FUNC(on_EventKeyCalcularTotal), this);
	gtk_signal_connect(GTK_OBJECT(vEfectivo),"key-release-event", GTK_SIGNAL_FUNC(on_EventKeyCalcularEfectivo), this);


}
void DlgUpdate::w_Cargar(long lxTipo)
{
	char* buff1 = NULL;
	char* buff2 = NULL;

	if(!w_GetLV(lxTipo-1,&buff1,&buff2)) return;
	gtk_entry_set_text((GtkEntry*)vGalones,buff1);
	gtk_entry_set_text((GtkEntry*)vTotal,buff2);
	free(buff1);free(buff2);buff1=NULL;buff2=NULL;

	if(!w_GetLV(4,&buff1,&buff2)) return;
	gtk_entry_set_text((GtkEntry*)vEfectivo,buff2);
	free(buff1);free(buff2);buff1=NULL;buff2=NULL;

	if(!w_GetLV(5,&buff1,&buff2)) return;
	gtk_entry_set_text((GtkEntry*)vDevuelta,buff2);
	free(buff1);free(buff2);buff1=NULL;buff2=NULL;
}
void DlgUpdate::w_CalcularGas()
{

	double dPrecio = w_GetPrecio();
	char* galones  = (char*)gtk_entry_get_text((GtkEntry*)vGalones);
	double dGalones = atof(galones);
	double dTotal   = dPrecio * dGalones;

	char cTotal[32];
	snprintf(cTotal,sizeof(cTotal),"%0.2f",dTotal);

	gtk_entry_set_text((GtkEntry*)vTotal,cTotal);

	if(gglTipo != 1) return;

	gtk_entry_set_text((GtkEntry*)vEfectivo,cTotal);
	gtk_entry_set_text((GtkEntry*)vDevuelta,"");
}

void DlgUpdate::w_CalcularTotal()
{

	double dPrecio  = w_GetPrecio();
	char* total     = (char*)gtk_entry_get_text((GtkEntry*)vTotal);
	double dTotal   = atof(total);
	double dGalones = dTotal / dPrecio;

	char cGalones[32];
	snprintf(cGalones,sizeof(cGalones),"%0.3f",dGalones);

	gtk_entry_set_text((GtkEntry*)vGalones,cGalones);

	if(gglTipo != 1) return;

	gtk_entry_set_text((GtkEntry*)vEfectivo,total);
	gtk_entry_set_text((GtkEntry*)vDevuelta,"");
}

void DlgUpdate::w_CalcularDevuelta()
{
	if(gglTipo != 1) return;

	char* total      = (char*)gtk_entry_get_text((GtkEntry*)vTotal);
	char* efectivo   = (char*)gtk_entry_get_text((GtkEntry*)vEfectivo);
	double dTotal    = atof(total);
	double dEfectivo = atof(efectivo);

	if(dTotal == dEfectivo)
	{
		gtk_entry_set_text((GtkEntry*)vDevuelta,"");
		return;
	}
	if(dTotal > dEfectivo)
	{
		gtk_entry_set_text((GtkEntry*)vDevuelta,"****");
		return;
	}

	double dDevuelta = dEfectivo - dTotal;

	char cDevuelta[32];
	snprintf(cDevuelta,sizeof(cDevuelta),"%0.2f",dDevuelta);

	gtk_entry_set_text((GtkEntry*)vDevuelta,cDevuelta);

}

void DlgUpdate::w_Save(bool bxPrint)
{
	char* galones  = (char*)gtk_entry_get_text((GtkEntry*)vGalones);
	char* total    = (char*)gtk_entry_get_text((GtkEntry*)vTotal);
	char* efectivo = (char*)gtk_entry_get_text((GtkEntry*)vEfectivo);
	char* cambio   = (char*)gtk_entry_get_text((GtkEntry*)vDevuelta);

	char*  wFecha  = w_GetFecha();
	double dPrecio = w_GetPrecio();

	long lTipo = gtk_combo_box_get_active((GtkComboBox*)vCombo);

	MyWorkLV wxx;
	char cGalones[32],cTotal[32],cEfectivo[32],cCambio[32];
	wxx.w_GetFormat(cGalones,32,atof(galones),3);
	wxx.w_GetFormat(cTotal,32,atof(total),2);
	wxx.w_GetFormat(cEfectivo,32,atof(efectivo),2);
	wxx.w_GetFormat(cCambio,32,atof(cambio),2);

	w_SetLV(lTipo,cGalones,cTotal);

	w_SetLV(4,"",cEfectivo);
	w_SetLV(5,"",cCambio);

	w_SumarTotal();

	if(bxPrint)
	{
		MySave sxx;
		sxx.w_Save();
	}
}

void DlgUpdate::w_SetLV(long lxLinea,char* buff1,char* buff2)
{

	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;


	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return;

	long k = 0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		if(k == lxLinea)
		{
			gtk_list_store_set ((GtkListStore*)pList, &iter,MyListview::TICK_GLS,buff1,MyListview::TICK_TOTAL,buff2,-1);
			break;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);

		k++;
	}


}

bool DlgUpdate::w_GetLV(long lxLinea,char** buff1,char** buff2)
{

	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;


	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return 0;

	long k = 0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		if(k == lxLinea)
		{
			gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,buff1,MyListview::TICK_TOTAL,buff2,-1);
			return 1;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);

		k++;
	}
	return 0;
}

void DlgUpdate::w_SumarTotal()
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return;

	MyWorkLV wxx;

	long k = 0;
	double dsGalones = 0.0;
	double dsTotal   = 0.0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		if(k < 3)
		{
			char* buffGalones;
			char* buffTotal;
			gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&buffGalones,MyListview::TICK_TOTAL,&buffTotal,-1);
			wxx.w_ParseDouble(buffGalones,-1);
			wxx.w_ParseDouble(buffTotal,-1);
			dsGalones += atof(buffGalones);
			dsTotal += atof(buffTotal);
			free(buffGalones);free(buffTotal);
		}

		if(k == 3)
		{
			char cGalones[32],cTotal[32];
			wxx.w_GetFormat(cGalones,32,dsGalones,3);
			wxx.w_GetFormat(cTotal,32,dsTotal,2);

			gtk_list_store_set ((GtkListStore*)pList, &iter,MyListview::TICK_GLS,cGalones,MyListview::TICK_TOTAL,cTotal,-1);
			break;
		}


		bAxx = gtk_tree_model_iter_next(pList,&iter);

		k++;
	}

}

void DlgUpdate::w_AddBonoGas(long lxCant)
{
	double dBono     = lxCant * w_GetBonogas();
	double dPrecio   = w_GetPrecio();
	double dGalones  = dBono / dPrecio;

	MyListview* pView = (MyListview*) w_GetListview();

	MyWorkLV wxx;
	if(wxx.w_GetLineasLV(pView->pTreeTickets) < 1)
	{
		pView->w_CargaInicial();
	}
	else
	{
		char* buff1;char* buff2;
		if(w_GetLV(5,&buff1,&buff2))
		{
			if(strlen(buff1) > 1)
			{
//				CargarDet cargarDet;
//				cargarDet.w_Cargar();

				wxx.w_UnSelectPosLV(pView->pTreeDetalle,-1);

				pView->w_CargaInicial();
			}
			free(buff1);free(buff2);
		}
	}


	char cGalones[32],cTotal[32];
	wxx.w_GetFormat(cGalones,32,dGalones,3);
	wxx.w_GetFormat(cTotal,32,dBono,2);

	w_SetLV(1,cGalones,cTotal);

	w_SumarTotal();
}

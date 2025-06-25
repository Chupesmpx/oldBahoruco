/*
 * DlgUpdate2.cpp
 *
 *  Created on: 16/11/2012
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
#include "DlgUpdate2.h"
#include "DlgUpdate.h"
#include "MyListview.h"
#include "myworklv.h"
#include "MySave.h"
#include "CargarDet.h"
#include "FileBonoGas.h"



gboolean on_EventKeyGas2(GtkWidget* widget,  GdkEventKey *event, gpointer data)
{
	DlgUpdate2* pDialogo = (DlgUpdate2*) data;
	pDialogo->w_Calcular(-1);
	return 0;

//	if(event->keyval == GDK_Return)
//	{
//		if(widget == pDialogo->vGalones)
//		{
//			pDialogo->w_Calcular(-1);
//			//gtk_dialog_response((GtkDialog*)pDialogo->voidMyDlg,GTK_RESPONSE_OK);
//			return 1;
//		}
//
//	}
//	return 0;

}

//void on_RadioDlg2(GtkButton *boton, gpointer data)
void on_RadioDlg2(GtkToggleButton *boton, gpointer data)
{
	DlgUpdate2* pDialogo = (DlgUpdate2*) data;

	if((void*)boton == pDialogo->voidRadio1) pDialogo->w_Calcular(1);
	if((void*)boton == pDialogo->voidRadio2) pDialogo->w_Calcular(2);
	if((void*)boton == pDialogo->voidRadio3) pDialogo->w_Calcular(3);
	if((void*)boton == pDialogo->voidRadio4) pDialogo->w_Calcular(4);

}


DlgUpdate2::DlgUpdate2()
{

}

DlgUpdate2::~DlgUpdate2()
{

}

void DlgUpdate2::w_Create()
{
	voidMyDlg = NULL;

	MyListview* pView = (MyListview*) w_GetListview();

	MyWorkLV wxx;
	if(wxx.w_GetLineasLV(pView->pTreeTickets) < 1)
	{
		pView->w_CargaInicial();
	}
	else
	{
		char* buff1;char* buff2;
		DlgUpdate dlgUpdate;
		if(dlgUpdate.w_GetLV(5,&buff1,&buff2))
		{
			if(strlen(buff1) > 1)
			{
				CargarDet cargarDet;
				cargarDet.w_Cargar();

				pView->w_CargaInicial();
			}
			free(buff1);free(buff2);
		}
	}

	const char* lpwTitulo = "Contado + bono gas";
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
	w_Controles();
//	w_Cargar(lxTipo);

//	gtk_widget_set_usize (pWidget,540,370);

	gint result = gtk_dialog_run(( GtkDialog*)pWidget);
	if(result == GTK_RESPONSE_YES)
	{
//		w_Save(true);
		gtk_widget_destroy(pWidget);
		return;
	}
	if(result == GTK_RESPONSE_OK)
	{
//		w_Save(false);
		gtk_widget_destroy(pWidget);
		return;
	}

	gtk_widget_destroy(pWidget);

	pView->w_CargaInicial();
}

void DlgUpdate2::w_Controles()
{
	GtkWidget* pLabel1  = gtk_label_new("Galones:");
	GtkWidget* pLabel11 = gtk_label_new(" Pesos");
	GtkWidget* pLabel2  = gtk_label_new("BonoGas:");
	GtkWidget* pLabel3  = gtk_label_new("");
	GtkWidget* pLabel4  = gtk_label_new("");


	gtk_widget_set_size_request(pLabel1,70,-1);
	gtk_widget_set_size_request(pLabel2,70,-1);
	gtk_widget_set_size_request(pLabel3,70,-1);
	gtk_widget_set_size_request(pLabel4,70,10);

	gtk_misc_set_alignment((GtkMisc*)pLabel1,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel11,0,0.5);
	gtk_misc_set_alignment((GtkMisc*)pLabel2,0,0.5);

	gtk_widget_show (pLabel1);gtk_widget_show (pLabel2);gtk_widget_show (pLabel3);
	gtk_widget_show (pLabel4);gtk_widget_show (pLabel11);


	GtkWidget* pEdit1  = gtk_entry_new();
	GtkWidget* pEdit2  = gtk_entry_new();

	gtk_entry_set_max_length((GtkEntry*)pEdit1,16);
	gtk_entry_set_max_length((GtkEntry*)pEdit2,16);

	vGalones  = pEdit1;
	vTotal    = pEdit2;

	gtk_editable_set_editable((GtkEditable*)vTotal,0);

	gtk_widget_set_size_request(pEdit1,90,-1);
	gtk_widget_set_size_request(pEdit2,90,-1);

	gtk_widget_show (pEdit1);gtk_widget_show (pEdit2);




	//////////////////////////////////////////////////////////////////////////////
	GtkWidget* pRadio1 = gtk_radio_button_new_with_label(NULL,"Un mes");
	GtkWidget* pRadio2 = gtk_radio_button_new_with_label(
						gtk_radio_button_get_group (GTK_RADIO_BUTTON (pRadio1)),
				        "Dos meses");
	GtkWidget* pRadio3 = gtk_radio_button_new_with_label(
						gtk_radio_button_get_group (GTK_RADIO_BUTTON (pRadio2)),
				        "Tres meses");

	GtkWidget* pRadio4 = gtk_radio_button_new_with_label(
						gtk_radio_button_get_group (GTK_RADIO_BUTTON (pRadio3)),
				        "Cuatro meses");

	voidRadio1 = pRadio1;
	voidRadio2 = pRadio2;
	voidRadio3 = pRadio3;
	voidRadio4 = pRadio4;

	gtk_widget_set_size_request(pRadio1,120,-1);
	gtk_widget_set_size_request(pRadio2,120,-1);
	gtk_widget_set_size_request(pRadio3,120,-1);
	gtk_widget_set_size_request(pRadio4,120,-1);

	gtk_widget_show (pRadio1);gtk_widget_show (pRadio2);gtk_widget_show (pRadio3);
	gtk_widget_show (pRadio4);
	//////////////////////////////////////////////////////////////////////////////

	GtkWidget* pSep1 = gtk_vseparator_new();
	gtk_widget_set_size_request(pSep1,30,-1);
	gtk_widget_show (pSep1);


	GtkWidget* pCajax= gtk_vbox_new (false, 4);

	GtkWidget* pCaja1 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pEdit1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pSep1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pEdit2, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel11, false, false, 0);

	GtkWidget* pCaja2 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pLabel2, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pRadio1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja2), pRadio2, false, false, 0);

	GtkWidget* pCaja3 = gtk_hbox_new (false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja3), pLabel3, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja3), pRadio3, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja3), pRadio4, false, false, 0);


	gtk_container_add (GTK_CONTAINER (pCajax), pCaja1);
	gtk_box_pack_start (GTK_BOX (pCajax), pLabel4, false, false, 0);

	gtk_container_add (GTK_CONTAINER (pCajax), pCaja2);
	gtk_container_add (GTK_CONTAINER (pCajax), pCaja3);

	gtk_container_set_border_width( GTK_CONTAINER (pCajax), 20 );

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidMyDlg)->vbox), pCajax );


	gtk_widget_show (pCaja1);gtk_widget_show (pCaja2);
	gtk_widget_show (pCaja3);gtk_widget_show (pCajax);



	gtk_signal_connect(GTK_OBJECT(vGalones),"key-press-event", GTK_SIGNAL_FUNC(on_EventKeyGas2), this);
	gtk_signal_connect(GTK_OBJECT(vGalones),"key-release-event", GTK_SIGNAL_FUNC(on_EventKeyGas2), this);

	gtk_signal_connect(GTK_OBJECT(pRadio1),"toggled", GTK_SIGNAL_FUNC(on_RadioDlg2), this);
	gtk_signal_connect(GTK_OBJECT(pRadio2),"toggled", GTK_SIGNAL_FUNC(on_RadioDlg2), this);
	gtk_signal_connect(GTK_OBJECT(pRadio3),"toggled", GTK_SIGNAL_FUNC(on_RadioDlg2), this);
	gtk_signal_connect(GTK_OBJECT(pRadio4),"toggled", GTK_SIGNAL_FUNC(on_RadioDlg2), this);

}

/*
bool DlgUpdate2::w_GetLV(long lxLinea,char** buff1,char** buff2)
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
*/
void DlgUpdate2::w_Calcular(int ixRadio)
{
	gtk_entry_set_text((GtkEntry*)vTotal,"");

	char* lpGalones = (char*)gtk_entry_get_text((GtkEntry*)vGalones);
	if(lpGalones == NULL) return;
	if(strlen(lpGalones) < 1) return;
	double dGalones = atof(lpGalones);

	double dBono   = 0.0;
	double dPrecio = w_GetPrecio();

	FileBonoGas fileBonoGas;
	if(!fileBonoGas.w_Read(&dBono)) return;

	int iCant = ixRadio;
	if(iCant == -1)
	{
		if(gtk_toggle_button_get_active((GtkToggleButton*)voidRadio1)) iCant = 1;
		if(gtk_toggle_button_get_active((GtkToggleButton*)voidRadio2)) iCant = 2;
		if(gtk_toggle_button_get_active((GtkToggleButton*)voidRadio3)) iCant = 3;
		if(gtk_toggle_button_get_active((GtkToggleButton*)voidRadio4)) iCant = 4;
	}

	double dGalsBono = (iCant * dBono) / dPrecio;
//	if(dGalsBono >= dGalones) return;

	double dDif   = dGalones - dGalsBono;
	double dTotal = dPrecio * dDif;

	if(dGalsBono >= dGalones) {dDif=dTotal=0.0;}

	char cTotal[32];
	snprintf(cTotal,32,"%0.2f",dTotal);

	gtk_entry_set_text((GtkEntry*)vTotal,cTotal);

	//////////////////////////////////////////////////////////////////

	MyWorkLV wxx;
	char cGalCont[32],cValCont[32];
	char cGalBono[32],cValBono[32];
	wxx.w_GetFormat(cGalCont,32,dDif,3);
	wxx.w_GetFormat(cValCont,32,dDif*dPrecio,2);

	wxx.w_GetFormat(cGalBono,32,dGalsBono,3);
	wxx.w_GetFormat(cValBono,32,iCant*dBono,2);


	DlgUpdate dlgUpdate;

	dlgUpdate.w_SetLV(0,cGalCont,cValCont);
	dlgUpdate.w_SetLV(1,cGalBono,cValBono);
	dlgUpdate.w_SetLV(4,"",cValCont);
	dlgUpdate.w_SetLV(5,"","");

	dlgUpdate.w_SumarTotal();

//	if(bxPrint)
//	{
//		MySave sxx;
//		sxx.w_Save();
//	}
}

/*
 * Toolbar.cpp
 *
 *  Created on: 15/09/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
//#include <gtk-2.0/gdk/gdkkeysyms.h>

#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "main.h"
#include "Toolbar.h"
#include "DlgFecha.h"
#include "MyFijo.h"
#include "myworklv.h"
#include "DlgUpdate.h"
#include "DlgUpdate2.h"
#include "MySave.h"
#include "MyListview.h"
#include "MyPrintTicket.h"
#include "MyPrintFact.h"



#define TOOL_MENU_EFECTIVO 1001
#define TOOL_MENU_CONTBONO 1002
#define TOOL_MENU_UNO      1003
#define TOOL_MENU_DOS      1004
#define TOOL_MENU_TRES     1005
#define TOOL_MENU_CUATRO   1006
#define TOOL_MENU_TARJETA  1007


gint on_ToolMenuBonoGas(GtkWidget *widget, GdkEvent  *event)
{
	if(event->type == GDK_ENTER_NOTIFY)
	{
		char* lpAxx = (char*)g_object_get_data((GObject*)widget,"Msg");
		if(lpAxx == NULL) return 0;

		w_SetStatusbar(lpAxx);
		return 0;
	}

	if(event->type == GDK_BUTTON_PRESS)
	{
		int iId = (int)g_object_get_data((GObject*)widget,"IdMsg");
		if(iId < 100) return 0;

		if(iId == TOOL_MENU_EFECTIVO)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_Create(1);
		}
		if(iId == TOOL_MENU_CONTBONO)
		{
			DlgUpdate2 dxx;
			dxx.w_Create();
		}

		if(iId == TOOL_MENU_UNO)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(1);
		}
		if(iId == TOOL_MENU_DOS)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(2);
		}
		if(iId == TOOL_MENU_TRES)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(3);
		}
		if(iId == TOOL_MENU_CUATRO)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(4);
		}
		if(iId == TOOL_MENU_TARJETA)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_Create(3);
		}

		return 1;
	}

	return 0;
}


void on_NewTicket(GtkWidget* widget, gpointer data)
{
//	MyListview* pView = (MyListview*) w_GetListview();
//	pView->w_CargaInicial();

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	MyWorkLV wxx;
	wxx.w_ClearTreeView(vTree);

	wxx.w_UnSelectPosLV(pView->pTreeDetalle,-1);

}

void on_CrearEfectivo(GtkWidget* widget, gpointer data)
{
	DlgUpdate dlgUpdate;
	dlgUpdate.w_Create(1);

}

void on_PrintTicket(GtkWidget* widget, gpointer data)
{
	MySave sxx;
	if(sxx.w_IsTicketSave())
	{
		MyPrintTicket myPrintTicket;
		myPrintTicket.w_Print();
	}
	else {sxx.w_Save();}
}

void on_PrintTicketFact(GtkWidget* widget, gpointer data)
{
	MySave sxx;
	if(sxx.w_IsTicketSave())
	{
		MyPrintTicket myPrintTicket;
		myPrintTicket.w_Print();
	}
	else {sxx.w_Save();}

	MyPrintFact myPrintFact;
	myPrintFact.w_Print(NULL);
}

void on_FechaTool(GtkWidget* widget, gpointer data)
{
	Toolbar* pTool = (Toolbar*) data;
	DlgFecha dlgFecha;
	dlgFecha.w_Create(w_GetMain(),pTool->voidEditFecha);
//	w_MessageBox_main("Fecha");
}

void on_SalirCatal(GtkWidget* widget, gpointer data)
{
	gtk_main_quit();
}

Toolbar::Toolbar()
{
	voidEditFecha  = NULL;
	voidEditPrecio = NULL;

}

Toolbar::~Toolbar()
{

}

void* Toolbar::w_Create()
{
	setlocale(LC_NUMERIC, "en_US");

	void* vCont = w_Create2();
	if(vCont == NULL) return NULL;

	char cFecha[32],cFechaEd[32],cPrecio[32];
	double dPrecio = 0.0;
	memset(cFecha,0,sizeof(cFecha));
	memset(cFechaEd,0,sizeof(cFechaEd));

	MyFijo myFijo;
	myFijo.w_GetFijo(cFecha,cFechaEd,&dPrecio);

	MyWorkLV wxx;
	wxx.w_GetFormat(cPrecio,32,dPrecio,2);
	gtk_entry_set_text((GtkEntry*)voidEditFecha,cFechaEd);
	gtk_entry_set_text((GtkEntry*)voidEditPrecio,cPrecio);

	w_SetFecha(cFecha);
	w_SetFechaEd(cFechaEd);
	w_SetPrecio();

	return vCont;
}

void* Toolbar::w_Create2()
{

	GtkWidget* pToolbar = gtk_toolbar_new();

	gtk_toolbar_append_space (GTK_TOOLBAR (pToolbar));

	w_AddItemToolbar_Stock(pToolbar,"New","Un ticket nuevo",GTK_STOCK_NEW,(void*)on_NewTicket);

	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));

	w_AddMenuItemToolbar(pToolbar);

	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));
	w_AddItemToolbar(pToolbar,"Print","Imprimir el ticket actual",(char*)"gtk-print.png",(void*)on_PrintTicket);
	w_AddItemToolbar(pToolbar,"+Factura","Imprimir el ticket actual + la factura",(char*)"gtk-print.png",(void*)on_PrintTicketFact);

	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));
	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));

	w_AddEditFechaItemToolbar(pToolbar);
	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));
	w_AddEditPrecioItemToolbar(pToolbar);

	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));
	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));


	w_AddItemToolbar_Stock(pToolbar,"Exit","salir",GTK_STOCK_QUIT,(void*)on_SalirCatal);

	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));
	gtk_toolbar_append_space(GTK_TOOLBAR (pToolbar));

	gtk_widget_show (pToolbar);
	return pToolbar;
}

void Toolbar::w_AddMenuItemToolbar(void* voidToolbar)
{

	// Menu //////////////////////////////////////////////////////////
	GtkWidget* pMenu = gtk_menu_new();

	GtkWidget* pImg = NULL;

	char cwFilename[1024];
	char* lpwDirectory = getenv("directorio");

	snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/contado.png",lpwDirectory);
	GtkWidget* pImg1 = gtk_image_new_from_file(cwFilename);

	snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/contado2.png",lpwDirectory);
	GtkWidget* pImg11 = gtk_image_new_from_file(cwFilename);

	snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/visa.png",lpwDirectory);
	GtkWidget* pImg6 = gtk_image_new_from_file(cwFilename);

	GtkWidget* pItem0 = gtk_image_menu_item_new_with_label("Continue");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem0,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem0);
	gtk_widget_show (pItem0);

	GtkWidget* pSep0 = gtk_separator_menu_item_new();
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pSep0);
	gtk_widget_show (pSep0);

	GtkWidget* pItem1 = gtk_image_menu_item_new_with_label("Contado");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem1,pImg1);
	gtk_image_menu_item_set_always_show_image((GtkImageMenuItem*)pItem1,TRUE);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem1);
	gtk_widget_show (pItem1);

	GtkWidget* pItem11 = gtk_image_menu_item_new_with_label("Contado+Bono");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem11,pImg11);
	gtk_image_menu_item_set_always_show_image((GtkImageMenuItem*)pItem11,TRUE);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem11);
	gtk_widget_show (pItem11);

	GtkWidget* pSep1 = gtk_separator_menu_item_new();
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pSep1);
	gtk_widget_show (pSep1);


	GtkWidget* pItem2 = gtk_image_menu_item_new_with_label("Un Mes");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem2,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem2);
	gtk_widget_show (pItem2);

	GtkWidget* pItem3 = gtk_image_menu_item_new_with_label("Dos Meses");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem3,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem3);
	gtk_widget_show (pItem3);

	GtkWidget* pItem4 = gtk_image_menu_item_new_with_label("Tres Meses");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem4,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem4);
	gtk_widget_show (pItem4);

	GtkWidget* pItem5 = gtk_image_menu_item_new_with_label("Cuatro Meses");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem5,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem5);
	gtk_widget_show (pItem5);

	GtkWidget* pSep2 = gtk_separator_menu_item_new();
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pSep2);
	gtk_widget_show (pSep2);


	GtkWidget* pItem6 = gtk_image_menu_item_new_with_label("Tarjeta");
	gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem6,pImg6);
	gtk_image_menu_item_set_always_show_image((GtkImageMenuItem*)pItem6,TRUE);
	gtk_menu_shell_append((GtkMenuShell*)pMenu, pItem6);
	gtk_widget_show (pItem6);

    g_signal_connect_swapped (G_OBJECT (pItem0), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem0));
    g_signal_connect_swapped (G_OBJECT (pItem1), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem1));
    g_signal_connect_swapped (G_OBJECT (pItem11), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem11));
    g_signal_connect_swapped (G_OBJECT (pItem2), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem2));
    g_signal_connect_swapped (G_OBJECT (pItem3), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem3));
    g_signal_connect_swapped (G_OBJECT (pItem4), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem4));
    g_signal_connect_swapped (G_OBJECT (pItem5), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem5));
    g_signal_connect_swapped (G_OBJECT (pItem6), "event",
                              G_CALLBACK (on_ToolMenuBonoGas),
                              G_OBJECT (pItem6));

	g_object_set_data((GObject*)pItem1,"IdMsg",(void*)TOOL_MENU_EFECTIVO);
	g_object_set_data((GObject*)pItem11,"IdMsg",(void*)TOOL_MENU_CONTBONO);
	g_object_set_data((GObject*)pItem2,"IdMsg",(void*)TOOL_MENU_UNO);
	g_object_set_data((GObject*)pItem3,"IdMsg",(void*)TOOL_MENU_DOS);
	g_object_set_data((GObject*)pItem4,"IdMsg",(void*)TOOL_MENU_TRES);
	g_object_set_data((GObject*)pItem5,"IdMsg",(void*)TOOL_MENU_CUATRO);
	g_object_set_data((GObject*)pItem6,"IdMsg",(void*)TOOL_MENU_TARJETA);
	g_object_set_data((GObject*)pItem0,"Msg",(void*)" ");
	g_object_set_data((GObject*)pItem1,"Msg",(void*)"Contado.");
	g_object_set_data((GObject*)pItem11,"Msg",(void*)"Contado mas Bono Gas.");
	g_object_set_data((GObject*)pItem2,"Msg",(void*)"Un Mes de bono gas.");
	g_object_set_data((GObject*)pItem3,"Msg",(void*)"Dos Meses de bono gas.");
	g_object_set_data((GObject*)pItem4,"Msg",(void*)"Tres Meses de bono gas.");
	g_object_set_data((GObject*)pItem5,"Msg",(void*)"Cuatro Meses de bono gas.");
	g_object_set_data((GObject*)pItem6,"Msg",(void*)"Con Tarjeta.");


	gtk_widget_show (pMenu);
	//Fin Menu /////////////////////////////////////////////////////////////


	char* lpDirectory = getenv("directorio");
	char cImagen[1024];
	snprintf(cImagen,sizeof(cImagen),"%s/Imagen/contado.png",lpDirectory);
	GtkWidget* pIcon = gtk_image_new_from_file (cImagen);
	GtkToolItem* pItem = gtk_menu_tool_button_new(pIcon,"Contado");

	gtk_menu_tool_button_set_menu((GtkMenuToolButton*)pItem,(GtkWidget*)pMenu);


	gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								 (GtkWidget*)pItem,
								 "Crear Ticket segun el tipo, efectivo, bonogas...",
								 NULL);

	gtk_signal_connect(GTK_OBJECT(pItem),"clicked", GTK_SIGNAL_FUNC(on_CrearEfectivo), this);

	gtk_widget_show ((GtkWidget*)pItem);



}

void Toolbar::w_AddEditFechaItemToolbar(void* voidToolbar)
{
	GtkWidget* pLabel  = gtk_label_new("  Fecha:");
	gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								pLabel,
								NULL,
								NULL);

	gtk_widget_show (pLabel);


	GtkWidget* entry = gtk_entry_new ();
	voidEditFecha = entry;
	gtk_widget_set_size_request(entry,120,-1);
	gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								 entry,
								 "Fecha del Ticket",
								 NULL);
	gtk_editable_set_editable((GtkEditable*)entry,0);
	gtk_widget_show (entry);


	 GtkWidget* pBoton  = gtk_button_new_with_label("...");
	 GtkWidget* pCajab = gtk_vbox_new (1, 0);
	 gtk_box_pack_start (GTK_BOX (pCajab), pBoton, 0, 0, 0);

	 gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								 pCajab,
								 "Modificar la fecha del ticket.",
								 NULL);
	 gtk_widget_show (pBoton);
	 gtk_widget_show (pCajab);

	 gtk_signal_connect(GTK_OBJECT(pBoton),"clicked", GTK_SIGNAL_FUNC(on_FechaTool), this);
}

void Toolbar::w_AddEditPrecioItemToolbar(void* voidToolbar)
{
	GtkWidget* pLabel  = gtk_label_new("  Precio:");
	gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								pLabel,
								NULL,
								NULL);

	gtk_widget_show (pLabel);


	GtkWidget* entry = gtk_entry_new ();
	voidEditPrecio = entry;
	gtk_widget_set_size_request(entry,80,-1);
	gtk_toolbar_append_widget (GTK_TOOLBAR (voidToolbar),
								 entry,
								 "Precio del dia.",
								 NULL);

	 gtk_widget_show (entry);

}

void Toolbar::w_AddItemToolbar(void* voidToolbar,char* lpxLabel,char* lpxTooltip,char* lpxImagen,void* voidxSignal)
{
	GtkWidget* iconw = NULL;
	GtkWidget* boton;
	GtkWidget* pItem;

	char* lpDirectory = getenv("directorio");

	if(lpxImagen != NULL)
	{
		char cImagen[1024];
		snprintf(cImagen,sizeof(cImagen),"%s/Imagen/%s",lpDirectory,lpxImagen);
		iconw = gtk_image_new_from_file (cImagen);

	}

	pItem = gtk_toolbar_append_item (GTK_TOOLBAR (voidToolbar),
							 lpxLabel,
							 lpxTooltip,
							 NULL,
							 iconw,
							 GTK_SIGNAL_FUNC (voidxSignal),
							 this);

	gtk_widget_show (pItem);
}

void Toolbar::w_AddItemToolbar_Stock(void* voidToolbar,char* lpxLabel,char* lpxTooltip,char* lpxStock,void* voidxSignal)
{
	GtkWidget* pItem;

	pItem = gtk_toolbar_insert_stock(GTK_TOOLBAR (voidToolbar),
							 lpxStock,
							 lpxLabel,
							 NULL,
							 GTK_SIGNAL_FUNC (voidxSignal),
							 this,
							 -1);

	gtk_widget_show (pItem);
}

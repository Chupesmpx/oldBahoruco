/*
 * MenuMain.cpp
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

#include "main.h"
#include "MenuMain.h"
#include "DlgBonogas.h"
#include "MyData.h"
#include "CargarDet.h"
#include "myworklv.h"
#include "MyListview.h"
#include "DlgUpdate.h"
#include "DlgUpdate2.h"
#include "DlgAcerca.h"
#include "MyPrintSeting.h"
#include "MyPrintTicket.h"
#include "MyPrintResum.h"
#include "MyPrintFact.h"
#include "MyPrintGls.h"
#include "DlgCliente.h"
#include "DlgResumen.h"


#define FILE_PRINT          1001
#define FILE_EXIT           1002
#define TOOLS_BONOGAS       1003
#define TOOLS_REFRESH       1004
#define TICKETS_NUEVO       1005
#define TICKETS_CONTADO     1006
#define TICKETS_UNMES       1007
#define TICKETS_DOSMESES    1008
#define TICKETS_TRESMESES   1009
#define TICKETS_CUATROMESES 1010
#define TICKETS_TARJETA     1011
#define RESUMEN_REFRESH     1012
#define DETALLE_ANULAR      1013
#define DETALLE_REFRESH     1014
#define HELP_ACERCA         1015
#define TICKETS_CONTBONO    1016
#define FILE_PRINT_TICKET   1017
#define FILE_PRINT_FACTURA  1018
#define FILE_PRINT_RESUMEN  1019
#define FILE_PRINT_LISTA    1020
#define RESUMEN_PRINT       1021

#define DETALLE_PRINT_TICKET  1022
#define DETALLE_PRINT_FACTURA 1023
#define FILE_PRINT_GALONES    1024
#define FILE_PRINT_VALORES    1025



gint on_MainMenu(GtkWidget *widget, GdkEvent  *event)
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

		if(iId == FILE_PRINT)
		{
			MyPrintSeting pxx;
			pxx.w_Setprint(w_GetMain());
		}
		if(iId == FILE_PRINT_TICKET)
		{
			MyPrintTicket pxx;
			pxx.w_Print();
		}
		if(iId == FILE_PRINT_FACTURA)
		{
			MyListview* pView = (MyListview*) w_GetListview();
			void* vTree = pView->pTreeDetalle;

			long lNulo;
			MyWorkLV wxx;
			if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_NULA,(int*)&lNulo,NULL,0))
			{
				w_MessageBox_main("Seleccione un ticket.");
				return 1;
			}

			char cCliente[64];
			DlgCliente dxx;
			if(dxx.w_Create(cCliente,sizeof(cCliente)))
			{
				MyPrintFact pxx;
				pxx.w_Print(cCliente);
			}
		}
		if(iId == FILE_PRINT_RESUMEN)
		{
//			double dInicial = 0.0;
//			double dFinal   = 0.0;
//			DlgResumen dxx;
//			if(dxx.w_Create(&dInicial,&dFinal))
//			{
				MyPrintResum pxx;
				pxx.w_Print();
//			}
		}
		if(iId == FILE_PRINT_GALONES)
		{
			MyPrintGls pxx;
			pxx.w_Print();
		}
		if(iId == FILE_PRINT_VALORES)
		{
			MyPrintGls pxx;
			pxx.w_Print(true);
		}

		if(iId == FILE_EXIT)
		{
			gtk_main_quit();
		}
		if(iId == TICKETS_NUEVO)
		{
			MyListview* pView = (MyListview*) w_GetListview();
			void* vTree = pView->pTreeTickets;

			MyWorkLV wxx;
			wxx.w_ClearTreeView(vTree);

			wxx.w_UnSelectPosLV(pView->pTreeDetalle,-1);

		}
		if(iId == TICKETS_CONTADO)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_Create(1);
		}
		if(iId == TICKETS_CONTBONO)
		{
			DlgUpdate2 dxx;
			dxx.w_Create();
		}
		if(iId == TICKETS_UNMES)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(1);
		}
		if(iId == TICKETS_DOSMESES)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(2);
		}
		if(iId == TICKETS_TRESMESES)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(3);
		}
		if(iId == TICKETS_CUATROMESES)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_AddBonoGas(4);
		}
		if(iId == TICKETS_TARJETA)
		{
			DlgUpdate dlgUpdate;
			dlgUpdate.w_Create(3);
		}
		if(iId == RESUMEN_PRINT)
		{
//			double dInicial = 0.0;
//			double dFinal   = 0.0;
//			DlgResumen dxx;
//			if(dxx.w_Create(&dInicial,&dFinal))
//			{
				MyPrintResum pxx;
				pxx.w_Print();
//			}
		}

		if(iId == RESUMEN_REFRESH)
		{
		}

		if(iId == DETALLE_PRINT_TICKET)
		{
			MyPrintTicket pxx;
			pxx.w_Print();
		}
		if(iId == DETALLE_PRINT_FACTURA)
		{
			MyPrintFact pxx;
			pxx.w_Print();
		}

		if(iId == DETALLE_ANULAR)
		{
			MyListview* pView = (MyListview*) w_GetListview();
			void* vTree = pView->pTreeDetalle;

			MyWorkLV wxx;
			long lPos = wxx.w_GetPosLV(vTree);
			if(lPos < 0) return 1;

			MyData dxx;
			dxx.w_Anular(lPos);

			CargarDet cxx;
			cxx.w_SetNula();
//			cxx.w_Cargar();
//			wxx.w_SelectPosLV(vTree,lPos);

		}
		if(iId == DETALLE_REFRESH)
		{
			CargarDet cargarDet;
			cargarDet.w_RefreshDetalle();
		}

		if(iId == TOOLS_BONOGAS)
		{
			DlgBonogas dlgBonogas;
			dlgBonogas.w_Create();
		}
		if(iId == TOOLS_REFRESH)
		{
			CargarDet cargarDet;
			cargarDet.w_RefreshDetalle();
		}
		if(iId == HELP_ACERCA)
		{
			DlgAcerca dxx;
			dxx.w_Create(w_GetMain());
		}

		return 1;
	}
	return 0;
}

MenuMain::MenuMain()
{
	vSubDetalle = NULL;
}

MenuMain::~MenuMain()
{

}

void* MenuMain::w_Create()
{

	GtkWidget* pMenu = gtk_menu_bar_new ();

	w_MenuFile(pMenu);
	w_MenuTickets(pMenu);
	w_MenuResumen(pMenu);
	w_MenuDetalle(pMenu);
	w_MenuTools(pMenu);
	w_MenuAyuda(pMenu);

	gtk_widget_show (pMenu);

	return pMenu;
}

bool MenuMain::w_MenuFile(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	w_SubMenuFile(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}

bool MenuMain::w_SubMenuFile(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"File",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"SetupPrint...",GTK_STOCK_PRINT,(char*)"Seleccionar el printer por defecto.",FILE_PRINT);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"PrintTicket",NULL,(char*)"Imprimir el Ticket seleccionado",FILE_PRINT_TICKET);
	w_CreateItem(voidSubMenu,(char*)"PrintFactura",NULL,(char*)"Imprimir la factura del ticket seleccionado.",FILE_PRINT_FACTURA);
	w_CreateItem(voidSubMenu,(char*)"PrintResumen",NULL,(char*)"Imprimir el resumen del dia.",FILE_PRINT_RESUMEN);
	w_CreateItem(voidSubMenu,(char*)"Print x Galon",NULL,(char*)"Imprimir lista por galones.",FILE_PRINT_GALONES);
	w_CreateItem(voidSubMenu,(char*)"Print x Valor",NULL,(char*)"Imprimir lista por valores.",FILE_PRINT_VALORES);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"Exit",NULL,(char*)"Salir",FILE_EXIT);

	return 1;
}

bool MenuMain::w_MenuTools(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("Tools");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	w_SubMenuTools(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}

bool MenuMain::w_SubMenuTools(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"Tools",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"BonoGas","bonogas.png",(char*)"Modificar Bono Gas",TOOLS_BONOGAS);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"Refresh",GTK_STOCK_REFRESH,(char*)"Refresh",TOOLS_REFRESH);

	return 1;
}

bool MenuMain::w_MenuTickets(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("Tickets");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	vSubTicket = pFileSub;
	w_SubMenuTickets(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}
bool MenuMain::w_SubMenuTickets(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"Tickets",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"New",GTK_STOCK_NEW,(char*)"Preparar para iniciar un nuevo ticket",TICKETS_NUEVO);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"Contado","contado.png",(char*)"Preparar un ticket al contado",TICKETS_CONTADO);
	w_CreateItem(voidSubMenu,(char*)"Contado+Bono","contado2.png",(char*)"Contado mas bono gas",TICKETS_CONTBONO);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"Un mes",NULL,(char*)"Preparar un ticket con un mes de bono gas",TICKETS_UNMES);
	w_CreateItem(voidSubMenu,(char*)"Dos meses",NULL,(char*)"Preparar un ticket con dos meses de bono gas",TICKETS_DOSMESES);
	w_CreateItem(voidSubMenu,(char*)"Tres meses",NULL,(char*)"Preparar un ticket con tres meses de bono gas",TICKETS_TRESMESES);
	w_CreateItem(voidSubMenu,(char*)"Cuatro meses",NULL,(char*)"Preparar un ticket con cuatro meses de bono gas",TICKETS_CUATROMESES);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"Tarjeta","visa.png",(char*)"Preparar un ticket con tarjeta",TICKETS_TARJETA);
//	w_CreateItem(voidSubMenu,(char*)"Tarjeta","visa2.xpm",(char*)"Preparar un ticket con tarjeta",TICKETS_TARJETA);

	return 1;
}

bool MenuMain::w_MenuResumen(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("Resumen");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	vSubResumen = pFileSub;
	w_SubMenuResumen(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}

bool MenuMain::w_SubMenuResumen(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"Resumen",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"print",GTK_STOCK_PRINT,(char*)"Imprimir el resumen",RESUMEN_PRINT);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"Refresh",GTK_STOCK_REFRESH,(char*)"Refresh resumen",RESUMEN_REFRESH);

	return 1;
}

bool MenuMain::w_MenuDetalle(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("Detalle");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	vSubDetalle = pFileSub;
	w_SubMenuDetalle(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}

bool MenuMain::w_SubMenuDetalle(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"Detalle",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"Anular/Activar",GTK_STOCK_CANCEL,(char*)"Anular y/o activar el ticket seleccionado",DETALLE_ANULAR);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"PrintTicket",GTK_STOCK_PRINT,(char*)"Imprimir el ticket seleccionado.",DETALLE_PRINT_TICKET);
	w_CreateItem2(voidSubMenu,(char*)"PrintFactura",NULL,(char*)"Imprimir la factura del ticket seleccionado",DETALLE_PRINT_FACTURA);
	w_CreateSep(voidSubMenu);
	w_CreateItem2(voidSubMenu,(char*)"Refresh",GTK_STOCK_REFRESH,(char*)"Refresh el detalle",DETALLE_REFRESH);

	return 1;
}

bool MenuMain::w_MenuAyuda(void* voidMenu)
{

	GtkWidget* pFile = gtk_menu_item_new_with_label("Help");
	gtk_menu_shell_append((GtkMenuShell*)voidMenu, pFile);

	GtkWidget* pFileSub = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(pFile), pFileSub);

	w_SubMenuAyuda(voidMenu,pFileSub);

	gtk_widget_show (pFile);
	gtk_widget_show (pFileSub);

	return 1;
}

bool MenuMain::w_SubMenuAyuda(void* voidMenu,void* voidSubMenu)
{
	w_CreateItem(voidSubMenu,(char*)"Help",NULL,(char*)"",0);
	w_CreateSep(voidSubMenu);
	w_CreateItem(voidSubMenu,(char*)"Acerca",NULL,(char*)"Acerca",HELP_ACERCA);

	return 1;
}

void MenuMain::w_CreateItem(void* voidSubMenu,char* lpxTitulo,char* lpxImagen,char* lpxMsg,int ixID)
{
	if(voidSubMenu == NULL || lpxTitulo == NULL) return;

	GtkWidget* pFileSub = (GtkWidget*) voidSubMenu;

	GtkWidget* pImg = NULL;
//	GdkPixbuf* pImg = NULL;

	GtkWidget* pItem = gtk_image_menu_item_new_with_label(lpxTitulo);
	if(lpxImagen != NULL)
	{
		char* lpwDirectory = getenv("directorio");
		if(lpwDirectory != NULL)
		{
			char cwFilename[1024];
			snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/%s",lpwDirectory,lpxImagen);

			pImg = gtk_image_new_from_file(cwFilename);
			gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem,pImg);
			gtk_image_menu_item_set_always_show_image((GtkImageMenuItem*)pItem,TRUE);

		}
	}
//	if(lpxImagen != NULL) gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pFileSub, pItem);

	gtk_widget_show (pItem);

    g_signal_connect_swapped (G_OBJECT (pItem), "event",
                              G_CALLBACK (on_MainMenu),
                              G_OBJECT (pItem));

	if(ixID > 0) g_object_set_data((GObject*)pItem,"IdMsg",(void*)ixID);
	g_object_set_data((GObject*)pItem,"Msg",(void*)lpxMsg);

}

void* MenuMain::w_CreateItem2(void* voidSubMenu,char* lpxTitulo,char* lpxImagen,char* lpxMsg,int ixID)
{
	if(voidSubMenu == NULL || lpxTitulo == NULL) return NULL;

	GtkWidget* pFileSub = (GtkWidget*) voidSubMenu;

	//char cFile[260];
	//snprintf(cFile,sizeof(cFile),"%s/Imagen/%s",w_GetDirectory(),lpxImagen);

	GtkWidget* pImg = gtk_image_new_from_stock(lpxImagen,GTK_ICON_SIZE_MENU);

	GtkWidget* pItem = gtk_image_menu_item_new_with_label(lpxTitulo);
	if(lpxImagen != NULL) gtk_image_menu_item_set_image((GtkImageMenuItem*)pItem,pImg);
	gtk_menu_shell_append((GtkMenuShell*)pFileSub, pItem);

	gtk_image_menu_item_set_always_show_image((GtkImageMenuItem*)pItem,TRUE);

	gtk_widget_show (pItem);

    g_signal_connect_swapped (G_OBJECT (pItem), "event",
                              G_CALLBACK (on_MainMenu),
                              G_OBJECT (pItem));

	if(ixID > 0) g_object_set_data((GObject*)pItem,"IdMsg",(void*)ixID);
	g_object_set_data((GObject*)pItem,"Msg",(void*)lpxMsg);

	return pItem;
}

void MenuMain::w_CreateSep(void* voidSubMenu)
{
	if(voidSubMenu == NULL) return;

	GtkWidget* pFileSub = (GtkWidget*) voidSubMenu;

	GtkWidget* pSep = gtk_separator_menu_item_new();
	gtk_menu_shell_append((GtkMenuShell*)pFileSub, pSep);

	gtk_widget_show (pSep);
}

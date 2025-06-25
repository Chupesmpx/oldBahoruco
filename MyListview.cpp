/*
 * MyListview.cpp
 *
 *  Created on: 11/11/2012
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
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif

#include "main.h"
#include "MyListview.h"
#include "CargarDet.h"
#include "MenuMain.h"
#include "myworklv.h"


void on_changed_Detalle(GtkWidget *widget, gpointer data)
{
	CargarDet dxx;
	dxx.w_CargarTicket();

}


gint on_menuListView(GtkWidget *widget, GdkEvent  *event)
{
	if(event->type == GDK_BUTTON_PRESS && event->button.button == 3)
	{
		gtk_widget_show_all(widget);
		gtk_menu_popup(GTK_MENU(widget), NULL, NULL, NULL, NULL,
	                event->button.button,
	                gdk_event_get_time((GdkEvent*)event));
	    return 1;
	}
	return 0;
}
/*
void w_onRowActivatedCtasFact (GtkTreeView *treeview,
                       GtkTreePath        *path,
                       GtkTreeViewColumn  *col,
                       gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   iter;

	//    g_print ("A row has been double-clicked!\n");

	model = gtk_tree_view_get_model(treeview);
	if (gtk_tree_model_get_iter(model, &iter, path))
	{
//		gchar *itemx;
//		gtk_tree_model_get(model, &iter, 1, &itemx, -1);

		Cuentas* pCtas = (Cuentas*) userdata;
		if(treeview == pCtas->pTreeCtas)
		{
			DlgUpdate1 dlgUpdate1;
			dlgUpdate1.w_Create(pCtas->pTreeCtas,2);
		}

		if(treeview == pCtas->pTreeDet)
		{
			DlgUpdate2 dlgUpdate;
			dlgUpdate.w_Create(pCtas->pTreeDet,2);
		}

//		g_free(itemx);

	}

}

gint on_KeyPressCtasFact(GtkWidget *widget,GdkEvent  *event, gpointer data)
{

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Insert)
	{
		DlgUpdate1 dlgUpdate1;
		dlgUpdate1.w_Create(widget,1);
		return 1;
	}

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Return)
	{
		DlgUpdate1 dlgUpdate1;
		dlgUpdate1.w_Create(widget,2);
		return 1;
	}

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Delete)
	{
		DlgUpdate1 dlgUpdate1;
		dlgUpdate1.w_Create(widget,3);
		return 1;
	}

	return 0;
}
gint on_KeyPressCtasDetFact(GtkWidget *widget,GdkEvent  *event, gpointer data)
{

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Insert)
	{
		DlgUpdate2 dlgUpdate;
		dlgUpdate.w_Create(widget,1);
		return 1;
	}

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Return)
	{
		DlgUpdate2 dlgUpdate;
		dlgUpdate.w_Create(widget,2);
		return 1;
	}

	if(event->type == GDK_KEY_PRESS && event->key.keyval == GDK_Delete)
	{
		DlgUpdate2 dlgUpdate;
		dlgUpdate.w_Create(widget,3);
		return 1;
	}

	return 0;
}
*/


MyListview::MyListview()
{

}

MyListview::~MyListview()
{

}

void* MyListview::w_Create()
{
	void* vScroll1 = w_Tickects();
	void* vScroll2 = w_Resumen();
	void* vScroll3 = w_Detalle();

	GtkWidget* pPanel1 = gtk_hpaned_new();
	gtk_paned_add1((GtkPaned*)pPanel1,(GtkWidget*)vScroll1);
	gtk_paned_add2((GtkPaned*)pPanel1,(GtkWidget*)vScroll2);


	GtkWidget* pPanel = gtk_vpaned_new();
	gtk_paned_add1((GtkPaned*)pPanel,(GtkWidget*)pPanel1);
	gtk_paned_add2((GtkPaned*)pPanel,(GtkWidget*)vScroll3);

//	gtk_paned_set_position((GtkPaned*)pPanel,170);
	gtk_paned_set_position((GtkPaned*)pPanel,180);
	gtk_paned_set_position((GtkPaned*)pPanel1,400);

	gtk_widget_show(pPanel1);
	gtk_widget_show(pPanel);

//	w_CargaInicial(pTreeTickets);
//	w_CargaInicial(pTreeResumen);

	CargarDet cargarDet;
	cargarDet.w_Cargar();

	MyWorkLV wxx;
	long lwPos = wxx.w_GetUltPosLV(pTreeDetalle);
	if(wxx.w_SelectPosLV(pTreeDetalle,lwPos))
	{
		wxx.w_UnSelectPosLV(pTreeDetalle,lwPos);
		wxx.w_ClearTreeView(pTreeTickets);
	}



	MenuMain* pMenu = (MenuMain*) w_GetMenu();

	g_signal_connect_swapped (G_OBJECT (pTreeTickets), "event",
							  G_CALLBACK (on_menuListView),
							  G_OBJECT (pMenu->vSubTicket));

	g_signal_connect_swapped (G_OBJECT (pTreeResumen), "event",
							  G_CALLBACK (on_menuListView),
							  G_OBJECT (pMenu->vSubResumen));

	g_signal_connect_swapped (G_OBJECT (pTreeDetalle), "event",
							  G_CALLBACK (on_menuListView),
							  G_OBJECT (pMenu->vSubDetalle));

//
//	g_signal_connect(pTreeCtas, "key-press-event",G_CALLBACK(on_KeyPressCtasFact), this);
//	g_signal_connect(pTreeDet, "key-press-event",G_CALLBACK(on_KeyPressCtasDetFact), this);


	return pPanel;

}

void* MyListview::w_Tickects()
{
	GtkWidget* pTree = gtk_tree_view_new();
	pTreeTickets = pTree;

	w_AddColumna(pTree,"Pos",TICK_POS,0,-10);
	w_AddColumna(pTree,"Gls",TICK_GLS,1,80);
	w_AddColumna(pTree,"Descripcion",TICK_DESCRIP,0,100);
	w_AddColumna(pTree,"Total",TICK_TOTAL,1,80);
	w_AddColumna(pTree," ",TICK_ULT,0,0);

	gtk_tree_view_set_rules_hint((GtkTreeView*)pTree,1);

	gtk_tree_view_set_headers_clickable ((GtkTreeView*)pTree,0);

	gtk_widget_show (pTree);

	GtkWidget* pScroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(pScroll),pTree);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)pScroll,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_widget_show (pScroll);

//	g_signal_connect(pTree, "row-activated", (GCallback) w_onRowActivatedCtasFact, this);
//
	GtkTreeSelection* pSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(pTree));
	gtk_tree_selection_set_mode(pSelect, GTK_SELECTION_NONE);
//	g_signal_connect(pSelect, "changed",G_CALLBACK(on_changed_CuentasFact), this);

	return pScroll;
}

void* MyListview::w_Resumen()
{
	GtkWidget* pTree = gtk_tree_view_new();
	pTreeResumen = pTree;

	w_AddColumna(pTree,"Pos",RESUMEN_POS,0,-10);
	w_AddColumna(pTree,"Gls",RESUMEN_GLS,1,100);
	w_AddColumna(pTree,"Descripcion",RESUMEN_DESCRIP,0,120);
	w_AddColumna(pTree,"Total",RESUMEN_TOTAL,1,100);
	w_AddColumna(pTree," ",RESUMEN_ULT,0,0);

	gtk_tree_view_set_rules_hint((GtkTreeView*)pTree,1);
	gtk_tree_view_set_headers_clickable ((GtkTreeView*)pTree,0);

	gtk_widget_show (pTree);

	GtkWidget* pScroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(pScroll),pTree);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)pScroll,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_widget_show (pScroll);

//	g_signal_connect(pTree, "row-activated", (GCallback) w_onRowActivatedCtasFact, this);
//
	GtkTreeSelection* pSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(pTree));
	gtk_tree_selection_set_mode(pSelect, GTK_SELECTION_NONE);
//	g_signal_connect(pSelect, "changed",G_CALLBACK(on_changed_CuentasFact), this);

	return pScroll;
}

void* MyListview::w_Detalle()
{
	GtkWidget* pTree = gtk_tree_view_new();
	pTreeDetalle = pTree;

	w_AddColumna(pTree,"Pos",DET_POS,0,-10);
	w_AddColumnaImag(pTree,DET_IMAGEN);
	w_AddColumna(pTree,"Ticket",DET_TICKET,1,70);
	w_AddColumna(pTree,"Fecha",DET_FECHA,0,110);
	w_AddColumna(pTree,"Hora",DET_HORA,0,85);
	w_AddColumna(pTree,"Gls",DET_GLS,1,75);
	w_AddColumna(pTree,"Precio",DET_PRECIO,1,75);
	w_AddColumna(pTree,"Total",DET_TOTAL,1,110);
	w_AddColumna(pTree,"Efectivo",DET_EFECTIVO,1,110);
	w_AddColumna(pTree,"Cambio",DET_CAMBIO,1,75);

	w_AddColumna(pTree,"Gls1",DET_GLS1,1,-10);
	w_AddColumna(pTree,"Gls2",DET_GLS2,1,-10);
	w_AddColumna(pTree,"Gls3",DET_GLS3,1,-10);
	w_AddColumna(pTree,"Total1",DET_TOTAL1,1,-10);
	w_AddColumna(pTree,"Total2",DET_TOTAL2,1,-10);
	w_AddColumna(pTree,"Total3",DET_TOTAL3,1,-10);

	w_AddColumna(pTree,"Nula",DET_NULA,0,-10);
	w_AddColumna(pTree,"Print",DET_PRINT,0,-10);

	w_AddColumna(pTree," ",DET_ULT,0,0);

	gtk_tree_view_set_rules_hint((GtkTreeView*)pTree,1);

	gtk_widget_show (pTree);

	GtkWidget* pScroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(pScroll),pTree);
	gtk_scrolled_window_set_policy((GtkScrolledWindow*)pScroll,GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_widget_show (pScroll);

//	g_signal_connect(pTree, "row-activated", (GCallback) w_onRowActivatedCtasFact, this);

	GtkTreeSelection* pSelect = gtk_tree_view_get_selection(GTK_TREE_VIEW(pTree));
	g_signal_connect(pSelect, "changed",G_CALLBACK(on_changed_Detalle), this);

	return pScroll;
}

void MyListview::w_AddColumna(void* voidxTree,const char* lpxTitulo,int ixSubItem,float fxAlign,int ixAncho)
{
	GtkCellRenderer*   renderer;
	GtkTreeViewColumn* pColumn;

	pColumn  = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_set_title(pColumn,lpxTitulo);
	gtk_tree_view_column_pack_start (pColumn, renderer, TRUE);
	gtk_tree_view_column_set_attributes (pColumn,renderer,"text",ixSubItem,NULL);
	gtk_tree_view_append_column ((GtkTreeView*)voidxTree, pColumn);


	if(ixAncho == -10) gtk_tree_view_column_set_visible(pColumn,0);
	if(ixAncho > 0) g_object_set( G_OBJECT( renderer ),"width", ixAncho ,NULL);


	gtk_tree_view_column_set_sort_column_id(pColumn,ixSubItem);

	gtk_tree_view_column_set_resizable(pColumn,ixSubItem);
	gtk_tree_view_column_set_alignment(pColumn,fxAlign);
	g_object_set( G_OBJECT( renderer ), "xalign", fxAlign ,NULL);

}
void MyListview::w_AddColumnaImag(void* voidxTree,int ixSubItem)
{
	GtkCellRenderer*   renderer;
	GtkTreeViewColumn* pColumn;

	pColumn  = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_set_title(pColumn,"");
	gtk_tree_view_column_pack_start (pColumn, renderer, FALSE);
	gtk_tree_view_column_set_attributes (pColumn,renderer,"pixbuf",ixSubItem,NULL);
	gtk_tree_view_append_column ((GtkTreeView*)voidxTree, pColumn);

}


void MyListview::w_CargaInicial()
{
	char* vxx[] =
	{
			"CONTADO",
			"BONO GAS",
			"TARJETA",
			"TOTALES",
			"EFECTIVO",
			"DEVUELTA"
	};
	int iCant = sizeof(vxx) / sizeof(vxx[0]);

	GtkListStore* pList = NULL;
	pList = gtk_list_store_new (5,	G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING);

	for(int i=0;i<iCant;i++)
	{
		GtkTreeIter iter;
		gtk_list_store_append (pList, &iter);
		gtk_list_store_set (pList, &iter,
							MyListview::TICK_POS,-1,
							MyListview::TICK_GLS,"",
							MyListview::TICK_DESCRIP,vxx[i],
							MyListview::TICK_TOTAL,"",
							MyListview::TICK_ULT,"",
		                    -1);

	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(pTreeTickets), (GtkTreeModel*)pList);
}


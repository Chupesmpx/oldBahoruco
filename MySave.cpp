/*
 * MySave.cpp
 *
 *  Created on: 13/11/2012
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
#include "MySave.h"
#include "MyListview.h"
#include "myworklv.h"
#include "MyData.h"
#include "CargarDet.h"
#include "MyPrintTicket.h"


MySave::MySave()
{

}

MySave::~MySave()
{

}

bool MySave::w_IsTotal()
{
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	char cTotal[32];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::TICK_TOTAL,NULL,cTotal,32)) return 0;

	if(strlen(cTotal) > 0) return 1;
	return 0;
}

bool MySave::w_IsTicketSave()
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	MyWorkLV wxx;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return 0;

	char* bfNoTicket;

	long k = 0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		if(k == 5)
		{
			gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&bfNoTicket,-1);
			break;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);

		k++;
	}
	if(k != 5) return 0;

	bool bRetorno = false;
	if(strlen(bfNoTicket) > 0) bRetorno = true;

	free(bfNoTicket);

	return bRetorno;

}

bool MySave::w_GetValores(	double* dGalon1,double* dGalon2,double* dGalon3,
							double* dTotal1,double* dTotal2,double* dTotal3,
							double* dEfectivo,double* dCambio)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	MyWorkLV wxx;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return 0;

	char* bfGalon1;char* bfGalon2;char* bfGalon3;
	char* bfTotal1;char* bfTotal2;char* bfTotal3;
	char* bfEfecivo;char* bfCambio;
	char* bfNoTicket;

	long k = 0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{

		if(k == 0) gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&bfGalon1,MyListview::TICK_TOTAL,&bfTotal1,-1);
		if(k == 1) gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&bfGalon2,MyListview::TICK_TOTAL,&bfTotal2,-1);
		if(k == 2) gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&bfGalon3,MyListview::TICK_TOTAL,&bfTotal3,-1);

		if(k == 4) gtk_tree_model_get(pList, &iter,MyListview::TICK_TOTAL,&bfEfecivo,-1);
		if(k == 5)
		{
			gtk_tree_model_get(pList, &iter,MyListview::TICK_GLS,&bfNoTicket,MyListview::TICK_TOTAL,&bfCambio,-1);
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);

		k++;
	}
	if(k < 1) return 0;

	wxx.w_ParseDouble(bfGalon1,-1);
	wxx.w_ParseDouble(bfGalon2,-1);
	wxx.w_ParseDouble(bfGalon3,-1);
	wxx.w_ParseDouble(bfTotal1,-1);
	wxx.w_ParseDouble(bfTotal2,-1);
	wxx.w_ParseDouble(bfTotal3,-1);
	wxx.w_ParseDouble(bfEfecivo,-1);
	wxx.w_ParseDouble(bfCambio,-1);

	*dGalon1 = atof(bfGalon1);
	*dGalon2 = atof(bfGalon2);
	*dGalon3 = atof(bfGalon3);

	*dTotal1 = atof(bfTotal1);
	*dTotal2 = atof(bfTotal2);
	*dTotal3 = atof(bfTotal3);

	*dEfectivo = atof(bfEfecivo);
	*dCambio   = atof(bfCambio);

	bool bRetorno = true;
	if(strlen(bfNoTicket) > 0) bRetorno = false;

	free(bfGalon1);free(bfGalon2);free(bfGalon3);
	free(bfTotal1);free(bfTotal2);free(bfTotal3);
	free(bfEfecivo);free(bfCambio);free(bfNoTicket);

	return bRetorno;

}
void MySave::w_Save()
{
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	MyWorkLV wxx;

	long   lFecha  = atol(w_GetFecha());
	double dPrecio = w_GetPrecio();

	double dGalon1,dGalon2,dGalon3;
	double dTotal1,dTotal2,dTotal3,dEfectivo,dCambio;
	if(!w_GetValores(&dGalon1,&dGalon2,&dGalon3,&dTotal1,&dTotal2,&dTotal3,&dEfectivo,&dCambio)) return;

	MyData myData;
	myData.w_Insert(lFecha,dPrecio,dEfectivo,dCambio,dGalon1,dGalon2,dGalon3,dTotal1,dTotal2,dTotal3);

	CargarDet cxx;
	cxx.w_Cargar();

	long lwPos = wxx.w_GetUltPosLV(pView->pTreeDetalle);
	wxx.w_SelectPosLV(pView->pTreeDetalle,lwPos);

	MyPrintTicket pxx;
	pxx.w_Print();


//	wxx.w_ClearTreeView(vTree);
}


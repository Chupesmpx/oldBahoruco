/*
 * CargarDet.cpp
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
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "main.h"
#include "CargarDet.h"
#include "MyListview.h"
#include "MyData.h"
#include "myworklv.h"
#include "myfecha.h"
#include "DlgUpdate.h"

CargarDet::CargarDet()
{
	buffData = NULL;
}

CargarDet::~CargarDet()
{
	w_Clear();
}

void CargarDet::w_Clear()
{
	if(buffData != NULL) free(buffData);buffData=NULL;
}

void CargarDet::w_Cargar()
{
	w_Clear();

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	MyWorkLV wxx;
	wxx.w_ClearTreeView(vTree);

	long lFecha = atol(w_GetFecha());

	MyData myData;
	if(!myData.w_Read(lFecha,&buffData)) return;
	if(buffData == NULL) return;

	long lCant,lRecord,lBuff;
	long p = 0;
	memcpy(&lRecord,buffData+p,4);p+=4;
	memcpy(&lCant,  buffData+p,4);p+=4;
	memcpy(&lBuff,  buffData+p,4);p+=4;

	GtkListStore* pList = NULL;
	pList = gtk_list_store_new (MyListview::DET_COLUMNS,
									G_TYPE_INT,GDK_TYPE_PIXBUF,   G_TYPE_INT,
									G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_INT,G_TYPE_INT,G_TYPE_STRING);

	long lCantTran = 0;
	double vGalones[5];
	double vTotales[5];
	double dsEfectivo = 0.00;
	double dsCambio   = 0.00;
	for(int i=0;i<5;i++){vTotales[i]=vGalones[i]=0.00;}

//	long pww = 0;
	for(int i=0;i<lCant;i++)
	{
		MyFecha myFecha;

		char cTiempo[32],cGalones[32],cTotal[32],cFecha[32],cPrecio[32];
		char cNula,cPrint;
		char cGalones1[32],cGalones2[32],cGalones3[32];
		char cTotal1[32],cTotal2[32],cTotal3[32];
		char cEfectivo[32],cCambio[32];
		char cHora,cMinutos,cSegundos;
		long lwwPos,lTicket,lFecha;
		double dGalones,dPrecio,dEfectivo,dTotal,dCambio;
		double dGalones1,dGalones2,dGalones3;
		double dTotal1,dTotal2,dTotal3;

		memcpy(&lwwPos, buffData+myData.gglPos+p,4);
		memcpy(&lTicket,buffData+myData.gglTicket+p,4);
		memcpy(&lFecha, buffData+myData.gglFecha+p,4);

		myFecha.w_GetFechaEd2(lFecha,cFecha,sizeof(cFecha));

		memcpy(&cNula,    buffData+myData.gglNula+p,1);
		memcpy(&cPrint,   buffData+myData.gglPrint+p,1);
		memcpy(&cHora,    buffData+myData.gglHora+p,1);
		memcpy(&cMinutos, buffData+myData.gglMinutos+p,1);
		memcpy(&cSegundos,buffData+myData.gglSegundos+p,1);
		snprintf(cTiempo,sizeof(cTiempo),"%02d:%02d:%02d",cHora,cMinutos,cSegundos);

		memcpy(&dPrecio,  buffData+myData.gglPrecio+p,8);
		memcpy(&dGalones1,buffData+myData.gglGalones1+p,8);
		memcpy(&dGalones2,buffData+myData.gglGalones2+p,8);
		memcpy(&dGalones3,buffData+myData.gglGalones3+p,8);
		memcpy(&dTotal1,  buffData+myData.gglTotal1+p,8);
		memcpy(&dTotal2,  buffData+myData.gglTotal2+p,8);
		memcpy(&dTotal3,  buffData+myData.gglTotal3+p,8);
		memcpy(&dEfectivo,buffData+myData.gglEfectivo+p,8);
		memcpy(&dCambio,  buffData+myData.gglCambio+p,8);

		wxx.w_GetFormat(cGalones1,32,dGalones1,3);
		wxx.w_GetFormat(cGalones2,32,dGalones2,3);
		wxx.w_GetFormat(cGalones3,32,dGalones3,3);

		wxx.w_GetFormat(cTotal1,32,dTotal1,2);
		wxx.w_GetFormat(cTotal2,32,dTotal2,2);
		wxx.w_GetFormat(cTotal3,32,dTotal3,2);

		dGalones = dGalones1 + dGalones2 + dGalones3;
		dTotal   = dTotal1 + dTotal2 + dTotal3;

		wxx.w_GetFormat(cGalones,32,dGalones,3);
		wxx.w_GetFormat(cPrecio,32,dPrecio,2);
		wxx.w_GetFormat(cTotal,32,dTotal,2);
		wxx.w_GetFormat(cEfectivo,32,dEfectivo,2);
		wxx.w_GetFormat(cCambio,32,dCambio,2);

		if(cNula < 1)
		{
			vGalones[1] += dGalones1;
			vGalones[2] += dGalones2;
			vGalones[3] += dGalones3;

			vTotales[1] += dTotal1;
			vTotales[2] += dTotal2;
			vTotales[3] += dTotal3;

			dsEfectivo += dEfectivo;
			dsCambio   += dCambio;
			lCantTran++;
		}

		GdkPixbuf* pIcon = NULL;
		if(cNula > 0)
		{
			char* lpwDirectory = getenv("directorio");
			if(lpwDirectory != NULL)
			{
				char cwFilename[1024];
				snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/nulo.png",lpwDirectory);
				pIcon = gdk_pixbuf_new_from_file(cwFilename,NULL);
			}
		}


		GtkTreeIter iter;
		gtk_list_store_append (pList, &iter);
		gtk_list_store_set (pList, &iter,
		                    MyListview::DET_POS,lwwPos,
		                    MyListview::DET_IMAGEN,pIcon,
		                    MyListview::DET_TICKET,lTicket,
		                    MyListview::DET_FECHA,cFecha,
		                    MyListview::DET_HORA,cTiempo,
		                    MyListview::DET_GLS,cGalones,
		                    MyListview::DET_PRECIO,cPrecio,
		                    MyListview::DET_TOTAL,cTotal,
		                    MyListview::DET_EFECTIVO,cEfectivo,
		                    MyListview::DET_CAMBIO,cCambio,

		                    MyListview::DET_GLS1,cGalones1,
		                    MyListview::DET_GLS2,cGalones2,
		                    MyListview::DET_GLS3,cGalones3,

		                    MyListview::DET_TOTAL1,cTotal1,
		                    MyListview::DET_TOTAL2,cTotal2,
		                    MyListview::DET_TOTAL3,cTotal3,

		                    MyListview::DET_NULA,cNula,
		                    MyListview::DET_PRINT,cPrint,

		                    MyListview::DET_ULT,"",
		                    -1);

		p+=lRecord;
//		pww+=lRecord;
	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(vTree), (GtkTreeModel*)pList);


	w_Resumen(lCantTran,vGalones,vTotales,dsEfectivo,dsCambio);

}

void CargarDet::w_Resumen(long lCantTran,double vGalones[5],double vTotales[5],double dsEfectivo,double dsCambio)
{
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeResumen;

	MyWorkLV wxx;

	GtkListStore* pList = gtk_list_store_new (MyListview::RESUMEN_COLUMNS,
									G_TYPE_INT,   G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING);
	char* vxx[] =
	{
			"",
			"CONTADO",
			"BONO GAS",
			"TARJETA",
			"TOTALES",
			"EFECTIVO",
			"DEVUELTA"
	};

	double dsTotal = 0.00;
	double dsGalones = 0.00;

	char cGalones[32],cTotal[32];

	GtkTreeIter iter;

	for(int i=1;i<4;i++)
	{
		dsGalones += vGalones[i];
		dsTotal   += vTotales[i];

		wxx.w_GetFormat(cGalones,32,vGalones[i],3);
		wxx.w_GetFormat(cTotal,32,vTotales[i],2);

		gtk_list_store_append (pList, &iter);
		gtk_list_store_set (pList, &iter,
							MyListview::RESUMEN_POS,-1,
							MyListview::RESUMEN_GLS,cGalones,
							MyListview::RESUMEN_DESCRIP,vxx[i],
							MyListview::RESUMEN_TOTAL,cTotal,
							MyListview::RESUMEN_ULT,"",
		                    -1);

	}

	//Totales
	wxx.w_GetFormat(cGalones,32,dsGalones,3);
	wxx.w_GetFormat(cTotal,32,dsTotal,2);

	gtk_list_store_append (pList, &iter);
	gtk_list_store_set (pList, &iter,
						MyListview::RESUMEN_POS,-1,
						MyListview::RESUMEN_GLS,cGalones,
						MyListview::RESUMEN_DESCRIP,vxx[4],
						MyListview::RESUMEN_TOTAL,cTotal,
						MyListview::RESUMEN_ULT,"",
	                    -1);

	//Efectivo
	wxx.w_GetFormat(cTotal,32,dsEfectivo,2);

	gtk_list_store_append (pList, &iter);
	gtk_list_store_set (pList, &iter,
						MyListview::RESUMEN_POS,-1,
						MyListview::RESUMEN_GLS,"",
						MyListview::RESUMEN_DESCRIP,vxx[5],
						MyListview::RESUMEN_TOTAL,cTotal,
						MyListview::RESUMEN_ULT,"",
	                    -1);

	//Cambio
	char cwCant[32];
	snprintf(cwCant,32,"%d Rec",lCantTran);
	wxx.w_GetFormat(cTotal,32,dsCambio,2);

	gtk_list_store_append (pList, &iter);
	gtk_list_store_set (pList, &iter,
						MyListview::RESUMEN_POS,-1,
						MyListview::RESUMEN_GLS,cwCant,
						MyListview::RESUMEN_DESCRIP,vxx[6],
						MyListview::RESUMEN_TOTAL,cTotal,
						MyListview::RESUMEN_ULT,"",
	                    -1);

	gtk_tree_view_set_model(GTK_TREE_VIEW(vTree), (GtkTreeModel*)pList);

}

void CargarDet::w_CargarTicket()
{

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree  = pView->pTreeTickets;
	void* vwTree = pView->pTreeDetalle;

	MyWorkLV wxx;
	if(wxx.w_GetPosLV(vwTree) < 0) return;

	GtkListStore* pList = gtk_list_store_new (MyListview::TICK_COLUMNS,
									G_TYPE_INT,   G_TYPE_STRING,G_TYPE_STRING,
									G_TYPE_STRING,G_TYPE_STRING);
	char* vxx[] =
	{
			"",
			"CONTADO",
			"BONO GAS",
			"TARJETA",
			"TOTALES",
			"EFECTIVO",
			"DEVUELTA"
	};


	int lTicket;
	char cGalones1[32],cGalones2[32],cGalones3[32];
	char cTotal1[32],cTotal2[32],cTotal3[32];
	char cEfectivo[32],cCambio[32];
	GtkTreeIter iter;

	wxx.w_GetItemTextLV(vwTree,MyListview::DET_TICKET,&lTicket,NULL,0);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_EFECTIVO,NULL,cEfectivo,32);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_CAMBIO,NULL,cCambio,32);

	wxx.w_GetItemTextLV(vwTree,MyListview::DET_GLS1,NULL, cGalones1,32);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_GLS2,NULL,cGalones2,32);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_GLS3,NULL,cGalones3,32);

	wxx.w_GetItemTextLV(vwTree,MyListview::DET_TOTAL1,NULL,cTotal1,32);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_TOTAL2,NULL,cTotal2,32);
	wxx.w_GetItemTextLV(vwTree,MyListview::DET_TOTAL3,NULL,cTotal3,32);

	char* vGalones[7];
	char* vTotal[7];
	for(int i=0;i<7;i++)
	{
		vGalones[i] = "";
		vTotal[i] = "";
	}

	vGalones[1] = cGalones1;
	vGalones[2] = cGalones2;
	vGalones[3] = cGalones3;
	vTotal[1]   = cTotal1;
	vTotal[2]   = cTotal2;
	vTotal[3]   = cTotal3;

	vTotal[5] = cEfectivo;
	vTotal[6] = cCambio;

	char cNoTicket[32];
	snprintf(cNoTicket,32,"No:%d  ",lTicket);
	vGalones[6] = cNoTicket;

	for(int i=1;i<7;i++)
	{
		gtk_list_store_append (pList, &iter);
		gtk_list_store_set (pList, &iter,
							MyListview::TICK_POS,-1,
							MyListview::TICK_GLS,vGalones[i],
							MyListview::TICK_DESCRIP,vxx[i],
							MyListview::TICK_TOTAL,vTotal[i],
							MyListview::TICK_ULT,"",
							-1);

	}

	gtk_tree_view_set_model(GTK_TREE_VIEW(vTree), (GtkTreeModel*)pList);

	DlgUpdate dlgUpdate;
	dlgUpdate.w_SumarTotal();

}

void CargarDet::w_RefreshDetalle()
{
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeResumen;

	MyWorkLV wxx;
	wxx.w_ClearTreeView(vTree);


	w_Cargar();

//	MyListview* pView = (MyListview*) w_GetListview();
	vTree = pView->pTreeTickets;

//	MyWorkLV wxx;
	wxx.w_ClearTreeView(vTree);

}

void CargarDet::w_SetPrint()
{
	GtkTreeIter       iter;
	GtkTreeModel*     pModel;
	GtkTreeSelection* pSelect;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	pSelect = gtk_tree_view_get_selection((GtkTreeView*)vTree);
	if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return;

	long lPrint = 0;
	gtk_tree_model_get(pModel, &iter,MyListview::DET_PRINT, &lPrint,  -1);
	lPrint++;
	gtk_list_store_set((GtkListStore*)pModel,&iter,MyListview::DET_PRINT,lPrint,-1);


}

void CargarDet::w_SetNula()
{
	GtkTreeIter       iter;
	GtkTreeModel*     pModel;
	GtkTreeSelection* pSelect;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	pSelect = gtk_tree_view_get_selection((GtkTreeView*)vTree);
	if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return;

	long lNula = 0;
	gtk_tree_model_get(pModel, &iter,MyListview::DET_NULA, &lNula,  -1);
	if(lNula == 1) {lNula = 0;}
	else {lNula = 1;}

	gtk_list_store_set((GtkListStore*)pModel,&iter,MyListview::DET_NULA,lNula,-1);

	if(lNula == 0)
	{
		gtk_list_store_set((GtkListStore*)pModel,&iter,MyListview::DET_IMAGEN,NULL,-1);
		w_RefreshResumen();
		return;
	}

	GdkPixbuf* pIcon = NULL;
	char* lpwDirectory = getenv("directorio");
	if(lpwDirectory != NULL)
	{
		char cwFilename[1024];
		snprintf(cwFilename,sizeof(cwFilename),"%s/Imagen/nulo.png",lpwDirectory);
		pIcon = gdk_pixbuf_new_from_file(cwFilename,NULL);

		gtk_list_store_set((GtkListStore*)pModel,&iter,MyListview::DET_IMAGEN,pIcon,-1);
	}

	w_RefreshResumen();

}

void CargarDet::w_RefreshResumen()
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return;

	long lCantTran = 0;
	double vGalones[5];
	double vTotales[5];
	double dsEfectivo = 0.00;
	double dsCambio   = 0.00;
	for(int i=0;i<5;i++){vTotales[i]=vGalones[i]=0.00;}

	MyWorkLV wxx;

	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		long lNula;
		char* lpGalones1;char* lpGalones2;char* lpGalones3;
		char* lpTotales1;char* lpTotales2;char* lpTotales3;
		char* lpEfectivo;char* lpCambio;
		gtk_tree_model_get(pList, &iter,
				MyListview::DET_NULA,&lNula,
				MyListview::DET_GLS1,&lpGalones1,
				MyListview::DET_GLS2,&lpGalones2,
				MyListview::DET_GLS3,&lpGalones3,
				MyListview::DET_TOTAL1,&lpTotales1,
				MyListview::DET_TOTAL2,&lpTotales2,
				MyListview::DET_TOTAL3,&lpTotales3,
				MyListview::DET_EFECTIVO,&lpEfectivo,
				MyListview::DET_CAMBIO,&lpCambio,
				-1);

		if(lNula == 1)
		{
			bAxx = gtk_tree_model_iter_next(pList,&iter);
			continue;
		}

		wxx.w_ParseDouble(lpGalones1,-1);wxx.w_ParseDouble(lpGalones2,-1);wxx.w_ParseDouble(lpGalones3,-1);
		wxx.w_ParseDouble(lpTotales1,-1);wxx.w_ParseDouble(lpTotales2,-1);wxx.w_ParseDouble(lpTotales3,-1);
		wxx.w_ParseDouble(lpEfectivo,-1);wxx.w_ParseDouble(lpCambio,-1);

		lCantTran++;

		vGalones[1] += atof(lpGalones1);
		vGalones[2] += atof(lpGalones2);
		vGalones[3] += atof(lpGalones3);

		vTotales[1] += atof(lpTotales1);
		vTotales[2] += atof(lpTotales2);
		vTotales[3] += atof(lpTotales3);

		dsEfectivo += atof(lpEfectivo);
		dsCambio   += atof(lpCambio);

		free(lpGalones1);free(lpGalones2);free(lpGalones3);
		free(lpTotales1);free(lpTotales2);free(lpTotales3);
		free(lpEfectivo);free(lpCambio);

		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}

	w_Resumen(lCantTran,vGalones,vTotales,dsEfectivo,dsCambio);
}

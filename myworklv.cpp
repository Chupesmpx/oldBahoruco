//      myworklv.cpp
//      

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //<---aqui se encuentran las funciones toupper y tolower

#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "myworklv.h"


MyWorkLV::MyWorkLV()
{
	
}


MyWorkLV::~MyWorkLV()
{
	
}

void MyWorkLV::w_Upper(char* lpxString,bool bxUpper)
{
//	if(bxUpper) return g_ascii_strup(lpString,strlen(lpString));
//	return g_ascii_strdown(lpString,strlen(lpString));

	if(lpxString == NULL) return;
	long lString = strlen(lpxString);

	char* lpwString = (char*)malloc(lString+1);
	if(lpwString == NULL) return;
	memset(lpwString,0,lString+1);

	for(int i=0;i<lString;i++)
	{
		char cAxx[2];memset(cAxx,0,2);
		memcpy(cAxx,lpxString+i,1);
		if(bxUpper) {cAxx[0] = toupper(cAxx[0]);}
		else {cAxx[0] = tolower(cAxx[0]);}
		memcpy(lpwString+i,cAxx,1);

	}
	memcpy(lpxString,lpwString,lString);
	free(lpwString);

}

void MyWorkLV::w_ClearTreeView(void* voidxMyTree)
{
	
	if(voidxMyTree == NULL) return;
	
	GtkTreeModel* pModel = gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pModel != NULL) gtk_list_store_clear((GtkListStore*)pModel);

}

bool MyWorkLV::w_GetItemTextLV(void* voidxMyTree,int ixSubItem,int* ixTexto,char* lpxTexto,long lxTexto)
{
	GtkTreeIter       iter;
	GtkTreeModel*     pModel;
	GtkTreeSelection* pSelect;

	pSelect = gtk_tree_view_get_selection((GtkTreeView*)voidxMyTree);
	if(pSelect == NULL) return 0;

	GList* vPaths = NULL;
	if(gtk_tree_selection_get_mode(pSelect) == GTK_SELECTION_MULTIPLE)
	{
		vPaths = gtk_tree_selection_get_selected_rows(pSelect,NULL);
		if(vPaths == NULL) return 0;

		pModel = gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);

		GList* gFila = g_list_first(vPaths);
		if(gFila == NULL) return 0;

		GtkTreePath* pwPath = (GtkTreePath*)gFila->data;
		gtk_tree_model_get_iter(pModel,&iter,(GtkTreePath*)pwPath);

	}
	else {if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return 0;}

	if(lpxTexto != NULL)
	{
		char* lpTexto = NULL;

		memset(lpxTexto,0,lxTexto);

		gtk_tree_model_get(pModel, &iter, ixSubItem,  &lpTexto,  -1);
		if(lpTexto == NULL) return 0;

		if(strlen(lpTexto) >= lxTexto) return 0;

		memcpy(lpxTexto,lpTexto,strlen(lpTexto));
		free(lpTexto);
	}

	if(ixTexto != NULL)
	{
		gtk_tree_model_get(pModel, &iter, ixSubItem,  ixTexto,  -1);
	}

	if(vPaths != NULL) g_list_free (vPaths);

	return 1;

}

bool MyWorkLV::w_SetItemLV(void* vxTree,int ixSub,char* lpxValor,long lxValor,bool bxNumeric,bool bxTreeView)
{
	GtkTreeIter       iter;
	GtkTreeModel*     pModel;
	GtkTreeSelection* pSelect;

	if(!bxNumeric && lpxValor == NULL) return 0;

	pSelect = gtk_tree_view_get_selection((GtkTreeView*)vxTree);

	GList* vPaths = NULL;
	if(gtk_tree_selection_get_mode(pSelect) == GTK_SELECTION_MULTIPLE)
	{
		vPaths = gtk_tree_selection_get_selected_rows(pSelect,NULL);
		if(vPaths == NULL) return 0;

		pModel = gtk_tree_view_get_model((GtkTreeView*)vxTree);

		GList* gFila = g_list_first(vPaths);
		if(gFila == NULL) return 0;

		GtkTreePath* pwPath = (GtkTreePath*)gFila->data;
		gtk_tree_model_get_iter(pModel,&iter,(GtkTreePath*)pwPath);
	}
	else
	{
		if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return 0;
	}

//	if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return 0;

	if(bxNumeric)
	{
		if(bxTreeView)
		{
			gtk_tree_store_set((GtkTreeStore*)pModel,&iter,ixSub,lxValor,-1);
		}
		else {gtk_list_store_set((GtkListStore*)pModel,&iter,ixSub,lxValor,-1);}
	}
	else
	{
		if(bxTreeView)
		{
			gtk_tree_store_set((GtkTreeStore*)pModel,&iter,ixSub,lpxValor,-1);
		}
		else {gtk_list_store_set((GtkListStore*)pModel,&iter,ixSub,lpxValor,-1);}
	}

	if(vPaths != NULL) g_list_free (vPaths);

	return 1;

}

long MyWorkLV::w_GetPosLV(void* voidxMyTree)
{
	GtkTreeIter       iter;
	GtkTreeModel*     pModel;
	GtkTreeSelection* pSelect;
		
	pSelect = gtk_tree_view_get_selection((GtkTreeView*)voidxMyTree);
	
	GList* vPaths = NULL;
	if(gtk_tree_selection_get_mode(pSelect) == GTK_SELECTION_MULTIPLE)
	{
		vPaths = gtk_tree_selection_get_selected_rows(pSelect,NULL);
		if(vPaths == NULL) return -1;

		pModel = gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);

		GList* gFila = g_list_first(vPaths);
		if(gFila == NULL) return -1;

		GtkTreePath* pwPath = (GtkTreePath*)gFila->data;
		gtk_tree_model_get_iter(pModel,&iter,(GtkTreePath*)pwPath);
	}
	else
	{
		if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return -1;
	}

//	if (!gtk_tree_selection_get_selected(pSelect, &pModel, &iter)) return -1;

	long lPos = -1;
	gtk_tree_model_get(pModel, &iter, 0,&lPos, -1);

	if(vPaths != NULL) g_list_free (vPaths);

	return lPos;
}

bool MyWorkLV::w_SelectPosLV(void* voidxMyTree,int ixPos)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pList == NULL) return 0;	
	
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
	
		int iwPos;
		gtk_tree_model_get(pList, &iter, 0,&iwPos,-1);
		if(iwPos == ixPos)
		{
			pSelect = gtk_tree_view_get_selection((GtkTreeView*)voidxMyTree);
			gtk_tree_selection_select_iter(pSelect,&iter);
			
			GtkTreePath* pPath = gtk_tree_model_get_path(pList,&iter);
			gtk_tree_view_scroll_to_cell((GtkTreeView*)voidxMyTree,pPath,NULL,false,0,0);
						
			return 1;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}
	
	return 0;
}

bool MyWorkLV::w_UnSelectPosLV(void* voidxMyTree,int ixPos)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	if(ixPos == -1) ixPos = w_GetPosLV(voidxMyTree);

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pList == NULL) return 0;

	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{

		int iwPos;
		gtk_tree_model_get(pList, &iter, 0,&iwPos,-1);
		if(iwPos == ixPos)
		{
			pSelect = gtk_tree_view_get_selection((GtkTreeView*)voidxMyTree);
			gtk_tree_selection_unselect_iter(pSelect,&iter);

			return 1;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}

	return 0;
}
bool MyWorkLV::w_SelectSeqLV(void* voidxMyTree,int ixSubItem,int ixSeq)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pList == NULL) return 0;	
	
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
	
		int iwSeq;
		gtk_tree_model_get(pList, &iter, ixSubItem,&iwSeq,-1);
		if(iwSeq == ixSeq)
		{
			pSelect = gtk_tree_view_get_selection((GtkTreeView*)voidxMyTree);
			gtk_tree_selection_select_iter(pSelect,&iter);
			
			GtkTreePath* pPath = gtk_tree_model_get_path(pList,&iter);
			gtk_tree_view_scroll_to_cell((GtkTreeView*)voidxMyTree,pPath,NULL,false,0,0);
			
			return 1;
		}

		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}
	
	return 0;
}
long MyWorkLV::w_GetLineasLV(void* vxTree)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vxTree);
	if(pList == NULL) return 0;

	long kLineas = 0;

	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		kLineas++;
		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}

	return kLineas;
}

long MyWorkLV::w_GetPrimeraPosLV(void* voidxMyTree)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pList == NULL) return -1;

	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	if(bAxx)
	{
		long lwPos;
		gtk_tree_model_get(pList, &iter, 0,&lwPos,-1);
		return lwPos;
	}
	return -1;
}

long MyWorkLV::w_GetUltPosLV(void* voidxMyTree)
{
	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)voidxMyTree);
	if(pList == NULL) return -1;

	long lUltima = -1;

	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{

		long lwPos;
		gtk_tree_model_get(pList, &iter, 0,&lwPos,-1);
		if(lwPos > lUltima) lUltima = lwPos;

		bAxx = gtk_tree_model_iter_next(pList,&iter);
	}

	return lUltima;
}

void MyWorkLV::w_SetSigno(char* lpxValor,long lxValor,bool bxParentesis)
{	
	char cAxx[32];memset(cAxx,0,sizeof(cAxx));
	memcpy(cAxx,lpxValor,strlen(lpxValor));
	memset(lpxValor,0,lxValor);
	if(bxParentesis)
	{
		snprintf(lpxValor,lxValor,"(%s)",cAxx);
	}
	else
	{
		snprintf(lpxValor,lxValor,"-%s",cAxx);
	}
}

bool MyWorkLV::w_GetFormat(char* lpxFormat,long lxFormat,double dxValor,int ixDecimales,bool bxParentesis)
{

	if(lpxFormat == NULL || lxFormat < 1 || lxFormat > 32) return 0;
	memset(lpxFormat,0,lxFormat);
	if(dxValor < 0.00001 && dxValor > -0.00001) return 1;


	bool bNegativo = false;
	if(dxValor < 0.000000)
	{
		bNegativo = true;
		dxValor *= -1.000000;
	}

	if(ixDecimales < 0 || ixDecimales > 6)
	{	
		snprintf(lpxFormat,lxFormat,"%f",dxValor);
		return 1;
	}

	char cAxx[32];

	if(ixDecimales == 0) snprintf(cAxx,sizeof(cAxx),"%0.0f",dxValor);
	if(ixDecimales == 1) snprintf(cAxx,sizeof(cAxx),"%0.1f",dxValor);
	if(ixDecimales == 2) snprintf(cAxx,sizeof(cAxx),"%0.2f",dxValor);
	if(ixDecimales == 3) snprintf(cAxx,sizeof(cAxx),"%0.3f",dxValor);
	if(ixDecimales == 4) snprintf(cAxx,sizeof(cAxx),"%0.4f",dxValor);
	if(ixDecimales == 5) snprintf(cAxx,sizeof(cAxx),"%0.5f",dxValor);
	if(ixDecimales == 6) snprintf(cAxx,sizeof(cAxx),"%0.6f",dxValor);

	long lAxx = strlen(cAxx);
	if(lAxx >= 24) return "";

	char cBxx[32];memset(cBxx,0,sizeof(cBxx));
	memcpy(cBxx,cAxx,lAxx-ixDecimales-1);
	if(lAxx <= (4 + ixDecimales))
	{
		memcpy(lpxFormat,cAxx,strlen(cAxx));
		if(bNegativo) w_SetSigno(lpxFormat,lxFormat,bxParentesis);
		//double dValor = atof(lpxFormat);
		//if(dValor < 0.00001 && dValor > -0.00001) memset(lpxFormat,0,lxFormat);
		return 1;
	}

	long lBxx = strlen(cBxx);
	if(lBxx >= 24) return 0;
	char cCxx[64];memset(cCxx,0,sizeof(cCxx));

	long p = lBxx + 32;
	int k = 0;
	int kx = 0;
	for(int i=lBxx;i>=0;i--)
	{
		memcpy(cCxx+p,cBxx+i,1);p--;k++;kx++;
		if(k >= 4)
		{
			memcpy(cCxx+p,",",1);p--;kx++;
			k = 1;
		}
	}

	if(kx >= sizeof(cAxx)) return 0;

	if(memcmp(cCxx+p+1,",",1) == 0)
	{
		p++;
		kx--;
	}

	memcpy(lpxFormat,cCxx+p+1,kx-1);
	memcpy(lpxFormat+kx-1,".",1);
	memcpy(lpxFormat+kx,cAxx+lAxx-ixDecimales,ixDecimales);	

	if(bNegativo) w_SetSigno(lpxFormat,lxFormat,bxParentesis);

	return 1;

}

void MyWorkLV::w_ParseDouble(char* lpxValor,long lxValor,bool bxCero)
{
	if(lpxValor == NULL) {lpxValor=(char*)"0.00";return;}
	if(strlen(lpxValor) < 1) {lpxValor=(char*)"0.00";return;}

	if(lxValor == -1) lxValor = strlen(lpxValor) + 1;
	if(lxValor < 2 || lxValor > 32) return;

	char cAxx[32];memset(cAxx,0,sizeof(cAxx));
	long lwxx = strlen(lpxValor);
	if(lwxx > 31) return;
	
	long p = 0;
	bool bSigno = false;
	for(int i=0;i<lwxx;i++)
	{
		if(memcmp(lpxValor+i,"(",1) == 0 || memcmp(lpxValor+i,")",1) == 0)
		{
			bSigno = true;
			continue;
		}
		if(memcmp(lpxValor+i,",",1) == 0) continue;
		if(memcmp(lpxValor+i," ",1) == 0) continue;

		memcpy(cAxx+p,lpxValor+i,1);p++;
	}

	memset(lpxValor,0,lxValor);
	if(bSigno)
	{
		snprintf(lpxValor,lxValor,"-%s",cAxx);
	}
	else
	{
		memcpy(lpxValor,cAxx,strlen(cAxx));
	}

	double dValor = atof(lpxValor);
	if(dValor < 0.00001 && dValor > -0.00001)
	{
		memset(lpxValor,0,lxValor);
		if(bxCero) memcpy(lpxValor,"0.00",4);
	}
}


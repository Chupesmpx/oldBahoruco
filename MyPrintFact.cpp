/*
 * MyPrintFact.cpp
 *
 *  Created on: 26/11/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <locale.h>
//#include <math.h>
#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "main.h"
#include "MyPrintFact.h"
#include "MyListview.h"
#include "myworklv.h"
#include "myfecha.h"


void on_fact_begin_print (GtkPrintOperation *operation,
	     GtkPrintContext   *context,
	     gpointer           user_data)
{

	gtk_print_operation_set_n_pages (operation,1);

}

void on_fact_end_print (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gpointer           user_data)
{

}

void on_fact_draw_page (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gint               page_nr,
	   gpointer           user_data)
{

	MyPrintFact* pPrint = (MyPrintFact*) user_data;
	pPrint->vContext = context;

//	double width = gtk_print_context_get_width (context);
	pPrint->ggdAnchoPag   = 75;
	pPrint->ggdMargenLeft = 5;
	pPrint->ggdMargenTop  = 5;

	pPrint->w_DrawPrint();

}


MyPrintFact::MyPrintFact()
{

}

MyPrintFact::~MyPrintFact()
{

}

void MyPrintFact::w_Print(char* lpxNameClte)
{
	GtkPrintOperation *operation;
	GtkPrintSettings *settings;
	gchar *dir, *ext;
	GError *error = NULL;

	////////////////////////////////////////////////////////////////////////////
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	long lNulo;
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_NULA,(int*)&lNulo,NULL,0)) return;
	////////////////////////////////////////////////////////////////////////////


	memset(ggcNameClte,0,sizeof(ggcNameClte));
	if(lpxNameClte == NULL) {memcpy(ggcNameClte,"AL CONTADO",10);}
	else {memcpy(ggcNameClte,lpxNameClte,strlen(lpxNameClte));}

	operation = gtk_print_operation_new ();

	g_signal_connect (G_OBJECT (operation), "begin-print",
			G_CALLBACK (on_fact_begin_print), this);
	g_signal_connect (G_OBJECT (operation), "draw-page",
			G_CALLBACK (on_fact_draw_page), this);
	g_signal_connect (G_OBJECT (operation), "end-print",
			G_CALLBACK (on_fact_end_print), this);

	gtk_print_operation_set_use_full_page (operation, FALSE);
	gtk_print_operation_set_unit (operation, GTK_UNIT_MM);
	gtk_print_operation_set_embed_page_setup (operation, TRUE);

	settings = gtk_print_settings_new ();
	dir = (gchar*)g_get_user_special_dir (G_USER_DIRECTORY_DOCUMENTS);
	if (dir == NULL)
	{
		dir = (gchar*)g_get_home_dir ();
	}
	if (g_strcmp0 (gtk_print_settings_get (settings, GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT), "ps") == 0)
	{
		ext = ".ps";
	}
	else
	{
		if (g_strcmp0 (gtk_print_settings_get (settings, GTK_PRINT_SETTINGS_OUTPUT_FILE_FORMAT), "svg") == 0)
		{
			ext = ".svg";
		}
		else
		{
			ext = ".pdf";
		}
	}

	char curi[1024];
	snprintf(curi,1024,"file://%s/gasFacturas%s",dir,ext);
	gtk_print_settings_set (settings, GTK_PRINT_SETTINGS_OUTPUT_URI, curi);
	gtk_print_operation_set_print_settings (operation, settings);

//	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (w_GetMain()), &error);
	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT,NULL,NULL);


//	 gtk_print_operation_set_export_filename (operation, "test.pdf");
//	 gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_EXPORT, NULL, NULL);

	g_object_unref (operation);
	g_object_unref (settings);

	if (error)
	{
		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW (w_GetMain()),
					   GTK_DIALOG_DESTROY_WITH_PARENT,
					   GTK_MESSAGE_ERROR,
					   GTK_BUTTONS_CLOSE,
					   "%s", error->message);
		g_error_free (error);

		g_signal_connect (dialog, "response",
			G_CALLBACK (gtk_widget_destroy), NULL);

		gtk_widget_show (dialog);
	}

}

void MyPrintFact::w_SetFont(void* pgxLayout,char* lpxFont)
{
	PangoFontDescription* desc = pango_font_description_from_string (lpxFont);
	pango_layout_set_font_description ((PangoLayout*)pgxLayout, desc);
	pango_font_description_free (desc);
}
void MyPrintFact::w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto)
{
	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
//	pango_layout_get_pixel_size ((PangoLayout*)pgxLayout,cx,cy);

	PangoRectangle rcx;
	pango_layout_get_pixel_extents((PangoLayout*)pgxLayout,&rcx,NULL);
	*cx = rcx.width  - rcx.x;
	*cy = rcx.height - rcx.y;
}

void MyPrintFact::w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto)
{
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	cairo_move_to (cr,dxLeft,dxTop);
	pango_cairo_show_layout (cr,(PangoLayout*)pgxLayout);

}

void MyPrintFact::w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble)
{
	char* lpBxx = "<span underline='single'>%s</span>";
	if(bxDoble) lpBxx = "<span underline='double'>%s</span>";
	long lAxx = strlen(lpBxx) + strlen(lpxTexto) + 4;
	char* lpAxx = (char*)malloc(lAxx);
	if(lpAxx == NULL) return;
	memset(lpAxx,0,lAxx);
	snprintf(lpAxx,lAxx,lpBxx,lpxTexto);

	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	pango_layout_set_markup((PangoLayout*)pgxLayout,lpAxx,-1);

	cairo_move_to (cr,dxLeft,dxTop);
	pango_cairo_show_layout (cr,(PangoLayout*)pgxLayout);

	free(lpAxx);
}
void MyPrintFact::w_hLinea(double dxLeft1,double dxLeft2,double dxTop)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,0.2);
	cairo_move_to (cr,dxLeft1,dxTop);
	cairo_line_to(cr,dxLeft2,dxTop);
	cairo_stroke(cr);

	g_object_unref(layout);

}

void MyPrintFact::w_DrawPrint()
{
	w_Encabezado();
	w_Detalle();
	w_Rayas();
}

void MyPrintFact::w_Encabezado()
{

	PangoLayout* layout;

	ggdLinea = ggdMargenTop;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 12 bold");

	char* lpAxx = "Envasadora Bahoruco Gas";

	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);

	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += cy;
	g_object_unref(layout);

	///////////////////////////////////////////////////////////////////////////
	lpAxx = "Calle San Bartolome Km 2 Neiba, R.D.";
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans oblique 10");
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = (ggdAnchoPag - cx) / 2;
	ggdLinea += 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += cy;

	lpAxx = "telf. 809-527-3715 cel 809-406-8720";
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += cy;

	lpAxx = "RNC 022-0002122-4";
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += 2*cy;
	g_object_unref(layout);
	//////////////////////////////////////////////////////////////////////////

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	//Factura

	long lNumero,lNula;
	char cNumero[16];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_NULA,(int*)&lNula,NULL,0)) lNula=0;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_TICKET,(int*)&lNumero,NULL,0)) lNumero=0;
	snprintf(cNumero,sizeof(cNumero),"%d",lNumero);

	lpAxx = "Factura:";
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,lpAxx);
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,ggdMargenLeft+cx,ggdLinea,cNumero);

	if(lNula > 0)
	{
		int cxw,cyw;
		w_GetSize(layout,&cxw,&cyw,cNumero);
		lpAxx = "** NULA ** NULA ** NULA **";
		w_ShowTexto(layout,ggdMargenLeft+cx+cxw,ggdLinea,lpAxx);
	}

	ggdLinea += cy;
	iLeft     = ggdMargenLeft + cx;

	//Fecha
	char cFecha[16];
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_FECHA,NULL,cFecha,sizeof(cFecha))) memset(cFecha,0,sizeof(cFecha));
	lpAxx = "Fecha:";
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,iLeft,ggdLinea,cFecha);
	ggdLinea += cy;


	//Cliente
	lpAxx = "Cliente:";
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,iLeft,ggdLinea,ggcNameClte);
	ggdLinea += cy;

	//Precio
	double dPrecio = w_GetPrecio();
	char cPrecio[32];
	snprintf(cPrecio,sizeof(cPrecio),"$%0.2f",dPrecio);

	lpAxx = "Precio:";
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,iLeft,ggdLinea,cPrecio);
	ggdLinea += cy;

	g_object_unref(layout);
}

void MyPrintFact::w_Detalle()
{
	PangoLayout* layout;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans bold 10");

	int cx,cy;
	int cxx,cyy;

	w_GetSize(layout,&cxx,&cyy,"A");//Uno

	int iLeft1 = ggdMargenLeft;
	w_GetSize(layout,&cx,&cy,"9999.999");//Galones
	int iAncho1 = cx;
	int iLeft2  = iLeft1 + cx + cxx;
	w_GetSize(layout,&cx,&cy,"XXXXXXXX");//Descripcion
	int iAncho2 = cx;
	int iLeft3  = iLeft2 + cx + cxx;
	w_GetSize(layout,&cx,&cy,"99,999.99");//Import
	int iAncho3 = cx;

	if((iLeft3 + iAncho3) > (ggdAnchoPag - 2*ggdMargenLeft))
	{
		iLeft3 = ggdAnchoPag - ggdMargenLeft - iAncho3;
	}

	w_hLinea(ggdMargenLeft,ggdAnchoPag-ggdMargenLeft,ggdLinea);

	g_object_unref(layout);
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");

	char* lpAxx = "Galones";
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft = iLeft1 + iAncho1 - cx;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);

	w_ShowTexto(layout,iLeft2,ggdLinea,"Detalle");

	lpAxx = "Import$";
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = iLeft3 + iAncho3 - cx;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);


	ggdLinea += cy;

	w_hLinea(ggdMargenLeft,ggdAnchoPag-ggdMargenLeft,ggdLinea);

	///////////////////////////////////////////////////////////////////////////

	g_object_unref(layout);

	w_Detalle2(iLeft1,iLeft2,iLeft3,iAncho1,iAncho2,iAncho3);
}

void MyPrintFact::w_Detalle2(int ixLeft1,int ixLeft2,int ixLeft3,int ixAncho1,int ixAncho2,int ixAncho3)
{
	PangoLayout*  layout;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeTickets;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return;

	MyWorkLV wxx;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");

	int cx,cy,cxx,cyy;
	w_GetSize(layout,&cxx,&cyy,"999,999.99");

	long k = 0;
	double dsGalones = 0.0;
	double dsTotal   = 0.0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		k++;

		char* lpGalones;
		char* lpDescrip;
		char* lpTotal;
		gtk_tree_model_get(pList, &iter,
				MyListview::TICK_GLS,&lpGalones,
				MyListview::TICK_DESCRIP,&lpDescrip,
				MyListview::TICK_TOTAL,&lpTotal,
				-1);

		if(strlen(lpGalones) < 1 && strlen(lpTotal) < 1)
		{
			bAxx = gtk_tree_model_iter_next(pList,&iter);
			if(k >= 3) break;
			continue;
		}

		w_GetSize(layout,&cx,&cy,lpGalones);
		int iLeft = ixLeft1 + ixAncho1 - cx;
		w_ShowTexto(layout,iLeft,ggdLinea,lpGalones);

		w_ShowTexto(layout,ixLeft2,ggdLinea,lpDescrip);

		w_GetSize(layout,&cx,&cy,lpTotal);
		iLeft = ixLeft3 + ixAncho3 - cx;
		w_ShowTexto(layout,iLeft,ggdLinea,lpTotal);

		ggdLinea += cyy;

		free(lpGalones);free(lpTotal);free(lpDescrip);

		bAxx = gtk_tree_model_iter_next(pList,&iter);

		if(k >= 3) break;
	}

	ggdLinea += 2*cyy;

	w_hLinea(ggdMargenLeft,ggdAnchoPag-ggdMargenLeft,ggdLinea);

	g_object_unref(layout);

	w_Total(ixLeft1,ixLeft2,ixLeft3,ixAncho1,ixAncho2,ixAncho3);
}

void MyPrintFact::w_Total(int ixLeft1,int ixLeft2,int ixLeft3,int ixAncho1,int ixAncho2,int ixAncho3)
{
	PangoLayout*  layout;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;


	MyWorkLV wxx;

	char cGalones[32],cTotal[32];
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_GLS,NULL,cGalones,sizeof(cGalones))) memset(cGalones,0,sizeof(cGalones));
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_TOTAL,NULL,cTotal,sizeof(cTotal))) memset(cTotal,0,sizeof(cTotal));


	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans bold 10");

	int cx,cy,cxx,cyy;
	w_GetSize(layout,&cxx,&cyy,"999,999.99");

	w_GetSize(layout,&cx,&cy,cGalones);
	int iLeft = ixLeft1 + ixAncho1 - cx;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,cGalones,true);

//	w_ShowTexto(layout,ixLeft2,ggdLinea,"  Totales");

	w_GetSize(layout,&cx,&cy,cTotal);
	iLeft = ixLeft3 + ixAncho3 - cx;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,cTotal,true);

	ggdLinea += 2*cyy;

	g_object_unref(layout);

	/////////////////////////////////////////////////////////////////////////

	char cEfectivo[32],cCambio[32];
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_EFECTIVO,NULL,cEfectivo,sizeof(cEfectivo))) memset(cEfectivo,0,sizeof(cEfectivo));
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_CAMBIO,NULL,cCambio,sizeof(cCambio))) memset(cCambio,0,sizeof(cCambio));

	if(strlen(cEfectivo) < 1) memcpy(cEfectivo,"***\0",4);
	if(strlen(cCambio) < 1) memcpy(cCambio,"***\0",4);


	char* lpAxx = "Efectivo:";
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,lpAxx);
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,ggdMargenLeft+cx,ggdLinea,cEfectivo);

	ggdLinea += cy;
	iLeft     = ggdMargenLeft + cx;

	lpAxx = "Cambio:";
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);
	w_ShowTexto(layout,ggdMargenLeft+cx,ggdLinea,cCambio);

	ggdLinea += 4*cyy;

	g_object_unref(layout);


}

void MyPrintFact::w_Rayas()
{
	PangoLayout*  layout;

	int iLargo = (ggdAnchoPag - 2*ggdMargenLeft) / 2  - 4;
	int iLeft1 = ggdMargenLeft;
	int iLeft2 = ggdMargenLeft + iLargo + 8;

	w_hLinea(iLeft1,iLeft1+iLargo,ggdLinea);
	w_hLinea(iLeft2,iLeft2+iLargo,ggdLinea);

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans oblique 8");

	int cx,cy;
	char* lpAxx = "Despachado";
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft = iLeft1 + (iLargo - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);

	lpAxx = "Recibido";
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = iLeft2 + (iLargo - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);

	ggdLinea += 2*cy;
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,"*");

	g_object_unref(layout);
}

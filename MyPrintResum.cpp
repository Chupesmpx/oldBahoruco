/*
 * MyPrintResum.cpp
 *
 *  Created on: 23/11/2012
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
#include "MyPrintResum.h"
#include "MyListview.h"
#include "myworklv.h"
#include "myfecha.h"

void on_res_begin_print (GtkPrintOperation *operation,
	     GtkPrintContext   *context,
	     gpointer           user_data)
{

	gtk_print_operation_set_n_pages (operation,1);

}

void on_res_end_print (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gpointer           user_data)
{

}

void on_res_draw_page (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gint               page_nr,
	   gpointer           user_data)
{

	MyPrintResum* pPrint = (MyPrintResum*) user_data;
	pPrint->vContext = context;

//	double width = gtk_print_context_get_width (context);
//	pPrint->dAnchoPag   = width;
//	pPrint->dMargenLeft = 0.03 * width;
//	pPrint->dMargenTop  = 3*pPrint->dMargenLeft;

	pPrint->dAnchoPag   = 75;
	pPrint->dMargenLeft = 3;
	pPrint->dMargenTop  = 3;

	pPrint->w_DrawPrint();

}


MyPrintResum::MyPrintResum()
{

}

MyPrintResum::~MyPrintResum()
{

}

//void MyPrintResum::w_Print(double dxInicial,double dxFinal)
void MyPrintResum::w_Print()
{
	GtkPrintOperation *operation;
	GtkPrintSettings *settings;
	gchar *dir, *ext;
	GError *error = NULL;

//	ggdInicial = dxInicial;
//	ggdFinal   = dxFinal;

	operation = gtk_print_operation_new ();

	g_signal_connect (G_OBJECT (operation), "begin-print",
			G_CALLBACK (on_res_begin_print), this);
	g_signal_connect (G_OBJECT (operation), "draw-page",
			G_CALLBACK (on_res_draw_page), this);
	g_signal_connect (G_OBJECT (operation), "end-print",
			G_CALLBACK (on_res_end_print), this);

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
	snprintf(curi,1024,"file://%s/gasResumen%s",dir,ext);
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

void MyPrintResum::w_SetFont(void* pgxLayout,char* lpxFont)
{
	PangoFontDescription* desc = pango_font_description_from_string (lpxFont);
	pango_layout_set_font_description ((PangoLayout*)pgxLayout, desc);
	pango_font_description_free (desc);
}
void MyPrintResum::w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto)
{
	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	pango_layout_get_pixel_size ((PangoLayout*)pgxLayout,cx,cy);
}

void MyPrintResum::w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto)
{
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	cairo_move_to (cr,dxLeft,dxTop);
	pango_cairo_show_layout (cr,(PangoLayout*)pgxLayout);

}

void MyPrintResum::w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble)
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
void MyPrintResum::w_DrawPrint()
{
	w_Encabezado();
	w_Detalle();
	w_Lecturas();
	w_Rayas();
}

void MyPrintResum::w_hLinea(double dxLeft1,double dxLeft2,double dxTop)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,0.3);
	cairo_move_to (cr,dxLeft1,dxTop);
	cairo_line_to(cr,dxLeft2,dxTop);
	cairo_stroke(cr);

	g_object_unref(layout);

}
void MyPrintResum::w_Linea(char* lpxGalones,char* lpxDescrip,char* lpxTotal)
{
	PangoLayout* layout;
	int cx,cy,cyy,cx1,cx2,cx3;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,gglpFont);
	w_GetSize(layout,&cx,&cyy,"A");
	w_GetSize(layout,&cx1,&cy,lpxGalones);
	w_GetSize(layout,&cx2,&cy,lpxDescrip);
	w_GetSize(layout,&cx3,&cy,lpxTotal);

	double dLeft1 = dMargenLeft + (ggiAncho1 - cx1);
	double dLeft2 = dMargenLeft + ggiAncho1 + cx;
	double dLeft3 = dMargenLeft + ggiAncho1 + cx + ggiAncho2 + cx + (ggiAncho3 - cx3);

	w_ShowTexto(layout,dLeft1,ggdLinea,lpxGalones);
	w_ShowTexto(layout,dLeft2,ggdLinea,lpxDescrip);
	w_ShowTexto(layout,dLeft3,ggdLinea,lpxTotal);

	g_object_unref(layout);

	ggdLinea += cyy;
}

void MyPrintResum::w_Encabezado()
{

	PangoLayout* layout;

	ggdLinea = dMargenTop;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 12 bold");

	char* lpAxx = "***  RESUMEN ***";

	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);

	int iLeft = (dAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += 2*cy;
	g_object_unref(layout);

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");

	long lFecha = atol(w_GetFecha());
	char cFecha[32];
	MyFecha myFecha;
	myFecha.w_GetFechaEd2(lFecha,cFecha,sizeof(cFecha));

	double dPrecio = w_GetPrecio();
	char cPrecio[32];
	snprintf(cPrecio,32,"$%0.2f",dPrecio);

	w_GetSize(layout,&cx,&cy,"Precio:");
	w_ShowTexto(layout,dMargenLeft,ggdLinea,"Fecha:");
	w_ShowTexto(layout,dMargenLeft+cx,ggdLinea,cFecha);
	ggdLinea += cy;

	w_ShowTexto(layout,dMargenLeft,ggdLinea,"Precio:");
	w_ShowTexto(layout,dMargenLeft+cx,ggdLinea,cPrecio);
	ggdLinea += cy;

	g_object_unref(layout);


	//Ancho de las columnas
	gglpFont = "sans 8";
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,gglpFont);

	w_GetSize(layout,&ggiAncho1,&cy,"99999999.999");//Galones
	w_GetSize(layout,&ggiAncho2,&cy,"BONO GAS");//Descrip
	w_GetSize(layout,&ggiAncho3,&cy,"999,999,999.999");//Total
	g_object_unref(layout);

	w_hLinea(dMargenLeft,dAnchoPag-dMargenLeft,ggdLinea);
	w_Linea("Galones","Descrip","Total");
	w_hLinea(dMargenLeft,dAnchoPag-dMargenLeft,ggdLinea);
}

void MyPrintResum::w_Detalle()
{
//	PangoLayout* layout;

//	GtkTreeSelection* pSelect;
	GtkTreeModel* pList = NULL;
	GtkTreeIter   iter;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeResumen;

	pList = (GtkTreeModel*)gtk_tree_view_get_model((GtkTreeView*)vTree);
	if(pList == NULL) return;

	MyWorkLV wxx;

	gglpFont = "sans bold 9";

	long k = 0;
	double dsGalones = 0.0;
	double dsTotal   = 0.0;
	bool bAxx = gtk_tree_model_get_iter_first(pList,&iter);
	while(bAxx)
	{
		char* lpGalones;
		char* lpDescrip;
		char* lpTotal;
		gtk_tree_model_get(pList, &iter,
				MyListview::RESUMEN_GLS,&lpGalones,
				MyListview::RESUMEN_DESCRIP,&lpDescrip,
				MyListview::RESUMEN_TOTAL,&lpTotal,
				-1);

		w_Linea(lpGalones,lpDescrip,lpTotal);

		free(lpGalones);free(lpTotal);free(lpDescrip);


		bAxx = gtk_tree_model_iter_next(pList,&iter);

	}

	w_hLinea(dMargenLeft,dAnchoPag-dMargenLeft,ggdLinea);
}

void MyPrintResum::w_Lecturas()
{
	PangoLayout* layout;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 12");

	char* lpAx1 = "Lectura Inicial:";
	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAx1);

	ggdLinea += 6*cy;

	g_object_unref(layout);

	/*
	PangoLayout* layout;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 12");

	char cInicial[32],cFinal[32],cDif[32];
	snprintf(cInicial,32,"%0.3f",ggdInicial);
	snprintf(cFinal,32,"%0.3f",ggdFinal);
	snprintf(cDif,32,"%0.3f",ggdFinal-ggdInicial);

	char* lpAx1 = "Lectura Inicial:";
	char* lpAx2 = "Lectura Final:";
	char* lpAx3 = "Diferencia:";

	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAx1);

	ggdLinea += cy;

	int iLeft1 = dMargenLeft;
	int iLeft2 = iLeft1 + cx;

	int iTop1 = ggdLinea;
	int iTop2 = iTop1 + cy;
	int iTop3 = iTop2 + cy;

	w_ShowTexto(layout,iLeft1,iTop1,lpAx1);
	w_ShowTexto(layout,iLeft1,iTop2,lpAx2);
	w_ShowTexto(layout,iLeft1,iTop3,lpAx3);

	g_object_unref(layout);

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans bold 12");

	w_GetSize(layout,&cx,&cy,cFinal);

	w_ShowTexto(layout,iLeft2,iTop1,cInicial);
	w_ShowTexto(layout,iLeft2,iTop2,cFinal);
	w_ShowTextoUnderLine(layout,iLeft2,iTop3,cDif,true);

	ggdLinea = iTop3 + 3*cy;

	g_object_unref(layout);
	*/
}

void MyPrintResum::w_Rayas()
{
	PangoLayout*  layout;

	int iLargo = (dAnchoPag - 2*dMargenLeft) / 2  - 4;
	int iLeft1 = dMargenLeft;
	int iLeft2 = dMargenLeft + iLargo + 8;

	w_hLinea(iLeft1,iLeft1+iLargo,ggdLinea);
	w_hLinea(iLeft2,iLeft2+iLargo,ggdLinea);

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans oblique 8");

	int cx,cy;
	char* lpAxx = "Preparado";
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft = iLeft1 + (iLargo - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);

	lpAxx = "Aprobado";
	w_GetSize(layout,&cx,&cy,lpAxx);
	iLeft = iLeft2 + (iLargo - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,lpAxx);

	ggdLinea += 2*cy;
	w_ShowTexto(layout,dMargenLeft,ggdLinea,"*");

	g_object_unref(layout);
}

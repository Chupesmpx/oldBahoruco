/*
 * MyPrintTicket.cpp
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
#include "MyPrintTicket.h"
#include "MyListview.h"
#include "myworklv.h"
#include "myfecha.h"
#include "MyData.h"
#include "CargarDet.h"



void on_ticket_begin_print (GtkPrintOperation *operation,
	     GtkPrintContext   *context,
	     gpointer           user_data)
{

	gtk_print_operation_set_n_pages (operation,1);

}

void on_ticket_end_print (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gpointer           user_data)
{

}

void on_ticket_draw_page (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gint               page_nr,
	   gpointer           user_data)
{

	MyPrintTicket* pPrint = (MyPrintTicket*) user_data;
	pPrint->vContext = context;

	pPrint->ggdAnchoPag   = 75;
	pPrint->ggdMargenLeft = 5;
	pPrint->ggdMargenTop  = 1;

	pPrint->w_DrawPrint();

}


MyPrintTicket::MyPrintTicket()
{

}

MyPrintTicket::~MyPrintTicket()
{

}

void MyPrintTicket::w_Print()
{
	GtkPrintOperation *operation;
	GtkPrintSettings *settings;
	GtkPrintOperationResult res;
	gchar *dir, *ext;
	GError *error = NULL;

	////////////////////////////////////////////////////////////////////////////
	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	long lNulo;
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_NULA,(int*)&lNulo,NULL,0)) return;
	if(lNulo > 0)
	{
		w_MessageBox_main("Ticket Nulo.");
		return;
	}
	////////////////////////////////////////////////////////////////////////////

	operation = gtk_print_operation_new ();

	g_signal_connect (G_OBJECT (operation), "begin-print",
			G_CALLBACK (on_ticket_begin_print), this);
	g_signal_connect (G_OBJECT (operation), "draw-page",
			G_CALLBACK (on_ticket_draw_page), this);
	g_signal_connect (G_OBJECT (operation), "end-print",
			G_CALLBACK (on_ticket_end_print), this);

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
	snprintf(curi,1024,"file://%s/gasTicket%s",dir,ext);
	gtk_print_settings_set (settings, GTK_PRINT_SETTINGS_OUTPUT_URI, curi);
	gtk_print_operation_set_print_settings (operation, settings);

//	res = gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (w_GetMain()), &error);
	res = gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT,NULL,NULL);


//	 res = gtk_print_operation_set_export_filename (operation, "test.pdf");
//	 res = gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_EXPORT, NULL, NULL);

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
	else
	{
		if(res != GTK_PRINT_OPERATION_RESULT_CANCEL)
		{
			MyListview* pView = (MyListview*) w_GetListview();
			void* vTree = pView->pTreeDetalle;

			MyWorkLV wxx;
			long lwPos = wxx.w_GetPosLV(vTree);
			MyData myData;
			myData.w_AddPrint(lwPos);

			CargarDet cargarDet;
			cargarDet.w_SetPrint();
		}
	}

}

void MyPrintTicket::w_SetFont(void* pgxLayout,char* lpxFont)
{
	PangoFontDescription* desc = pango_font_description_from_string (lpxFont);
	pango_layout_set_font_description ((PangoLayout*)pgxLayout, desc);
	pango_font_description_free (desc);
}
void MyPrintTicket::w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto)
{
	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
//	pango_layout_get_pixel_size ((PangoLayout*)pgxLayout,cx,cy);

	PangoRectangle rcx;
	pango_layout_get_pixel_extents((PangoLayout*)pgxLayout,&rcx,NULL);
	*cx = rcx.width  - rcx.x;
	*cy = rcx.height - rcx.y;
}

void MyPrintTicket::w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto)
{
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	cairo_move_to (cr,dxLeft,dxTop);
	pango_cairo_show_layout (cr,(PangoLayout*)pgxLayout);

}

void MyPrintTicket::w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble)
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
void MyPrintTicket::w_hLinea(double dxLeft1,double dxLeft2,double dxTop)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,0.5);
	cairo_move_to (cr,dxLeft1,dxTop);
	cairo_line_to(cr,dxLeft2,dxTop);
	cairo_stroke(cr);

	g_object_unref(layout);

}
void MyPrintTicket::w_vLinea(double dxTop1,double dxTop2,double dxLeft)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,0.5);
	cairo_move_to (cr,dxLeft,dxTop1);
	cairo_line_to(cr,dxLeft,dxTop2);

	cairo_stroke(cr);

	g_object_unref(layout);

}

void MyPrintTicket::w_DrawPrint()
{

	w_NoTicket();
	w_Titulo();
	w_Galones();
	w_PrecioValor();
	w_Fecha();
	w_Cuadro();

	w_Imagen();
}

void MyPrintTicket::w_NoTicket()
{

	PangoLayout* layout;

	ggdLinea = ggdMargenTop;


	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	long lNumero,lPrint;
	char cNumero[16];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_TICKET,(int*)&lNumero,NULL,0)) lNumero=0;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_PRINT,(int*)&lPrint,NULL,0)) lPrint=0;
	if(lPrint > 0) {snprintf(cNumero,sizeof(cNumero),"*%d*",lNumero);}
	else {snprintf(cNumero,sizeof(cNumero),"%d",lNumero);}

	int cx,cy;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 14");
	w_GetSize(layout,&cx,&cy,cNumero);
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,cNumero);

//	char cPrecio[32];
//	double dPrecio = w_GetPrecio();
//	snprintf(cPrecio,sizeof(cPrecio),"%0.2f/gls",dPrecio);
//	w_GetSize(layout,&cx,&cy,cPrecio);
//	int iLeft = (ggdAnchoPag - cx) / 2;
//	w_ShowTexto(layout,iLeft,ggdLinea,cPrecio);

	ggdLinea += 2*cy;

	g_object_unref(layout);
}

void MyPrintTicket::w_Titulo()
{
	PangoLayout* layout;

	int cx,cy;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	char* lpAxx = "Bahoruco Gas";
	w_SetFont(layout,"sans 16");
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,lpAxx);

	ggdImagenTop = ggdLinea + cy;

	ggdLinea += 3*cy;

	g_object_unref(layout);
}

void MyPrintTicket::w_Galones()
{
	PangoLayout* layout;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	char cGalones[16];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_GLS,NULL,cGalones,sizeof(cGalones))) memset(cGalones,0,sizeof(cGalones));

	int cx,cy;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans bold 18");
	w_GetSize(layout,&cx,&cy,cGalones);
	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,cGalones);

	ggdLinea += 2*cy;

	g_object_unref(layout);
}

void MyPrintTicket::w_PrecioValor()
{
	PangoLayout* layout;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	char cGalones[16],cTotal[32];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_GLS,NULL,cGalones,sizeof(cGalones))) memset(cGalones,0,sizeof(cGalones));
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_TOTAL,NULL,cTotal,sizeof(cTotal))) memset(cTotal,0,sizeof(cTotal));

	double dPrecio = w_GetPrecio();

	char cAxx[64];
	snprintf(cAxx,sizeof(cAxx),"x%0.2f = %s",dPrecio,cTotal);

	int cx,cy;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,cAxx);
	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,cAxx);

	ggdLinea += 2*cy;

	g_object_unref(layout);
}

void MyPrintTicket::w_Fecha()
{
	PangoLayout* layout;

	MyListview* pView = (MyListview*) w_GetListview();
	void* vTree = pView->pTreeDetalle;

	char cFecha[16],cHora[16];
	MyWorkLV wxx;
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_FECHA,NULL,cFecha,sizeof(cFecha))) memset(cFecha,0,sizeof(cFecha));
	if(!wxx.w_GetItemTextLV(vTree,MyListview::DET_HORA,NULL,cHora,sizeof(cHora))) memset(cHora,0,sizeof(cHora));

	char cAxx[64];
	snprintf(cAxx,sizeof(cAxx),"%s  %s",cFecha,cHora);

	int cx,cy;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,cAxx);
	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTexto(layout,iLeft,ggdLinea,cAxx);

	ggdLinea += cy;

	g_object_unref(layout);
}

void MyPrintTicket::w_Cuadro()
{
	PangoLayout* layout;

	w_hLinea(ggdMargenLeft,ggdAnchoPag-ggdMargenLeft,ggdMargenTop);
	w_hLinea(ggdMargenLeft,ggdAnchoPag-ggdMargenLeft,ggdLinea);

	w_vLinea(ggdMargenTop,ggdLinea,ggdMargenLeft);
	w_vLinea(ggdMargenTop,ggdLinea,ggdAnchoPag-ggdMargenLeft);


	int cx,cy;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	char* lpAxx = "*";
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,lpAxx);
	ggdLinea += 2*cy;
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);

	g_object_unref(layout);
}

void MyPrintTicket::w_Imagen()
{

	char cFileName[1024];
	char* lpDirectory = getenv("directorio");
	if(lpDirectory == NULL) return;
	snprintf(cFileName,sizeof(cFileName),"%s/Imagen/logoTicket.png",lpDirectory);

	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_size(cFileName,8,19,NULL);
	if(pixbuf == NULL) return;

	int iLeft = ggdMargenLeft + 5.0;
	int iTop  = ggdImagenTop  + 3.0;
	cairo_t *cr = gtk_print_context_get_cairo_context ((GtkPrintContext*)vContext);
	gdk_cairo_set_source_pixbuf(cr, pixbuf,iLeft,iTop);
	cairo_paint (cr);

	g_object_unref (pixbuf);
}

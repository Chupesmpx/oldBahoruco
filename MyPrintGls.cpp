/*
 * MyPrintGls.cpp
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
#include "MyPrintGls.h"


void on_gls_begin_print (GtkPrintOperation *operation,
	     GtkPrintContext   *context,
	     gpointer           user_data)
{

	gtk_print_operation_set_n_pages (operation,1);

}

void on_gls_end_print (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gpointer           user_data)
{

}

void on_gls_draw_page (GtkPrintOperation *operation,
	   GtkPrintContext   *context,
	   gint               page_nr,
	   gpointer           user_data)
{

	MyPrintGls* pPrint = (MyPrintGls*) user_data;
	pPrint->vOperation = operation;
	pPrint->vContext   = context;

	pPrint->ggdHeight = gtk_print_context_get_height(context);

	pPrint->ggdAnchoPag   = 75;
	pPrint->ggdMargenLeft = 1;
	pPrint->ggdMargenTop  = 0;

	pPrint->w_DrawPrint();


}

MyPrintGls::MyPrintGls()
{

}

MyPrintGls::~MyPrintGls()
{

}

void MyPrintGls::w_Print(bool bxPorValor)
{
	GtkPrintOperation *operation;
	GtkPrintSettings *settings;
	gchar *dir, *ext;
	GError *error = NULL;

	ggbPorValor = bxPorValor;

	operation = gtk_print_operation_new ();

	g_signal_connect (G_OBJECT (operation), "begin-print",
			G_CALLBACK (on_gls_begin_print), this);
	g_signal_connect (G_OBJECT (operation), "draw-page",
			G_CALLBACK (on_gls_draw_page), this);
	g_signal_connect (G_OBJECT (operation), "end-print",
			G_CALLBACK (on_gls_end_print), this);

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
	snprintf(curi,1024,"file://%s/gasxGls%s",dir,ext);
	gtk_print_settings_set (settings, GTK_PRINT_SETTINGS_OUTPUT_URI, curi);
	gtk_print_operation_set_print_settings (operation, settings);

	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, GTK_WINDOW (w_GetMain()), &error);
//	gtk_print_operation_run (operation, GTK_PRINT_OPERATION_ACTION_PRINT,NULL,NULL);


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

void MyPrintGls::w_SetFont(void* pgxLayout,char* lpxFont)
{
	PangoFontDescription* desc = pango_font_description_from_string (lpxFont);
	pango_layout_set_font_description ((PangoLayout*)pgxLayout, desc);
	pango_font_description_free (desc);
}
void MyPrintGls::w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto)
{
	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	pango_layout_get_pixel_size ((PangoLayout*)pgxLayout,cx,cy);

//	PangoRectangle rcx;
//	pango_layout_get_pixel_extents((PangoLayout*)pgxLayout,&rcx,NULL);
//	*cx = rcx.width  - rcx.x;
//	*cy = rcx.height - rcx.y;
}

void MyPrintGls::w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto)
{
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	pango_layout_set_text ((PangoLayout*)pgxLayout,lpxTexto, -1);
	cairo_move_to (cr,dxLeft,dxTop);
	pango_cairo_show_layout (cr,(PangoLayout*)pgxLayout);

}

void MyPrintGls::w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble)
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
void MyPrintGls::w_hLinea(double dxLeft1,double dxLeft2,double dxTop,double dxAncho)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,dxAncho);
	cairo_move_to (cr,dxLeft1,dxTop);
	cairo_line_to(cr,dxLeft2,dxTop);
	cairo_stroke(cr);

	g_object_unref(layout);

}
void MyPrintGls::w_vLinea(double dxTop1,double dxTop2,double dxLeft,double dxAncho)
{
	PangoLayout* layout;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	cairo_t* cr = gtk_print_context_get_cairo_context((GtkPrintContext*)vContext);

	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr,dxAncho);
	cairo_move_to (cr,dxLeft,dxTop1);
	cairo_line_to(cr,dxLeft,dxTop2);

	cairo_stroke(cr);

	g_object_unref(layout);

}

void MyPrintGls::w_DrawPrint()
{
	w_Encabezado();
	w_Precio();

	if(ggbPorValor) {w_PorValor();}
	else {w_PorGalon();}

	w_AstericoFinal();

//	w_Imagen();
}

//void MyPrintGls::w_DrawPrint2()
//{
//	w_Encabezado();
//}

void MyPrintGls::w_Encabezado()
{
	PangoLayout* layout;

	ggdLinea = ggdMargenTop;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 14 bold");

	char* lpAxx = "Bahoruco Gas";

	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);

	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,lpAxx);
	ggdLinea += 2*cy;
	g_object_unref(layout);

}

void MyPrintGls::w_Precio()
{
	PangoLayout* layout;

//	ggdLinea = 40;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 16");

	double dPrecio = w_GetPrecio();
	char cPrecio[32];
	snprintf(cPrecio,32,"%0.2f",dPrecio);


	char* lpAxx = "Precio RD$:";
	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);

	int iLeft = (ggdAnchoPag - cx) / 2;
	w_ShowTextoUnderLine(layout,iLeft,ggdLinea,lpAxx);
	int iTop1 = ggdLinea;
	ggdLinea += cy;
	g_object_unref(layout);

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans bold 16");

	int cx1,cy1;
	w_GetSize(layout,&cx1,&cy1,cPrecio);
	int iLeft2 = (ggdAnchoPag - cx1) / 2;
	w_ShowTexto(layout,iLeft2,ggdLinea,cPrecio);
	ggdLinea += cy1 + 2;
	int iTop2 = ggdLinea;
	g_object_unref(layout);


	w_hLinea(iLeft-2.25,iLeft+cx+2.25,iTop1);
	w_hLinea(iLeft-2.25,iLeft+cx+2.25,iTop2);

	w_vLinea(iTop1,iTop2,iLeft-2);
	w_vLinea(iTop1,iTop2,iLeft+cx+2);

	ggdLinea += cy1;
}

void MyPrintGls::w_Imagen()
{
	char cFileName[1024];
	char* lpDirectory = getenv("directorio");
	if(lpDirectory == NULL) return;
	snprintf(cFileName,sizeof(cFileName),"%s/Imagen/logo1.png",lpDirectory);

	//289x144   144.5x72   96.33x48   72.25x36
//	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(cFileName,NULL);
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file_at_size(cFileName,72.25,36.0,NULL);
	if(pixbuf == NULL) return;

	int iLeft = 3;
	int iTop  = 2;
	cairo_t *cr = gtk_print_context_get_cairo_context ((GtkPrintContext*)vContext);
	gdk_cairo_set_source_pixbuf(cr, pixbuf,iLeft,iTop);
	cairo_paint (cr);

	g_object_unref (pixbuf);
}
void MyPrintGls::w_PorGalon()
{
	PangoLayout* layout;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 16");

	int cxx,cyy;
	w_GetSize(layout,&cxx,&cyy,"A");

	char* lpAxx = "Galones";
	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft1  = ggdMargenLeft;
	int iAncho1 = cx;

	lpAxx = "ValorRD$ ";
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft2  = iLeft1 + iAncho1 + 5*cxx;
	int iAncho2 = cx;

	//Correcion
	int ixx = (ggdAnchoPag - (iAncho1 + iAncho2 + 5*cxx)) / 2;
	iLeft1 += ixx;
	iLeft2 += ixx;

	int iTop1 = ggdLinea;

	lpAxx = "Galones";
	lpAxx = "Galones";w_ShowTexto(layout,iLeft1,ggdLinea,lpAxx);
	lpAxx = "ValorRD$";w_ShowTexto(layout,iLeft2,ggdLinea,lpAxx);
	ggdLinea += cyy;

	int iTop2 = ggdLinea;

	w_hLinea(iLeft1-2,iLeft2+iAncho2+2,iTop1,0.5);
	w_hLinea(iLeft1-2,iLeft2+iAncho2+2,iTop2,0.5);

	g_object_unref(layout);

	double dPrecio = w_GetPrecio();

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 16");

	for(int i=1;i<31;i++)
	{
		char cGalones[32],cValor[32];
		snprintf(cGalones,32,"%d",i);
		snprintf(cValor,32,"%0.2f",i*dPrecio);

		w_GetSize(layout,&cx,&cy,cGalones);
		int iLeft = iLeft1 + (iAncho1 - cx);
		w_ShowTexto(layout,iLeft,ggdLinea,cGalones);

		w_GetSize(layout,&cx,&cy,cValor);
		int iLeftx = iLeft2 + (iAncho2 - cx);
		w_ShowTexto(layout,iLeftx,ggdLinea,cValor);

		w_hLinea(iLeft1,iLeft2+iAncho2,ggdLinea,0.2);

		ggdLinea += cy;

//		if((ggdLinea) >= ggdHeight) break;
//		if(i >= 15) break;

//		printf("%d\r",i);
	}


	g_object_unref(layout);
}

void MyPrintGls::w_PorValor()
{

	PangoLayout* layout;

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 16");

	int cxx,cyy;
	w_GetSize(layout,&cxx,&cyy,"A");

//	char* lpAxx = "Galones";
	char* lpAxx = "ValorRD$ ";
	int cx,cy;
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft1  = ggdMargenLeft;
	int iAncho1 = cx;

//	lpAxx = "ValorRD$ ";
	lpAxx = "Galones";
	w_GetSize(layout,&cx,&cy,lpAxx);
	int iLeft2  = iLeft1 + iAncho1 + 5*cxx;
	int iAncho2 = cx;

	//Correcion
	int ixx = (ggdAnchoPag - (iAncho1 + iAncho2 + 5*cxx)) / 2;
	iLeft1 += ixx;
	iLeft2 += ixx;

	int iTop1 = ggdLinea;

	lpAxx = "Galones";
	lpAxx = "ValorRD$";w_ShowTexto(layout,iLeft1,ggdLinea,lpAxx);
	lpAxx = "Galones";w_ShowTexto(layout,iLeft2,ggdLinea,lpAxx);
//	lpAxx = "ValorRD$";w_ShowTexto(layout,iLeft2,ggdLinea,lpAxx);
	ggdLinea += cyy;

	int iTop2 = ggdLinea;

	w_hLinea(iLeft1-2,iLeft2+iAncho2+2,iTop1,0.5);
	w_hLinea(iLeft1-2,iLeft2+iAncho2+2,iTop2,0.5);

	g_object_unref(layout);

	double dPrecio = w_GetPrecio();

	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	w_SetFont(layout,"sans 16");

	double dValor = 0.0;
	for(int i=1;i<46;i++)
	{
		dValor += 50.00;
		double dGalones = dValor / dPrecio;

		char cGalones[32],cValor[32];
		snprintf(cGalones,32,"%0.3f",dGalones);
		snprintf(cValor,32,"%0.2f",dValor);

//		w_GetSize(layout,&cx,&cy,cGalones);
//		int iLeft = iLeft1 + (iAncho1 - cx);
//		w_ShowTexto(layout,iLeft,ggdLinea,cGalones);
//
//		w_GetSize(layout,&cx,&cy,cValor);
//		int iLeftx = iLeft2 + (iAncho2 - cx);
//		w_ShowTexto(layout,iLeftx,ggdLinea,cValor);

		w_GetSize(layout,&cx,&cy,cValor);
		int iLeft = iLeft1 + (iAncho1 - cx);
		w_ShowTexto(layout,iLeft,ggdLinea,cValor);

		w_GetSize(layout,&cx,&cy,cGalones);
		int iLeftx = iLeft2 + (iAncho2 - cx);
		w_ShowTexto(layout,iLeftx,ggdLinea,cGalones);

		w_hLinea(iLeft1,iLeft2+iAncho2,ggdLinea,0.2);

		ggdLinea += cy;

//		if((ggdLinea) >= ggdHeight) break;
//		if(i >= 15) break;

//		printf("%d\r",i);
	}


	g_object_unref(layout);
}

void MyPrintGls::w_AstericoFinal()
{
	PangoLayout* layout;

	int cx,cy;
	layout = gtk_print_context_create_pango_layout((GtkPrintContext*)vContext);
	char* lpAxx = "*";
	w_SetFont(layout,"sans 10");
	w_GetSize(layout,&cx,&cy,lpAxx);
	ggdLinea += 2*cy;
	w_ShowTexto(layout,ggdMargenLeft,ggdLinea,lpAxx);

	g_object_unref(layout);
}

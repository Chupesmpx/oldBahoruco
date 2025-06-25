/*
 * DlgAcerca.cpp
 *
 *  Created on: 15/11/2012
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

#include "DlgAcerca.h"

DlgAcerca::DlgAcerca()
{

}

DlgAcerca::~DlgAcerca()
{

}

void DlgAcerca::w_Create(void* vxParent)
{

	GtkWidget* pWidget =  gtk_dialog_new_with_buttons(
							"Acerca",
							(GtkWindow*)vxParent,
						    //GTK_DIALOG_DESTROY_WITH_PARENT,
						    GTK_DIALOG_MODAL,
//						    GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
						    GTK_STOCK_OK,  GTK_RESPONSE_OK,
//						    GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
						    NULL);

	GdkColor clx = {0,65535,65535,65535};
	gtk_widget_modify_bg(pWidget,GTK_STATE_NORMAL,&clx);


	w_Controles(pWidget);

//	gtk_widget_set_usize (pWidget,330,250);
	gtk_widget_set_usize (pWidget,400,320);

	gint result = gtk_dialog_run(( GtkDialog*)pWidget);
	if(result == GTK_RESPONSE_OK)
	{

	}
	//if(result == GTK_RESPONSE_CANCEL) break;

	gtk_widget_destroy(pWidget);

}

void DlgAcerca::w_Controles(void* voidDlg)
{

	//PangoContext *      gtk_widget_create_pango_context     (GtkWidget *widget);
	//PangoContext *      gtk_widget_get_pango_context        (GtkWidget *widget);
//	gtk_label_set_attributes(GtkLabel *label,PangoAttrList *attrs);

	GtkWidget* pLabel1 = gtk_label_new("Bahoruco Gas");
//	gtk_label_set_use_underline((GtkLabel*)pLabel1,1);
//	gtk_widget_modify_font (pLabel1,pango_font_description_from_string ("Monospace Oblique bold 16"));

	gtk_label_set_markup((GtkLabel*)pLabel1, "<span font='16' weight='bold' style='Oblique' underline='double' foreground='#ff0000'>Bahoruco Gas</span>");
	gtk_widget_show (pLabel1);


	char* lpDirectory = getenv("directorio");
	char cImagen[1024];
//	snprintf(cImagen,sizeof(cImagen),"%s/Imagen/otro.png",lpDirectory);
	snprintf(cImagen,sizeof(cImagen),"%s/Imagen/logo1.png",lpDirectory);
	GtkWidget* pImagen = gtk_image_new_from_file(cImagen);
	gtk_widget_show (pImagen);


	GtkWidget* pLink1 = gtk_link_button_new_with_label("http://www.bahorucogas.com","Bahoruco Gas");
//	gtk_button_set_relief((GtkButton*)pLink1,GTK_RELIEF_NONE);
//	gtk_widget_set_usize (pLink1,-1,40);
	gtk_widget_show (pLink1);
	GtkWidget* pCajal = gtk_hbox_new (1, 0);
	gtk_box_pack_start (GTK_BOX (pCajal), pLink1, false, false, 0);



	GtkWidget* pCajax= gtk_hbox_new (false, 4);

	GtkWidget* pCaja1 = gtk_vbox_new (0, 20);
	gtk_box_pack_start (GTK_BOX (pCaja1), pLabel1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pImagen, false, false, 0);
//	gtk_box_pack_start (GTK_BOX (pCaja1), pLink1, false, false, 0);
	gtk_box_pack_start (GTK_BOX (pCaja1), pCajal, false, false, 0);

//	gtk_container_add (GTK_CONTAINER (pCaja1), pCajal);
	gtk_container_add (GTK_CONTAINER (pCajax), pCaja1);

//	gtk_container_set_border_width( GTK_CONTAINER (pCaja1), 20 );
	gtk_container_set_border_width( GTK_CONTAINER (pCajax), 20 );

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidDlg)->vbox), pCajax );


	gtk_widget_show (pCajal);
	gtk_widget_show (pCaja1);
	gtk_widget_show (pCajax);

}

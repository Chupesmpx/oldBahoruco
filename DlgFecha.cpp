/*
 * DlgFecha.cpp
 *
 *  Created on: 18/09/2012
 *      Author: vrafael
 */
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <shlwapi.h>
	#include <gtk/gtk.h>
#else
	#include <gtk-2.0/gtk/gtk.h>
#endif


#include "main.h"
#include "DlgFecha.h"
#include "myfecha.h"
#include "CargarDet.h"

void on_DlgFechaDia(GtkCalendar *calendar,gpointer data)
{
	DlgFecha* pDlg = (DlgFecha*) data;
	pDlg->w_Save();
}


DlgFecha::DlgFecha()
{

}

DlgFecha::~DlgFecha()
{

}

bool DlgFecha::w_Create(void* vxParent,void* vxEdit)
{
	voidEditExt = vxEdit;

	ggbRetorno = false;
	GtkWidget* pWidget =  gtk_dialog_new_with_buttons(
							"Fecha",
							(GtkWindow*)vxParent,
						    //GTK_DIALOG_DESTROY_WITH_PARENT,
						    GTK_DIALOG_MODAL,
//						    GTK_STOCK_APPLY, GTK_RESPONSE_APPLY,
//						    GTK_STOCK_OK,  GTK_RESPONSE_OK,
//						    GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
						    NULL);

	w_Controles(pWidget,vxEdit);

//	gtk_widget_set_usize (pWidget,540,370);

	gint result = gtk_dialog_run(( GtkDialog*)pWidget);
	if(result == GTK_RESPONSE_OK)
	{
		w_Save();
	}
	//if(result == GTK_RESPONSE_CANCEL) break;

	gtk_widget_destroy(pWidget);

	return ggbRetorno;
}

void DlgFecha::w_Controles(void* voidDlg,void* vxEdit)
{

	char* lpFecha = w_GetFecha();
	if(lpFecha == NULL) return;

	MyFecha myFecha;
	int iAno,iMes,iDia;
	myFecha.w_Get_iFecha(atoi(lpFecha),&iAno,&iMes,&iDia);

	GtkWidget* pDate = gtk_calendar_new();
	voidFecha = pDate;

	guint iuAno = iAno;
	guint iuMes = iMes - 1;
	guint iuDia = iDia;
	gtk_calendar_select_month((GtkCalendar*)pDate,iuMes,iuAno);
	gtk_calendar_select_day((GtkCalendar*)pDate,iuDia);

	gtk_widget_show(pDate);

	gtk_container_add( GTK_CONTAINER (GTK_DIALOG (voidDlg)->vbox), pDate );

	gtk_signal_connect(GTK_OBJECT(pDate),"day-selected", GTK_SIGNAL_FUNC(on_DlgFechaDia), this);
}

void DlgFecha::w_Save()
{
	guint iAno,iMes,iDia;
	gtk_calendar_get_date((GtkCalendar*)voidFecha,&iAno,&iMes,&iDia);
	iMes++;

	MyFecha myFecha;
	char cFechaED[32],cFechaED2[32];
	int iFecha = myFecha.w_GetFechaInt(iAno,iMes,iDia);
	snprintf(cFechaED2,32,"%04d%02d%02d",iAno,iMes,iDia);
	myFecha.w_GetFechaEd2(iFecha,cFechaED,sizeof(cFechaED));
	gtk_entry_set_text((GtkEntry*)voidEditExt,cFechaED);
	w_SetFecha(cFechaED2);
	w_SetFechaEd(cFechaED);

	CargarDet cargarDet;
	cargarDet.w_RefreshDetalle();

}

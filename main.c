// main.c

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
#include "Ventana.h"
#include "MenuMain.h"
#include "Statusbar.h"
#include "Toolbar.h"
#include "MyFijo.h"
#include "myworklv.h"
#include "MyListview.h"
#include "FileBonoGas.h"



	void* pMain          = NULL;
	void* voidMyStatus   = NULL;
	void* voidMyToolbar  = NULL;
	void* voidMyListview = NULL;
	void* voidMyMenu     = NULL;

	char*  ggbuffFecha   = NULL;
	char*  ggbuffFechaEd = NULL;
	double ggdPrecio     = 0.00;

void w_SetStatusbar(char* lpxTexto)
{

//	void* voidStatus = NULL;
//	char* lpwPuntero = getenv("punteroStatus");
//	memcpy(&voidStatus,(void*)lpwPuntero,4);

	Statusbar* pStatus = (Statusbar*) voidMyStatus;
	pStatus->w_PushItem(lpxTexto);

}


int main(int argc, char **argv)
{
	setlocale(LC_NUMERIC, "en_US");

	char cDirectory[2048];
	memset(cDirectory,0,sizeof(cDirectory));
	if(!w_GetDirectory(cDirectory,sizeof(cDirectory))) return 0;


#ifdef _WIN32
	long lenBuff  = 12 + strlen(cDirectory);
	char* buffEnv = (char*)malloc(lenBuff);
	if(buffEnv == NULL) return 0;
	snprintf(buffEnv,lenBuff,"directorio=%s",cDirectory);
	putenv(buffEnv);
	free(buffEnv);
#else
	setenv("directorio",cDirectory,1);
#endif


	gtk_init(&argc, &argv);

//	//Acceso
//	{
//		MyAccesodl myAccesodl;
//		if(!myAccesodl.w_Cargar(cDirectory)) {myAccesodl.w_Close();return 0;}
//		if(!myAccesodl.w_Acceso(NULL,cDirectory)){myAccesodl.w_Close();return 0;}
//		myAccesodl.w_Close();
//	}

	Ventana ventana;
	void* pVentana = ventana.w_Create();
	if(pVentana == NULL) return 0;
	pMain = pVentana;

	GtkWidget* pContenedor  = gtk_vbox_new (0, 0);

	//Menu
	MenuMain menuMain;
	voidMyMenu = &menuMain;
	void* pMenu = menuMain.w_Create();
	gtk_box_pack_start (GTK_BOX (pContenedor), (GtkWidget*)pMenu, 0, 0, 0);

	//ToolBar
	Toolbar toolbar;
	voidMyToolbar = &toolbar;
	void* voidContTool = toolbar.w_Create();
	gtk_box_pack_start (GTK_BOX (pContenedor), (GtkWidget*)voidContTool, 0, 0, 0);

	//ListView
	MyListview myListview;
	voidMyListview = &myListview;
	void* pContLV = myListview.w_Create();
//	gtk_box_pack_start (GTK_BOX (pContenedor), (GtkWidget*)pContLV, 0, 0, 0);
	gtk_container_add (GTK_CONTAINER (pContenedor),(GtkWidget*)pContLV);


	//Statusbar
	Statusbar statusbar;
	voidMyStatus = &statusbar;
	void* pStatus = statusbar.w_Create();
	gtk_box_pack_end (GTK_BOX (pContenedor),(GtkWidget*)pStatus, false, false, 0);


	gtk_container_add (GTK_CONTAINER (pVentana), pContenedor);
	gtk_widget_show(pContenedor);



	gtk_main();


	MyFijo myFijo;
	myFijo.w_SetFijo();

	if(ggbuffFecha != NULL) free(ggbuffFecha);ggbuffFecha=NULL;
	if(ggbuffFechaEd != NULL) free(ggbuffFechaEd);ggbuffFechaEd=NULL;

	return 0;
}

int w_getMin(int x,int y)
{
	if(x == y || x < y) return x;
	return y;
}
bool w_GetDirectory(char* lpxBuff,long lxBuff)
{

#ifdef _WIN32
	char cAxx[1024];
	GetModuleFileName(NULL, cAxx, 1024);

	long lAxx = strlen(cAxx);
	for(int i=(lAxx-1);i>0;i--)
	{
		if(memcmp(cAxx+i,"\\",1) == 0)
		{
			memcpy(cAxx+i,"\0",1);
			break;
		}
	}

	memcpy(lpxBuff,cAxx,strlen(cAxx));
	return 1;
#else
	if(lpxBuff == NULL) return 0;
	memset(lpxBuff,0,lxBuff);

	char szTmp[32],cBuff[2048];
	long len = sizeof(cBuff);
	sprintf(szTmp, "/proc/%d/exe", getpid());
//	int bytes = MIN(readlink(szTmp, cBuff, len), len - 1);
	int bytes = w_getMin(readlink(szTmp, cBuff, len), len - 1);
	if(bytes >= 0) cBuff[bytes] = '\0';

	for(int i=bytes;i>0;i--)
	{
		if(memcmp(cBuff+i,"/",1) == 0)
		{
			cBuff[i] = '\0';
			break;
		}
	}

	memcpy(lpxBuff,cBuff,strlen(cBuff));

	return 1;

#endif

}

void w_MessageBox_main(char* lpxMsg)
{
	 GtkWidget* dialog = gtk_message_dialog_new
	 					(
						 GTK_WINDOW(pMain),
						 GTK_DIALOG_DESTROY_WITH_PARENT,
						 GTK_MESSAGE_INFO,
						 GTK_BUTTONS_OK,
						 lpxMsg
						 );

	 gint ixx = gtk_dialog_run (GTK_DIALOG (dialog));
	 gtk_widget_destroy (dialog);

}

char* w_GetFecha()
{
	if(ggbuffFecha == NULL)
	{
		ggbuffFecha = (char*)malloc(32);
		if(ggbuffFecha == NULL) return "19990101";
		memset(ggbuffFecha,0,32);
		memcpy(ggbuffFecha,"20121101",8);
	}
	return ggbuffFecha;
}
char* w_GetFechaEd()
{
	if(ggbuffFechaEd == NULL)
	{
		ggbuffFechaEd = (char*)malloc(32);
		if(ggbuffFechaEd == NULL) return "01-01-1999";
		memset(ggbuffFechaEd,0,32);
		memcpy(ggbuffFechaEd,"01-01-1999",10);
	}
	return ggbuffFechaEd;
}

void w_SetFecha(char* xbuffFecha)
{
	if(xbuffFecha == NULL) return;
	long lxx = strlen(xbuffFecha);
	if(lxx != 8) return;
	if(ggbuffFecha == NULL) w_GetFecha();
	memset(ggbuffFecha,0,32);
	memcpy(ggbuffFecha,xbuffFecha,lxx);
}
void w_SetFechaEd(char* xbuffFecha)
{
	if(xbuffFecha == NULL) return;
	long lxx = strlen(xbuffFecha);
//	if(lxx != 8) return;
	if(ggbuffFechaEd == NULL) w_GetFechaEd();
	memset(ggbuffFechaEd,0,32);
	memcpy(ggbuffFechaEd,xbuffFecha,lxx);
}

double w_GetPrecio()
{
	w_SetPrecio();
	return ggdPrecio;
}
void w_SetPrecio()
{
	Toolbar* pTool = (Toolbar*) voidMyToolbar;
	void* vEdit = pTool->voidEditPrecio;
	if(vEdit == NULL) return;

	char* precio = (char*)gtk_entry_get_text((GtkEntry*)vEdit);

	MyWorkLV wxx;
	wxx.w_ParseDouble(precio,-1,0);

	ggdPrecio = atof(precio);
}

long w_getFechaActual()
{
	int iDia,iMes,iAno;
	time_t txx = time(NULL);
	struct tm* tww = localtime(&txx);
	iDia = tww->tm_mday;
	iMes = tww->tm_mon;
	iAno = tww->tm_year + 1900;
	iMes++;

	char cFecha[32];
	snprintf(cFecha,sizeof(cFecha),"%04d%02d%02d",iAno,iMes,iDia);

	return atol(cFecha);
}

void w_getHoraActual(char* cHora,char* cMinutos,char* cSegundos)
{

	int iHora,iMin,iSeg;
	time_t txx = time(NULL);
	struct tm* tww = localtime(&txx);
	*cHora     = tww->tm_hour;
	*cMinutos  = tww->tm_min;
	*cSegundos = tww->tm_sec;

}

double w_GetBonogas()
{
	double dBonoGas = 0.00;
	FileBonoGas fxx;
	if(!fxx.w_Read(&dBonoGas)) return 0.00;
	return dBonoGas;
}
void w_SaveBonogas(double dBonoGas)
{
	FileBonoGas fxx;
	fxx.w_Write(dBonoGas);
}

void* w_GetMenu(){return voidMyMenu;}
void* w_GetListview(){return voidMyListview;}
void* w_GetMain(){return pMain;}

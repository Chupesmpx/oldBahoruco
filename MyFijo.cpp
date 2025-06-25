/*
 * MyFijo.cpp
 *
 *  Created on: 11/11/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include "main.h"
#include "MyFijo.h"


MyFijo::MyFijo()
{

}

MyFijo::~MyFijo()
{

}

void MyFijo::w_SetFileName()
{
	memset(ggcFileName,0,sizeof(ggcFileName));

	char* buffDirectory = getenv("directorio");

	if(buffDirectory == NULL) return;
	snprintf(ggcFileName,sizeof(ggcFileName),"%s/data/gasFijo",buffDirectory);
}

bool MyFijo::w_Read(void* vstruct)
{
	w_SetFileName();
	FILE* wfile = fopen(ggcFileName,"r");
	if(wfile == NULL) return 0;
	fread(vstruct,sizeof(GASFIJO1),1,wfile);
	fclose(wfile);
	return 1;
}
void MyFijo::w_Write(void* vstruct)
{
	w_SetFileName();
	FILE* wfile = fopen(ggcFileName,"w");
	if(wfile == NULL) return;
	fwrite(vstruct,sizeof(GASFIJO1),1,wfile);
	fclose(wfile);
}

void MyFijo::w_SetFijo()
{
	setlocale(LC_NUMERIC, "en_US");

	GASFIJO1 st;
	memset(&st,0,sizeof(st));
	w_Read(&st);
	char* lpFecha   = w_GetFecha();
	char* lpFechaEd = w_GetFechaEd();
	memcpy(st.cFecha,  lpFecha,  strlen(lpFecha));
	memcpy(st.cFechaEd,lpFechaEd,strlen(lpFechaEd));
	st.dPrecio = w_GetPrecio();
	w_Write(&st);
}

bool MyFijo::w_GetFijo(char* buffFecha,char* buffFechaEd,double* dPrecio)
{
	setlocale(LC_NUMERIC, "en_US");

	GASFIJO1 st;
	memset(&st,0,sizeof(st));
	if(!w_Read(&st)) return 0;
	if(buffFecha   != NULL) memcpy(buffFecha,  st.cFecha,  strlen(st.cFecha)+1);
	if(buffFechaEd != NULL) memcpy(buffFechaEd,st.cFechaEd,strlen(st.cFechaEd)+1);
	if(dPrecio     != NULL) *dPrecio = st.dPrecio;
	return 1;

}

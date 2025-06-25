/*
 * MyData.cpp
 *
 *  Created on: 11/11/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <locale.h>


#include "main.h"
#include "MyData.h"

MyData::MyData()
{

	long p = 0;
	gglEliminado = p;p++;
	gglNula      = p;p++;
	gglPrint     = p;p++;
	gglHora      = p;p++;
	gglMinutos   = p;p++;
	gglSegundos  = p;p++;
	gglPos       = p;p+=4;
	gglTicket    = p;p+=4;
	gglFecha     = p;p+=4;
	gglPrecio    = p;p+=8;
	gglGalones1  = p;p+=8;
	gglGalones2  = p;p+=8;
	gglGalones3  = p;p+=8;
	gglTotal1    = p;p+=8;
	gglTotal2    = p;p+=8;
	gglTotal3    = p;p+=8;
	gglEfectivo  = p;p+=8;
	gglCambio    = p;p+=8;

	gglRecord = p;


	buffData     = NULL;
	buffNameFile = NULL;
}

MyData::~MyData()
{
	w_Clear();
}
void MyData::w_Clear()
{
	if(buffData != NULL) free(buffData);buffData=NULL;
	if(buffNameFile != NULL) free(buffNameFile);buffNameFile=NULL;
}

#ifdef _WIN32
	bool MyData::w_GetFileName(char* buffFileName,long lFileName)
	{
		if(buffFileName == NULL || lFileName < 7) return 0;

		memset(buffFileName,0,lFileName);

		if(buffNameFile == NULL)
		{
			char* buffDirectory = getenv("directorio");
			if(buffDirectory == NULL) return 0;
			char* buffAxx = "%s\\data\\gas.dat";
			long lAxx = strlen(buffDirectory) + strlen(buffAxx);
			buffNameFile = (char*)malloc(lAxx+1);
			if(buffNameFile == NULL) return 0;
			memset(buffNameFile,0,lAxx+1);
			snprintf(buffNameFile,lAxx,buffAxx,buffDirectory);
		}

		long lFile = strlen(buffNameFile);
		if(lFile >= lFileName) return 0;
		memcpy(buffFileName,buffNameFile,lFile);

		return 1;
	}
#else
	bool MyData::w_GetFileName(char* buffFileName,long lFileName)
	{
		if(buffFileName == NULL || lFileName < 7) return 0;

		memset(buffFileName,0,lFileName);

		if(buffNameFile == NULL)
		{
			char* buffDirectory = getenv("directorio");
			if(buffDirectory == NULL) return 0;
			char* buffAxx = "%s/data/gas.dat";
			long lAxx = strlen(buffDirectory) + strlen(buffAxx);
			buffNameFile = (char*)malloc(lAxx+1);
			if(buffNameFile == NULL) return 0;
			memset(buffNameFile,0,lAxx+1);
			snprintf(buffNameFile,lAxx,buffAxx,buffDirectory);
		}

		long lFile = strlen(buffNameFile);
		if(lFile >= lFileName) return 0;
		memcpy(buffFileName,buffNameFile,lFile);

		return 1;
	}
#endif

bool MyData::w_Create()
{
	w_Clear();

	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return 0;

	FILE* wfile = fopen(cFileName,"w");
	if(wfile == NULL) return 0;
	fclose(wfile);
	return 1;

}

bool MyData::w_Read(long lFecha,char** buffRet)
{
	char cEliminado;

	w_Clear();

	if(buffRet == NULL) return 0;

	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return 0;

	char* buffAxx = (char*)malloc(gglRecord);
	if(buffAxx == NULL) return 0;

	FILE* wfile = fopen(cFileName,"rb");
	if(wfile == NULL) {free(buffAxx);return 0;}

	//Cant Records
	long lCant = 0;
	while(1)
	{
		if(fread(buffAxx,gglRecord,1,wfile) < 1) break;

		memcpy(&cEliminado,buffAxx+gglEliminado,1);
		if(cEliminado > 0) continue;

		long lwFecha;
		memcpy(&lwFecha,buffAxx+gglFecha,4);
		if(lwFecha != lFecha) continue;

		lCant++;
	}
	if(lCant < 1)
	{
		fclose(wfile);
		free(buffAxx);
		return 1;
	}

	long lData = lCant * gglRecord;
	buffData = (char*)malloc(lData);
	if(buffData == NULL) {free(buffAxx);fclose(wfile);return 0;}

	long p = 0;
	long pww = 0;
	fseek(wfile,0,SEEK_SET);
	while(1)
	{
		if(fread(buffAxx,gglRecord,1,wfile) < 1) break;

		memcpy(&cEliminado,buffAxx+gglEliminado,1);
		if(cEliminado > 0) {pww+=gglRecord;continue;}

		long lwFecha;
		memcpy(&lwFecha,buffAxx+gglFecha,4);
		if(lwFecha != lFecha) {pww+=gglRecord;continue;}

		if((p + gglRecord) > lData) break;
		memcpy(buffAxx+gglPos,&pww,4);
		memcpy(buffData+p,buffAxx,gglRecord);

		p+=gglRecord;
		pww+=gglRecord;

	}

	fclose(wfile);
	free(buffAxx);

	long lRet = 12 + lData;

	*buffRet = (char*)malloc(lRet);
	if(*buffRet == NULL) return 0;

	p = 0;
	memcpy(*buffRet+p,&gglRecord,4);p+=4;//Long del Record
	memcpy(*buffRet+p,&lCant,4);p+=4;//Cantidad de Records
	memcpy(*buffRet+p,&lData,4);p+=4;//Long efectiva
	memcpy(*buffRet+p,buffData,lData);p+=lData;

	return 1;
}


long MyData::w_GetUltTicket()
{
	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return -1;

	FILE* wfile = fopen(cFileName,"rb");
	if(wfile == NULL) return -1;

	char* buffAxx = (char*)malloc(gglRecord);
	if(buffAxx == NULL) return -1;

	long lRet = 0;
	while(feof(wfile) < 1)
	{
		if(fread(buffAxx,gglRecord,1,wfile) < 1) break;
		memcpy(&lRet,buffAxx+gglTicket,4);
	}

	fclose(wfile);
	free(buffAxx);

	return lRet;
}

bool MyData::w_Insert(long lFecha,
		double dPrecio,double dEfectivo,double dCambio,
		double dGalones1,double dGalones2,double dGalones3,
		double dTotal1,double dTotal2,double dTotal3)
{

	if(dGalones1 < 0.00001 && dGalones2 < 0.00001 && dGalones3 < 0.00001) return 0;

	long lTicket = w_GetUltTicket();
	if(lTicket < 0) return 0;
	lTicket++;

	char cHora;
	char cMinutos;
	char cSegundos;
	w_getHoraActual(&cHora,&cMinutos,&cSegundos);

//	char cTipo = (char)lTipo;

	char* buffAxx = (char*)malloc(gglRecord);
	if(buffAxx == NULL) return 0;
	memset(buffAxx,0,gglRecord);

	memcpy(buffAxx+gglHora,&cHora,1);
	memcpy(buffAxx+gglMinutos,&cMinutos,1);
	memcpy(buffAxx+gglSegundos,&cSegundos,1);

//	memcpy(buffAxx+gglTipo,&cTipo,1);

	memcpy(buffAxx+gglTicket,&lTicket,4);
	memcpy(buffAxx+gglFecha,&lFecha,4);

	memcpy(buffAxx+gglPrecio,&dPrecio,8);
	memcpy(buffAxx+gglEfectivo,&dEfectivo,8);
	memcpy(buffAxx+gglCambio,&dCambio,8);

	memcpy(buffAxx+gglGalones1,&dGalones1,8);
	memcpy(buffAxx+gglGalones2,&dGalones2,8);
	memcpy(buffAxx+gglGalones3,&dGalones3,8);
	memcpy(buffAxx+gglTotal1,&dTotal1,8);
	memcpy(buffAxx+gglTotal2,&dTotal2,8);
	memcpy(buffAxx+gglTotal3,&dTotal3,8);


	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return 0;

	FILE* wfile = fopen(cFileName,"rb+");
	if(wfile == NULL) return 0;

	fseek(wfile,0,SEEK_END);
//	*lRetPos = ftell(wfile);

	fwrite(buffAxx,gglRecord,1,wfile);
	fclose(wfile);

	return 1;

}

bool MyData::w_Anular(long lxPos)
{
	char cNulo;

	w_Clear();

	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return 0;

	char* buffAxx = (char*)malloc(gglRecord);
	if(buffAxx == NULL) return 0;

	FILE* wfile = fopen(cFileName,"rb");
	if(wfile == NULL) {free(buffAxx);return 0;}

	fseek(wfile,lxPos,SEEK_SET);

	if(fread(buffAxx,gglRecord,1,wfile) < 1)
	{
		fclose(wfile);
		free(buffAxx);return 0;
	}
	fclose(wfile);

	memcpy(&cNulo,buffAxx+gglNula,1);
	if(cNulo == 1) {cNulo = 0;}
	else {cNulo = 1;}
	memcpy(buffAxx+gglNula,&cNulo,1);

	wfile = fopen(cFileName,"rb+");
	if(wfile == NULL) {free(buffAxx);return 0;}

	fseek(wfile,lxPos,SEEK_SET);

	fwrite(buffAxx,gglRecord,1,wfile);

	fclose(wfile);

	return 1;
}

bool MyData::w_AddPrint(long lxPos)
{
	char cPrint;

	w_Clear();

	char cFileName[1024];
	if(!w_GetFileName(cFileName,sizeof(cFileName))) return 0;

	char* buffAxx = (char*)malloc(gglRecord);
	if(buffAxx == NULL) return 0;

	FILE* wfile = fopen(cFileName,"rb");
	if(wfile == NULL) {free(buffAxx);return 0;}

	fseek(wfile,lxPos,SEEK_SET);

	if(fread(buffAxx,gglRecord,1,wfile) < 1)
	{
		fclose(wfile);
		free(buffAxx);return 0;
	}
	fclose(wfile);

	memcpy(&cPrint,buffAxx+gglPrint,1);
	cPrint++;
	memcpy(buffAxx+gglPrint,&cPrint,1);

	wfile = fopen(cFileName,"rb+");
	if(wfile == NULL) {free(buffAxx);return 0;}

	fseek(wfile,lxPos,SEEK_SET);

	fwrite(buffAxx,gglRecord,1,wfile);

	fclose(wfile);

	return 1;
}

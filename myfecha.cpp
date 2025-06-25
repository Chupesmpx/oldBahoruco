//      myfecha.cpp
//      

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myfecha.h"


MyFecha::MyFecha()
{
	
}


MyFecha::~MyFecha()
{
	
}

long MyFecha::w_GetDiaMaximo(long lxAno,long lxMes)
{
	if(lxMes < 1 )  return 1;
	if(lxMes > 12 ) return 31;
	int vDia[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	long lxx = lxAno / 4;
	if((lxx * 4) == lxAno) vDia[2]++;
	return vDia[lxMes];
}

void MyFecha::w_GetFechaEd(char* lpxFecha,long lxFecha,long lxAno,long lxMes,long lxDia)
{
	if(lpxFecha == NULL || lxFecha < 1) return;
	memset(lpxFecha,0,lxFecha);
	if(lxFecha < 12) return;
	if(lxMes < 1 || lxMes > 12) return;
	
	long lMaxDia = w_GetDiaMaximo(lxAno,lxMes);
	if(lxDia < 1) lxDia = 1;
	if(lxDia > lMaxDia) lxDia = lMaxDia;
	
	char* vMes[] =  {"","ENE","FEB","MAR","ABR","MAY","JUN","JUL","AGO","SEP","OCT","NOV","DIC"};
	snprintf(lpxFecha,lxFecha,"%02d/%s/%04d",lxDia,vMes[lxMes],lxAno);
	
}
void MyFecha::w_GetFechaEd2(int ixFecha,char* lpxFecha,long lxFecha)
{
	int iAno,iMes,iDia;
	memset(lpxFecha,0,lxFecha);
	if(!w_Get_iFecha(ixFecha,&iAno,&iMes,&iDia)) return;
	w_GetFechaEd(lpxFecha,lxFecha,iAno,iMes,iDia);
}
void MyFecha::w_GetHoraEd(char* lpInputHora,char* lpxHora,long lxHora,bool bxSegundos)
{
	char cHora[8],cMin[8],cSeg[8];
	int iHora,iMin,iSeg;

	if(lpxHora == NULL) return;
	memset(lpxHora,0,lxHora);
	if(lpInputHora == NULL) return;
	if(strlen(lpInputHora) < 6) return;

	memset(cHora,0,8);memset(cMin,0,8);memset(cSeg,0,8);
	memcpy(cHora,lpInputHora,2);
	memcpy(cMin,lpInputHora+2,2);
	memcpy(cSeg,lpInputHora+4,2);

	iHora=atoi(cHora);iMin=atoi(cMin);iSeg=atoi(cSeg);

	char* lpam = "am";
	if(iHora > 12) {iHora -= 12;lpam="pm";}

	if(bxSegundos)
	{
		snprintf(lpxHora,lxHora,"%d:%02d:%02d %s",iHora,iMin,iSeg,lpam);
	}
	else
	{
		snprintf(lpxHora,lxHora,"%d:%02d %s",iHora,iMin,lpam);
	}
}

int MyFecha::w_GetFechaInt(long lxAno,long lxMes,long lxDia)
{

	if(lxMes < 1 || lxMes > 12) return -1;
	
	long lMaxDia = w_GetDiaMaximo(lxAno,lxMes);
	if(lxDia < 1) return -1;
	if(lxDia > lMaxDia) return -1;
	
	char cAxx[16];
	snprintf(cAxx,sizeof(cAxx),"%04d%02d%02d",lxAno,lxMes,lxDia);
	return atoi(cAxx);
}

bool MyFecha::w_Get_iFecha(int ixFecha,int* ixAno,int* ixMes,int* ixDia)
{
	char cAxx[32];
	snprintf(cAxx,sizeof(cAxx),"%d",ixFecha);
	if(strlen(cAxx) != 8) return 0;
	
	char cAno[8],cMes[8],cDia[8];
	memset(cAno,0,8);memset(cMes,0,8);memset(cDia,0,8);
	memcpy(cAno,cAxx,4);
	memcpy(cMes,cAxx+4,2);
	memcpy(cDia,cAxx+6,2);
	
	if(ixAno != NULL)
	{
		int iAno = atoi(cAno);
		if(iAno < 1990 || iAno > 3000) return 0;
		*ixAno = iAno;
	}
	if(ixMes != NULL)
	{
		int iMes = atoi(cMes);
		if(iMes < 1 || iMes > 12) return 0;
		*ixMes = iMes;
	}
	if(ixDia != NULL)
	{
		int iDia = atoi(cDia);
		if(iDia < 1 || iDia > w_GetDiaMaximo(atoi(cAno),atoi(cMes))) return 0;
		*ixDia = iDia;
	}
	
	return 1;
}

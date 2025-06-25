/*
 * FileBonoGas.cpp
 *
 *  Created on: 13/11/2012
 *      Author: vrafael
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include "main.h"
#include "FileBonoGas.h"

FileBonoGas::FileBonoGas()
{

}

FileBonoGas::~FileBonoGas()
{

}

void FileBonoGas::w_SetFileName()
{
	memset(ggcFileName,0,sizeof(ggcFileName));

	char* buffDirectory = getenv("directorio");

	if(buffDirectory == NULL) return;
	snprintf(ggcFileName,sizeof(ggcFileName),"%s/data/BonoGas",buffDirectory);
}

bool FileBonoGas::w_Read(double* dBonogas)
{
	w_SetFileName();
	FILE* wfile = fopen(ggcFileName,"r");
	if(wfile == NULL) return 0;
	fread(dBonogas,8,1,wfile);
	fclose(wfile);
	return 1;
}
void FileBonoGas::w_Write(double dBonogas)
{
	w_SetFileName();
	FILE* wfile = fopen(ggcFileName,"w");
	if(wfile == NULL) return;
	fwrite(&dBonogas,8,1,wfile);
	fclose(wfile);
}

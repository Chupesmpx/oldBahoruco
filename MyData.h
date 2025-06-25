/*
 * MyData.h
 *
 *  Created on: 11/11/2012
 *      Author: vrafael
 */

#ifndef MYDATA_H_
#define MYDATA_H_

class MyData
{
public:
	MyData();
	virtual ~MyData();

	bool w_Insert(long lFecha,
			double dPrecio,double dEfectivo,double dCambio,
			double dGalones1,double dGalones2,double dGalones3,
			double dTotal1,double dTotal2,double dTotal3);


	bool w_Read(long lFecha,char** buffRet);
	void w_Clear();
	bool w_Create();
	bool w_Anular(long lxPos);
	bool w_AddPrint(long lxPos);

	long gglPos,gglEliminado,gglNula,gglPrint;
	long gglHora,gglMinutos,gglSegundos,gglTicket,gglFecha;
	long gglPrecio,gglEfectivo,gglCambio;
	long gglGalones1,gglGalones2,gglGalones3;
	long gglTotal1,gglTotal2,gglTotal3;


private:
	bool w_GetFileName(char* buffFileName,long lFileName);
	long w_GetUltTicket();

	long gglRecord;
	char* buffData;
	char* buffNameFile;

};

#endif /* MYDATA_H_ */

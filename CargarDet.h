/*
 * CargarDet.h
 *
 *  Created on: 12/11/2012
 *      Author: vrafael
 */

#ifndef CARGARDET_H_
#define CARGARDET_H_

class CargarDet
{
public:
	CargarDet();
	virtual ~CargarDet();

	void w_Cargar();
	void w_Clear();
	void w_CargarTicket();
	void w_RefreshDetalle();
	void w_SetPrint();
	void w_SetNula();
	void w_RefreshResumen();

private:

	void w_Resumen(long lCantTran,double vGalones[5],double vTotales[5],double dsEfectivo,double dsCambio);

	char* buffData;

};

#endif /* CARGARDET_H_ */

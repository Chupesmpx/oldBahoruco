/*
 * MyListview.h
 *
 *  Created on: 11/11/2012
 *      Author: vrafael
 */

#ifndef MYLISTVIEW_H_
#define MYLISTVIEW_H_

class MyListview
{
public:
	MyListview();
	virtual ~MyListview();

	enum TICKETS
	{
		TICK_POS,
		TICK_GLS,
		TICK_DESCRIP,
		TICK_TOTAL,
		TICK_ULT,
		TICK_COLUMNS
	};

	enum RESUMEN
	{
		RESUMEN_POS,
		RESUMEN_GLS,
		RESUMEN_DESCRIP,
		RESUMEN_TOTAL,
		RESUMEN_ULT,
		RESUMEN_COLUMNS
	};

	enum DETALLE
	{
	   DET_POS,
	   DET_IMAGEN,
	   DET_TICKET,
	   DET_FECHA,
	   DET_HORA,
	   DET_GLS,
	   DET_PRECIO,
	   DET_TOTAL,
	   DET_EFECTIVO,
	   DET_CAMBIO,
	   DET_GLS1,
	   DET_GLS2,
	   DET_GLS3,
	   DET_TOTAL1,
	   DET_TOTAL2,
	   DET_TOTAL3,
	   DET_NULA,
	   DET_PRINT,
	   DET_ULT,
	   DET_COLUMNS
	};


	void* w_Create();

	void w_CargaInicial();

	void* pTreeTickets;
	void* pTreeResumen;
	void* pTreeDetalle;

private:

	void* w_Tickects();
	void* w_Resumen();
	void* w_Detalle();

	void w_AddColumna(void* voidxTree,const char* lpxTitulo,int ixSubItem,float fxAlign=0.0,int ixAncho=0);
	void w_AddColumnaImag(void* voidxTree,int ixSubItem);

};

#endif /* MYLISTVIEW_H_ */

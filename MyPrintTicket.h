/*
 * MyPrintTicket.h
 *
 *  Created on: 26/11/2012
 *      Author: vrafael
 */

#ifndef MYPRINTTICKET_H_
#define MYPRINTTICKET_H_

class MyPrintTicket
{
public:
	MyPrintTicket();
	virtual ~MyPrintTicket();

	void w_Print();
	void w_DrawPrint();

	void* vContext;
	double ggdAnchoPag;
	double ggdMargenLeft;
	double ggdMargenTop;

private:
	void w_SetFont(void* pgxLayout,char* lpxFont);
	void w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto);
	void w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto);
	void w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble = false);
	void w_Linea(char* lpxGalones,char* lpxDescrip,char* lpxTotal);
	void w_hLinea(double dxLeft1,double dxLeft2,double dxTop);
	void w_vLinea(double dxTop1,double dxTop2,double dxLeft);

	void w_NoTicket();
	void w_Titulo();
	void w_Galones();
	void w_Fecha();
	void w_Cuadro();
	void w_Imagen();
	void w_PrecioValor();

	double ggdLinea;
	double ggdImagenTop;


};

#endif /* MYPRINTTICKET_H_ */

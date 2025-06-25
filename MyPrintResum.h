/*
 * MyPrintResum.h
 *
 *  Created on: 23/11/2012
 *      Author: vrafael
 */

#ifndef MYPRINTRESUM_H_
#define MYPRINTRESUM_H_

class MyPrintResum
{
public:
	MyPrintResum();
	virtual ~MyPrintResum();

	void w_Print();
	void w_DrawPrint();

	void* vContext;
	double dAnchoPag;
	double dMargenLeft;
	double dMargenTop;

private:
	void w_SetFont(void* pgxLayout,char* lpxFont);
	void w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto);
	void w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto);
	void w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble);
	void w_Linea(char* lpxGalones,char* lpxDescrip,char* lpxTotal);
	void w_hLinea(double dxLeft1,double dxLeft2,double dxTop);

	void w_Encabezado();
	void w_Detalle();
	void w_Lecturas();
	void w_Rayas();

//	double ggdInicial,ggdFinal;
	double ggdLinea;

	int ggiAncho1,ggiAncho2,ggiAncho3;
	char* gglpFont;

};

#endif /* MYPRINTRESUM_H_ */

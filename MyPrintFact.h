/*
 * MyPrintFact.h
 *
 *  Created on: 26/11/2012
 *      Author: vrafael
 */

#ifndef MYPRINTFACT_H_
#define MYPRINTFACT_H_

class MyPrintFact
{
public:
	MyPrintFact();
	virtual ~MyPrintFact();

	void w_Print(char* lpxNameClte = NULL);
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

	void w_Encabezado();
	void w_Detalle();
	void w_Detalle2(int ixLeft1,int ixLeft2,int ixLeft3,int ixAncho1,int ixAncho2,int ixAncho3);
	void w_Total(int ixLeft1,int ixLeft2,int ixLeft3,int ixAncho1,int ixAncho2,int ixAncho3);
	void w_Rayas();

	double ggdLinea;
	char ggcNameClte[64];

};

#endif /* MYPRINTFACT_H_ */

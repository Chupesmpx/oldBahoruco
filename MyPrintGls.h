/*
 * MyPrintGls.h
 *
 *  Created on: 26/11/2012
 *      Author: vrafael
 */

#ifndef MYPRINTGLS_H_
#define MYPRINTGLS_H_

class MyPrintGls
{
public:
	MyPrintGls();
	virtual ~MyPrintGls();

	void w_Print(bool bxPorValor = false);
	void w_DrawPrint();
//	void w_DrawPrint2();

	void* vOperation;
	void* vContext;

	double ggdAnchoPag;
	double ggdMargenLeft;
	double ggdMargenTop;
	double ggdHeight;

private:
	void w_SetFont(void* pgxLayout,char* lpxFont);
	void w_GetSize(void* pgxLayout,int* cx,int* cy,char* lpxTexto);
	void w_ShowTexto(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto);
	void w_ShowTextoUnderLine(void* pgxLayout,double dxLeft,double dxTop,char* lpxTexto,bool bxDoble = false);
	void w_Linea(char* lpxGalones,char* lpxDescrip,char* lpxTotal);
	void w_hLinea(double dxLeft1,double dxLeft2,double dxTop,double dxAncho = 0.5);
	void w_vLinea(double dxTop1,double dxTop2,double dxLeft,double dxAncho = 0.5);

	void w_Encabezado();
	void w_Precio();
	void w_PorGalon();
	void w_PorValor();
	void w_Imagen();
	void w_AstericoFinal();

	double ggdLinea;
	bool ggbPorValor;

};

#endif /* MYPRINTGLS_H_ */

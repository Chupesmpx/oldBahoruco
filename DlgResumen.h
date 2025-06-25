/*
 * DlgResumen.h
 *
 *  Created on: 29/11/2012
 *      Author: vrafael
 */

#ifndef DLGRESUMEN_H_
#define DLGRESUMEN_H_

class DlgResumen
{
public:
	DlgResumen();
	virtual ~DlgResumen();

	bool w_Create(double* dxInicial,double* dxFinal);

	void* voidMyDlg;
	void* vInicial;
	void* vFinal;

private:
	void w_Controles();

};

#endif /* DLGRESUMEN_H_ */

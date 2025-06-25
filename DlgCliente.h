/*
 * DlgCliente.h
 *
 *  Created on: 28/11/2012
 *      Author: vrafael
 */

#ifndef DLGCLIENTE_H_
#define DLGCLIENTE_H_

class DlgCliente
{
public:
	DlgCliente();
	virtual ~DlgCliente();

	bool w_Create(char* lpxCliente,long lxCliente);

	void* voidMyDlg;
	void* vNameClte;

private:
	void w_Controles();

};

#endif /* DLGCLIENTE_H_ */

/*
 * DlgBonogas.h
 *
 *  Created on: 13/11/2012
 *      Author: vrafael
 */

#ifndef DLGBONOGAS_H_
#define DLGBONOGAS_H_

class DlgBonogas
{
public:
	DlgBonogas();
	virtual ~DlgBonogas();

	void w_Create();

	void* voidMyDlg;
	void* vBonogas;

private:
	void w_Controles();
	void w_Save();

};

#endif /* DLGBONOGAS_H_ */

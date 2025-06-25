/*
 * DlgFecha.h
 *
 *  Created on: 18/09/2012
 *      Author: vrafael
 */

#ifndef DLGFECHA_H_
#define DLGFECHA_H_

class DlgFecha
{
public:
	DlgFecha();
	virtual ~DlgFecha();

	bool w_Create(void* vxParent,void* vxEdit);
	void w_Save();

	void* voidFecha;

private:
	void w_Controles(void* voidDlg,void* vxEdit);

	void* voidEditExt;

	bool ggbRetorno;
};

#endif /* DLGFECHA_H_ */

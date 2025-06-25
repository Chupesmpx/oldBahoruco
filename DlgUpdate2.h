/*
 * DlgUpdate2.h
 *
 *  Created on: 16/11/2012
 *      Author: vrafael
 */

#ifndef DLGUPDATE2_H_
#define DLGUPDATE2_H_

class DlgUpdate2
{
public:
	DlgUpdate2();
	virtual ~DlgUpdate2();

	void w_Create();
	void w_Calcular(int ixRadio);

	void* voidMyDlg;

	void* vGalones;
	void* vTotal;

	void* voidRadio1;
	void* voidRadio2;
	void* voidRadio3;
	void* voidRadio4;

private:
	void w_Controles();
//	bool w_GetLV(long lxLinea,char** buff1,char** buff2);

};

#endif /* DLGUPDATE2_H_ */

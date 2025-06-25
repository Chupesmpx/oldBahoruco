/*
 * DlgUpdate.h
 *
 *  Created on: 12/11/2012
 *      Author: vrafael
 */

#ifndef DLGUPDATE_H_
#define DLGUPDATE_H_

class DlgUpdate
{
public:
	DlgUpdate();
	virtual ~DlgUpdate();

	void w_Create(long lxTipo);
	void w_SumarTotal();
	void w_AddBonoGas(long lxCant);

	void w_CalcularGas();
	void w_CalcularTotal();
	void w_CalcularDevuelta();

	bool w_GetLV(long lxLinea,char** buff1,char** buff2);
	void w_SetLV(long lxLinea,char* buff1,char* buff2);

	void* voidMyDlg;
	void* vGalones;
	void* vCombo;
//	void* vMonto;
	void* vTotal;
	void* vEfectivo;
	void* vDevuelta;

private:
	void w_Controles(long lxTipo);
	void w_Save(bool bxPrint);
	void w_Cargar(long lxTipo);

	long gglTipo;

};

#endif /* DLGUPDATE_H_ */

/*
 * MenuMain.h
 *
 *  Created on: 15/09/2012
 *      Author: vrafael
 */

#ifndef MENUMAIN_H_
#define MENUMAIN_H_

class MenuMain
{
public:
	MenuMain();
	virtual ~MenuMain();

	void* w_Create();

	void* vSubTicket;
	void* vSubResumen;
	void* vSubDetalle;

private:

	bool w_MenuFile(void* voidMenu);
	bool w_SubMenuFile(void* voidMenu,void* voidSubMenu);

	bool w_MenuTools(void* voidMenu);
	bool w_SubMenuTools(void* voidMenu,void* voidSubMenu);

	bool w_MenuTickets(void* voidMenu);
	bool w_SubMenuTickets(void* voidMenu,void* voidSubMenu);

	bool w_MenuResumen(void* voidMenu);
	bool w_SubMenuResumen(void* voidMenu,void* voidSubMenu);

	bool w_MenuDetalle(void* voidMenu);
	bool w_SubMenuDetalle(void* voidMenu,void* voidSubMenu);

	bool w_MenuAyuda(void* voidMenu);
	bool w_SubMenuAyuda(void* voidMenu,void* voidSubMenu);

	void  w_CreateItem(void* voidSubMenu,char* lpxTitulo,char* lpxImagen,char* lpxMsg,int ixID);
	void* w_CreateItem2(void* voidSubMenu,char* lpxTitulo,char* lpxImagen,char* lpxMsg,int ixID);
	void  w_CreateSep(void* voidSubMenu);
};

#endif /* MENUMAIN_H_ */

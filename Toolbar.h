/*
 * Toolbar.h
 *
 *  Created on: 15/09/2012
 *      Author: vrafael
 */

#ifndef TOOLBAR_H_
#define TOOLBAR_H_

class Toolbar
{
public:
	Toolbar();
	virtual ~Toolbar();

	void* w_Create();

	void* voidEditFecha;
	void* voidEditPrecio;

private:
	void* w_Create2();

	void w_AddItemToolbar(void* voidToolbar,char* lpxLabel,char* lpxTooltip,char* lpxImagen,void* voidxSignal);
	void w_AddItemToolbar_Stock(void* voidToolbar,char* lpxLabel,char* lpxTooltip,char* lpxStock,void* voidxSignal);
	void w_AddMenuItemToolbar(void* voidToolbar);
	void w_AddEditFechaItemToolbar(void* voidToolbar);
	void w_AddEditPrecioItemToolbar(void* voidToolbar);

};

#endif /* TOOLBAR_H_ */

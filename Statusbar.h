/*
 * Statusbar.h
 *
 *  Created on: 15/09/2012
 *      Author: vrafael
 */

#ifndef STATUSBAR_H_
#define STATUSBAR_H_

class Statusbar
{
public:
	Statusbar();
	virtual ~Statusbar();

	void* w_Create();
	void w_PushItem(char* lpxTexto);

	void* voidStatusbar;
	int ggid;

};

#endif /* STATUSBAR_H_ */

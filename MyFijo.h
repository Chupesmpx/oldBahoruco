/*
 * MyFijo.h
 *
 *  Created on: 11/11/2012
 *      Author: vrafael
 */

#ifndef MYFIJO_H_
#define MYFIJO_H_

class MyFijo
{
public:
	MyFijo();
	virtual ~MyFijo();

	struct GASFIJO1
	{
		char   cFecha[32];
		char   cFechaEd[32];
		double dPrecio;
	};

	void w_SetFijo();
	bool w_GetFijo(char* buffFecha,char* buffFechaEd,double* dPrecio);

	void w_SetFileName();
	bool w_Read(void* vstruct);
	void w_Write(void* vstruct);

	char ggcFileName[1024];

};

#endif /* MYFIJO_H_ */

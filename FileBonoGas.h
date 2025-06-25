/*
 * FileBonoGas.h
 *
 *  Created on: 13/11/2012
 *      Author: vrafael
 */

#ifndef FILEBONOGAS_H_
#define FILEBONOGAS_H_

class FileBonoGas
{
public:
	FileBonoGas();
	virtual ~FileBonoGas();

	void w_SetFileName();
	bool w_Read(double* dBonogas);
	void w_Write(double dBonogas);

	char ggcFileName[1024];

};

#endif /* FILEBONOGAS_H_ */

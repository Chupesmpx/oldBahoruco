/*
 * MySave.h
 *
 *  Created on: 13/11/2012
 *      Author: vrafael
 */

#ifndef MYSAVE_H_
#define MYSAVE_H_

class MySave
{
public:
	MySave();
	virtual ~MySave();

	void w_Save();
	bool w_IsTicketSave();

private:
	bool w_IsTotal();

	bool w_GetValores(	double* dGalon1,double* dGalon2,double* dGalon3,
						double* dTotal1,double* dTotal2,double* dTotal3,
						double* dEfectivo,double* dCambio);


};

#endif /* MYSAVE_H_ */

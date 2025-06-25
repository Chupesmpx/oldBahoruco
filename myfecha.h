//      myfecha.h
//      

#ifndef MYFECHA_H
#define MYFECHA_H

class MyFecha
{
	public:
		MyFecha();
		virtual ~MyFecha();
		
	long w_GetDiaMaximo(long lxAno,long lxMes);
	void w_GetFechaEd(char* lpxFecha,long lxFecha,long lxAno,long lxMes,long lxDia);
	void w_GetFechaEd2(int ixFecha,char* lpxFecha,long lxFecha);
	int  w_GetFechaInt(long lxAno,long lxMes,long lxDia);
	bool w_Get_iFecha(int ixFecha,int* ixAno,int* ixMes,int* ixDia);
	void w_GetHoraEd(char* lpInputHora,char* lpxHora,long lxHora,bool bxSegundos=false);
	
	private:


};

#endif /* MYFECHA_H */ 

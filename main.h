// main.h

#ifndef MAIN_H_
#define MAIN_H_

//void w_ClearBuff(const char* lpxNameBuff);

void w_SetStatusbar(char* lpxTexto);
bool w_GetDirectory(char* lpxBuff,long lxBuff);
void w_MessageBox_main(char* lpxMsg);

void  w_SetFecha(char* xbuffFecha);
void  w_SetFechaEd(char* xbuffFecha);
char* w_GetFecha();
char* w_GetFechaEd();

void w_SetPrecio();
double w_GetPrecio();

long w_getFechaActual();
void w_getHoraActual(char* cHora,char* cMinutos,char* cSegundos);

double w_GetBonogas();
void   w_SaveBonogas(double dBonoGas);

void* w_GetMain();
void* w_GetListview();
void* w_GetMenu();

#endif /* MAIN_H_ */

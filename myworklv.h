//      myworklv.h
//      


#ifndef MYWORKLV_H
#define MYWORKLV_H

class MyWorkLV
{
	public:
		MyWorkLV();
		virtual ~MyWorkLV();

	void w_Upper(char* lpxString,bool bxUpper);
	void w_ClearTreeView(void* voidxMyTree);
	bool w_GetItemTextLV(void* voidxMyTree,int ixSubItem,int* ixTexto,char* lpxTexto,long lxTexto);
	bool w_SetItemLV(void* vxTree,int ixSub,char* lpxValor,long lxValor,bool bxNumeric,bool bxTreeView = false);
	long w_GetPosLV(void* voidxMyTree);
	bool w_SelectPosLV(void* voidxMyTree,int ixPos);
	bool w_UnSelectPosLV(void* voidxMyTree,int ixPos);
	bool w_SelectSeqLV(void* voidxMyTree,int ixSubItem,int ixSeq);
	long w_GetLineasLV(void* vxTree);
	long w_GetPrimeraPosLV(void* voidxMyTree);
	long w_GetUltPosLV(void* voidxMyTree);
	void w_SetSigno(char* lpxValor,long lxValor,bool bxParentesis);
	bool w_GetFormat(char* lpxFormat,long lxFormat,double dxValor,int ixDecimales=2,bool bxParentesis=false);
	void w_ParseDouble(char* lpxValor,long lxValor,bool bxCero=false);
		
	private:

};

#endif /* MYWORKLV_H */ 

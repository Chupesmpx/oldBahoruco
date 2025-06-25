#make gasMain

DIRO=../gasMain/obj/
DIRA=../gasMain/App/


#DBG=-ggdb -w
DBG=-w
#DBG=

#LIBX=`pkg-config --cflags gtk+-2.0	webkit-1.0`
#LIBX=`pkg-config --cflags gtk+-2.0`
#LIBX=

INCXX=	-mms-bitfields	\
		-IC:/gtk/include/gtk-2.0		-IC:/gtk/lib/gtk-2.0/include	\
		-IC:/gtk/include/atk-1.0		-IC:/gtk/include/cairo			\
		-IC:/gtk/include/gdk-pixbuf-2.0 -IC:/gtk/include/pango-1.0		\
		-IC:/gtk/include/glib-2.0		-IC:/gtk/lib/glib-2.0/include	\
		-IC:/gtk/include				-IC:/gtk/include/freetype2		\
		-IC:/gtk/include/libpng14
		
LIBXX=	-LC:/gtk/lib	-lgtk-win32-2.0		-lgdk-win32-2.0		\
		-latk-1.0		-lgio-2.0			-lpangowin32-1.0	\
		-lgdi32			-lpangocairo-1.0	-lgdk_pixbuf-2.0	\
		-lpango-1.0 	-lcairo				-lgobject-2.0		\
		-lgmodule-2.0	-lgthread-2.0		-lglib-2.0			\
		-lintl
		

OBJ=	$(DIRO)main.o			$(DIRO)Ventana.o		$(DIRO)MenuMain.o		\
		$(DIRO)Statusbar.o		$(DIRO)Toolbar.o		$(DIRO)DlgFecha.o		\
		$(DIRO)myfecha.o		$(DIRO)MyFijo.o			$(DIRO)myworklv.o		\
		$(DIRO)MyListview.o		$(DIRO)MyData.o			$(DIRO)DlgUpdate.o		\
		$(DIRO)CargarDet.o		$(DIRO)DlgBonogas.o		$(DIRO)FileBonoGas.o	\
		$(DIRO)MySave.o			$(DIRO)DlgAcerca.o		$(DIRO)DlgUpdate2.o		\
		$(DIRO)MyPrintSeting.o	$(DIRO)MyPrintTicket.o	$(DIRO)MyPrintResum.o	\
		$(DIRO)MyPrintFact.o	$(DIRO)MyPrintGls.o		$(DIRO)DlgCliente.o		\
		$(DIRO)DlgResumen.o
	
PROG=	$(DIRA)gasMain.exe

all:	$(OBJ)
		g++	-mwindows	-o	$(PROG)	$(OBJ)	$(LIBXX)
#			$(DBG)	`pkg-config	--libs gtk+-2.0	gmodule-2.0`
#			$(DBG)	`pkg-config	--libs gtk+-2.0	webkit-1.0	gmodule-2.0`


$(DIRO)main.o:	main.c	main.h
	g++	$(DBG)	-c	main.c	-o	$(DIRO)main.o				$(INCXX)
	
$(DIRO)Ventana.o:	Ventana.cpp	Ventana.h
	g++	$(DBG)	-c	Ventana.cpp	-o	$(DIRO)Ventana.o		$(INCXX)

$(DIRO)MenuMain.o:	MenuMain.cpp	MenuMain.h
	g++	$(DBG)	-c	MenuMain.cpp	-o	$(DIRO)MenuMain.o	$(INCXX)

$(DIRO)Statusbar.o:	Statusbar.cpp	Statusbar.h
	g++	$(DBG)	-c	Statusbar.cpp	-o	$(DIRO)Statusbar.o	$(INCXX)

$(DIRO)Toolbar.o:	Toolbar.cpp	Toolbar.h
	g++	$(DBG)	-c	Toolbar.cpp	-o	$(DIRO)Toolbar.o		$(INCXX)

$(DIRO)DlgFecha.o:	DlgFecha.cpp	DlgFecha.h
	g++	$(DBG)	-c	DlgFecha.cpp	-o	$(DIRO)DlgFecha.o	$(INCXX)

$(DIRO)myfecha.o:	myfecha.cpp	myfecha.h
	g++	$(DBG)	-c	myfecha.cpp	-o	$(DIRO)myfecha.o	$(INCXX)

$(DIRO)MyFijo.o:	MyFijo.cpp	MyFijo.h
	g++	$(DBG)	-c	MyFijo.cpp	-o	$(DIRO)MyFijo.o	$(INCXX)

$(DIRO)myworklv.o:	myworklv.cpp	myworklv.h
	g++	$(DBG)	-c	myworklv.cpp	-o	$(DIRO)myworklv.o	$(INCXX)

$(DIRO)MyListview.o:	MyListview.cpp	MyListview.h
	g++	$(DBG)	-c	MyListview.cpp	-o	$(DIRO)MyListview.o	$(INCXX)

$(DIRO)MyData.o:	MyData.cpp	MyData.h
	g++	$(DBG)	-c	MyData.cpp	-o	$(DIRO)MyData.o

$(DIRO)DlgUpdate.o:	DlgUpdate.cpp	DlgUpdate.h
	g++	$(DBG)	-c	DlgUpdate.cpp	-o	$(DIRO)DlgUpdate.o	$(INCXX)

$(DIRO)CargarDet.o:	CargarDet.cpp	CargarDet.h
	g++	$(DBG)	-c	CargarDet.cpp	-o	$(DIRO)CargarDet.o	$(INCXX)

$(DIRO)DlgBonogas.o:	DlgBonogas.cpp	DlgBonogas.h
	g++	$(DBG)	-c	DlgBonogas.cpp	-o	$(DIRO)DlgBonogas.o	$(INCXX)

$(DIRO)FileBonoGas.o:	FileBonoGas.cpp	FileBonoGas.h
	g++	$(DBG)	-c	FileBonoGas.cpp	-o	$(DIRO)FileBonoGas.o	$(INCXX)

$(DIRO)MySave.o:	MySave.cpp	MySave.h
	g++	$(DBG)	-c	MySave.cpp	-o	$(DIRO)MySave.o			$(INCXX)

$(DIRO)DlgAcerca.o:	DlgAcerca.cpp	DlgAcerca.h
	g++	$(DBG)	-c	DlgAcerca.cpp	-o	$(DIRO)DlgAcerca.o			$(INCXX)

$(DIRO)DlgUpdate2.o:	DlgUpdate2.cpp	DlgUpdate2.h
	g++	$(DBG)	-c	DlgUpdate2.cpp	-o	$(DIRO)DlgUpdate2.o	$(INCXX)

$(DIRO)MyPrintSeting.o:	MyPrintSeting.cpp	MyPrintSeting.h
	g++	$(DBG)	-c	MyPrintSeting.cpp	-o	$(DIRO)MyPrintSeting.o		$(INCXX)

$(DIRO)MyPrintTicket.o:	MyPrintTicket.cpp	MyPrintTicket.h
	g++	$(DBG)	-c	MyPrintTicket.cpp	-o	$(DIRO)MyPrintTicket.o		$(INCXX)

$(DIRO)MyPrintResum.o:	MyPrintResum.cpp	MyPrintResum.h
	g++	$(DBG)	-c	MyPrintResum.cpp	-o	$(DIRO)MyPrintResum.o		$(INCXX)

$(DIRO)MyPrintFact.o:	MyPrintFact.cpp	MyPrintFact.h
	g++	$(DBG)	-c	MyPrintFact.cpp	-o	$(DIRO)MyPrintFact.o		$(INCXX)

$(DIRO)MyPrintGls.o:	MyPrintGls.cpp	MyPrintGls.h
	g++	$(DBG)	-c	MyPrintGls.cpp	-o	$(DIRO)MyPrintGls.o		$(INCXX)

$(DIRO)DlgCliente.o:	DlgCliente.cpp	DlgCliente.h
	g++	$(DBG)	-c	DlgCliente.cpp	-o	$(DIRO)DlgCliente.o		$(INCXX)

$(DIRO)DlgResumen.o:	DlgResumen.cpp	DlgResumen.h
	g++	$(DBG)	-c	DlgResumen.cpp	-o	$(DIRO)DlgResumen.o		$(INCXX)

clean:
	rm -f $(OBJ)
	rm -f $(PROG)



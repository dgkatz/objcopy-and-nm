# Makefile

all: getsyms getsyms_dl getsections getsections_dl gettext

gettext: objcopy.c
	gcc -c objcopy.c
	gcc -o objcopy objcopy.c -lbfd

getsyms_dl: getsyms_dl.c libobjdata.so
	gcc -o getsyms_dl getsyms_dl.c -ldl -lbfd

getsections_dl: getsections_dl.c libobjdata.so
	gcc -o getsections_dl getsections_dl.c -ldl -lbfd

getsyms: getsyms.c libobjdata.a
	gcc -o getsyms getsyms.c -L. libobjdata.a -lbfd

getsections: getsections.c libobjdata.a
	gcc -o getsections getsections.c -L. libobjdata.a -lbfd

libobjdata.a: objsect.o objsym.o
	ar rc libobjdata.a objsect.o objsym.o

libobjdata.so: objsect.o  objsym.o
	gcc -shared -o libobjdata.so objsect.o objsym.o -lbfd

objsect.o: objsect.c
	gcc -fpic -c objsect.c

objsym.o: objsym.c
	gcc -fpic -c objsym.c

clean:
	rm -rf getsections getsyms objsect.o objsym.o  inttohex.o getsections_dl.c~ Makefile~ stats.txt objsym.c~ getsyms_dl.c~ objcopy.o text-output.txt

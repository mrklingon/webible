
CC=gcc
FHOME=./
#FHOME=c:/cygwin/home/joela/projects/bible/



webible: webible.c makefile
	$(CC)   -DFILEHOME=\"$(FHOME)\" -o webible webible.c
	strip webible
	cp webible kjbible
	cp webible drbible
	cp webible latbible
	cp webible klbible

install: webible
	cp webible drbible kjbible klbible latbible c:/windows/command

ship: webible.c makefile webible readme.web kjbible
	rm webship.zip
	zip webship webible.c makefile webible kjbible drbible readme.web

drbible: webible.c makefile
	$(CC) -mno-cygwin -DFILEHOME=\"$(FHOME)\" -g -o drbible webible.c

latbible: webible.c makefile
	$(CC) -mno-cygwin -DFILEHOME=\"$(FHOME)\" -g -o latbible webible.c

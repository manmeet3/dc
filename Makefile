# $Id: Makefile,v 1.11 2015-02-03 18:27:45-08 - - $

MKFILE    = Makefile
DEPSFILE  = ${MKFILE}.deps
NOINCLUDE = ci clean spotless
NEEDINCL  = ${filter ${NOINCLUDE}, ${MAKECMDGOALS}}
GMAKE     = gmake --no-print-directory

GCC       = gcc -g -O0 -Wall -Wextra -std=gnu11
MKDEPS    = gcc -MM

CSOURCE   = debug.c stack.c bigint.c token.c main.c
LSOURCE   = scanner.l
CHEADER   = debug.h stack.h bigint.h token.h yyextern.h
OBJECTS   = ${CSOURCE:.c=.o} ${LSOURCE:.l=.o}
EXECBIN   = mydc
SOURCES   = ${CHEADER} ${CSOURCE} ${LSOURCE} ${MKFILE}
LISTING   = Listing.ps

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o $@ ${OBJECTS}

%.o : %.c
	${GCC} -c $<

scanner.c : scanner.l
	flex -oscanner.c scanner.l

ci : ${SOURCES}
	cid + ${SOURCES}
	checksource ${SOURCES}

lis : ${SOURCES} ${DEPSFILE}
	mkpspdf ${LISTING} ${SOURCES} ${DEPSFILE}

clean :
	- rm ${LSOURCE:.l=.c} ${OBJECTS} ${DEPSFILE} core

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}

deps : ${CSOURCE} ${CHEADER}
	@ echo "# ${DEPSFILE} created `date`" >${DEPSFILE}
	${MKDEPS} ${CSOURCE} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps ci all lis

ifeq "${NEEDINCL}" ""
include ${DEPSFILE}
endif


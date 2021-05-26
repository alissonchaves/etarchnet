ifeq ($(OS),Windows_NT)

BINDIR = bin\\
GENDIR = precalc\\
INCDIR = include\\
LIBDIR = lib\\
OBJDIR = obj\\
SRCDIR = src\\
TABDIR = tab\\
TSTDIR = test\\
EXADIR = program\\

CC     = cl
LINK   = link
RM     = del /q
STRIP  = dir
OBJEXT = .obj
LIBEXT = .lib
EXEEXT = .exe
OFLAG  = -Fo
XFLAG  = /NOLOGO /OUT:
AR     = lib
ARQC   = /NOLOGO /OUT:
ARQ    = /NOLOGO
RANLIB = dir
CFLAGS = -Ox -Ot -MT -GT -volatile:iso -I${INCDIR} -nologo -J -sdl -Wall -WX \
	-wd4464 -wd4668 -wd4710 -wd4711 -wd4201 -wd4820

else

BINDIR = bin/
GENDIR = precalc/
INCDIR = include/
LIBDIR = lib/
OBJDIR = obj/
SRCDIR = src/
TABDIR = tab/
TSTDIR = test/
EXADIR = program/

CC     = cc
LINK   = cc
RM     = /bin/rm -f
RMFORCE= /bin/rm -Rf
CP     = /bin/cp -a
LN     = /bin/ln -s
MKDIR = /bin/mkdir
STRIP  = strip
OBJEXT = .o
LIBEXT = .a
EXEEXT =
OFLAG  = -o
XFLAG  = -o
AR     = ar
ARQC   = qc 
ARQ    = q
RANLIB = ranlib
CFLAGS = -Wall -Wextra -Wstrict-prototypes -Wshadow -Wpointer-arith \
	-Wcast-qual -Wcast-align -Wwrite-strings -Wredundant-decls \
	-Wnested-externs -Werror -O3 \
	-funsigned-char -I${INCDIR}

endif

#
# Default compile commands for the source files
#

${OBJDIR}%${OBJEXT} : ${SRCDIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<

${TSTDIR}${OBJDIR}%${OBJEXT} : ${TSTDIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<

${GENDIR}${OBJDIR}%${OBJEXT} : ${GENDIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<

${EXADIR}${OBJDIR}%${OBJEXT} : ${EXADIR}%.c
	${CC} -c ${CFLAGS} ${OFLAG}$@ $<

all:							\
	${LIBDIR}libcrc${LIBEXT}			\
	etarch-cv${EXEEXT}				\
	etarch-int${EXEEXT}				\
	etarch-ws${EXEEXT}				\
	etarch-chat${EXEEXT}				\
	etarch-sf${EXEEXT}				

install:
	${MKDIR} /etc/etarch
	${CP} etarch-cv /etc/etarch
	${CP} etarch-int /etc/etarch
	${CP} etarch-ws /etc/etarch
	${CP} etarch-chat /etc/etarch
	${CP} etarch-sf /etc/etarch
	${CP} program/etarch.conf /etc/etarch
	${LN} /etc/etarch/etarch-cv /usr/local/bin/etarch-cv
	${LN} /etc/etarch/etarch-int /usr/local/bin/etarch-int
	${LN} /etc/etarch/etarch-ws /usr/local/bin/etarch-ws
	${LN} /etc/etarch/etarch-chat /usr/local/bin/etarch-chat
	${LN} /etc/etarch/etarch-sf /usr/local/bin/etarch-sf

uninstall:
	${RMFORCE} /etc/etarch
	${RM} /usr/local/bin/etarch-cv
	${RM} /usr/local/bin/etarch-int
	${RM} /usr/local/bin/etarch-ws
	${RM} /usr/local/bin/etarch-chat
	${RM} /usr/local/bin/etarch-sf

clean:
	${RM} ${OBJDIR}*${OBJEXT}
	${RM} ${TABDIR}*.inc
	${RM} ${EXADIR}${OBJDIR}*${OBJEXT}
	${RM} ${TSTDIR}${OBJDIR}*${OBJEXT}
	${RM} ${GENDIR}${OBJDIR}*${OBJEXT}
	${RM} ${LIBDIR}libcrc${LIBEXT}
	${RM} ${BINDIR}prc${EXEEXT}
	${RM} etarch-cv${EXEEXT}
	${RM} etarch-int${EXEEXT}
	${RM} etarch-ws${EXEEXT}
	${RM} etarch-chat${EXEEXT}
	${RM} etarch-sf${EXEEXT}

${BINDIR}prc${EXEEXT} :					\
		${GENDIR}${OBJDIR}precalc${OBJEXT}	\
		${GENDIR}${OBJDIR}crc32_table${OBJEXT}	\
		${GENDIR}${OBJDIR}crc64_table${OBJEXT}	\
		Makefile
	${LINK}	${XFLAG}${BINDIR}prc${EXEEXT}		\
		${GENDIR}${OBJDIR}precalc${OBJEXT}	\
		${GENDIR}${OBJDIR}crc32_table${OBJEXT}	\
		${GENDIR}${OBJDIR}crc64_table${OBJEXT}
	${STRIP} ${BINDIR}prc${EXEEXT}

#
# The etarch-cv program can be run to calculate the CRC values of manual input or
# of the contents of one or more files.
#

etarch-cv${EXEEXT} :					\
		${EXADIR}${OBJDIR}etarch-cv${OBJEXT}	\
		${LIBDIR}libcrc${LIBEXT}		\
		Makefile
	${LINK}	${XFLAG}etarch-cv${EXEEXT}			\
		${EXADIR}${OBJDIR}etarch-cv${OBJEXT}	\
		${LIBDIR}libcrc${LIBEXT}
	${STRIP} etarch-cv${EXEEXT}

etarch-int${EXEEXT} :					\
		${EXADIR}${OBJDIR}etarch-int${OBJEXT}	\
		Makefile
	${LINK}	${XFLAG}etarch-int${EXEEXT}			\
		${EXADIR}${OBJDIR}etarch-int${OBJEXT}	
	${STRIP} etarch-int${EXEEXT}

etarch-ws${EXEEXT} :					\
		${EXADIR}${OBJDIR}etarch-ws${OBJEXT}	\
		Makefile
	${LINK}	${XFLAG}etarch-ws${EXEEXT}			\
		${EXADIR}${OBJDIR}etarch-ws${OBJEXT}	
	${STRIP} etarch-ws${EXEEXT}

etarch-chat${EXEEXT} :					\
		${EXADIR}${OBJDIR}etarch-chat${OBJEXT}	\
		Makefile
	${LINK}	${XFLAG}etarch-chat${EXEEXT}			\
		${EXADIR}${OBJDIR}etarch-chat${OBJEXT}	
	${STRIP} etarch-chat${EXEEXT}

etarch-sf${EXEEXT} :					\
		${EXADIR}${OBJDIR}etarch-sf${OBJEXT}	\
		Makefile
	${LINK}	${XFLAG}etarch-sf${EXEEXT}			\
		${EXADIR}${OBJDIR}etarch-sf${OBJEXT}	
	${STRIP} etarch-sf${EXEEXT}

#
# libcrc is the library which can be linked with other applications. The
# extension of the library depends on the operating system used.
#

${LIBDIR}libcrc${LIBEXT} :			\
	${OBJDIR}crc8${OBJEXT}			\
	${OBJDIR}crc16${OBJEXT}			\
	${OBJDIR}crc32${OBJEXT}			\
	${OBJDIR}crc64${OBJEXT}			\
	${OBJDIR}crcccitt${OBJEXT}		\
	${OBJDIR}crcdnp${OBJEXT}		\
	${OBJDIR}crckrmit${OBJEXT}		\
	${OBJDIR}crcsick${OBJEXT}		\
	${OBJDIR}nmea-chk${OBJEXT}		\
	Makefile
		${RM}        ${LIBDIR}libcrc${LIBEXT}
		${AR} ${ARQC}${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc16${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc32${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc64${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crc8${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcccitt${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcdnp${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crckrmit${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}crcsick${OBJEXT}
		${AR} ${ARQ} ${LIBDIR}libcrc${LIBEXT} ${OBJDIR}nmea-chk${OBJEXT}
		${RANLIB}    ${LIBDIR}libcrc${LIBEXT}

#
# Lookup table include file dependencies
#

${TABDIR}gentab32.inc			: ${BINDIR}prc${EXEEXT}
	${BINDIR}prc --crc32 ${TABDIR}gentab32.inc

${TABDIR}gentab64.inc			: ${BINDIR}prc${EXEEXT}
	${BINDIR}prc --crc64 ${TABDIR}gentab64.inc

#
# Individual source files with their header file dependencies
#

${OBJDIR}crc8${OBJEXT}			: ${SRCDIR}crc8.c ${INCDIR}checksum.h

${OBJDIR}crc16${OBJEXT}			: ${SRCDIR}crc16.c ${INCDIR}checksum.h

${OBJDIR}crc32${OBJEXT}			: ${SRCDIR}crc32.c ${INCDIR}checksum.h ${TABDIR}gentab32.inc

${OBJDIR}crc64${OBJEXT}			: ${SRCDIR}crc64.c ${INCDIR}checksum.h ${TABDIR}gentab64.inc

${OBJDIR}crcccitt${OBJEXT}		: ${SRCDIR}crcccitt.c ${INCDIR}checksum.h

${OBJDIR}crcdnp${OBJEXT}		: ${SRCDIR}crcdnp.c ${INCDIR}checksum.h

${OBJDIR}crckrmit${OBJEXT}		: ${SRCDIR}crckrmit.c ${INCDIR}checksum.h

${OBJDIR}crcsick${OBJEXT}		: ${SRCDIR}crcsick.c ${INCDIR}checksum.h

${OBJDIR}nmea-chk${OBJEXT}		: ${SRCDIR}nmea-chk.c ${INCDIR}checksum.h

${EXADIR}${OBJDIR}etarch-cv${OBJEXT}	: ${EXADIR}etarch-cv.c ${INCDIR}checksum.h

${EXADIR}${OBJDIR}etarch-int${OBJEXT}	: ${EXADIR}etarch-int.c ${INCDIR}checksum.h

${EXADIR}${OBJDIR}etarch-ws${OBJEXT}	: ${EXADIR}etarch-ws.c ${INCDIR}checksum.h

${EXADIR}${OBJDIR}etarch-chat${OBJEXT}	: ${EXADIR}etarch-chat.c 

${EXADIR}${OBJDIR}etarch-sf${OBJEXT}	: ${EXADIR}etarch-sf.c 

${TSTDIR}${OBJDIR}testcrc${OBJEXT}	: ${TSTDIR}testcrc.c ${TSTDIR}testall.h ${INCDIR}checksum.h

${TSTDIR}${OBJDIR}testnmea${OBJEXT}	: ${TSTDIR}testnmea.c ${TSTDIR}testall.h ${INCDIR}checksum.h

${GENDIR}${OBJDIR}crc32_table${OBJEXT}	: ${GENDIR}crc32_table.c ${GENDIR}precalc.h ${INCDIR}checksum.h

${GENDIR}${OBJDIR}crc64_table${OBJEXT}	: ${GENDIR}crc64_table.c ${GENDIR}precalc.h ${INCDIR}checksum.h

${GENDIR}${OBJDIR}precalc${OBJEXT}	: ${GENDIR}precalc.c ${GENDIR}precalc.h
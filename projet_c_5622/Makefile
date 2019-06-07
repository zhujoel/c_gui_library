# Variable definitions.

PLATFORM	= $(shell uname)
CC		= gcc
OBJDIR		= ./objs
TESTS		= ./tests
SOURCE  = ./src
INCLUDES	= ./include
INCFLAGS	:= -I${INCLUDES} -I${TESTS}
#OPTFLAGS	:= -Os -DNDEBUG
OPTFLAGS	:= -g
CCFLAGS		:= -pg -c ${OPTFLAGS} -Wall -std=c99

# The list of objects to include in the library

LIBEIOBJS	:=



# Platform specific definitions (OS X, Linux)

ifeq (${PLATFORM},Darwin)

	# Building for Mac OS X

	PLATDIR		= _osx
	INCFLAGS	:= ${INCFLAGS} -I/opt/local/include/SDL
	LINK		= ${CC}
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase.a
	LIBS		= ${LIBEIBASE} -L/opt/local/lib -lSDL -lSDL_ttf -lSDL_image -framework AppKit
	CCFLAGS		:= ${CCFLAGS} -D__OSX__

else

	# Building for Linux

	PLATDIR		= _x11
	INCFLAGS	:= ${INCFLAGS} -I/usr/include/SDL
	LINK		= ${CC}
#	ARCH	        = 32
	ARCH	        = 64
	LIBEI		= ${OBJDIR}/libei.a
	LIBEIBASE	= ${PLATDIR}/libeibase${ARCH}.a
	LIBS		= ${LIBEIBASE} -lSDL -lSDL_ttf -lSDL_image -lm
	CCFLAGS		:= ${CCFLAGS} -D__LINUX__ -m${ARCH}
	LDFLAGS		= -m${ARCH} -g

endif



# Main target of the makefile. To build specific targets, call "make <target_name>"

TARGETS		=	${LIBEI} \
			minimal lines frame button hello_world puzzle two048 text copy pointe rounded_frame draw_button draw_image arc fill polygon toplevel
all : ${TARGETS}



########## Test-programs



# minimal

minimal : ${OBJDIR}/minimal.o ${LIBEIBASE}
	${LINK} -o minimal ${LDFLAGS} ${OBJDIR}/minimal.o ${LIBS}

${OBJDIR}/minimal.o : ${TESTS}/minimal.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/minimal.c -o ${OBJDIR}/minimal.o

# lines

lines : ${OBJDIR}/lines.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o lines ${OBJDIR}/lines.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/lines.o : ${TESTS}/lines.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/lines.c -o ${OBJDIR}/lines.o

${OBJDIR}/draw.o : ${SOURCE}/ei_draw.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_draw.c -o ${OBJDIR}/draw.o

# frame

frame : ${OBJDIR}/frame.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o frame ${LDFLAGS} ${OBJDIR}/frame.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/frame.o : ${TESTS}/frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/frame.c -o ${OBJDIR}/frame.o

${OBJDIR}/appli.o : ${SOURCE}/ei_application.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_application.c -o ${OBJDIR}/appli.o

${OBJDIR}/widget.o : ${SOURCE}/ei_widget.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_widget.c -o ${OBJDIR}/widget.o

${OBJDIR}/widgetclass.o : ${SOURCE}/ei_widgetclass.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_widgetclass.c -o ${OBJDIR}/widgetclass.o

${OBJDIR}/placer.o : ${SOURCE}/ei_placer.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_placer.c -o ${OBJDIR}/placer.o

# button

button : ${OBJDIR}/button.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o button ${LDFLAGS} ${OBJDIR}/button.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/button.o : ${TESTS}/button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/button.c -o ${OBJDIR}/button.o

# hello_world

hello_world : ${OBJDIR}/hello_world.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o hello_world ${LDFLAGS} ${OBJDIR}/hello_world.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/hello_world.o : ${TESTS}/hello_world.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/hello_world.c -o ${OBJDIR}/hello_world.o

${OBJDIR}/event.o : ${SOURCE}/ei_event.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${SOURCE}/ei_event.c -o ${OBJDIR}/event.o


# puzzle

puzzle : ${OBJDIR}/puzzle.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o puzzle ${LDFLAGS} ${OBJDIR}/puzzle.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/puzzle.o : ${TESTS}/puzzle.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/puzzle.c -o ${OBJDIR}/puzzle.o


# two048

two048 : ${OBJDIR}/two048.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o two048 ${LDFLAGS} ${OBJDIR}/two048.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/two048.o : ${TESTS}/two048.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/two048.c -o ${OBJDIR}/two048.o


# text

text : ${OBJDIR}/text.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o text ${OBJDIR}/text.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/text.o : ${TESTS}/text.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/text.c -o ${OBJDIR}/text.o


# copy

copy : ${OBJDIR}/copy.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o copy ${OBJDIR}/copy.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/copy.o : ${TESTS}/copy.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/copy.c -o ${OBJDIR}/copy.o

# pointe

pointe : ${OBJDIR}/pointe.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o pointe ${OBJDIR}/pointe.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/pointe.o : ${TESTS}/pointe.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/pointe.c -o ${OBJDIR}/pointe.o

# fill

fill : ${OBJDIR}/fill.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o fill ${OBJDIR}/fill.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/fill.o : ${TESTS}/fill.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/fill.c -o ${OBJDIR}/fill.o

# arc

arc : ${OBJDIR}/arc.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o arc ${OBJDIR}/arc.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/arc.o : ${TESTS}/arc.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/arc.c -o ${OBJDIR}/arc.o

# rounded_frame
rounded_frame : ${OBJDIR}/rounded_frame.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o rounded_frame ${OBJDIR}/rounded_frame.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/rounded_frame.o : ${TESTS}/rounded_frame.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/rounded_frame.c -o ${OBJDIR}/rounded_frame.o

# draw_button
draw_button : ${OBJDIR}/draw_button.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o draw_button ${OBJDIR}/draw_button.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/draw_button.o : ${TESTS}/draw_button.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/draw_button.c -o ${OBJDIR}/draw_button.o

# draw_image
draw_image : ${OBJDIR}/draw_image.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o draw_image ${OBJDIR}/draw_image.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/draw_image.o : ${TESTS}/draw_image.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/draw_image.c -o ${OBJDIR}/draw_image.o

# polygon
polygon : ${OBJDIR}/polygon.o ${OBJDIR}/draw.o ${LIBEIBASE} ${LIBEI}
	${LINK} -pg -o polygon ${OBJDIR}/polygon.o ${OBJDIR}/draw.o ${LIBEI} ${LIBS}

${OBJDIR}/polygon.o : ${TESTS}/polygon.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/polygon.c -o ${OBJDIR}/polygon.o

# toplevel

toplevel : ${OBJDIR}/toplevel.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEIBASE} ${LIBEI}
	${LINK} -o toplevel ${LDFLAGS} ${OBJDIR}/toplevel.o ${OBJDIR}/appli.o ${OBJDIR}/widget.o ${OBJDIR}/widgetclass.o ${OBJDIR}/draw.o ${OBJDIR}/placer.o ${OBJDIR}/event.o ${LIBEI} ${LIBS}

${OBJDIR}/toplevel.o : ${TESTS}/toplevel.c
	${CC} ${CCFLAGS} ${INCFLAGS} ${TESTS}/toplevel.c -o ${OBJDIR}/toplevel.o


# Building of the library libei

${LIBEI} : ${LIBEIOBJS}
	ar rcs ${LIBEI} ${LIBEIOBJS}



# Building of the doxygen documentation.

doc :
	doxygen docs/doxygen.cfg



# Removing all built files.

clean:
	rm -f ${TARGETS}
	rm -f *.exe
	rm -f ${OBJDIR}/*
	rm -f gmon.out
	rm -f a.txt

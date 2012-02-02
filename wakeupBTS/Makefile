
#
# wakeupBTS Makefile.
#

NAME     = wakeupBTS
OBJECTS  = spi.o cc_transmitter.o app.o
CPU      = msp430f2274
CFLAGS   = -mmcu=${CPU} -O2 -Wall -g
CC       = msp430-gcc

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

clean:
	rm -f ${NAME}.elf ${NAME}.a43 ${NAME}.lst ${OBJECTS}

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies
app.o: app.c app.h

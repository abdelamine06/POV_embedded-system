
all:main.bin

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=13000000 -mmcu=atmega328p
PORT=/dev/rfcomm0
SRC=src/


usart.o: usart.c
	${CC} ${CFLAGS} -c -o usart.o usart.c

hall.o: hall.c
	${CC} ${CFLAGS} -c -o hall.o hall.c

spi.o: spi.c
	${CC} ${CFLAGS} -c -o spi.o spi.c

timer.o: timer.c
	${CC} ${CFLAGS} -c -o timer.o timer.c

interrupt.o: interrupt.c
	${CC} ${CFLAGS} -c -o interrupt.o interrupt.c

main.out: main.c usart.o spi.o hall.o timer.o interrupt.o
	${CC} ${CFLAGS} -o main.out main.c usart.o spi.o hall.o timer.o interrupt.o

main.bin: main.out
		${OBJCOPY} -O binary main.out main.bin

install:
		avrdude -p m328p -c usbasp -P ${PORT} -U flash:w:main.bin

clean:
		rm *.o *.out *.bin

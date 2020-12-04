all:
	avr-gcc -DF_CPU=16000000 -Os -mmcu=atmega328p main.c
	avr-objcopy -O binary a.out a.bin
	
install:
	avrdude -p m328p -c usbasp -P  /dev/rfcomm0 -U flash:w:a.bin
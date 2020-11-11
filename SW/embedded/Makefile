#!/usr/bin/make -f
# Makefile for embedded part of test-board
# Author: Stefan Misik (mail@stefanmisik.eu)

CC     = avr-gcc
OBJCPY = avr-objcopy
OBJDMP = avr-objdump
SIZE   = avr-size
AD     = avrdude

# GCC MCU
MCU  ?= atmega328p
# Avrdude part name
PART = m328p
# Avrdude programmer name
PROG = usbasp
# Avrdude port name
PORT = usb
# Project name
PROJ = test-board
# Source files
SRC  = alarm.c timer.c lcd.c outputs.c temperature.c main.c keypad.c


CFLAGS		= -c -ggdb -Os -mmcu=$(MCU)
LDFLAGS		= -ggdb -Os -mmcu=$(MCU)
SIZEFLAGS	= -C --mcu=$(MCU)

#############################################
OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all clean stats prog fuses

all: $(PROJ).elf $(PROJ).hex $(PROJ).lss $(SRC)

stats: $(PROJ).elf
	$(SIZE) $(SIZEFLAGS) $<

prog: $(PROJ).hex
	$(AD) -c $(PROG) -p $(PART) -P $(PORT) -U flash:w:$<:i

fuses:
	$(AD) -c $(PROG) -p $(PART) -P $(PORT) -B 10 -D             \
	    -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0xFF:m

$(PROJ).elf: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

$(PROJ).hex: $(PROJ).elf
	$(OBJCPY) -R .eeprom -O ihex $< $@

$(PROJ).lss: $(PROJ).elf
	$(OBJDMP) -h -S $<  > $@

%.o: %.c	
	$(CC) $(CFLAGS) $< -o $@


clean:
	$(RM) $(PROJ).elf $(PROJ).hex $(PROJ).lss $(OBJ)


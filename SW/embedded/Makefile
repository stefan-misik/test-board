# Makefile for embedded part of test-board
# Author: Stefan Misik (mail@stefanmisik.eu)

CC     = avr-gcc
OBJCPY = avr-objcopy
OBJDMP = avr-objdump
SED	   = sed

MCU  ?= atmega328p
PROJ = test-board
SRC  = lcd.c outputs.c temperature.c main.c


CFLAGS  = -c -ggdb -Os -mmcu=$(MCU)
LDFLAGS = -ggdb -Os -mmcu=$(MCU)

#############################################
OBJ = $(addsuffix .o, $(basename $(SRC)))

.PHONY: all clean

all: $(PROJ).elf $(PROJ).hex $(PROJ).lss $(SRC)


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


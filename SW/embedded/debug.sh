#!/bin/bash

# Exit on failure
set -e

simavr -mcu atmega328p -freq 20000000 -g -ff test-board.hex &

avr-gdb test-board.elf -ex 'target remote localhost:1234'

kill %1

#!/bin/bash
avrdude -c usbasp -p m48 -U flash:w:"./DDS_ctrl.hex":a

#writing to fuse
#avrdude -c usbasp -p m48 -u -U lfuse:w:****:m

#incteructive mode
#avrdude -c usbasp -p m168 -t

#fuse
#UPPER BIT
#MSB
#1101 1111 : 0xdf

#LOWER BIT
#1110 0010 : 0xe2
#LSB 4bit is clock config.(8MHz)
#MSB 1bit is clock 1/8 prescaler off

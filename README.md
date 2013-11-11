# Moisture blow off valve timer for air compressor.

 This software is designed to routinely(curently 1 week intervals) blow off condensate in an air compressor. The software
 is written for the ATMEL ATtiny24 microcontroller. Features include an led light that flashes every minute, a switch that alows manual blow off control.
 
##PIN MAPPING
* PA0 - PUSHBUTTON MANUAL DUMP
* PB0 - STATUS LED
* PB1 - VALVE TRIGGER(ACTIVE HIGH)

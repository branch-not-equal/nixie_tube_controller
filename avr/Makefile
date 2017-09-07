###############################################################################
# Makefile for the project Nixie tube contrller
###############################################################################

## General Flags
PROJECT = NixieCtrl
MCU     = atmega168
#MCU     = atmega48
#MCU_DUDE= m48
MCU_DUDE= m168
TARGET  = $(PROJECT).elf
CC      = /usr/local/CrossPack-AVR/bin/avr-gcc
OBJCOPY = /usr/local/CrossPack-AVR/bin/avr-objcopy
OBJDUMP = /usr/local/CrossPack-AVR/bin/avr-objdump
SIZE    = /usr/local/CrossPack-AVR/bin/avr-size


## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS  = $(COMMON)
CFLAGS += -Wall -std=gnu99 -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d


## Assembly specific flags
ASMFLAGS  = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS  = $(COMMON)
LDFLAGS +=  -Wl,-Map=$(PROJECT).map


## Intel Hex file production flags
HEX_FLASH_FLAGS   = -R .eeprom
HEX_EEPROM_FLAGS  = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES  = -I"./" 
INCLUDES += -I"./nixieDriver"
INCLUDES += -I"./debug_module"


## Objects that must be built in order to link
OBJECTS += main.o nixieCtrl.o dprintf.o nixieDriver.o uart.o msg.o timer.o

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss size

## Compile

main.o: ./main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nixieCtrl.o: ./nixieCtrl.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dprintf.o: ./debug_module/dprintf.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

nixieDriver.o: ./nixieDriver/nixieDriver.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o:   ./uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timer.o:     ./timer.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

msg.o:   ./msg.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	$(OBJCOPY) -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	$(OBJCOPY) $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	$(OBJDUMP) -h -S $< > $@

size: ${TARGET}
	@echo
	@$(SIZE) ${TARGET} #-C --mcu=${MCU}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf $(PROJECT).hex $(PROJECT).eep $(PROJECT).lss $(PROJECT).map

.PHONY: flash
flash:
	avrdude -c avrispmkII -p $(MCU_DUDE) -U flash:w:"$(PROJECT).hex":a
#	avrdude -c usbasp -p $(MCU_DUDE) -U flash:w:"$(PROJECT).hex":a

# Read eeprom.  
read_eeprom: 
	avrdude -c avrispmkII -p $(MCU_DUDE) -U eeprom:r:$(PROJECT).eep:i
#	python 12H12Y_LogReader.py < $(TARGET).eep


# avrdude terminal mode.  
info_terminal: 
	echo avrdude -c avrispmkII -p $(MCU_DUDE) -t


## Other dependencies
#-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)


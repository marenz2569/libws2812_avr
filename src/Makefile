DEVICE	= atmega328p
CLOCK	= 16000000
OBJECTS	= ws2812.o color.o light_ws2812.o
LIB	= ws2812_avr
LIBDIR	= -Llib
LIBINCLUDE	= -Ilib/ws2812_avr
EXAMPLES	= fade_red_cyan rainbow strobe

COMPILE	= avr-g++ -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=c++11 -fno-exceptions

.PHONY: clean lib

all:	lib example

%.o:	%.cpp
	$(COMPILE) -c $< -o $@

clean:
	rm -rf $(OBJECTS) lib$(LIB).a ../lib/*

lib:	$(OBJECTS)
	ar rcs lib$(LIB).a $(OBJECTS)
	cp lib$(LIB).a ../lib/
	rm -rf ../lib/$(LIB)
	mkdir ../lib/$(LIB)
	cp *.h ../lib/$(LIB)

$(EXAMPLES):	lib examples/$@/main.o examples/$@/main.hex examples/$@/main.memory-used

%.elf:	%.o
	$(COMPILE) -o main.elf $(OBJECTS) -static $(LIBDIR) $(LIBINCLUDE) $(LIBS)

%.hex:	%.elf
	rm -f $@
	avr-objcopy -j .text -j .data -O ihex $< $@

%.memory-used:	%.elf
	avr-size --mcu=$(DEVICE) -C $<

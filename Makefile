DEVICE	= atmega328p
CLOCK	= 16000000
OBJECTS	= src/ws2812.o src/color.o src/light_ws2812.o
LIB	= ws2812_avr
LIBS	= -lws2812_avr
LIBDIR	= -Llib
LIBINCLUDE	= -Ilib/ws2812_avr
EXAMPLES	= fade_red_cyan rainbow strobe

COMPILE	= avr-g++ -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=c++11 -fno-exceptions

.PHONY: clean

all:	lib $(EXAMPLES) docs

%.o:	%.cpp
	$(COMPILE) -c $< -o $@ $(LIBINCLUDE)

clean:
	rm -rf $(OBJECTS) src/lib$(LIB).a lib/* examples/*/*.o examples/*/*.elf examples/*/*.hex html/

lib:	$(OBJECTS)
	ar rcs src/lib$(LIB).a $(OBJECTS)
	cp src/lib$(LIB).a lib/
	rm -rf lib/$(LIB)
	mkdir lib/$(LIB)
	cp src/*.h lib/$(LIB)

$(EXAMPLES):	lib
	make examples/$@/main.hex
	make examples/$@/main.memory-used

%.elf:	%.o
	$(COMPILE) -o $@ $< -static $(LIBDIR) $(LIBINCLUDE) $(LIBS)

%.hex:	%.elf
	rm -f $@
	avr-objcopy -j .text -j .data -O ihex $< $@

%.memory-used:	%.elf
	avr-size --mcu=$(DEVICE) -C $<

docs:
	doxygen DOXYFILE

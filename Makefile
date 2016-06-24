DEVICE		= atmega328p
CLOCK		= 16000000
OBJECTS		= src/ws2812.o src/ws2812_get_led_count.o src/ws2812_get_rgb_at.o src/ws2812_deconstructor.o src/color_hsi2rgb.o src/color_rgb2hsi.o src/fadelinear.o src/fadethroughiaxis.o src/light_ws2812.o
LIB		= ws2812_avr
LIBS		= -lws2812_avr
LIBDIR		= -Llib
LIBINCLUDE	= -Ilib/ws2812_avr
EXAMPLES	= fade_red_cyan rainbow strobe fade_i_axis_red_cyan

COMPILE		= avr-g++ -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=c++11 -fno-exceptions

PUR		= \033[0;35m
NC		= \033[0m

.PHONY: clean docs

all:	lib $(EXAMPLES) docs

%.o:	%.cpp
	$(COMPILE) -c $< -o $@ $(LIBINCLUDE)

clean:
	rm -rf $(OBJECTS) src/lib$(LIB).a lib/* examples/*/*.o examples/*/*.elf examples/*/*.hex html/

lib:	$(OBJECTS)
	echo "$(PUR)Building library $(LIB)$(NC)"
	ar rcs src/lib$(LIB).a $(OBJECTS)
	cp src/lib$(LIB).a lib/
	rm -rf lib/$(LIB)
	mkdir lib/$(LIB)
	cp src/*.h lib/$(LIB)

$(EXAMPLES):	lib
	echo "$(PUR)Building example $@$(NC)"
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
	echo "$(PUR)Building docs$(NC)"
	doxygen DOXYFILE

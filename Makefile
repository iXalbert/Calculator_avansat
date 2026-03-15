CC = gcc
CFLAGS = -Wall `pkg-config --cflags sdl2`
LIBS = `pkg-config --libs sdl2` -lSDL2_ttf

calculator_avansat_2: calculator_avansat_gui.c
	$(CC) $(CFLAGS) -o calculator_avansat_2 calculator_avansat_gui.c $(LIBS)

clean:
	rm -f calculator_avansat_2

.PHONY: clean

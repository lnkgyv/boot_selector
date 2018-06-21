APP = boot_selector

SOURCES = main.c

CC = gcc

LIBNCURSES = -lncurses
LIBPANEL = -lpanel
LIBMENU = -lmenu
LIBCONFIG = -lconfig

LIBRARIES = $(LIBNCURSES) $(LIBPANEL) $(LIBMENU)

#$(APP): $(SOURCES)
#	$(CC) -o $(APP) $(SOURCES) $(LIBRARIES)

main: main.o config_reader.o
	gcc -o main main.o config_reader.o $(LIBCONFIG)

config_reader.o: config_reader.c
	gcc -c -o config_reader.o config_reader.c $(LIBCONFIG)

main.o: main.c
	gcc -c -o main.o main.c $(LIBCONFIG)

clean:
	rm -f $(APP)
	rm -f config_reader
	rm -f main.o config_reader.o
	rm -f main

.PHONY: clean $(APP) main main.o config_reader.o

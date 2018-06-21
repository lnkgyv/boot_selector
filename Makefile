APP = boot_selector

# OS X compiler installed from brew ports
#CC = /usr/local/opt/gcc/bin/gcc-8
CC = gcc

COMPONENTS = main.o config_reader.o

LIBNCURSES = -lncurses
LIBPANEL = -lpanel
LIBMENU = -lmenu
LIBCONFIG = -lconfig

LIBRARIES = $(LIBNCURSES) $(LIBPANEL) $(LIBMENU) $(LIBCONFIG)

$(APP): $(COMPONENTS)
	$(CC) -o $(APP) $(COMPONENTS) $(LIBRARIES)

config_reader.o: config_reader.c
	$(CC) -c -o config_reader.o $< $(LIBCONFIG)

main.o: main.c
	$(CC) -c -o main.o main.c $(LIBCONFIG)

clean:
	rm -f $(APP)
	rm -f $(COMPONENTS)

.PHONY: clean $(APP) $(COMPONENTS)

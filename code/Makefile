CC=gcc
CFLAGS=-c -Wall
SOURCES=main.c DBMS.c display_menu.c list.c work_menu.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=city_electric_scooters

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
IDIR =inc
CC=gcc-12
PROG_NAME = Friend_of_Friends
CFLAGS=-I$(IDIR) -g -Wall -lm -O3 -fopenmp
BDIR = bin

ODIR=obj
LDIR=lib

_DEPS = read_data_n.h read_data.h periodic_separation.h list_combine.h #definitions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o read_data_n.o read_data.o periodic_separation.o list_combine.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

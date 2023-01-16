IDIR =../inc
CC=gcc
PROG_NAME = Friend_of_Friends
CFLAGS=-I$(IDIR) -Wall -lm -O3 -fopenmp
BDIR = ../bin

ODIR=../obj
LDIR=../lib

_DEPS = definitions.h read_data_n.h read_data.h periodic_separation.h Nodes.h list_combine.h DM_loop.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ =  read_data_n.o read_data.o periodic_separation.o list_combine.o Nodes.o DM_loop.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

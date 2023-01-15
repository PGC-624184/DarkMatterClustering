IDIR =../inc
CC=gcc-12
PROG_NAME = Friend_of_Friends
CFLAGS=-I$(IDIR) -Wall -lm -O3 -fopenmp
BDIR = ../bin

ODIR=../obj
LDIR=../lib

_DEPS = read_data_n.h read_data.h definitions.h periodic_separation.h Nodes.h list_combine.h DM_Loop.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = read_data_n.o main.o read_data.o periodic_separation.o list_combine.o Nodes.o DM_Loop.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

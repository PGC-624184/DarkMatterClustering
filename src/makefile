IDIR =../inc
CC=clang
PROG_NAME = Friend_of_Friends
CFLAGS=-I$(IDIR) -Wall -lm -O3
BDIR = ../bin

ODIR=../obj
LDIR=../lib

_DEPS = read_data_n.h read_data.h definitions.h periodic_separation.h Nodes.h list_combine.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

_OBJ = read_data_n.o main.o read_data.o periodic_separation.o list_combine.o Nodes.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(BDIR)/$(PROG_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

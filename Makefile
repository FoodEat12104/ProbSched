# Compilação do projeto ProbSched
CC = cc
INCLUDES = -Iinclude
CFLAGS = -Wall -c $(INCLUDES)
LDFLAGS = -lm
SRC = src
OBJ = main.o process.o scheduler.o stats.o distributions.o utils.o

all: probsched

probsched: $(OBJ)
	$(CC) -o probsched $(OBJ) $(LDFLAGS)

main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) $(SRC)/main.c -o main.o

process.o: $(SRC)/process.c
	$(CC) $(CFLAGS) $(SRC)/process.c -o process.o

scheduler.o: $(SRC)/scheduler.c
	$(CC) $(CFLAGS) $(SRC)/scheduler.c -o scheduler.o

stats.o: $(SRC)/stats.c
	$(CC) $(CFLAGS) $(SRC)/stats.c -o stats.o

distributions.o: $(SRC)/distributions.c
	$(CC) $(CFLAGS) $(SRC)/distributions.c -o distributions.o
	
utils.o: $(SRC)/utils.c
	$(CC) $(CFLAGS) $(SRC)/utils.c -o utils.o

clean limpar:
	rm -f probsched *.o
	rm -f *~
	echo "Remover: Ficheiros executáveis, objetos e temporários."


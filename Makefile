# Compilação do projeto ProbSched
CC = cc
INCLUDES = -Iinclude
CFLAGS = -Wall -c $(INCLUDES)
LDFLAGS = -lm
SRC = src
OBJ = main.o process.o queue.o scheduler.o utils.o distributions.o

all: probsched

probsched: $(OBJ)
	$(CC) -o probsched $(OBJ) $(LDFLAGS)

main.o: $(SRC)/main.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/main.c -o main.o

process.o: $(SRC)/process.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/process.c -o process.o

queue.o: $(SRC)/queue.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/queue.c -o queue.o

scheduler.o: $(SRC)/scheduler.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/scheduler.c -o scheduler.o

#stats.o: $(SRC)/stats.c include/probsched.h
#	$(CC) $(CFLAGS) $(SRC)/stats.c -o stats.o

utils.o: $(SRC)/utils.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/utils.c -o utils.o

distributions.o: $(SRC)/distributions.c include/probsched.h
	$(CC) $(CFLAGS) $(SRC)/distributions.c -o distributions.o

clean limpar:
	rm -f probsched *.o
	rm -f *~
	echo "Remover: Ficheiros executáveis, objetos e temporários."


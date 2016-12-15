#			Programmation Repartie (4I400)
#		Makefile pour TD 5. Introduction à la communication inter-processus (IPC)

CC =gcc
LFLAGS = -lpthread
CFLAGS =-W -Wall -ansi -pedantic

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

all: directories $(BIN)remonte_ipc $(BIN)remonte_partagee $(BIN)multi_remonte $(BIN)multi_remonte_file_unique $(BIN)wait_barrier $(BIN)prod_conso_partagees
 
directories: ${OBJ} ${BIN} ${INC} ${SRC}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${INC}:
	mkdir ${INC}
${SRC}:
	mkdir ${SRC}


$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) $(LFLAGS) -o $@ $<

$(OBJ)%.o: $(SRC)%.c $H
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -rf $(OBJ)*.o $(BIN)*


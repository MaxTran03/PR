#			Programmation Repartie (4I400)
#		Makefile pour TD4 . Introduction au processus légers


CC =gcc
LFLAGS = -lpthread
CFLAGS =-W -Wall -ansi -pedantic

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

all: directories $(BIN)thread_upper $(BIN)thread_uppermax $(BIN)producteur_consommateur $(BIN)producteurs_consommateurs $(BIN)signal_thread $(BIN)thread_func $(BIN)thread_arborescent_sync
 
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

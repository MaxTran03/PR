#			Programmation Repartie (4I400)
#		Makefile pour TD 3. Introduction au processus légers


CC =gcc
LFLAGS = -lpthread
CFLAGS =-W -Wall -ansi -pedantic

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

all: directories $(BIN)thread_create $(BIN)thread_rand $(BIN)thread_wait $(BIN)thread_detach $(BIN)thread_setdetach $(BIN)thread_broadcast
 
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

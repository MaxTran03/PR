#			Programmation Repartie (4I400)
#		Makefile pour TD 8. Socket et Parallélisme

CC =gcc
LFLAGS = -lpthread -lrt
CFLAGS =-W -Wall -ansi -pedantic

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

all: directories  $(BIN)notifier_async $(BIN)suspendre_async $(BIN)temporiser_async $(BIN)remonte_async $(BIN)inverseur_async $(BIN)creation_tr $(BIN)remonte_signal $(BIN)signale_barriere
 
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


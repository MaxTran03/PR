CC=gcc -Wall -ansi
BIN=bin
INC=include
OBJ=obj
SRC=src
LIB=lib
TEST=test

all: directories ${BIN}/max_func ${BIN}/max_calcul ${BIN}/nfork ${BIN}/arbo ${BIN}/chaine_proc ${BIN}/synchro_proc ${BIN}/kill_proc ${BIN}/stop_and_go

directories: ${OBJ} ${BIN} ${INC} ${LIB} ${SRC} ${TEST}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${INC}:
	mkdir ${INC}
${LIB}:
	mkdir ${LIB}
${SRC}:
	mkdir ${SRC}
${TEST}:
	mkdir ${TEST}

#####################################
################ TD1 ################
#####################################



${BIN}/max_func: ${OBJ}/max_func.o
	${CC} -o ${BIN}/max_func ${OBJ}/max_func.o
	
${OBJ}/max_func.o: ${SRC}/max_func.c ${INC}/max_func.h
	${CC} -c -o ${OBJ}/max_func.o ${SRC}/max_func.c -I${INC}


${BIN}/max_calcul: ${OBJ}/max_calcul.o
	${CC} -o ${BIN}/max_calcul ${OBJ}/max_calcul.o
	
${OBJ}/max_calcul.o: ${SRC}/max_calcul.c 
	${CC} -c -o ${OBJ}/max_calcul.o ${SRC}/max_calcul.c
	
	
${BIN}/nfork: ${OBJ}/nfork.o
	${CC} -o ${BIN}/nfork ${OBJ}/nfork.o
	
${OBJ}/nfork.o: ${SRC}/nfork.c ${INC}/nfork.h
	${CC} -c -o ${OBJ}/nfork.o ${SRC}/nfork.c -I${INC}
	

${BIN}/arbo: ${OBJ}/arbo.o
	${CC} -o ${BIN}/arbo ${OBJ}/arbo.o
	
${OBJ}/arbo.o: ${SRC}/arbo.c
	${CC} -c -o ${OBJ}/arbo.o ${SRC}/arbo.c
	

#######################################
################ TME 1 ################
#######################################

${BIN}/chaine_proc: ${OBJ}/chaine_proc.o
	${CC} -o ${BIN}/chaine_proc ${OBJ}/chaine_proc.o
	
${OBJ}/chaine_proc.o: ${SRC}/chaine_proc.c
	${CC} -c -o ${OBJ}/chaine_proc.o ${SRC}/chaine_proc.c


${BIN}/synchro_proc: ${OBJ}/synchro_proc.o
	${CC} -o ${BIN}/synchro_proc ${OBJ}/synchro_proc.o
	
${OBJ}/synchro_proc.o: ${SRC}/synchro_proc.c
	${CC} -c -o ${OBJ}/synchro_proc.o ${SRC}/synchro_proc.c
	
	
${BIN}/kill_proc: ${OBJ}/kill_proc.o
	${CC} -o ${BIN}/kill_proc ${OBJ}/kill_proc.o
	
${OBJ}/kill_proc.o: ${SRC}/kill_proc.c
	${CC} -c -o ${OBJ}/kill_proc.o ${SRC}/kill_proc.c
	

${BIN}/stop_and_go: ${OBJ}/stop_and_go.o
	${CC} -o ${BIN}/stop_and_go ${OBJ}/stop_and_go.o
	
${OBJ}/stop_and_go.o: ${SRC}/stop_and_go.c
	${CC} -c -o ${OBJ}/stop_and_go.o ${SRC}/stop_and_go.c
	
	

clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~




	
	


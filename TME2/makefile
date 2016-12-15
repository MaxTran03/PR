CC=gcc -Wall -ansi
BIN=bin
INC=include
OBJ=obj
SRC=src
LIB=lib
TEST=test

all: directories ${BIN}/forkfilemax ${BIN}/mycp ${BIN}/remonte ${BIN}/lectures ${BIN}/liste-rep ${BIN}/extended-grep ${BIN}/inverser-fichier ${BIN}/inverser-pread

directories: ${OBJ} ${BIN} ${INC} ${SRC}

${OBJ}:
	mkdir ${OBJ}
${BIN}:
	mkdir ${BIN}
${INC}:
	mkdir ${INC}
${SRC}:
	mkdir ${SRC}
${TEST}:

#####################################
################ TD2 ################
#####################################



${BIN}/forkfilemax: ${OBJ}/forkfilemax.o
	${CC} -o ${BIN}/forkfilemax ${OBJ}/forkfilemax.o
	
${OBJ}/forkfilemax.o: ${SRC}/forkfilemax.c
	${CC} -c -o ${OBJ}/forkfilemax.o ${SRC}/forkfilemax.c


${BIN}/mycp: ${OBJ}/mycp.o
	${CC} -o ${BIN}/mycp ${OBJ}/mycp.o
	
${OBJ}/mycp.o: ${SRC}/mycp.c
	${CC} -c -o ${OBJ}/mycp.o ${SRC}/mycp.c


${BIN}/remonte: ${OBJ}/remonte.o
	${CC} -o ${BIN}/remonte ${OBJ}/remonte.o
	
${OBJ}/remonte.o: ${SRC}/remonte.c
	${CC} -c -o ${OBJ}/remonte.o ${SRC}/remonte.c


######################################
################ TME2 ################
######################################

${BIN}/lectures: ${OBJ}/lectures.o
	${CC} -o ${BIN}/lectures ${OBJ}/lectures.o
	
${OBJ}/lectures.o: ${SRC}/lectures.c
	${CC} -c -o ${OBJ}/lectures.o ${SRC}/lectures.c


${BIN}/liste-rep: ${OBJ}/liste-rep.o
	${CC} -o ${BIN}/liste-rep ${OBJ}/liste-rep.o
	
${OBJ}/liste-rep.o: ${SRC}/liste-rep.c
	${CC} -c -o ${OBJ}/liste-rep.o ${SRC}/liste-rep.c
	

${BIN}/extended-grep: ${OBJ}/extended-grep.o
	${CC} -o ${BIN}/extended-grep ${OBJ}/extended-grep.o
	
${OBJ}/extended-grep.o: ${SRC}/extended-grep.c
	${CC} -c -o ${OBJ}/extended-grep.o ${SRC}/extended-grep.c
	

${BIN}/inverser-fichier: ${OBJ}/inverser-fichier.o
	${CC} -o ${BIN}/inverser-fichier ${OBJ}/inverser-fichier.o
	
${OBJ}/inverser-fichier.o: ${SRC}/inverser-fichier.c
	${CC} -c -o ${OBJ}/inverser-fichier.o ${SRC}/inverser-fichier.c
	

${BIN}/inverser-pread: ${OBJ}/inverser-pread.o
	${CC} -o ${BIN}/inverser-pread ${OBJ}/inverser-pread.o
	
${OBJ}/inverser-pread.o: ${SRC}/inverser-pread.c
	${CC} -c -o ${OBJ}/inverser-pread.o ${SRC}/inverser-pread.c
	
	
clean:
	rm -f ${OBJ}/* ${BIN}/*

cleanall:
	rm -rf ${OBJ} ${BIN}
	rm -f ${INC}/*~ ${SRC}/*~ *~


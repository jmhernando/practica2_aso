EXECS=parking coche camion
MPICC?=mpicc

all: ${EXECS}

parking: parking.c
	${MPICC} -o parking parking.c

coche: coche.c
	${MPICC} -o coche coche.c

camion: camion.c
	${MPICC} -o camion camion.c

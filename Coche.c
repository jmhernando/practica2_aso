#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char **argv){
    int rank,size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status estado;

    int entrada[2]={0,0};
    int salida[2]={0,1};
    int hueco,id;

    while(1){
        int espera=rand() % 5;
        MPI_Comm_rank(MPI_COMM_WORLD, &id);
        MPI_Send(entrada, 2, MPI_INT, 0, 0, MPI_COMM_WORLD);//Mandamos solicitud de entrada
        MPI_Recv(&hueco, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &estado);//Esperamos respuesta

        if(hueco!=0){ //Si hay plaza
            sleep(espera); //Esperamos
            MPI_Send(salida, 2, MPI_INT, 0, 0, MPI_COMM_WORLD); //Mandamos solicitud de salida
            sleep(espera);
        }else{ //Si no hay plaza esperamos
            sleep(espera);
        }
    }
    MPI_Finalize();
}
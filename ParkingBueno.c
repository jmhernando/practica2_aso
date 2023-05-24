/*LIBRERIAS*/

#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*FUNCIONES*/

void salir();
int vehiculoEntra(int tipo_vehiculo,int id);
void imprimirParking(int plantas, int plazas);

/*VARIABLES*/
int parking[100][100];      //Creamos el array que simulará el parking, como máximo 100 plazas y 100 plantas.
int plazas, plantas,id_vehiculo;

int main(int argc, char **argv){
    int rank, size;
    MPI_Init(&argc, &argv); //Inicializa el entorno de MPI.
    MPI_Comm_size(MPI_COMM_WORLD,&size);    //Obtener el tamaño total de procesos.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);    //Obtiene el proceso actual.

    if(argc == 3){
        plantas = atoi(argv[1]);
        plazas = atoi(argv[2]);
    }else{
        printf("No has introducido los argumentos correctamente.\n");
        printf("La forma correcta de ejecución del programa debe ser: \n mpirun -np 1 main 'plantas' 'plazas' : -np 'N_procesos_coche' coche : -np 'N_procesos_camion' camion.\n");
        printf("Las ' ' se omitirán.\n");
        printf("Dónde N_procesos_coche es un número entero que representará el número de coches.\n");
        printf("Dónde N_procesos_camion es un número entero que representará el número de camiones.\n");
        return (1);     //Terminamos de ejecutar el programa puesto que se ha escrito mal el comando.
    }

    printf("Comando ejecutado correctamente. Generando simulación...\n");

    for(int i=0;i<plantas;i++){
        for(int j=0;j<plazas;j++){
            parking[i][j]=0;
        }
    }
    printf("Se ha creado el parking.\n");

    MPI_Status process_status;      //Obtendremos información de los envíos y otros datos.
    MPI_Barrier(MPI_COMM_WORLD);    //Flag que para al procesos hasta que TODOS los procesos lleguen a este punto.

    char accion_vehiculo[2][10];    //Este array servirá como mensaje en el que se analizará que tipo de vehiculo
    // y la acción que realiza (SALE, ENTRA)

    int aux; // Declarar la variable aux fuera del bucle while

    while (1) {
        MPI_Recv(&accion_vehiculo, 2 * 10, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &process_status);
        id_vehiculo = process_status.MPI_SOURCE;

        if (strcmp(accion_vehiculo[1], "COCHE") == 0 && strcmp(accion_vehiculo[0], "ENTRA") == 0) {
            if (vehiculoEntra(0, id_vehiculo) == -1) {
                aux = 0;
                MPI_Send(&aux, 1, MPI_INT, id_vehiculo, 1, MPI_COMM_WORLD);  // Le decimos al coche que no hay sitio.
                printf("No hay sitio en el parking. El vehiculo con id: %d espera.", id_vehiculo);
            } else {
                aux = 1;
                MPI_Send(&aux, 1, MPI_INT, id_vehiculo, 1, MPI_COMM_WORLD);  // Le decimos al coche que hay sitio.
            }
        }
    }

}

int vehiculoEntra(int tipo_vehiculo, int id_vehiculo){
    switch (tipo_vehiculo) {
        case 1:
            for(int i=0;i<plantas;i++){
                for(int j=0;j<plazas;j++){
                    if(parking[i][j]==0){
                        parking[i][j] = id_vehiculo;
                        int plazas_libres=0;

                        for(int j=0;j<plazas;j++){
                            if(parking[i][j]==0){
                                plazas_libres++;
                            }
                        }
                        printf("ENTRADA: Coche %d aparca en planta %d - plaza %d. Plazas libres en la planta: %d \n",id_vehiculo,i,j, plazas_libres);
                        imprimirParking(plantas,plazas_libres);
                        return 1;       //Terminamos la ejecución puesto que ya tenemos el resultado de la operación.
                    }
                }
            }
            break;
        case 2:
            for(int i=0;i<plantas;i++){
                for(int j=0;j<plazas;j++){
                    if(j+1<=plazas && parking[i][j]==0 && parking[i][j+1]==0){
                        parking[i][j] = id_vehiculo+100;        //El enunciado requiere que un camión tenga el id +100
                        parking[i][j+1] = id_vehiculo+100;
                        int plazas_libres=0;

                        for(int j=0;j<plazas;j++){
                            if(parking[i][j]==0){
                                plazas_libres++;
                            }
                        }
                        printf("ENTRADA: Camion %d aparca en planta %d - plaza %d y plaza %d. Plazas libres en la planta: %d \n",id_vehiculo,i,j,j+1, plazas_libres);
                        imprimirParking(plantas,plazas_libres);
                        return 1;       //Terminamos la ejecución puesto que ya tenemos el resultado de la operación.
                    }
                }
            }
            break;
        default :
            printf("Ha ocurrido un error inesperado.");
            return -1;
    }
    return -1;
}

void imprimirParking(int plantas, int plazas){
    printf("Parking:\n");
    for(int i=0; i < plantas;i++){
        printf("Planta %d:",i);
        for(int j=0; j < plazas; j++){
            printf("[%d]",parking[i][j]);
        }
        printf("\n");
    }
}

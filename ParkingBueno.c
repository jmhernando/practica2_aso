/*LIBRERIAS*/

#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*FUNCIONES*/

void salir();
int comprobar();


/*VARIABLES*/

int plazas, plantas;

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

    int parking [plantas][plazas];      //Creamos el array que simulará el parking.
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

    while(1){

    }





}


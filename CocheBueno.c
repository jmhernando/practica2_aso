/*LIBRERIAS*/

#include <mpi/mpi.h>        //La usaremos para usar la programación en paralelo.
#include <string.h>         //Lo usamos para los arrays de caracteres.
#include <stdlib.h>         //Usaremos algunas funciones guardadas en esta librería (rand()).
#include <unistd.h>         //Tenemos acceso a algunas funciones que usaremos. (Sleep()).


int main(int argc,char **argv){                 //Función principa.
    //Rank permite distinguir un proceso de otro.
    //Size permite saber el número de procesos que vamos a tener.
    int rank, size;
    MPI_Init(&argc, &argv); //Inicializa el entorno de MPI.
    MPI_Comm_size(MPI_COMM_WORLD,&size);    //Obtener el tamaño total de procesos.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);    //Obtiene el proceso actual.

    MPI_Status process_status;      //Obtendremos información de los envíos y otros datos.
    MPI_Barrier(MPI_COMM_WORLD);    //Flag que para al procesos hasta que TODOS los procesos lleguen a este punto.

    //place servirá para saber si hay sitio en el parking.
    //Id será el número del coche. Su representación será numérica.
    int place,id;

    //Se han creado dos arrays que representarán la salida y entrada de coche.
    //Será el mensaje que se envíe.
    char coche_entra[2][10];
    strcpy(coche_entra[0],"ENTRA");
    strcpy(coche_entra[1],"COCHE");

    char coche_sale[2][10];
    strcpy(coche_sale[0],"SALE");
    strcpy(coche_sale[1],"COCHE");

    while(1){       //Queremos que se ejecute constantemente.
        int waiting = rand() % 6;   //El proceso se quedará esperando entre un rango de 0 y 5 segundos.
        MPI_Comm_rank(MPI_COMM_WORLD, &id); //Obtenemos el proceso actual y lo guardamos en la variable id.
        //Se envía el array coche_entra, el segundo parámetro es 2*10 al ser un array bidimensional.
        MPI_Send(coche_entra, 2 * 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&place, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &process_status);

        if(place!=0){   //Si hay plaza entramos en el if
            sleep(waiting); //Simulamos que se ha aparcado el automovil.
            //Enviamos la solicitud pero de salir del parking.
            MPI_Send(coche_sale, 2*10, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
            sleep(waiting);
        }
        else{
            sleep(waiting%3); //Esperamos un tiempo para volver a preguntar.
        }
    }
    MPI_Finalize();
}


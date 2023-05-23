#include <mpi/mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int parking[1000][1000]; //si 0 vacio / otro lleno

void salirParking(int plantas, int plazasPorPlanta, int id); //Declaracion de la funcion
int comprobarHueco(int tipo,int plantas,int plazasPorPlanta,int id); //Declaracion de la funcion
void imprimir(int plantas, int plazasPorPlanta); //Declaracion de la funcion

int main(int argc,char **argv){
    //Secuencia de comandos necesaria para inicializar la libreria MPI
    int rank,size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    //Creación y recogida del tamaño del parking por argumento

    int plantas, plazasPorPlanta;

    if(argc == 3){
        plantas = atoi(argv[1]);
        plazasPorPlanta = atoi(argv[2]);
    }else{
        printf("Faltan argumentos!!!\n");
    }

    printf("Creando parking...\n");

    //Rellenamos el parking con 0, ya que esto indica plaza vacía

    for(int ii=0;ii<plantas;ii++){
        for(int jj=0;jj<plazasPorPlanta;jj++){
            parking[ii][jj] = 0;
        }
    }
    printf("Parking creado!\n");
    MPI_Barrier(MPI_COMM_WORLD); //Es un punto de sincronización, cuando todos los procesos lleguen a este punto
    //continua la ejecución.
    MPI_Status estado;

    //Creamos un array que llevará la información entre los procesos
    int mensaje[2]; //[tipo] [operacion]
    /*
        tipo -> 0=coche / 1=camion
        operacion -> 0=entrada / 1=salida
    */

    while(1){
        //Usamos la función recibir de la librería MPI esperando un mensaje de un vehiculo


        /*    &mensaje: Es el puntero al búfer donde se almacenará el mensaje recibido. En este caso, se espera que mensaje sea una variable de tipo int o un arreglo de int con espacio suficiente para almacenar 2 elementos.

    2: Es el número de elementos que se esperan recibir en el mensaje. En este caso, se espera recibir 2 elementos de tipo int.

    MPI_INT: Es el tipo de datos del mensaje que se espera recibir. En este caso, el mensaje se trata como un arreglo de int.

    MPI_ANY_SOURCE: Es el identificador del proceso emisor del cual se desea recibir el mensaje. En este caso, se utiliza MPI_ANY_SOURCE para indicar que se acepta el mensaje de cualquier proceso emisor.

    0: Es la etiqueta o identificador del mensaje que se espera recibir. En este caso, se utiliza 0 para indicar que se acepta cualquier etiqueta.

    MPI_COMM_WORLD: Es el comunicador utilizado para la comunicación entre procesos. En este caso, se utiliza MPI_COMM_WORLD, que es el comunicador por defecto que incluye todos los procesos del programa MPI.

    &estado: Es el puntero a una variable MPI_Status donde se almacenará información detallada sobre el estado de la operación de recepción. Después de la llamada a MPI_Recv, se puede utilizar estado para obtener información como el identificador del proceso emisor, el tamaño del mensaje recibido y otros detalles.

En resumen, la línea de código MPI_Recv(&mensaje, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado); indica que el proceso actual espera recibir un mensaje de cualquier proceso emisor, con una etiqueta de 0, conteniendo 2 elementos de tipo int. Después de la llamada, la variable mensaje contendrá el mensaje recibido y la variable estado proporcionará información adicional sobre la operación de recepción.*/
        MPI_Recv(&mensaje, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &estado);
        int id = estado.MPI_SOURCE;
        if(mensaje[0]==0){ //Si es COCHE
            if(mensaje[1]==0){//Comprobando si entra
                //Pasamos coche como argumento
                if(comprobarHueco(0,plantas,plazasPorPlanta,id)==-1){
                    int temp = 0; //Indicamos al coche que no hay hueco
                    MPI_Send(&temp, 1, MPI_INT, id, 1, MPI_COMM_WORLD); //Respondemos al coche
                    printf("No hay hueco. El coche %d se marcha... \n",id);
                }else{
                    int temp = 1; //Indicamos al coche que hay hueco
                    MPI_Send(&temp, 1, MPI_INT, id, 1, MPI_COMM_WORLD); //Respondemos al coche
                }
            }else{//Si no entra, sale
                salirParking(plantas,plazasPorPlanta,id);
            }
        }else if(mensaje[0]==1){//Si es CAMION
            if(mensaje[1]==0){//Comprobando si entra
                if(comprobarHueco(1,plantas,plazasPorPlanta,(id + 100))==-1){
                    int valor=0; //Indicamos al CAMION que no hay hueco
                    MPI_Send(&valor, 1, MPI_INT, id, 2, MPI_COMM_WORLD); //Respondemos al camion
                    printf("No hay hueco. El CAMION %d se marcha... \n",id);
                }else{
                    int valor = 1; //Indicamos al CAMION que hay hueco
                    MPI_Send(&valor, 1, MPI_INT, id, 2, MPI_COMM_WORLD); //Respondemos al camion
                }
            }else{//Si no entra, sale
                salirParking(plantas,plazasPorPlanta,(id + 100));
            }
        }
    }
    MPI_Finalize();
    return 0;
}

//Funcion que devuelve 1 si encuentra plaza libre, ya sea para un coche o un camion
int comprobarHueco(int tipo,int plantas,int plazasPorPlanta,int id){
    if(tipo==0){
        for(int ii=0;ii<plantas;ii++){
            for(int jj=0;jj<plazasPorPlanta;jj++){
                if(parking[ii][jj]==0){
                    parking[ii][jj] = id;
                    int plazasLibresHueco=0;

                    for(int jj=0;jj<plazasPorPlanta;jj++){
                        if(parking[ii][jj]==0){
                            plazasLibresHueco++;
                        }
                    }

                    printf("ENTRADA: Coche %d aparca en planta %d - plaza %d. Plazas libres en la planta: %d \n",id,ii,jj, plazasLibresHueco);
                    imprimir(plantas,plazasPorPlanta);
                    return 1;
                }
            }
        }
    }else if(tipo==1){
        for(int ii=0;ii<plantas;ii++){
            for(int jj=0;jj<plazasPorPlanta;jj++){
                if(parking[ii][jj]==0 && parking[ii][jj+1]==0 && jj+1<plazasPorPlanta){
                    parking[ii][jj]=id;
                    parking[ii][jj+1]=id;
                    int plazasLibresHueco2=0;

                    for(int jj=0;jj<plazasPorPlanta;jj++){
                        if(parking[ii][jj]==0){
                            plazasLibresHueco2++;
                        }
                    }

                    printf("ENTRADA: Camión %d aparca en planta %d - plaza %d. Plazas libres en la planta: %d \n",id,ii,jj, plazasLibresHueco2);
                    imprimir(plantas,plazasPorPlanta);
                    return 1;
                }
            }
        }
    }
    return -1;
}

//Funcion que saca un vehiculo del parking
void salirParking(int plantas, int plazasPorPlanta, int id){
    int tipo = 0;
    for(int ii=0;ii<plantas;ii++){
        for(int jj=0;jj<plazasPorPlanta;jj++){
            if(parking[ii][jj]==id){
                int plazasLibresHueco3=0;
                if(parking[ii][jj] == parking[ii][jj+1]) {
                    parking[ii][jj + 1] = 0;
                    tipo=1;
                }
                parking[ii][jj] = 0;
                for(int jj=0;jj<plazasPorPlanta;jj++){
                    if(parking[ii][jj]==0){
                        plazasLibresHueco3++;
                    }
                }

                if(tipo==1){//Si es un camion
                    printf("El CAMION %d sale del parking. Plazas libres en la planta: %d\n",id, plazasLibresHueco3);
                    imprimir(plantas,plazasPorPlanta);
                }else{
                    printf("El COCHE %d sale del parking. Plazas libres en la planta: %d\n",id, plazasLibresHueco3);
                    imprimir(plantas,plazasPorPlanta);
                }
            }
        }
    }
}

//Funcion que imprime el parking
void imprimir(int plantas, int plazasPorPlanta){
    for(int ii=0;ii<plantas;ii++){
        printf("Planta %d = ",ii);
        for(int jj=0;jj<plazasPorPlanta;jj++){
            printf("[%d] ", parking[ii][jj]);
        }
        printf("\n");
    }
    printf("\n");
}

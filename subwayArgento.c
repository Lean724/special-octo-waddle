 #include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50

//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
	sem_t sem_freir;
	sem_t sem_hornear_pan;
	sem_t sem_armar;
	sem_t sem_finalizo;
	
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[8];
};

//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
	//creo el nombre de la accion de la funcion 
	char *accion = "cortar";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion
	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	//doy la se??al a la siguiente accion (cortar me habilita mezclar)
    sem_post(&mydata->semaforos_param.sem_mezclar);
	
    pthread_exit(NULL);
}

void* mezclar(void *data) {
	char *accion = "mezclar";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 20000 );
	sem_post(&mydata->semaforos_param.sem_freir);
	sem_post(&mydata->semaforos_param.sem_hornear_pan);
	pthread_exit(NULL);
}

void* armar(void *data) {
	char *accion = "armar";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 40000 );
	sem_post(&mydata->semaforos_param.sem_finalizo);
	pthread_exit(NULL);
}

void* finalizo(void *data) {
	char *accion = "finalizo";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 20000 );
	pthread_exit(NULL);
	
}

void* freir(void *data) {
	char *accion = "freir";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 50000 );
	sem_post(&mydata->semaforos_param.sem_armar);
	pthread_exit(NULL);
}

void* hornear(void *data) {
	char *accion = "hornear";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 90000 );
	sem_post(&mydata->semaforos_param.sem_armar);
	pthread_exit(NULL);
}


void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_freir;
	sem_t sem_hornear_pan;
	sem_t sem_armar;
	sem_t sem_finalizo;
	
	
	//variables hilos
	pthread_t p1; 

	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_freir = sem_freir;
	pthread_data->semaforos_param.sem_hornear_pan = sem_hornear_pan;
	pthread_data->semaforos_param.sem_armar =  sem_armar;
	pthread_data->semaforos_param.sem_finalizo = sem_finalizo;
	
	

	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) 
	//Falta leer por teclado
     strcpy(pthread_data->pasos_param[0].accion, "cortar");
	 strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
     strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");
	 strcpy(pthread_data->pasos_param[0].ingredientes[2], "pepino");


	strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
    strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");
	
	strcpy(pthread_data->pasos_param[2].accion, "freir");
	strcpy(pthread_data->pasos_param[2].ingredientes[0], "milanesa");
    
	strcpy(pthread_data->pasos_param[3].accion, "hornear");
	strcpy(pthread_data->pasos_param[3].ingredientes[0], "pan");

	strcpy(pthread_data->pasos_param[4].accion, "armar");
	strcpy(pthread_data->pasos_param[4].ingredientes[0], "pan");
	strcpy(pthread_data->pasos_param[4].ingredientes[1], "milanesa");
	strcpy(pthread_data->pasos_param[4].ingredientes[2], "lechuga");
	strcpy(pthread_data->pasos_param[4].ingredientes[3], "tomate");
	strcpy(pthread_data->pasos_param[4].ingredientes[4], "pepino");
	strcpy(pthread_data->pasos_param[4].ingredientes[3], "cebolla Morada");


	strcpy(pthread_data->pasos_param[5].accion, "finalizo");
		strcpy(pthread_data->pasos_param[4].ingredientes[0], "");
    
    
	 
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_freir),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_hornear_pan),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_armar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_finalizo),0,0);


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;

	//cortar
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia	
	
	//mezclar

	rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia	
	
	
	//freir

	rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                freir,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia	
	
	
	//armar

	rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                armar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia	
																	
	//termino	
	rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                finalizo,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia	
	
	pthread_join (p1,NULL);

	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_freir);
	sem_destroy(&sem_hornear_pan);
	sem_destroy(&sem_armar);
	sem_destroy(&sem_finalizo);
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
 
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3;
	pthread_t equipo4;
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);
    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3);
    rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);
								
  
   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
    pthread_exit(NULL);
}


//Para compilar:   gcc sba.c -o sba -lpthread
//Para ejecutar:   ./sba

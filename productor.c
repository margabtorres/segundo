#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/msg.h>

#include <unistd.h>


#include <time.h>


/*	SEGUNDO OBLIGATORIO
Se quiere construir un sistema que administre la mensajería de un sistema.

Dos procesos se encuentran en ejecución siempre, enviando información al Administrador de Red. 

El primero envía información con forma de números entre 10 y 19 y el segundo entre 30 y 39; es decir, fraccionan
el mensaje en 10 partes. Luego de envíar cada número, los procesos descansan por un período de tiempo aleatorio.

--------------------------------------

Los procesos envían información y esperan a que el Administrador les responda: Error o Listo. 
Si la respuesta es Error el Administrador desecha el mensaje y solicita nuevamente el envío, 
si devuelve Listo no se envía otra vez el error y el dato es procesado por el Administrador de Red.

El 20% de las veces el Administrador envía una señal de Error, el resto el resultado es Listo.

--------------------------------------

Los dos procesos envían la información por el mismo canal, por lo que los mensajes hacia el Administrador pueden intercalarse.
El Administrador almacena la información recibida de forma secuencial, agrupando los mensajes según el emisor, es decir,
agrupa los mensajes para el primer emisor todos juntos y hace lo mismo para el segundo emisor; idealmente en una sola estructura de datos.

Al mismo tiempo, el Administrador informa si ha recibido el mensaje completo e informa al cliente, quien espera un ratito
aleatorio y finaliza su ejecución. Se debe mostrar cómo el Administrador de Red va procesando los datos.

*/

/*
	Mensajes
*/

#define ID_SMS1 1
#define ID_SMS2 2

/*COLORES*/
#define VERDE "\x1b[42m"
#define RESET "\e[0m"

typedef struct tipo_mensaje{
	long id_mensaje;
	int mensaje;
	int status;
} tipo_mensaje;


int main(){
	key_t clave1, clave2;
	int id_cola1, id_cola2;	
	
	tipo_mensaje un_mensaje;
	tipo_mensaje respuesta;
	
	//definimos la cola de mensajes
	clave1 = ftok("./msg1",25);
	id_cola1 = msgget(clave1, 0600|IPC_CREAT);		//se crea la cola de mensaje, id_cola es el identificador de cola
	
	//definimos la cola de mensajes
	clave2 = ftok("./msg2",26);
	id_cola2 = msgget(clave2, 0600|IPC_CREAT);		//se crea la cola de mensaje, id_cola es el identificador de cola
	
	int i;
	i = fork();	//se creas nuevo proceso
	
	if (i > 0){			//proceso 1 que envia mensajes del 10 al 19
		
		/*
		arranca en 10
		*/
		int n;
		n = 10;			
		while (n<=19){
			msgrcv(id_cola2, &respuesta,sizeof(respuesta.mensaje)+sizeof(respuesta.status),ID_SMS1, 0);
			
			un_mensaje.id_mensaje = ID_SMS1;
			if (respuesta.status == 1){
				//n = respuesta.mensaje;
				un_mensaje.mensaje = n;
				msgsnd(id_cola1, &un_mensaje,sizeof(un_mensaje.mensaje)+sizeof(un_mensaje.status), 0);  //con msgsnd se envia el mensaje
				printf ("Emisor! Mensaje:%d ID:%li\n", un_mensaje.mensaje,un_mensaje.id_mensaje);
				n += 1;
			}else if (respuesta.status == 0){

				//n = respuesta.mensaje;
				un_mensaje.mensaje = n-1;

				msgsnd(id_cola1, &un_mensaje,sizeof(un_mensaje.mensaje)+sizeof(un_mensaje.status), 0);  //con msgsnd se envia el mensaje
				//un_mensaje.mensaje = n;
				printf (VERDE "Reenviando! Mensaje:%d ID:%li" RESET "\n", un_mensaje.mensaje,un_mensaje.id_mensaje);
			}
		sleep(2);
		}
		printf ("Terminé de enviar 1\n");
	}
	else				//proceso 2 que envia mensajes del 30 al 39
	{
		int n;
		n = 30;
		while (n<=39){
			msgrcv(id_cola2, &respuesta,sizeof(respuesta.mensaje)+sizeof(respuesta.status),ID_SMS2, 0);
			
			un_mensaje.id_mensaje = ID_SMS2;
			if (respuesta.status == 1){
				//n = respuesta.mensaje;
				un_mensaje.mensaje = n;
				msgsnd(id_cola1, &un_mensaje, sizeof(un_mensaje.mensaje)+sizeof(un_mensaje.status), 0); //con msgsnd se envia el mensaje					
				printf ("Emisor! Mensaje:%d ID:%li\n", un_mensaje.mensaje,un_mensaje.id_mensaje);
				n += 1;
			}else if (respuesta.status == 0){
				//n = respuesta.mensaje;
				un_mensaje.mensaje=n-1;
				msgsnd(id_cola1, &un_mensaje, sizeof(un_mensaje.mensaje)+sizeof(un_mensaje.status), 0); //con msgsnd se envia el mensaje
				printf (VERDE "Reenviando! Mensaje:%d ID:%li" RESET "\n", un_mensaje.mensaje,un_mensaje.id_mensaje);
			}
			sleep(2);
		
		}
		printf ("Terminé de enviar 2\n");
	}
	
	msgctl(id_cola2, IPC_RMID, 0);
	return (1);
}




	